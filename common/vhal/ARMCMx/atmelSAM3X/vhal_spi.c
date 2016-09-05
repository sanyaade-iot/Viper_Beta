#if VHAL_SPI

#include "vhal.h"
#include "vhal_spi.h"
#include "vhal_dma.h"
#include "vhal_gpio.h"


//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)

typedef struct {
	uint8_t id_spi;
	uint8_t rxch;
	uint8_t txch;
	uint8_t pad;
	uint8_t clk;
	uint8_t trflags;
	uint8_t btsdelay;
	uint8_t bcsdelay;
	void *port;
	Spi *spi;
	dmatrans_t dmarx;
	dmatrans_t dmatx;
	void *thread;
} SPI_Config;


static VSemaphore spi_sem[2];
static SPI_Config spi_cfg[2];
static uint8_t spi_status[2];
static uint8_t spi_initialized = 0;




static void dma_rx_callback(uint32_t channel, uint32_t err, void *arg) {
	(void)channel;
	SPI_Config *cfg = (SPI_Config *)arg;
	if (err) {
		//TODO: handle error
		//SAM3X_SPI_DMA_ERROR_HOOK(cfg);
	}
	dmaStop(cfg->rxch);
	vosSysLockIsr();
	if (cfg->thread != NULL) {
		vosThResumeIsr(cfg->thread);
		cfg->thread = NULL;
	}
	vosSysUnlockIsr();
}


static void dma_tx_callback(uint32_t channel, uint32_t err, void *arg) {
	(void)channel;
	SPI_Config *cfg = (SPI_Config *)arg;
	// if (err)
	// 	SAM3X_SPI_DMA_ERROR_HOOK(spip);
	while ((cfg->spi->SPI_SR & SPI_SR_TXEMPTY) == 0);
	dmaStop(cfg->txch);
}

int vhalInitSPI(void *data) {
	(void)data;
	if (!spi_initialized) {
		dma_init();
		memset(spi_cfg, 0, sizeof(spi_cfg));
		memset(spi_sem, 0, sizeof(spi_sem));
		memset(spi_status, 0, sizeof(spi_status));

		spi_sem[0] = vosSemCreate(1);
		spi_sem[1] = vosSemCreate(1);

		spi_cfg[0].id_spi = ID_SPI0;
		spi_cfg[0].spi = SPI0;
		spi_cfg[1].id_spi = ID_SPI1;
		spi_cfg[1].spi = SPI1;

		spi_initialized = 1;
	}
	return 0;
}

int vhalSpiInit(uint32_t spiy, vhalSpiConf *conf) {
	if (spiy >= PERIPHERAL_NUM(spi))
		return -1;
	int spiid = GET_PERIPHERAL_ID(spi, spiy);
	SPI_Config *cfg  = &spi_cfg[spiid];
	Spi *spi = cfg->spi;

	//SPI is prescaled by 1 to 255 with respect to main clock
	if ( conf->clock > _system_frequency  || conf->clock < _system_frequency / 255) {
		return -1;
	}
	cfg->clk = _system_frequency / conf->clock;
	cfg->clk=12;
	//POLARITY & PHASE
	switch (conf->mode) {
		case SPI_MODE_LOW_FIRST:
			cfg->trflags = SPI_POL_ZERO | SPI_PHASE_SF;
			break;
		case SPI_MODE_LOW_SECOND:
			cfg->trflags = SPI_POL_ZERO | SPI_PHASE_SS;
			break;
		case SPI_MODE_HIGH_FIRST:
			cfg->trflags = SPI_POL_ONE | SPI_PHASE_SF;
			break;
		case SPI_MODE_HIGH_SECOND:
			cfg->trflags = SPI_POL_ONE | SPI_PHASE_SS;
			break;
		default:
			return -1;
	}

	//WORD SIZE
	switch (conf->bits) {
		case SPI_BITS_8:
			cfg->trflags |= SAM3X_SPI_BITS_8;
			break;
		case SPI_BITS_16:
			cfg->trflags |= SAM3X_SPI_BITS_16;
			break;
		default:
			return -1;
	}

	//PIN CONFIGURATION
	cfg->port = PIN_PORT(conf->nss);
	cfg->pad = PIN_PAD(conf->nss);
	//Low level cfg
	cfg->rxch = 0;
	cfg->txch = 1;
	cfg->btsdelay = 0;
	cfg->bcsdelay = 1;



	vhalPinSetToPeripheral(conf->sclk, PRPH_SPI, SAM3X_PIN_PR(conf->sclk)|6);
	vhalPinSetToPeripheral(conf->miso, PRPH_SPI, SAM3X_PIN_PR(conf->miso)|6);
	vhalPinSetToPeripheral(conf->mosi, PRPH_SPI, SAM3X_PIN_PR(conf->mosi)|6);
	vhalPinSetMode(conf->nss, PINMODE_OUTPUT_PUSHPULL);

	pmc_enable_clk_l32(cfg->id_spi);
	spi_disable(spi);
	spi_reset(spi);
	spi_reset(spi);


	spi_set_master_mode(spi);
	spi_disable_mode_fault_detect(spi);
	spi_disable_loopback(spi);

	/* fixed peripheral: the cs pin is managed by gpio */
	spi_set_fixed_peripheral_select(spi);
	spi_set_peripheral_chip_select_value(spi, spi_get_pcs(0));
	spi_disable_peripheral_select_decode(spi);
	spi_set_delay_between_chip_select(spi, 1);

	//nss param is at zero: we support only PA:28 as the cs pin
	spi_set_transfer_delay(spi, 0, cfg->bcsdelay, cfg->btsdelay);
	spi_set_bits_per_transfer(spi, 0, cfg->trflags & 0xfffffff0);
	spi_set_baudrate_div(spi, 0, cfg->clk);
	spi_set_clock_polarity(spi, 0, cfg->trflags & SPI_POL_ONE);
	spi->SPI_CSR[0] |= SPI_CSR_CSAAT;
	spi_set_clock_phase(spi, 0, cfg->trflags & SPI_PHASE_SS);


	//dma hw channels, check page 339 of RM --> they go in DMAC->CHANNEL->CFG register for hw handshake
	DMA_MPCFG_SETUP(&cfg->dmatx, (1 + spiid * 4));
	DMA_PMCFG_SETUP(&cfg->dmarx, (2 + spiid * 4));
	dmaEnsureStop(cfg->rxch);
	dmaEnsureStop(cfg->txch);
	dmaChannelActivate(cfg->rxch, dma_rx_callback, (void *)cfg, &cfg->dmarx);
	dmaChannelActivate(cfg->txch, dma_tx_callback, (void *)cfg, &cfg->dmatx);

	spi_enable(spi);

	printf("SPI: %x\n",cfg->spi);
	printf("PMC: %x\n",PMC->PMC_PCSR0);
	printf("CR: %x\n",cfg->spi->SPI_CR);
	printf("MR: %x\n",cfg->spi->SPI_MR);
	printf("IMR: %x\n",cfg->spi->SPI_IMR);
	printf("CSR: %x\n",cfg->spi->SPI_CSR[0]);
	printf("WPMR: %x\n",cfg->spi->SPI_WPMR);

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
	vhalPinFastClear(spi_cfg[GET_PERIPHERAL_ID(spi, spi)].port, spi_cfg[GET_PERIPHERAL_ID(spi, spi)].pad);
	return 0;
}
int vhalSpiUnselect(uint32_t spi) {
	vhalPinFastSet(spi_cfg[GET_PERIPHERAL_ID(spi, spi)].port, spi_cfg[GET_PERIPHERAL_ID(spi, spi)].pad);
	return 0;
}


