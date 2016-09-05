#include "vhal.h"
#include "hal.h"
#include "vhal_gpio.h"



/* GPIO */

const PinPort _vhalpinports[] STORED = {IOPORTA, IOPORTB, IOPORTC, IOPORTD};



/*void GPIO_Init_Pin(GPIO_TypeDef *GPIOx, int pad, uint32_t mode)
{
    uint32_t pos = 0x00 , currentpin = 0x00, pinpos = pad;


    pos = ((uint32_t)0x01) << pad;
  
    GPIOx->MODER  &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
    GPIOx->MODER |= (mode&STM32_MODE_MASK) << (pinpos * 2));

    if ((GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF))
    {
        GPIOx->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pinpos * 2));
        GPIOx->OSPEEDR |= ((mode&STM32_OSPEED_MASK)>>3) << (pinpos * 2));

        GPIOx->OTYPER  &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)pinpos)) ;
        GPIOx->OTYPER |= (uint16_t)(mode&STM32_OSPEED_MASK) << ((uint16_t)pinpos));
    }

    GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
    GPIOx->PUPDR |= (((uint32_t)GPIO_InitStruct->GPIO_PuPd) << (pinpos * 2));


}*/

int vhalInitGPIO(void *data) {
    (void)data;
    //Nothing to be done, we still depend on ChibiOS PAL driver
    memset(_vhalpinstatus, 0xff, sizeof(PinStatus)*_vhalpinnums);
    return 0;
}

const uint8_t _pinmodes[] STORED = {PAL_MODE_INPUT, PAL_MODE_INPUT, PAL_MODE_INPUT_PULLUP, PAL_MODE_INPUT_PULLDOWN, PAL_MODE_OUTPUT_PUSHPULL, PAL_MODE_OUTPUT_OPENDRAIN, PAL_MODE_OUTPUT_OPENDRAIN, PAL_MODE_INPUT_ANALOG};



/* prms:
    [0..7] --> cr register value [mode 0..1][cnf 2..3]
    [8..15] -->  shift needed for mask in AFIO_MAPR
    [16..23] --> mask for AFIO_MAPR
    [24..31] --> value for AFIO_MAPR

*/


int vhalPinSetToPeripheral(int vpin, int prph, uint32_t prms) {
    PIN_SET_TO_PRPH(vpin, prph);
    ioportid_t port = CPIN_PORT(vpin);
    int pad = PIN_PAD(vpin);
    int gpc = prms&0xff;
    int afshift = (prms&0xff00)>>8;
    int afmask = (prms&0xff0000)>>16;
    int afval = (prms&0xff000000)>>24;

    //set gpiomode
    if(pad>7){
        port->CRH = (port->CRH & (~(0xf<<4*(pad-8))))|(gpc<<4*(pad-8));
    } else {
        port->CRL = (port->CRL & (~(0xf<<4*pad)))|(gpc<<4*pad);
    }
    //set afio
    AFIO->MAPR = (AFIO->MAPR & (~(afmask<<afshift))) | (afval<<afshift);

    //palSetPadMode(CPIN_PORT(vpin), PIN_PAD(vpin), prms);
    return 0;
}


#define PIN_SET_TO_PRPH(vpin,prh) _vhalpinstatus[PIN_CLASS_ID(vpin)]=prh

int vhalPinSetMode(int vpin, int mode) {
    PIN_SET_TO_PRPH(vpin, 0xff);
    palSetPadMode(CPIN_PORT(vpin), PIN_PAD(vpin), _pinmodes[mode & 0x0f]);
    //palSetPadMode(IOPORTA, 8, PAL_MODE_OUTPUT_PUSHPULL);
    return 0;
}


int vhalPinRead(int vpin) {
    return palReadPad(CPIN_PORT(vpin), PIN_PAD(vpin));
}

int vhalPinWrite(int vpin, int value) {
    if (value)
        palSetPad(CPIN_PORT(vpin), PIN_PAD(vpin));
    else
        palClearPad(CPIN_PORT(vpin), PIN_PAD(vpin));
    return 0;
}

int vhalPinToggle(int vpin) {
    palTogglePad(CPIN_PORT(vpin), PIN_PAD(vpin));
    return 0;
}


void vhalPinFastSet(void *port, int pad) {
    palSetPad((GPIO_PORT)port, pad);
}

void vhalPinFastClear(void *port, int pad) {
    palClearPad((GPIO_PORT)port, pad);
}
int vhalPinFastRead(void *port, int pad) {
    return palReadPad((GPIO_PORT)port, pad);
}
void *vhalPinGetPort(int vpin) {
    return PIN_PORT(vpin);
}
int vhalPinGetPad(int vpin) {
    return PIN_PAD(vpin);
}

void iwdg_reset(void *data) {
    IWDG->KR = 0xAAAA;
}
