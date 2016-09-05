#if VHAL_SPI
#include "vhal.h"
#include "vhal_common.h"
#include "vhal_gpio.h"
#include "vhal_dma.h"
#include "vhal_spi.h"


//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)


#define NUM_SPI 3

typedef struct _spi_drv {
	uint32_t cr1;
	SPI_TypeDef *spi;
	uint8_t drvid;
	uint8_t status;
	uint8_t dmabits;
	uint8_t spi_n;
	uint16_t nss;
	VSemaphore sem;
	VThread thread;
} SpiDrv;

const SPI_TypeDef *spireg[] = {SPI1, SPI2, SPI3};

typedef struct _dma_spi {
	uint8_t dmarx;
	uint8_t dmatx;
} SpiDMA;


const SpiDMA spidma[NUM_SPI] = {
	{VHAL_SPI1_RX_DMA_STREAM, VHAL_SPI1_TX_DMA_STREAM},
	{VHAL_SPI2_RX_DMA_STREAM, VHAL_SPI2_TX_DMA_STREAM},
	{VHAL_SPI3_RX_DMA_STREAM, VHAL_SPI3_TX_DMA_STREAM},
};


static SpiDrv spidrv[NUM_SPI];

static uint8_t _spi_initialized = 0;

int vhalInitSPI(void *data) {
	(void)data;
	if (!_spi_initialized) {
		dma_init();
		memset(spidrv, 0, sizeof(spidrv));

		int i;
		for (i = 0; i < PERIPHERAL_NUM(spi); i++) {
			int idx = GET_PERIPHERAL_ID(spi, i);
			spidrv[idx].sem = vosSemCreate(1);
			spidrv[idx].spi = spireg[idx];
			spidrv[idx].drvid = idx;
			printf("Initialized SPI %i at %i at %x\n", i, idx, spidrv[idx].spi);
		}
		_spi_initialized=1;
	}
	return 0;
}



//TODO: handle error!
void spi_dma_rx_callback(uint32_t stream, uint32_t sr, void *args) {
	//if (sr & DMA_SxCR_TCIE) {
	SpiDrv *spid = (SpiDrv *)args;
	dma_stream_disable(spidma[spid->drvid].dmarx);
	dma_stream_disable(spidma[spid->drvid].dmatx);
	vosSysLockIsr();
	if (spid->thread != NULL) {
		vosThResumeIsr(spid->thread);
		spid->thread = NULL;
	}
	vosSysUnlockIsr();
	//}
}

//TODO: handle error!
void spi_dma_tx_callback(uint32_t stream, uint32_t sr, void *args) {
	if (sr & DMA_CCR1_TCIE) {
		//do nothing, last interrupt is rx
	}
}


void _rcc_spi_on(uint32_t spi_n) {
	//enable clocks
	printf("spi_on %i\n", spi_n);
	spi_n++;
	switch (spi_n) {
		case 1:
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
			break;
		case 2:
			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
			break;
		case 3:
			RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
			break;
	}

}

void _rcc_spi_off(uint32_t spi_n) {
	//enable clocks
	spi_n++;
	switch (spi_n) {
		case 1:
			RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
			break;
		case 2:
			RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;
			break;
		case 3:
			RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;
			break;
	}

}



