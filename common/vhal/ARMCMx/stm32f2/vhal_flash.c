#include "vhal.h"

#ifdef STM32F2XX
#include "stm32f2xx_flash.h"
#else
#include "stm32f4xx_flash.h"
#endif




#if defined STM32F2XX
//Flash Sector Layout for F205xx
const uint32_t const _fl_sec_addr[] STORED = {
    0x8000000,
    0x8004000,//16k
    0x8008000,//16k
    0x800C000,//16k
    0x8010000,//16k
#if defined STM32F2xxxB
    0x8020000,//64k
#endif
#if defined STM32F2xxxC
    0x8020000,//64k
    0x8040000,//128k
#endif
#if defined STM32F2xxxE
    0x8020000,//64k
    0x8040000,//128k
    0x8060000,//128k
    0x8080000,//128k
#endif
#if defined STM32F2xxxF
    0x8020000,//64k
    0x8040000,//128k
    0x8060000,//128k
    0x8080000,//128k
    0x80A0000,//128k
    0x80C0000,//128k
#endif
#if defined STM32F2xxxG
    0x8020000,//64k
    0x8040000,//128k
    0x8060000,//128k
    0x8080000,//128k
    0x80A0000,//128k
    0x80C0000,//128k
    0x80E0000,//128k
    0x8100000,//128k
#endif
};
#endif

int vhalFlashGetSector(void *addr) {
    uint32_t a = (uint32_t)addr;
    uint32_t i;
    for (i = 0; i < sizeof(_fl_sec_addr) / sizeof(uint32_t) - 1; i++) {
        if (a >= _fl_sec_addr[i] && a < _fl_sec_addr[i + 1]) {
            return 0x08 * i; //0x10 * (i / 2) + 0x08 * (i % 2); //see FLASH_Sector_N in flash.h
        }
    }
    return -1;
}


int vhalFlashErase(void *addr, uint32_t size) {
    uint32_t uwStartSector = vhalFlashGetSector(addr);
    uint32_t uwEndSector = vhalFlashGetSector( ((uint8_t *)addr) + size);
    FLASH_Status sta = FLASH_COMPLETE;
    uint32_t obwrp = 0;
    uint32_t i;
    for (i = uwStartSector; i <= uwEndSector; i += 0x08) {
        obwrp |= (1 << (i / 0x08));
    }
    FLASH_OB_Unlock();
    FLASH_OB_WRPConfig(obwrp, 0);
    sta = FLASH_OB_Launch();
    FLASH_OB_Lock();
    if (sta != FLASH_COMPLETE)
        return 1;


    /* Get the number of the start and end sectors */
    for (i = uwStartSector; i <= uwEndSector; i += 0x08) { //see FLASH_Sector_N in flash.h
        iwdg_reset(NULL);
        FLASH_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR |
                        FLASH_FLAG_PGSERR);
        sta = (FLASH_EraseSector(i, VoltageRange_2));
        FLASH_Lock();
        if (sta != FLASH_COMPLETE)
            break;
        //debug("Erasing sector %x -> %x = %x\r\n",uwStartSector,uwEndSector,i);
    }
    return sta != FLASH_COMPLETE;
}

int vhalFlashWrite(void *addr, uint8_t *data, uint32_t len) {
    int ret = 0;
    uint32_t i;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR |
                    FLASH_FLAG_PGSERR);
    for (i = 0; i < len; i += 4) {
        uint16_t dd1 = (uint16_t)(data[i] | (data[i + 1] << 8));
        uint16_t dd2 = (uint16_t)((data[i + 2]) | (data[i + 3] << 8));
        /*debug("writing @%i %i @%i %i\r\n",((uint32_t)addr) + i,dd1,((uint32_t)addr) + i+2,dd2);
        ret+=4;*/
        FLASH_Status st = FLASH_ProgramHalfWord(((uint32_t)addr) + i, dd1);
        if (st != FLASH_COMPLETE) {
            ret = st;
            break;
        }
        ret += (st == FLASH_COMPLETE) ? 2 : 0;
        st = FLASH_ProgramHalfWord(((uint32_t)addr) + 2 + i, dd2);
        if (st != FLASH_COMPLETE) {
            ret = st;
            break;
        }
        ret += (st == FLASH_COMPLETE) ? 2 : 0;
    }
    FLASH_Lock();
    return ret;
}

void *vhalFlashAlignToSector(void *addr) {
    uint32_t i;
    for (i = 0; i < sizeof(_fl_sec_addr) / sizeof(uint32_t); i++) {
        if ( ((uint32_t)addr) < _fl_sec_addr[i]) {
            return (void *)_fl_sec_addr[i];
        }
    }
    return NULL;
}
