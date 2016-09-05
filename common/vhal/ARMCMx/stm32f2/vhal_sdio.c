#if defined(VHAL_SDIO)

#include "vhal.h"
#include "vhal_sdio.h"
#include "vhal_dma.h"
#include "vhal_gpio.h"
#include "vbl.h"

uint8_t sdio_initialized = 0;


typedef struct _sdio_struct {
	SDIO_TypeDef *sdio;
	VThread thread;
	uint32_t mode;
	sdio_callback callback;
} SDIO_drv;


SDIO_drv sdiodrv[1];

void vhal_SDIO_Isr(void);

#define SDIO_enable_irq() (SDIO->MASK = SDIO_MASK_SDIOITIE | SDIO_MASK_CMDRENDIE | SDIO_MASK_CMDSENTIE)
#define SDIO_disable_irq() (SDIO->MASK=0)

int vhalInitSDIO(void *data) {
	(void)data;
	if (!sdio_initialized) {
		sdio_initialized = 1;
		dma_init();
		vosInstallHandler(SDIO_IRQn, vhal_SDIO_Isr);
		sdiodrv[0].sdio = SDIO;
		sdiodrv[0].thread = NULL;
		sdiodrv[0].mode = 0;
		sdiodrv[0].callback = NULL;
	}
}

int vhalSdioInit(uint32_t sdio, sdio_callback cbk){
	if (sdio != 0)
		return -1;
	SDIO_drv *drv = &sdiodrv[sdio];

	drv->mode = SDIO_TRANSFER_MODE(512, 1, 0);
	drv->callback = cbk;
	//clock sdio
	RCC->APB2ENR |= RCC_APB2ENR_SDIOEN;

	//vhalIrqClearPending(SDIO_IRQn);
	vhalIrqEnable(SDIO_IRQn);

	//setup pins
	int i;
	for (i = 0; i < 10; i++) {
		vhalPinSetToPeripheral(_vm_sdio_pins[sdio].sdio_pins[i], PRPH_SDC, ALTERNATE_FN(SDIO_AF) | STM32_OSPEED_HIGHEST | STM32_PUDR_PULLUP  | STM32_OTYPE_PUSHPULL);
	}

	vhalSdioSetClock(sdio,0);

	/* enable irq */
	SDIO->ICR = (uint32_t) 0xffffffff;

	return 0;
}

int vhalSdioSetClock(uint32_t sdio, uint32_t clockfreq) {
	SDIO_drv *drv = &sdiodrv[sdio];
	uint32_t clkcr;

	if (clockfreq) {
		// hi freq
		// set to clockdiv 0 = 48Mhz/2
		// rising edge, bypass disable, no powersave, bus mode 1b
		// flow control disabled
		clkcr =0;
	} else {
		//lo freq
		clkcr |= 120 | SDIO_CLKCR_PWRSAV;
	}

	switch (SDIO_BLOCKMODE(drv->mode)) {
	case 1:
		clkcr |= 0;
		break;
	case 4:
		clkcr |= SDIO_CLKCR_WIDBUS_0;
		break;
	case 8:
		clkcr |= SDIO_CLKCR_WIDBUS_1;
		break;
	}

	//reset
	RCC->APB2RSTR |= RCC_APB2RSTR_SDIORST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SDIORST;

	uint32_t sdioreg = SDIO->CLKCR;
	/* Clear CLKDIV, PWRSAV, BYPASS, WIDBUS, NEGEDGE, HWFC_EN bits */
	sdioreg &= CLKCR_CLEAR_MASK;


	//configure
	sdioreg |= clkcr;

	/* Write to SDIO CLKCR */
	SDIO->CLKCR = sdioreg;

	/* power up */
	SDIO->POWER &= PWR_PWRCTRL_MASK;
	SDIO->POWER |= SDIO_POWER_PWRCTRL;


	if (!clockfreq) {
		/* set wait mode on CLK */
		SDIO->DCTRL |= SDIO_DCTRL_RWMOD;
	}

	/* enable sdio clock */
	SDIO->CLKCR |= SDIO_CLKCR_CLKEN;
	return 0;
}

int vhalSdioSetMode(uint32_t sdio, uint32_t mode){
	SDIO_drv *drv = &sdiodrv[sdio];
	
	drv->mode =mode;
	return 0;
}

int vhalSdioWrite(uint32_t sdio, uint32_t command, uint32_t mode, uint8_t *data, uint32_t datasize, uint32_t* response){
	return -1;
}

int vhalSdioRead(uint32_t sdio, uint32_t command, uint32_t mode, uint8_t *data, uint32_t datasize, uint32_t* response){
	return -1;
}
int vhalSdioDone(uint32_t sdio){
	return -1;
}


void vhal_SDIO_Isr(void){
	vosEnterIsr();
	if(sdiodrv[0].callback){
		sdiodrv[0].callback();
	}
	vosExitIsr();
}



#endif