int vhalSpiExchange(uint32_t spi, void *tosend, void *toread, uint32_t blocks) {
	SPI_Config *cfg = &spi_cfg[GET_PERIPHERAL_ID(spi, spi)];
	uint32_t nilbuf1 = 0;
	uint32_t nilbuf2 = 0;

	dmaEnsureStop(cfg->rxch);
	dmaEnsureStop(cfg->txch);
	if (toread == NULL && tosend == NULL) {
		DMA_M2P_TRANSFER_EXT(&cfg->dmatx, blocks, &nilbuf1, spi_get_tx_access(cfg->spi), DMA_FIXED, DMA_FIXED);
		DMA_P2M_TRANSFER_EXT(&cfg->dmarx, blocks, spi_get_rx_access(cfg->spi), &nilbuf2, DMA_FIXED, DMA_FIXED);
	} else if (toread == NULL) {
		DMA_M2P_TRANSFER_EXT(&cfg->dmatx, blocks, tosend, spi_get_tx_access(cfg->spi), DMA_INCR, DMA_FIXED);
		DMA_P2M_TRANSFER_EXT(&cfg->dmarx, blocks, spi_get_rx_access(cfg->spi), &nilbuf1, DMA_FIXED, DMA_FIXED);
	} else if (tosend == NULL) {
		DMA_M2P_TRANSFER_EXT(&cfg->dmatx, blocks, &nilbuf1, spi_get_tx_access(cfg->spi), DMA_FIXED, DMA_FIXED);
		DMA_P2M_TRANSFER_EXT(&cfg->dmarx, blocks, spi_get_rx_access(cfg->spi), toread, DMA_FIXED, DMA_INCR);
	} else {
		DMA_M2P_TRANSFER_EXT(&cfg->dmatx, blocks, tosend, spi_get_tx_access(cfg->spi), DMA_INCR, DMA_FIXED);
		DMA_P2M_TRANSFER_EXT(&cfg->dmarx, blocks, spi_get_rx_access(cfg->spi), toread, DMA_FIXED, DMA_INCR);
	}
	dmaSetupTransfer(cfg->txch, &cfg->dmatx);
	dmaSetupTransfer(cfg->rxch, &cfg->dmarx);
	printf("DMAC: %x\n",Dmac);
	printf("DMAC->IMR: %x\n",Dmac->DMAC_EBCIMR);
	printf("DMAC->CH[%i]->CFG: %x\n",cfg->rxch,Dmac->DMAC_CH_NUM[cfg->rxch].DMAC_CFG);
	printf("DMAC->CH[%i]->CFG: %x\n",cfg->txch,Dmac->DMAC_CH_NUM[cfg->txch].DMAC_CFG);
	printf("PMC: %x\n",PMC->PMC_PCSR1);
	cfg->thread = vosThCurrent();
	vosSysLock();
	dmaStart(cfg->rxch);
	dmaStart(cfg->txch);
	vosThSuspend();
	vosSysUnlock();

	return 0;
}

int vhalSpiDone(uint32_t spi) {
	int spiid = GET_PERIPHERAL_ID(spi, spi);
	if (spi_status[spiid]) {
		spi_reset(spi_cfg[spiid].spi);
		spi_disable(spi_cfg[spiid].spi);
		pmc_disable_clk_l32(spi_cfg[spiid].id_spi);
		dmaChannelDeactivate(spi_cfg[spiid].rxch);
		dmaChannelDeactivate(spi_cfg[spiid].txch);

		spi_status[spiid] = 0;
	}
	return 0;
}
#else

#endif