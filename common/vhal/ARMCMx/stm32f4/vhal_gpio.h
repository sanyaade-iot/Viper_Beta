#ifndef __VHAL_GPIO__
#define __VHAL_GPIO__


#define GPIO_PORT ioportid_t
#define CPIN_PORT(vpin) ((GPIO_PORT)PIN_PORT(vpin))

#define vPinSetModeEx(port,pad,mode) palSetPadMode(port,pad,mode)
#define vPinSetMode(port,pad,mode) palSetPadMode(port,pad,_pinmodes[mode])
#define vPinRead(port,pad) palReadPad(port,pad)
#define vPinWrite(port,pad,val) palWritePad(port,pad,val)


#define STM32_MODE_MASK             (0x0003)
#define STM32_MODE_INPUT            (0 << 0)
#define STM32_MODE_OUTPUT           (1 << 0)
#define STM32_MODE_ANALOG           (3 << 0)
#define STM32_MODE_ALTERNATE        (2 << 0)

#define STM32_OTYPE_MASK            (0x0004)
#define STM32_OTYPE_PUSHPULL        (0 << 2)
#define STM32_OTYPE_OPENDRAIN       (1 << 2)

#define STM32_OSPEED_MASK           (0x0008)
#define STM32_OSPEED_LOWEST         (0 << 3)
#define STM32_OSPEED_MID            (1 << 3)
#define STM32_OSPEED_MID2           (2 << 3)
#define STM32_OSPEED_HIGHEST        (3 << 3)

#define STM32_PUDR_MASK             (0x0030)
#define STM32_PUDR_NOPULL           (0 << 5)
#define STM32_PUDR_PULLUP           (1 << 5)
#define STM32_PUDR_PULLDOWN         (2 << 5)

#define STM32_ALTERNATE_MASK        (0xff80)
#define STM32_ALTERNATE(n)          ((n) << 7)

#define ALTERNATE_FN(n) (STM32_MODE_ALTERNATE | STM32_ALTERNATE(n))

extern const uint8_t _pinmodes[];


//RM page 282 and following
#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_ANALOG 3
#define GPIO_MODE_ALTERNATE 2

#define GPIO_OTYPE_PUSHPULL	0
#define GPIO_OTYPE_OPENDRAIN	1


#endif
