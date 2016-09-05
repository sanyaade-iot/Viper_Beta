#include "port.h"
#include "vbl.h"
#include "lang.h"



#define NUM_PINS   50
const uint16_t _vhalpinnums = NUM_PINS;

#define PA PORT_A
#define PB PORT_B
#define PC PORT_C
#define PD PORT_D
#define PH PORT_H

PinStatus _vhalpinstatus[NUM_PINS];

#include "port.def"


VHAL_PORT_DECLARATIONS();


/* PERIPHERAL MAPS */

BEGIN_PERIPHERAL_MAP(serial) \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(6), \
PERIPHERAL_ID(1), \
END_PERIPHERAL_MAP(serial);


BEGIN_PERIPHERAL_MAP(spi) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
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
END_PERIPHERAL_MAP(pwm);


BEGIN_PERIPHERAL_MAP(icu) \
PERIPHERAL_ID(1), \
PERIPHERAL_ID(2), \
PERIPHERAL_ID(3), \
PERIPHERAL_ID(4), \
END_PERIPHERAL_MAP(icu);


BEGIN_PERIPHERAL_MAP(htm) \
PERIPHERAL_ID(5), \
PERIPHERAL_ID(8), \
PERIPHERAL_ID(9), \
PERIPHERAL_ID(10), \
PERIPHERAL_ID(11), \
END_PERIPHERAL_MAP(htm);



/* vbl layer */

const SerialPins const _vm_serial_pins[] STORED = {
    {RX0, TX0},
    {RX2, TX2},
    {RX1, TX2},
};


void *begin_bytecode_storage(int size) {
    uint8_t *cm = codemem;
    vhalFlashErase(cm, size);
    return cm;
}

void *bytecode_store(void *where, uint8_t *buf, uint16_t len) {
    uint32_t bb = len - len % 4;

    if (where < (void *)0x8005000 || ((((uint32_t)where)+len+4)>(uint32_t)&__flash_end__))
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



