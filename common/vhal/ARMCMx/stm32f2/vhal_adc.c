
#if VHAL_ADC

#include "vhal.h"
#include "vhal_gpio.h"
#include "vhal_dma.h"
#include "vhal_adc.h"

//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)

typedef struct _cg {
	uint32_t        num_channels;
	uint32_t                  cr1;
	uint32_t                  cr2;
	uint32_t                  smpr1;
	uint32_t                  smpr2;
	uint32_t                  sqr1;
	uint32_t                  sqr2;
	uint32_t                  sqr3;
} CGInfo;


typedef struct _adc {
	ADC_TypeDef *adc;
	CGInfo cg;
	VThread *thread;
	uint8_t status;
	uint8_t cycles;
	uint8_t dmachannel;
	uint8_t dmastream;
} ADCInfo;



ADCInfo _adcinfo[1];

static uint8_t adc_initialized=0;

int vhalInitADC(void *data) {
	(void)data;
	if (!adc_initialized) {
		dma_init();
		//page 165 RM_401_BCDE
		_adcinfo[0].dmastream = 12;
		_adcinfo[0].dmachannel = 0;
		_adcinfo[0].adc = ADC1;
		adc_initialized=1;
	}
	return 0;
}

const uint16_t _adc_smp[] = {15, 27, 40, 68, 96, 124, 156, 492};


//TODO: support more than one adc
void adc_dma_callback(uint32_t stream, uint32_t sr, void *args) {
	if (sr & DMA_SxCR_TCIE) {
		dma_stream_disable(stream);
		_adcinfo[0].adc->CR1 = 0;
		_adcinfo[0].adc->CR2 = 0;
		_adcinfo[0].adc->CR2 = ADC_CR2_ADON;
		vosSysLockIsr();
		if (_adcinfo[0].thread != NULL) {
			vosThResumeIsr(_adcinfo[0].thread);
			_adcinfo[0].thread = NULL;
		}
		vosSysUnlockIsr();
	}
}

int vhalAdcGetPeripheralForPin(int vpin) {
	if (PIN_CLASS(vpin) != PINCLASS_ANALOG)
		return -1;
	return PIN_CLASS_DATA1(vpin);
}

int vhalAdcInit(uint32_t adc, vhalAdcConf *conf) {
	if (adc >= PERIPHERAL_NUM(adc))
		return -1;
	int adcid = GET_PERIPHERAL_ID(adc, adc);
	if (!_adcinfo[adcid].status) {
		int cycles, ok_cycles = -1;
		if (conf != NULL) {
			for (cycles = 0; cycles < 8; cycles++) {//cycles per sample
				uint32_t cur_freq = _system_frequency / VHAL_ADC_PRESCALER / _adc_smp[cycles];
				printf("Checking %i vs %i vs %i\n",cur_freq,conf->samples_per_second,_system_frequency);
				if (conf->samples_per_second <= cur_freq) {
					ok_cycles = cycles;
				} else {
					if (ok_cycles < 0) {
						//unsupported sample frequency
						return -1;
					} else {
						break;
					}
				}
			}
			conf->samples_per_second = _system_frequency / VHAL_ADC_PRESCALER / _adc_smp[ok_cycles];
		} else ok_cycles = 0;
		/* TODO: add config for different ADC freqs */
		/* activates ADC @ 21 MHz (APB2/DIV4) */
		/* TODO: add trigger configuration */
		/* TODO: add resolution config */
		//adcStart(adcdrv[adcid], NULL);
		printf("Before register messing\n");
		
		if (dma_get_stream(_adcinfo[adcid].dmastream, adc_dma_callback,NULL) < 0)
			return -1;
		_adcinfo[adcid].status = 1;
		_adcinfo[adcid].cycles = ok_cycles;
		//enable RCC
		RCC->APB2ENR |= (1 << (8 + adcid));
		ADC->CCR &= (~(3 << 16)); //DIV2: APB2 is 60 Mhz, max adc is 30 Mhz, Vhal_ADC_PRESCALER is 4 (120MHz /4)

		_adcinfo[adcid].adc->CR1 = 0;
		_adcinfo[adcid].adc->CR2 = 0;
		_adcinfo[adcid].adc->CR2 = ADC_CR2_ADON;
		
	}
	return 0;
}

