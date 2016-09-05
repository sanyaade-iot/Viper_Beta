#include "vosal.h"
#include "vhal.h"
#include "vbl.h"
#include "lang.h"


//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)


err_t _icu_ctl(int nargs, PObject *self, PObject **args, PObject **res) {
    (void)self;
    int32_t code;
    *res = MAKE_NONE();
    PARSE_PY_ARGCODE(code);

    switch (code) {
        case DRV_CMD_READ: {
            uint32_t time_window = 0;
            int32_t time_unit = 0;
            int32_t trigger = 0;
            int32_t samples = 0;
            int32_t pin;
            int32_t pull;

            if (parse_py_args("iiiiii", nargs, args, &pin, &trigger, &samples, &time_window, &time_unit,&pull) != 6) goto ret_err_type;
            if (samples < 0 || samples > 0xffff || time_unit < MICROS || time_unit > SECONDS || time_window < 0)
                goto ret_err_type;
            time_window = TIME_U(time_window, time_unit);
            uint32_t cfg = ICU_CFG(ICU_MODE_BOTH, (trigger) ? ICU_TRIGGER_HIGH : ICU_TRIGGER_LOW, 0,(pull) ? ICU_INPUT_PULLUP:ICU_INPUT_PULLDOWN);
            uint32_t bufsize = samples;
            uint32_t *buffer = (uint32_t *) gc_malloc(sizeof(uint32_t) * bufsize);

                //printf("Starting ICU...\n");
                RELEASE_GIL();
                code = vhalIcuStart(pin, cfg, time_window, buffer,&bufsize);
                ACQUIRE_GIL();
                //printf("Finished ICU...\n");
                if (code < 0) {
                    code = -code;
                    gc_free(buffer);
                    goto ret_err;
                } else {
                    PTuple *tpl = psequence_new(PTUPLE,bufsize);
                    for (trigger = 0; trigger < bufsize; trigger++) {
                        PTUPLE_SET_ITEM(tpl, trigger, PSMALLINT_NEW(buffer[trigger]));
                    }
                    *res = (PObject *)tpl;
                    gc_free(buffer);
                    goto ret_ok;
                }
        }
        break;
        default:
            goto ret_unsup;
    }
ret_ok:
    return ERR_OK;
ret_err_type:
    return ERR_TYPE_EXC;
ret_unsup:
    return ERR_UNSUPPORTED_EXC;
ret_err:
    return code;

}


const VBLDriver icudriver = {
    PRPH_ICU,
    _icu_ctl
};

err_t _vbl_icu_init(int nargs, PObject *self, PObject **args, PObject **res){
    *res = MAKE_NONE();
    vbl_install_driver(&icudriver);
    vhalInitICU(NULL);
    return ERR_OK;
}