int vhalSpiInit(uint32_t spi, vhalSpiConf *conf) {
	if (spi >= PERIPHERAL_NUM(spi))
		return -1;
	int spiid = GET_PERIPHERAL_ID(spi, spi);
	SpiDrv *spid = &spidrv[spiid];

	//in use!
	if (spid->status)
		return -1;


	spid->cr1 = 1;

	//determine nearest clock speed: page 734 of RM_f10x: BR[0:2] in CR1
	//SPI1 is clocked by apb2 == _system_frequency
	//SPI2&3 are clocked by apb1 == _system_frequency/2
	int i, cnt = 2;
	uint32_t frq = (spiid==0) ? (_system_frequency):(_system_frequency/2);
	for (i = 0; i < 8; i++, cnt *= 2) {
		if ((frq / cnt) <= conf->clock) {
			spid->cr1 = (i << 3);
			printf("found clock with i %i cnt %i and cr1 %i\n",i,cnt,spid->cr1);
			conf->clock = (frq / cnt);
			break;
		}
	}
	if (spid->cr1 == 1) {
		//clock not supported
		return -1;
	}

	//POLARITY & PHASE
	switch (conf->mode) {
		case SPI_MODE_LOW_FIRST:
			spid->cr1 |= 0;
			break;
		case SPI_MODE_LOW_SECOND:
			spid->cr1 |= 0 | SPI_CR1_CPHA;
			break;
		case SPI_MODE_HIGH_FIRST:
			spid->cr1 |= SPI_CR1_CPOL | 0;
			break;
		case SPI_MODE_HIGH_SECOND:
			spid->cr1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;
			break;
		default:
			return -1;
	}

	//WORD SIZE
	switch (conf->bits) {
		case SPI_BITS_8:
			spid->dmabits = DMA_8_BIT;
			break;
		case SPI_BITS_16:
			spid->dmabits = DMA_16_BIT;
			spid->cr1 |= SPI_CR1_DFF;
			break;
		default:
			return -1;
	}

	//cfg->cr1 = SPI_CR1_CPHA | SPI_CR1_BR_1;
	//PIN CONFIGURATION
	spid->spi_n = spiid;
	spid->nss = conf->nss;

	//get dma
	if (dma_get_stream(spidma[spiid].dmarx, spi_dma_rx_callback, spid) < 0)
		return -1;
	if (dma_get_stream(spidma[spiid].dmatx, spi_dma_tx_callback, spid) < 0)
		return -1;


	//set pins
	vhalPinSetToPeripheral(conf->sclk, PRPH_SPI, PIN_PARAMS(PINPRM_MODE_50MHZ, PINPRM_CNF_ALTERNATE_PP, PIN_CLASS_DATA2(conf->sclk), PIN_CLASS_DATA0(conf->sclk), PIN_CLASS_DATA1(conf->sclk)));
	vhalPinSetToPeripheral(conf->mosi, PRPH_SPI, PIN_PARAMS(PINPRM_MODE_50MHZ, PINPRM_CNF_ALTERNATE_PP, PIN_CLASS_DATA2(conf->mosi), PIN_CLASS_DATA0(conf->mosi), PIN_CLASS_DATA1(conf->mosi)));
	vhalPinSetToPeripheral(conf->miso, PRPH_SPI, PIN_PARAMS(PINPRM_MODE_INPUT, PINPRM_CNF_INPUT, PIN_CLASS_DATA2(conf->miso), PIN_CLASS_DATA0(conf->miso), PIN_CLASS_DATA1(conf->miso)));
	vhalPinSetToPeripheral(conf->nss, PRPH_SPI, PIN_PARAMS(PINPRM_MODE_50MHZ, PINPRM_CNF_GPIO_PP, PIN_CLASS_DATA2(conf->miso), PIN_CLASS_DATA0(conf->miso), PIN_CLASS_DATA1(conf->miso)));


	// --- configure spi ---
	_rcc_spi_on(spiid);


	//set registers

	//set cr1: only master mode and NSS is handled by software
	spid->spi->CR1 = 0;
	spid->spi->CR1 = spid->cr1 | SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI;
	//set cr2: set nss and dma interrupts
	spid->spi->CR2 = SPI_CR2_SSOE | SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;

	//enable spi
	spid->spi->CR1 |= SPI_CR1_SPE;

	printf("configured spi %i -> %i -> %i @clock %i %x :: %x\n", spi, spiid, spid->drvid, conf->clock, spid->cr1,spid->spi);
	printf("CR1 = %x\n", spid->spi->CR1);
	printf("CR2 = %x\n", spid->spi->CR2);
	printf("RCC = %x %x\n", RCC->APB1ENR, RCC->APB2ENR);

	spid->status = 1;
	return 0;
}

int vhalSpiLock(uint32_t spi) {
	if (spi >= PERIPHERAL_NUM(spi))
		return -1;
	vosSemWait(spidrv[GET_PERIPHERAL_ID(spi, spi)].sem);
	return 0;
}
int vhalSpiUnlock(uint32_t spi) {
	vosSemSignal(spidrv[GET_PERIPHERAL_ID(spi, spi)].sem);
	return 0;
}


int vhalSpiSelect(uint32_t spi) {
	//clear nss
	SpiDrv *spid = &spidrv[GET_PERIPHERAL_ID(spi, spi)];
	vhalPinWrite(spid->nss,0);
	return 0;
}
int vhalSpiUnselect(uint32_t spi) {
	//set nss
	SpiDrv *spid = &spidrv[GET_PERIPHERAL_ID(spi, spi)];
	vhalPinWrite(spid->nss,1);
	return 0;
}

