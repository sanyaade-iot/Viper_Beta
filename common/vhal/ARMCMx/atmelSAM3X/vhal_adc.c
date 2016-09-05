#if VHAL_ADC

#include "vhal.h"
#include "vhal_gpio.h"
#include "vhal_adc.h"


//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)


typedef struct {
	uint32_t num_channels;
	uint32_t channels;
	uint32_t seq1;
	uint32_t seq2;
} CGInfo;



typedef struct _adc {
	Adc *adc;
	CGInfo cg;
	VThread *thread;
	uint32_t mr;
	uint8_t status;
} ADCInfo;



ADCInfo _adcinfo[1];


void vhal_IRQ_ADC() {
	
	uint32_t status;
	vosEnterIsr();
	status = _adcinfo[0].adc->ADC_ISR;
	if (status & (ADC_ISR_RXBUFF | ADC_ISR_ENDRX)) {
		// we are at end buffer
		vosSysLockIsr();
		adc_stop();
		if (_adcinfo[0].thread) {
			vosThResumeIsr(_adcinfo[0].thread);
			_adcinfo[0].thread = NULL;
		}
		vosSysUnlockIsr();
	}
	vosExitIsr();

}

int vhalInitADC(void *data) {
	(void)data;
	_adcinfo[0].thread = NULL;
	_adcinfo[0].status = 0;
	_adcinfo[0].adc = ADC;
	vosInstallHandler(ADC_IRQn, vhal_IRQ_ADC);
	printf("ADC INIT\n");
	return 0;
}


int vhalAdcGetPeripheralForPin(int vpin) {
	if (PIN_CLASS(vpin) != PINCLASS_ANALOG)
		return -1;
	return PIN_CLASS_DATA1(vpin);
}

int vhalAdcInit(uint32_t adc, vhalAdcConf *conf) {
	if (adc >= PERIPHERAL_NUM(adc))
		return -1;
	if (!_adcinfo[0].status) {
		pmc_enable_clk_g32(ID_ADC);
		Adc *adcd = _adcinfo[0].adc;
		adcd->ADC_CR = ADC_CR_SWRST;
		adcd->ADC_MR = 0;
		adcd->ADC_PTCR = (ADC_PTCR_RXTDIS | ADC_PTCR_TXTDIS);
		adcd->ADC_RCR = 0;
		adcd->ADC_RNCR = 0;
		_adcinfo[0].mr = ADC_MR_PRESCAL(1) | ADC_MR_STARTUP_SUT0
		               | ADC_MR_TRACKTIM(11) | ADC_MR_TRANSFER(1) |  ADC_MR_SETTLING_AST3
		               | ADC_MR_LOWRES_BITS_12
		               | ADC_MR_SLEEP_NORMAL;
		adcd->ADC_MR = _adcinfo[0].mr; 

		conf->samples_per_second = 1000000; //approx
		printf("ADC ok pre\n");
		vhalIrqEnable(ADC_IRQn);
		printf("ADC ok post %x %x\n",adcd->ADC_CR,adcd->ADC_MR);
		_adcinfo[0].status = 1;
	}
	return 0;
}


int vhalAdcPrepareCapture(uint32_t adc, vhalAdcCaptureInfo *info) {
	int i;
	CGInfo *cg = &_adcinfo[0].cg;
	if (info->npins > 16 || info->npins <= 0)
		return -1;

	cg->num_channels = info->npins;
	cg->channels = 0;
	cg->seq1 = cg->seq2 = 0;
	for (i = 0; i < info->npins; i++) {
		uint16_t vpin = info->pins[i];
		if (PIN_CLASS(vpin) != PINCLASS_ANALOG)
			return -2;
		vhalPinSetMode(vpin, PINMODE_INPUT_ANALOG);
		int adc_ch = PIN_CLASS_DATA0(vpin);
		cg->channels |= (1 << adc_ch); //set channel on in channel mask
		if (i < 8) {
			cg->seq1 |= (adc_ch) << (4 * i);
		} else {
			cg->seq2 |= (adc_ch) << (4 * (i - 8));
		}
	}
	info->sample_size = 2;
	if (info->samples > 1 && info->samples % 2 != 0)
		info->samples++;
	return info->sample_size * info->samples * info->npins;
}


void adc_convert(uint8_t *buffer, uint32_t samples) {
	_adcinfo[0].thread = vosThCurrent();
	printf("ADC CONVERT %x %i %i\n",buffer,samples,_adcinfo[0].cg.channels);
	vosSysLock();
	Adc *adcd = _adcinfo[0].adc;
	adcd->ADC_MR = _adcinfo[0].mr;
	adcd->ADC_RPR = (uint32_t) buffer;
	adcd->ADC_RCR = samples;
	adcd->ADC_RNPR = 0;
	adcd->ADC_RNCR = 0;
	// disable all channels/
	adcd->ADC_CHDR = 0xffff;
	// set active channels
	adcd->ADC_CHER = _adcinfo[0].cg.channels;
	// enable pdc interrupt
	adcd->ADC_IER = ADC_IER_RXBUFF | ADC_IER_ENDRX;
	//adcd->ADC_IER = ADC_IER_DRDY;
	// enable pdc 
	adcd->ADC_PTCR = ADC_PTCR_RXTEN;
	adcd->ADC_MR |= ADC_MR_FREERUN_ON;
	adcd->ADC_CR = ADC_CR_START;
	vosThSuspend();
	vosSysUnlock();
}
void adc_stop() {
	// stop adc 
	Adc *adcd = _adcinfo[0].adc;
	adcd->ADC_CR = ADC_CR_SWRST;
	// disable pdc interrupt
	adcd->ADC_IDR = ADC_IER_RXBUFF | ADC_IER_ENDRX | ADC_IER_DRDY;
	//disable all channels
	adcd->ADC_CHDR = 0xffff;
	// disable pdc
	adcd->ADC_RPR = 0;
	adcd->ADC_RCR = 0;
	adcd->ADC_RNCR = adcd->ADC_RNPR = 0;
	adcd->ADC_PTCR = 0;
}


/*TODO: add support for more than 1 adc */
int vhalAdcRead(uint32_t adc, vhalAdcCaptureInfo *info) {
	CGInfo *cg = &_adcinfo[0].cg;
	switch (info->capture_mode) {
		case ADC_CAPTURE_SINGLE:
			if (cg->num_channels == 1) {
				uint16_t *buf = (uint16_t *)info->buffer;
				buf[0]=0xffff;
				adc_convert(buf, info->samples);
			} else {
				//sequence capture must be simulated, sam3 adc has too many constraints on channels [RM pag 1325>>]
				//yeah, ugly :(
				int i, j, ch;
				uint16_t *buf = (uint16_t *)info->buffer;
				for (j = 0; j < info->samples; j++) {
					for (i = 0; i < info->npins; i++) {
						if (i < 8) {
							ch = (cg->seq1 >> (4 * i)) & 0xf;
						} else {
							ch = (cg->seq1 >> (4 * (i - 8))) & 0xf;
						}
						cg->channels = (1 << ch);
						adc_convert(buf++, 1);
					}
				}
			}
			break;
	}
	return 0;
}

int vhalAdcDone(uint32_t adc) {
	if (_adcinfo[0].status) {
		pmc_disable_clk_g32(ID_ADC);
		vhalIrqDisable(ADC_IRQn);
		_adcinfo[0].status = 0;
	}
	return 0;
}
#else


#endif