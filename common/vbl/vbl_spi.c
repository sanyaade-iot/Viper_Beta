#include "vosal.h"
#include "vhal.h"
#include "vbl.h"
#include "lang.h"


//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)


#define _SPIDRIVER_START    0
#define _SPIDRIVER_STOP     1
#define _SPIDRIVER_SELECT   2
#define _SPIDRIVER_UNSELECT 3
#define _SPIDRIVER_LOCK     4
#define _SPIDRIVER_UNLOCK   5


#define _SPIDRIVER_READ     6
#define _SPIDRIVER_SKIP     7
#define _SPIDRIVER_WRITE    8
#define _SPIDRIVER_EXCHANGE 9

err_t _spi_ctl(int nargs, PObject *self, PObject **args, PObject **res) {
    (void)self;
    int32_t code;
    int32_t drvid;
    *res = MAKE_NONE();
    PARSE_PY_ARGCODE(code);
    PARSE_PY_INT(drvid); //number of vm spi prph: SPI0, SPI1, etcc...

    switch (code) {
        case _SPIDRIVER_START: {
            vhalSpiConf conf;
            uint32_t mode;
            uint32_t bits;
            uint32_t nss;
            if (parse_py_args("iiii", nargs, args, &conf.clock, &bits, &mode, &nss) != 4) goto ret_err_type;
            conf.mode = mode;
            conf.bits = bits;
            conf.master = 1;
            conf.nss = nss;
            printf("VBL_SPI_START asked: %i\n",drvid);
            //drvid = _vm_spi_pins[drvid].spiid; //number of vm spi driver: spi0,spi1...spiN
            conf.mosi = _vm_spi_pins[drvid].mosi;
            conf.miso = _vm_spi_pins[drvid].miso;
            conf.sclk = _vm_spi_pins[drvid].sclk;
            printf("VBL_SPI_START: #%i\n",_vm_spi_pins[drvid].spiid);
            code = vhalSpiInit(_vm_spi_pins[drvid].spiid, &conf);
            printf("VBL_SPI_START: %i\n",code);
            if (code < 0)
                return -code;
        }
        break;
        case _SPIDRIVER_STOP: {
            drvid = _vm_spi_pins[drvid].spiid; //number of spi driver: spi0,spi1...spiN
            code = vhalSpiDone(drvid);
            if (code < 0)
                return -code;
        }
        break;
        case _SPIDRIVER_LOCK: {
            drvid = _vm_spi_pins[drvid].spiid; //number of spi driver: spi0,spi1...spiN
            code = vhalSpiLock(drvid);
            if (code < 0)
                return -code;
        }
        break;
        case _SPIDRIVER_UNLOCK: {
            drvid = _vm_spi_pins[drvid].spiid; //number of spi driver: spi0,spi1...spiN
            code = vhalSpiUnlock(drvid);
            if (code < 0)
                return -code;
        }
        break;
        default:
        {
            drvid = _vm_spi_pins[drvid].spiid; //number of spi driver: spi0,spi1...spiN
            int32_t len;
            uint8_t *toread = NULL;
            uint8_t *tosend = NULL;
            if (code == _SPIDRIVER_READ) {
                PARSE_PY_INT(len);
                if (len < 0)
                    goto ret_err_type;
                *res = psequence_new(PBYTES, len);
                toread = PSEQUENCE_BYTES(*res);
            } else if (code == _SPIDRIVER_WRITE) {
                if (parse_py_args("s", nargs, args, &tosend, &len) != 1) goto ret_err_type;
            } else if (code == _SPIDRIVER_EXCHANGE) {
                if (parse_py_args("s", nargs, args, &tosend, &len) != 1) goto ret_err_type;
                *res = psequence_new(PBYTES, len);
                toread = PSEQUENCE_BYTES(*res);
            }
            //printf("VBL_%i >>%x  <<%x (%i)\n",code,tosend,toread,len);
            RELEASE_GIL();
            code = vhalSpiExchange(drvid, tosend, toread, len);
            ACQUIRE_GIL();
            //printf("VBL_ %i\n",code);
            if (code < 0)
                return -code;
        }
        break;
    }


    return ERR_OK;
ret_err_type:
    return ERR_TYPE_EXC;
ret_unsup:
    return ERR_UNSUPPORTED_EXC;

}


const VBLDriver spidriver = {
    PRPH_SPI,
    _spi_ctl
};



err_t _vbl_spi_init(int nargs, PObject *self, PObject **args, PObject **res) {
    *res = MAKE_NONE();
    vbl_install_driver(&spidriver);
    vhalInitSPI(NULL);
    return ERR_OK;
}