int vhalAdcPrepareCapture(uint32_t adc, vhalAdcCaptureInfo *info) {
	int i;
	CGInfo *cg = &_adcinfo[GET_PERIPHERAL_ID(adc, adc)].cg;
	if (info->npins > 16 || info->npins <= 0)
		return -1;
	cg->smpr1 = 0;
	cg->smpr2 = 0;
	cg->num_channels = info->npins;
	cg->cr1 = 0;
	cg->sqr3 = 0;
	cg->sqr2 = 0;
	cg->sqr1 = (info->npins - 1) << 20;
	int cycles = _adcinfo[GET_PERIPHERAL_ID(adc, adc)].cycles;
	for (i = 0; i < info->npins; i++) {
		uint16_t vpin = info->pins[i];
		if (PIN_CLASS(vpin) != PINCLASS_ANALOG)
			return -2;
		int adc_ch = PIN_CLASS_DATA0(vpin);
		vhalPinSetMode(vpin, PINMODE_INPUT_ANALOG);

		//group capture
		if (i <= 5) {
			cg->sqr3 |= (adc_ch) << (5 * i);
		} else if (i <= 11) {
			cg->sqr2 |= (adc_ch) << (5 * (i - 6));
		} else {
			cg->sqr1 |= (adc_ch) << (5 * (i - 12));
		}
		//set sample rate for each pin
		if (adc_ch <= 9) {
			cg->smpr1 |= (cycles) << (3 * adc_ch);
		} else {
			cg->smpr2 |= (cycles) << (3 * (adc_ch - 10));
		}
	}
	cg->cr2 = ADC_CR2_SWSTART;
	if (info->samples > 1 && info->samples % 2 != 0)
		info->samples++;
	info->sample_size = 2;
	return info->sample_size * info->samples * info->npins;
}



/*TODO: add support for more than 1 adc */
int vhalAdcRead(uint32_t adc, vhalAdcCaptureInfo *info) {
	int adcid = GET_PERIPHERAL_ID(adc, adc);
	CGInfo *cg = &_adcinfo[adcid].cg;
	switch (info->capture_mode) {
		case ADC_CAPTURE_SINGLE:
			_adcinfo[adcid].thread = vosThCurrent();
			vosSysLock();
			dma_conf_stream(_adcinfo[adcid].dmastream, (uint32_t)&_adcinfo[adcid].adc->DR, (uint32_t)info->buffer, info->samples*info->npins, DMA_16_BIT, _adcinfo[adcid].dmachannel,DMA_MEM_PRPH_INCR(1, 0));
			_adcinfo[adcid].adc->SR    = 0;
			_adcinfo[adcid].adc->SMPR1 = cg->smpr1;
			_adcinfo[adcid].adc->SMPR2 = cg->smpr2;
			_adcinfo[adcid].adc->SQR1  = cg->sqr1;
			_adcinfo[adcid].adc->SQR2  = cg->sqr2;
			_adcinfo[adcid].adc->SQR3  = cg->sqr3;
			_adcinfo[adcid].adc->CR1   = cg->cr1 | ADC_CR1_SCAN;

			dma_stream_enable(_adcinfo[adcid].dmastream);
			_adcinfo[adcid].adc->CR2 = cg->cr2 | ADC_CR2_CONT  | ADC_CR2_DMA | ADC_CR2_DDS   | ADC_CR2_ADON;
			vosThSuspend();
			vosSysUnlock();
			break;
		default:
			return -1;
	}
	return 0;
}

int vhalAdcDone(uint32_t adc) {
	int adcid = GET_PERIPHERAL_ID(adc, adc);
	if (_adcinfo[adcid].status) {
		RCC->APB2ENR &= ~(1 << (8 + adcid));
		dma_free_stream(_adcinfo[adcid].dmastream);
		_adcinfo[adcid].adc->CR2 = 0;
		_adcinfo[adcid].adc->CR1 = 0;
		_adcinfo[adcid].status = 0;
	}
	return 0;
}

#else


#endif