uint16_t nullrx = 0;
uint16_t nulltx = 0;

int vhalSpiExchange(uint32_t spi, void *tosend, void *toread, uint32_t bytes) {

	uint32_t spiid = GET_PERIPHERAL_ID(spi, spi);
	uint32_t mpincrx;
	uint32_t mpinctx;
	SpiDrv *spid = &spidrv[spiid];

	if (toread == NULL && tosend == NULL) {
		toread = (void *)&nullrx;
		tosend = (void *)&nulltx;
		mpincrx = mpinctx = DMA_MEM_PRPH_INCR(0, 0);
	} else if (toread == NULL) {
		toread = (void *)&nullrx;
		mpincrx = DMA_MEM_PRPH_INCR(0, 0);
		mpinctx = DMA_MEM_PRPH_INCR(1, 0);
	} else if (tosend == NULL) {
		tosend = (void *)&nulltx;
		mpincrx = DMA_MEM_PRPH_INCR(1, 0);
		mpinctx = DMA_MEM_PRPH_INCR(0, 0);
	} else {
		mpinctx = DMA_MEM_PRPH_INCR(1, 0);
		mpincrx = DMA_MEM_PRPH_INCR(1, 0);
	}
	printf("vhal_spi configuring dma transfer %i %i\n", bytes / (spid->dmabits + 1), spidma[spiid].dmarx);
	dma_conf_stream(spidma[spiid].dmarx, &spid->spi->DR, toread, bytes / (spid->dmabits + 1), spid->dmabits, mpincrx);
	dma_conf_stream(spidma[spiid].dmatx, tosend, &spid->spi->DR, bytes / (spid->dmabits + 1), spid->dmabits, mpinctx);
	spid->thread = vosThCurrent();
	vosSysLock();
	dma_stream_enable(spidma[spiid].dmarx);
	dma_stream_enable(spidma[spiid].dmatx);
	vosThSuspend();
	vosSysUnlock();
	printf("vhal_spi returning transfer\n");

	return 0;
}

int vhalSpiDone(uint32_t spi) {
	int spiid = GET_PERIPHERAL_ID(spi, spi);
	if (spidrv[spiid].status) {
		spidrv[spiid].spi->CR1 = 0;
		spidrv[spiid].spi->CR2 = 0;
		dma_free_stream(spidma[spiid].dmarx);
		dma_free_stream(spidma[spiid].dmatx);
		_rcc_spi_off(PIN_CLASS_DATA0(spidrv[spiid].spi_n));
		spidrv[spiid].status = 0;
	}
	return 0;
}



#else

