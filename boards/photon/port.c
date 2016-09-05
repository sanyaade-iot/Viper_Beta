#include "port.h"
#include "vbl.h"
#include "lang.h"



#define NUM_PINS   33
const uint16_t _vhalpinnums = NUM_PINS;

#define PA PORT_A
#define PB PORT_B
#define PC PORT_C
#define PD PORT_D
#define PE PORT_E
#define PF PORT_F
#define PG PORT_G
#define PH PORT_H

PinStatus _vhalpinstatus[NUM_PINS];

#include "port.def"


VHAL_PORT_DECLARATIONS();


/* PERIPHERAL MAPS */

BEGIN_PERIPHERAL_MAP(serial) \
PERIPHERAL_ID(7), \
PERIPHERAL_ID(1), \
END_PERIPHERAL_MAP(serial);


BEGIN_PERIPHERAL_MAP(spi) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(3), \
END_PERIPHERAL_MAP(spi);

BEGIN_PERIPHERAL_MAP(adc) \
PERIPHERAL_ID(1), \
END_PERIPHERAL_MAP(adc);


BEGIN_PERIPHERAL_MAP(pwm) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(3), \
PERIPHERAL_ID(4), \
PERIPHERAL_ID(5), \
END_PERIPHERAL_MAP(pwm);


BEGIN_PERIPHERAL_MAP(icu) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(3), \
PERIPHERAL_ID(4), \
PERIPHERAL_ID(5), \
END_PERIPHERAL_MAP(icu);


BEGIN_PERIPHERAL_MAP(htm) \
PERIPHERAL_ID(6), \
PERIPHERAL_ID(7), \
PERIPHERAL_ID(9), \
PERIPHERAL_ID(11), \
END_PERIPHERAL_MAP(htm);



/* vbl layer */

const SerialPins const _vm_serial_pins[] STORED = {
    {RX0, TX0},
    {RX1, TX1},
};

const SpiPins const _vm_spi_pins[] STORED = {
    {MOSI0, MISO0, SCLK0, 0, 2},
    {MOSI1, MISO1, SCLK1, 0, 0},
};

//first channels, than clock, than cmd
const SdioPins const _vm_sdio_pins[] STORED = {
    {
        {D22,D23,D24,D25,D22,D23,D24,D25,D20,D21}
    }
};


void init_sleep_micro(){

}

void *begin_bytecode_storage(int size) {
    uint8_t *cm = codemem;
    if (vhalFlashErase(cm, size))
        return NULL;
    return cm;
}

void *bytecode_store(void *where, uint8_t *buf, uint16_t len) {
    uint32_t bb = len - len % 4;

    if (where < (void *)0x8020000 || ((((uint32_t)where)+len+4)>(uint32_t)&__flash_end__))
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



/* PHOTON module: bootloader checks that this struct is just after the vector table with platform=6*/

typedef struct module_dependency_t {
    uint8_t module_function;        // module function, lowest 4 bits
    uint8_t module_index;           // moudle index, lowest 4 bits.
    uint16_t module_version;        // version/release number of the module.
} module_dependency_t;

typedef struct module_info_t {
    const void* module_start_address;   /* the first byte of this module in flash */
    const void* module_end_address;     /* the last byte (exclusive) of this smodule in flash. 4 byte crc starts here. */
    uint8_t reserved;
    uint8_t reserved2;
    uint16_t module_version;            /* 16 bit version */
    uint16_t platform_id;               /* The platform this module was compiled for. */
    uint8_t  module_function;           /* The module function */
    uint8_t  module_index;
    module_dependency_t dependency;
    uint32_t reserved3;
} module_info_t;


__attribute__ ((section("photon_module_info")))
module_info_t _photon_module_info = {
    (void*)0x8020000,
    (void*)0x8020000,
    0,0,0,6,0,0,{0,0,0},0
};

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
    0x2b04,        /* idVendor (ST).                   */
    0xc006,        /* idProduct.                       */
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
 /*
static const uint8_t vcom_string2[] = {
  USB_DESC_BYTE(56),                   
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING),
  'P', 0, 'a', 0, 'r', 0, 't', 0, 'i', 0, 'c', 0, 'l', 0, 'e', 0,
  ' ', 0, 'P', 0, 'h', 0, 'o', 0, 't', 0,'o', 0,'n', 0, ' ', 0, 'V', 0, 'i', 0,
  'p', 0, 'e', 0, 'r', 0, 'i', 0, 'z', 0, 'e', 0, 'd', 0, ' ', 0,
  ' ', 0
};
*/

static const uint8_t vcom_string2[] = {
  USB_DESC_BYTE(60),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'P', 0, 'a', 0, 'r', 0, 't', 0, 'i', 0, 'c', 0, 'l', 0, 'e', 0,
  ' ', 0, 'P', 0, 'h', 0, 'o', 0, 't', 0, 'o', 0, 'n', 0, ' ', 0, 'V', 0, 'i', 0,
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
