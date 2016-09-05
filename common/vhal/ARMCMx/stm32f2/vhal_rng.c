#include "vhal.h"



#if BOARD_HAS_RNG==1
#ifdef STM32F2XX

#include "vhal_common.h"

typedef struct 
{
  __IO uint32_t CR;  /*!< RNG control register, Address offset: 0x00 */
  __IO uint32_t SR;  /*!< RNG status register,  Address offset: 0x04 */
  __IO uint32_t DR;  /*!< RNG data register,    Address offset: 0x08 */
} RNG_TypeDef;

#define RNG_BASE              (AHB2PERIPH_BASE + 0x60800)
#define RNG                 ((RNG_TypeDef *) RNG_BASE)
/********************  Bits definition for RNG_CR register  *******************/
#define RNG_CR_RNGEN                         ((uint32_t)0x00000004)
#define RNG_CR_IE                            ((uint32_t)0x00000008)

/********************  Bits definition for RNG_SR register  *******************/
#define RNG_SR_DRDY                          ((uint32_t)0x00000001)
#define RNG_SR_CECS                          ((uint32_t)0x00000002)
#define RNG_SR_SECS                          ((uint32_t)0x00000004)
#define RNG_SR_CEIS                          ((uint32_t)0x00000020)
#define RNG_SR_SEIS                          ((uint32_t)0x00000040)


#define  RCC_AHB2ENR_RNGEN                   ((uint32_t)0x00000040)

#else
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"
#endif

uint32_t vhalRngGenerate(void) {
    while(!RNG->SR);
    if (RNG->SR & RNG_SR_DRDY){
        return RNG->DR;
    }
    RNG->SR=0; //clear errors
    return 0;
}


int vhalRngSeed(uint32_t seed) {
    return 0;
}

int vhalInitRNG(void*data){
    (void)data;
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    RNG->CR=0;
    RNG->CR |= RNG_CR_RNGEN;
    vhalRngGenerate();
    return 0;
}


#else


int vhalInitRNG(void*data){
	//seed by uid
	(void)data;
    vhalRngSeed(*((uint32_t *)0x1FFF7A10));
    return 0;
}

#endif