/*
#include "vhal.h"
#include "vhal_gpio.h"
#include "hal.h"


//smt32f1 has max 3 spi peripherals

static vhalSpiConf *spi_conf[3];
static VSemaphore spi_sem[3];
static uint8_t spi_status[3];
static SPIDriver *spidrv[3];
static SPIConfig spicfg[3];




int vhalInitSPI(void *data) {
	(void)data;
	memset(spi_conf, 0, sizeof(spi_conf));
	memset(spi_sem, 0, sizeof(spi_sem));
	memset(spi_status, 0, sizeof(spi_status));

	int i;
	for (i = 0; i < PERIPHERAL_NUM(spi); i++) {
		int idx = GET_PERIPHERAL_ID(spi, i);
		spi_sem[idx] = vosSemCreate(1);
	}

#if STM32_SPI_USE_SPI1
	spidrv[PERIPHERAL_ID(1)] = &SPID1;
#endif
#if STM32_SPI_USE_SPI2
	spidrv[PERIPHERAL_ID(2)] = &SPID2;
#endif
#if STM32_SPI_USE_SPI3
	spidrv[PERIPHERAL_ID(3)] = &SPID3;
#endif

	return 0;
}

int vhalSpiInit(uint32_t spi, vhalSpiConf *conf) {
	if (spi >= PERIPHERAL_NUM(spi))
		return -1;
	int spiid = GET_PERIPHERAL_ID(spi, spi);
	SPIConfig *cfg  = &spicfg[spiid];
	spi_conf[spiid] = conf;
	int clk = 1;

	cfg->end_cb = NULL;
	//determine nearest clock speed: page 734 of RM_f101: BR[0:2] in CR1
	//SPI is clocked by apb2 == _system_frequency
	int i, cnt = 2;
	for (i = 0; i < 8; i++, cnt *= 2) {
		if ((_system_frequency / cnt) <= conf->clock) {
			clk = (i << 3);
			conf->clock = (_system_frequency / cnt);
			break;
		}
	}
	if (clk == 1) {
		//clock not supported
		return -1;
	}
	cfg->cr1 = clk;

	//POLARITY & PHASE
	switch (conf->mode) {
		case SPI_MODE_LOW_FIRST:
			cfg->cr1 |= 0;
			break;
		case SPI_MODE_LOW_SECOND:
			cfg->cr1 |= 0 | SPI_CR1_CPHA;
			break;
		case SPI_MODE_HIGH_FIRST:
			cfg->cr1 |= SPI_CR1_CPOL | 0;
			break;
		case SPI_MODE_HIGH_SECOND:
			cfg->cr1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;
			break;
		default:
			return -1;
	}

	//WORD SIZE
	switch (conf->bits) {
		case SPI_BITS_8:
			break;
		case SPI_BITS_16:
			cfg->cr1 |= SPI_CR1_DFF;
			break;
		default:
			return -1;
	}

	//cfg->cr1 = SPI_CR1_CPHA | SPI_CR1_BR_1;
	//PIN CONFIGURATION
	cfg->ssport = CPIN_PORT(conf->nss);
	cfg->sspad = PIN_PAD(conf->nss);

	vhalPinSetToPeripheral(conf->sclk, PRPH_SPI, PIN_PARAMS(PINPRM_MODE_50MHZ, PINPRM_CNF_ALTERNATE_PP, PIN_CLASS_DATA2(conf->sclk), PIN_CLASS_DATA0(conf->sclk), PIN_CLASS_DATA1(conf->sclk)));
	vhalPinSetToPeripheral(conf->mosi, PRPH_SPI, PIN_PARAMS(PINPRM_MODE_50MHZ, PINPRM_CNF_ALTERNATE_PP, PIN_CLASS_DATA2(conf->mosi), PIN_CLASS_DATA0(conf->mosi), PIN_CLASS_DATA1(conf->mosi)));
	vhalPinSetToPeripheral(conf->miso, PRPH_SPI, PIN_PARAMS(PINPRM_MODE_INPUT, PINPRM_CNF_INPUT, PIN_CLASS_DATA2(conf->miso), PIN_CLASS_DATA0(conf->miso), PIN_CLASS_DATA1(conf->miso)));
	vhalPinSetToPeripheral(conf->nss, PRPH_SPI, PIN_PARAMS(PINPRM_MODE_50MHZ, PINPRM_CNF_GPIO_PP, PIN_CLASS_DATA2(conf->miso), PIN_CLASS_DATA0(conf->miso), PIN_CLASS_DATA1(conf->miso)));
	//master ignored, chibios has only master support
	spiStart(spidrv[spiid], cfg);
	spi_status[spiid] = 1;
	return 0;
}

int vhalSpiLock(uint32_t spi) {
	if (spi >= PERIPHERAL_NUM(spi))
		return -1;
	vosSemWait(spi_sem[GET_PERIPHERAL_ID(spi, spi)]);
	return 0;
}
int vhalSpiUnlock(uint32_t spi) {
	vosSemSignal(spi_sem[GET_PERIPHERAL_ID(spi, spi)]);
	return 0;
}

int vhalSpiSelect(uint32_t spi) {
	spiSelect(spidrv[GET_PERIPHERAL_ID(spi, spi)]);
	return 0;
}
int vhalSpiUnselect(uint32_t spi) {
	spiUnselect(spidrv[GET_PERIPHERAL_ID(spi, spi)]);
	return 0;
}


int vhalSpiExchange(uint32_t spi, void *tosend, void *toread, uint32_t blocks) {
	SPIDriver *drv = spidrv[GET_PERIPHERAL_ID(spi, spi)];
	if (toread == NULL && tosend == NULL) {
		spiIgnore(drv, blocks);
	} else if (toread == NULL) {
		spiSend(drv, blocks, tosend);
	} else if (tosend == NULL) {
		spiReceive(drv, blocks, toread);
	} else {
		spiExchange(drv, blocks, tosend, toread);
	}
	return 0;
}

int vhalSpiDone(uint32_t spi) {
	int spiid = GET_PERIPHERAL_ID(spi, spi);
	if (spi_status[spiid]) {
		spiStop(spidrv[spiid]);
		spi_status[spiid] = 0;
	}
	return 0;
}
*/

#endif