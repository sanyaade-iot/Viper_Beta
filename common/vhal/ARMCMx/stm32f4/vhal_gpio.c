#include "vhal.h"
#include "hal.h"
#include "vhal_gpio.h"



/* GPIO */

const PinPort _vhalpinports[] STORED = {IOPORTA, IOPORTB, IOPORTC, IOPORTD, IOPORTE, IOPORTF, IOPORTG, IOPORTH};



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

const uint8_t _pinmodes[] STORED = {PAL_MODE_INPUT, PAL_MODE_INPUT, PAL_MODE_INPUT_PULLUP, PAL_MODE_INPUT_PULLDOWN, PAL_MODE_OUTPUT_PUSHPULL|PAL_STM32_PUDR_PULLDOWN, PAL_MODE_OUTPUT_OPENDRAIN, PAL_MODE_OUTPUT_OPENDRAIN, PAL_MODE_INPUT_ANALOG};


int vhalPinSetToPeripheral(int vpin, int prph, uint32_t prms) {
    PIN_SET_TO_PRPH(vpin, prph);
    palSetPadMode(CPIN_PORT(vpin), PIN_PAD(vpin), prms);
    return 0;
}


extern void vbl_printf_stdout(uint8_t *fmt,...);

//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)

int vhalPinSetMode(int vpin, int mode) {
    PIN_SET_TO_PRPH(vpin, 0xff);
    printf("port: %x %x\n",CPIN_PORT(vpin),IOPORTB);
    printf("pad: %x %i\n",PIN_PAD(vpin),mode);
    palSetPadMode(CPIN_PORT(vpin), PIN_PAD(vpin),  _pinmodes[mode & 0x0f]);
    printf("regs: %x %x %x %x\n",CPIN_PORT(vpin)->MODER,CPIN_PORT(vpin)->AFRL,CPIN_PORT(vpin)->OTYPER,CPIN_PORT(vpin)->PUPDR);

    return 0;
}


int vhalPinRead(int vpin) {
    return palReadPad(CPIN_PORT(vpin), PIN_PAD(vpin));
}

int vhalPinWrite(int vpin, int value) {
    if (value){
        palSetPad(CPIN_PORT(vpin), PIN_PAD(vpin));
        //palSetPad(IOPORTB, 4);
        //printf("regs: %x %x\n",CPIN_PORT(vpin)->MODER,CPIN_PORT(vpin)->ODR);
    }
    else{
        palClearPad(CPIN_PORT(vpin), PIN_PAD(vpin));
        //palClearPad(IOPORTB, 4);
        //printf("regs: %x %x\n",CPIN_PORT(vpin)->MODER,CPIN_PORT(vpin)->ODR);
    }
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

