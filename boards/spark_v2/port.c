#include "port.h"
#include "vbl.h"
#include "lang.h"


/*
Core Pin out
============

  There are 24 pis on the Spark Core module.

  Spark     Spark Function                                         STM32F103CBT6
  Name      Pin #                           Pin #
  -------- ------ ------------------------------------------------ ---------------
   RAW     JP1-1  Input Power                                       N/A
   GND     JP1-2  GND
   A0     JP1-12  PA[00] WKUP/USART2_CTS/ADC12_IN0/TIM2_CH1_ETR     10
   A1     JP1-11  PA[01] USART2_RTS/ADC12_IN1/TIM2_CH2              11
   TX      JP1-3  PA[02] USART2_TX/ADC12_IN2/TIM2_CH3               12
   RX      JP1-4  PA[03] USART2_RX/ADC12_IN3/TIM2_CH4               13
   A2     JP1-10  PA[04] SPI1_NSS/USART2_CK/ADC12_IN4               14
   A3      JP1-9  PA[05] SPI1_SCK/ADC12_IN5                         15
   A4      JP1-8  PA[06] SPI1_MISO/ADC12_IN6/TIM3_CH1               16
   A5      JP1-7  PA[07] SPI1_MOSI/ADC12_IN7/TIM3_CH2               17
     LED2         PA[08] USART1_CK/TIM1_CH1/MCO                     29
     LED3         PA[09] USART1_TX/TIM1_CH2                         30
     LED4         PA[10] USART1_RX/TIM1_CH3                         31

     LED1,D7      PA[13] JTMS/SWDIO                                 34
   D7      JP2-5  PA[13] JTMS/SWDIO                                 34 Common with Blue LED LED_USR
   D6      JP2-6  PA[14] JTCK/SWCLK                                 37
   D5      JP2-7  PA[15] JTDI                                       38

  +3V3     JP2-1  V3.3 Out of Core                                  NA
   RST     JP2-2  NRST                                              7
   VDDA    JP2-3  ADC Voltage                                       9
   GND     JP2-4  GND

Core Internal IO
================

  Spark       Function                                          STM32F103CBT6
    Name                                    Pin #
  --------     ------------------------------------------------ ---------------
   A7      JP1-5  PB[01] ADC12_IN9/TIM3_CH4                         19
   A6      JP1-6  PB[00] ADC12_IN8/TIM3_CH3                         18
     BTN          PB[02] BOOT1                                      20
   D4      JP2-8  PB[03] JTDO                                       39
   D3      JP2-9  PB[04] NJTRST                                     40
   D2     JP2-10  PB[05] I2C1_SMBA                                  41
   D1     JP2-11  PB[06] I2C1_SCL/TIM4_CH1                          42
   D0     JP2-12  PB[07] I2C1_SDA/TIM4_CH2                          43
  WIFI_EN      PB[08] TIM4_CH3                                   45        CC3000 Module enable6
  MEM_CS       PB[09] TIM4_CH4                                   46       SST25VF016B Chip Select
  USB_DISC     PB[10] I2C2_SCL/USART3_TX                         21
  WIFI_INT     PB[11] I2C2_SDA/USART3_RX                         22        CC3000 Host interface SPI interrupt
  WIFI_CS      PB[12] SPI2_NSS/I2C2_SMBA/USART3_CK/TIM1_BKIN     25        CC3000 Chip Select
  SPI_CLK      PB[13] SPI2_SCK/USART3_CTS/TIM1_CH1N              26
  SPI_MISO     PB[14] SPI2_MISO/USART3_RTS/TIM1_CH2N             27
  SPI_MOSI     PB[15] SPI2_MOSI/TIM1_CH3N                        28

*/



#define NUM_PINS   32
const uint16_t _vhalpinnums = NUM_PINS;

#define PA PORT_A
#define PB PORT_B
#define PC PORT_C
#define PD PORT_D

PinStatus _vhalpinstatus[NUM_PINS];

#include "port.def"


VHAL_PORT_DECLARATIONS();


/* PERIPHERAL MAPS */

BEGIN_PERIPHERAL_MAP(serial) \
PERIPHERAL_ID(4), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(1), \
END_PERIPHERAL_MAP(serial);


BEGIN_PERIPHERAL_MAP(spi) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
END_PERIPHERAL_MAP(spi);

