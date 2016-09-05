#include "vbl.h"
#include "port.h"
#include "lang.h"


#define NUM_PINS   77

#define PA PORT_A
#define PB PORT_B
#define PC PORT_C
#define PD PORT_D

PinStatus _vhalpinstatus[NUM_PINS];

#include "port.def"

VHAL_PORT_DECLARATIONS();


/* PERIPHERAL MAPS */

/*     USART0 USART1 USART2 USART3 UART */
/*code    1      2      3      4     5  */
/*map     1      2      4      3     0  */
BEGIN_PERIPHERAL_MAP(serial) \
PERIPHERAL_ID(5), \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(4), \
PERIPHERAL_ID(3), \
END_PERIPHERAL_MAP(serial);

const SerialPins const _vm_serial_pins[] STORED = {
    {RX0, TX0},
    {RX1, TX1},
    {RX2, TX2},
    {RX3, TX3},
    {RX4, TX4},
};


const SpiPins const _vm_spi_pins[] STORED = {
    {MOSI0, MISO0, SCLK0, 0, 0}
};

const SdioPins const _vm_sdio_pins[] STORED = {
};

BEGIN_PERIPHERAL_MAP(spi) \
PERIPHERAL_ID(1), \
END_PERIPHERAL_MAP(spi);

BEGIN_PERIPHERAL_MAP(adc) \
PERIPHERAL_ID(1), \
END_PERIPHERAL_MAP(adc);


BEGIN_PERIPHERAL_MAP(dac) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
END_PERIPHERAL_MAP(dac);

BEGIN_PERIPHERAL_MAP(can) \
PERIPHERAL_ID(1), \
END_PERIPHERAL_MAP(can);



BEGIN_PERIPHERAL_MAP(pwm) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(3), \
PERIPHERAL_ID(4), \
PERIPHERAL_ID(5), \
PERIPHERAL_ID(6), \
PERIPHERAL_ID(7), \
PERIPHERAL_ID(8), \
PERIPHERAL_ID(9), \
PERIPHERAL_ID(10), \
PERIPHERAL_ID(11), \
PERIPHERAL_ID(12), \
END_PERIPHERAL_MAP(pwm);


BEGIN_PERIPHERAL_MAP(icu) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(3), \
PERIPHERAL_ID(4), \
PERIPHERAL_ID(5), \
END_PERIPHERAL_MAP(icu);


BEGIN_PERIPHERAL_MAP(htm) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(3), \
PERIPHERAL_ID(4), \
PERIPHERAL_ID(5), \
PERIPHERAL_ID(6), \
PERIPHERAL_ID(7), \
PERIPHERAL_ID(8), \
PERIPHERAL_ID(9), \
END_PERIPHERAL_MAP(htm);

/* vbl layer */


void *begin_bytecode_storage(int size) {
    //address of the second bank
    if (codemem < 0xC0000)
        return (uint8_t *)0xC0000;
    else
        return codemem;
}

void *bytecode_store(void *where, uint8_t *buf, uint16_t len) {
    return flash_write_buffer((uint32_t *)where, buf, len);
}

void *end_bytecode_storage() {
    return 0;
}

void *vbl_get_adjusted_codemem(void *codemem) {
    if (codemem < 0xC0000)
        return (uint8_t *)0xC0000;
    else
        return codemem;
}