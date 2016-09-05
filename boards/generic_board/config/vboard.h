#ifndef __VBOARD__
#define __VBOARD__

extern uint8_t __ramvectors__[];

#define CORTEX_FLASH_VTABLE 0x8000000
#define CORTEX_VTOR_INIT ((uint32_t)(&__ramvectors__))
#define CORTEX_VECTOR_COUNT	91

#endif