BEGIN_PERIPHERAL_MAP(adc) \
PERIPHERAL_ID(1), \
END_PERIPHERAL_MAP(adc);


BEGIN_PERIPHERAL_MAP(pwm) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(3), \
PERIPHERAL_ID(4), \
END_PERIPHERAL_MAP(pwm);


BEGIN_PERIPHERAL_MAP(icu) \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(3), \
PERIPHERAL_ID(4), \
END_PERIPHERAL_MAP(icu);


BEGIN_PERIPHERAL_MAP(htm) \
PERIPHERAL_ID(4), \
END_PERIPHERAL_MAP(htm);


/* vbl layer */

const SerialPins const _vm_serial_pins[] STORED = {
    {RX2, TX2},
    {RX1, TX1},
    {RX0, TX0},
};

const SpiPins const _vm_spi_pins[] STORED = {
    {MOSI0, MISO0,SCLK0,0,0},
    {MOSI1, MISO1,SCLK1,0,1},
    {MOSI2, MISO2,SCLK2,0,0},
};

//first channels, than clock, than cmd
const SdioPins const _vm_sdio_pins[] STORED = {
};


void *begin_bytecode_storage(int size) {
    
    uint8_t *cm = codemem;
    vhalFlashErase(cm, size);
    return cm;
}

void *bytecode_store(void *where, uint8_t *buf, uint16_t len) {
    
    uint32_t bb = len - len % 4;

    if (where < (void *)0x8000000 || ((((uint32_t)where)+len+4)>(uint32_t)&__flash_end__))
        return NULL;

    int ret = vhalFlashWrite(where, buf, bb);

    debug("bstored %i of %i\r\n", ret, bb);
    if ((uint32_t)ret != bb)
        return NULL;

    if (bb != len) {
        buf += bb;
        uint8_t bbuf[4];
        int i;
        for (i = 0; i < len % 4; i++) {
            bbuf[i] = buf[i];
        }
        ret = vhalFlashWrite( ((uint8_t *)where) + bb, bbuf, 4);
        if (ret != 4)
            return NULL;
        bb += 4;
    }

    return ((uint8_t *)where) + bb;
}

void *end_bytecode_storage() {
    return 0;
}

void *vbl_get_adjusted_codemem(void *codemem) {
    return vhalFlashAlignToSector(codemem);
}

/*   USB Layer   */


#if VHAL_CDC

#include "vhal_cdc.h"
#include "vhal_nfo.h"
/*
 * USB Device Descriptor.
 */
const uint8_t vcom_device_descriptor_data[18] = {
    USB_DESC_DEVICE       (0x0110,        /* bcdUSB (1.1).                    */
    0x02,          /* bDeviceClass (CDC).              */
    0x00,          /* bDeviceSubClass.                 */
    0x00,          /* bDeviceProtocol.                 */
    0x40,          /* bMaxPacketSize.                  */
    0x1d50,        /* idVendor (ST).                   */
    0x607D,        /* idProduct.                       */
    0x0200,        /* bcdDevice.                       */
    1,             /* iManufacturer.                   */
    2,             /* iProduct.                        */
    3,             /* iSerialNumber.                   */
    1)             /* bNumConfigurations.              */
};





/* Configuration Descriptor tree for a CDC.*/
const uint8_t vcom_configuration_descriptor_data[67] = {
    /* Configuration Descriptor.*/
    USB_DESC_CONFIGURATION(67,            /* wTotalLength.                    */
    0x02,          /* bNumInterfaces.                  */
    0x01,          /* bConfigurationValue.             */
    0,             /* iConfiguration.                  */
    0xC0,          /* bmAttributes (self powered).     */
    50),           /* bMaxPower (100mA).               */
    /* Interface Descriptor.*/
    USB_DESC_INTERFACE    (0x00,          /* bInterfaceNumber.                */
    0x00,          /* bAlternateSetting.               */
    0x01,          /* bNumEndpoints.                   */
    0x02,          /* bInterfaceClass (Communications
                                           Interface Class, CDC section
                                           4.2).                            */
    0x02,          /* bInterfaceSubClass (Abstract
                                         Control Model, CDC section 4.3).   */
    0x01,          /* bInterfaceProtocol (AT commands,
                                           CDC section 4.4).                */
    0),            /* iInterface.                      */
    /* Header Functional Descriptor (CDC section 5.2.3).*/
    USB_DESC_BYTE         (5),            /* bLength.                         */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x00),         /* bDescriptorSubtype (Header
                                           Functional Descriptor.           */
    USB_DESC_BCD          (0x0110),       /* bcdCDC.                          */
    /* Call Management Functional Descriptor. */
    USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x01),         /* bDescriptorSubtype (Call Management
                                           Functional Descriptor).          */
    USB_DESC_BYTE         (0x00),         /* bmCapabilities (D0+D1).          */
    USB_DESC_BYTE         (0x01),         /* bDataInterface.                  */
    /* ACM Functional Descriptor.*/
    USB_DESC_BYTE         (4),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x02),         /* bDescriptorSubtype (Abstract
                                           Control Management Descriptor).  */
    USB_DESC_BYTE         (0x02),         /* bmCapabilities.                  */
    /* Union Functional Descriptor.*/
    USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x06),         /* bDescriptorSubtype (Union
                                           Functional Descriptor).          */
    USB_DESC_BYTE         (0x00),         /* bMasterInterface (Communication
                                           Class Interface).                */
    USB_DESC_BYTE         (0x01),         /* bSlaveInterface0 (Data Class
                                           Interface).                      */
    /* Endpoint 2 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD1_INTERRUPT_REQUEST_EP | 0x80,
    0x03,          /* bmAttributes (Interrupt).        */
    0x0008,        /* wMaxPacketSize.                  */
    0xFF),         /* bInterval.                       */
    /* Interface Descriptor.*/
    USB_DESC_INTERFACE    (0x01,          /* bInterfaceNumber.                */
    0x00,          /* bAlternateSetting.               */
    0x02,          /* bNumEndpoints.                   */
    0x0A,          /* bInterfaceClass (Data Class
                                           Interface, CDC section 4.5).     */
    0x00,          /* bInterfaceSubClass (CDC section
                                           4.6).                            */
    0x00,          /* bInterfaceProtocol (CDC section
                                           4.7).                            */
    0x00),         /* iInterface.                      */
    /* Endpoint 3 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD1_DATA_AVAILABLE_EP,       /* bEndpointAddress.*/
    0x02,          /* bmAttributes (Bulk).             */
    0x0040,        /* wMaxPacketSize.                  */
    0x00),         /* bInterval.                       */
    /* Endpoint 1 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD1_DATA_REQUEST_EP | 0x80,  /* bEndpointAddress.*/
    0x02,          /* bmAttributes (Bulk).             */
    0x0040,        /* wMaxPacketSize.                  */
    0x00)          /* bInterval.                       */
};


/*
 * U.S. English language identifier.
 */
const uint8_t vcom_string0[] = {
    USB_DESC_BYTE(4),                     /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

/*
 * Vendor string.
 */
static const uint8_t vcom_string1[] = {
  USB_DESC_BYTE(18),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'P', 0, 'a', 0, 'r', 0, 't', 0, 'i', 0, 'c', 0, 'l', 0, 'e', 0
};

/*
 * Device Description string.
 */
static const uint8_t vcom_string2[] = {
  USB_DESC_BYTE(56),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'P', 0, 'a', 0, 'r', 0, 't', 0, 'i', 0, 'c', 0, 'l', 0, 'e', 0,
  ' ', 0, 'C', 0, 'o', 0, 'r', 0, 'e', 0, ' ', 0, 'V', 0, 'i', 0,
  'p', 0, 'e', 0, 'r', 0, 'i', 0, 'z', 0, 'e', 0, 'd', 0, ' ', 0,
  ' ', 0, ' ', 0, ' ', 0
};


/*
 * Serial Number string: filled in cdc with uid
 */
uint8_t vcom_string3[UID_BYTES*4+2];


/*
 * Configuration Descriptor wrapper.
 */
const USBDescriptor vcom_configuration_descriptor = {
    sizeof vcom_configuration_descriptor_data,
    vcom_configuration_descriptor_data
};

/*
 * Strings wrappers array.
 */
const USBDescriptor vcom_strings[] = {
    {sizeof vcom_string0, vcom_string0},
    {sizeof vcom_string1, vcom_string1},
    {sizeof vcom_string2, vcom_string2},
    {sizeof vcom_string3, vcom_string3}
};

/*
 * Device Descriptor wrapper.
 */
const USBDescriptor vcom_device_descriptor = {
    sizeof vcom_device_descriptor_data,
    vcom_device_descriptor_data
};

#endif
