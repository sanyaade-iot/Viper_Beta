#if VHAL_PWM || VHAL_ICU || VHAL_HTM

#include "vhal.h"
#include "vhal_irqs.h"
#include "vhal_common.h"
#include "vhal_tim.h"
#include "stm32_timers.h"

//RCC
void rcc_enable_tim(int x);
void rcc_disable_tim(int x);



//RM rcc chapter, register APB1ENR and APB2ENR
void rcc_enable_tim(int x) {
  x++;
  if (x >= 2 && x <= 5) {
    //TIM2..5
    RCC->APB1ENR |= (1 << (x - 2));
  } else if (x == 1) {
    RCC->APB2ENR |= 1;
  } else if (x >= 9 && x <= 11) {
    //TIM9.11
    RCC->APB2ENR |= (1 << (16 + x - 9));
  }
}

void rcc_disable_tim(int x) {
  x++;
  if (x >= 2 && x <= 5) {
    //TIM2..5
    RCC->APB1ENR &= ~(1 << (x - 2));
  } else if (x == 1) {
    RCC->APB2ENR &= ~1;
  } else if (x >= 9 && x <= 11) {
    //TIM9.11
    RCC->APB2ENR &= ~(1 << (16 + x - 9));
  }
}


/* ========================================================================
    TIMER CONSTANTS
   ======================================================================== */



const uint8_t const tim_irqs[TIMNUM]  = {
  TIM1_UP_TIM10_IRQn,
  TIM2_IRQn,
  TIM3_IRQn,
  TIM4_IRQn,
  TIM5_IRQn,
  0,
  0,
  0, //disabled for the moment
  TIM1_BRK_TIM9_IRQn,
  TIM1_UP_TIM10_IRQn,
  TIM1_TRG_COM_TIM11_IRQn
};

TIM_TypeDef *const tim_l[TIMNUM]  = {TIM1, TIM2, TIM3, TIM4, TIM5, NULL, NULL, NULL, TIM9, TIM10, TIM11};


/* ========================================================================
    TIMER STATUS
   ======================================================================== */


//  0    1     2    3    4    5    6     7
// free     fn           ch1  ch2  ch3   ch4

#define TIM_HTM_ONE_SHOT  0x01
#define TIM_HTM_RECURRENT 0x03
#define TIM_STATUS_PWM    0x05
#define TIM_STATUS_ICU    0x07
#define TIM_CH(x)         (1<<(4+x))
#define TIMER_STATUS(tm) (tim_s[tm]&0x07)
#define TIMER_SET_STATUS(tm,x) tim_s[tm]=(x)
#define TIMER_STATUS_CH_ON(tm,x) tim_s[tm]|=TIM_CH(x)
#define TIMER_STATUS_CH_OFF(tm,x) tim_s[tm]&=~TIM_CH(x)
#define TIMER_STATUS_CH(tm) (tim_s[tm]&0xf0)
#define TIMER_STATUS_HAS_CH(tm,x) (tim_s[tm]&TIM_CH(x))
#define TIMER_STATUS_HAS_ONLY_CH(tm,x) (( (tim_s[tm]&0xf0)>>4 )==(1<<(x)))


#define  TIM_STATUS_USABLE                    0
#define  TIM_STATUS_MARKED                    2
#define  ICU_STATUS_USABLE                    0
#define  ICU_STATUS_RUNNING                   2

#define VP_TOTIM(X) (PIN_CLASS_DATA0(X) -1)
#define VP_TOCHA(X) (PIN_CLASS_DATA1(X) -1)
#define VP_TOAF(X)  (PIN_CLASS_DATA2(X))








#if VHAL_ICU || VHAL_HTM || VHAL_PWM


/* ========================================================================
    IRQ HANDLERS
   ======================================================================== */

extern void _timIrq_wrapper(uint32_t tm);

void vhalIRQ_TIM11(void) {
  vosEnterIsr();
  _timIrq_wrapper(10);
  vosExitIsr();
}

void vhalIRQ_TIM9(void) {
  vosEnterIsr();
  _timIrq_wrapper(8);
  vosExitIsr();
}

void vhalIRQ_TIM5(void) {
  vosEnterIsr();
  _timIrq_wrapper(4);
  vosExitIsr();
}
void vhalIRQ_TIM4(void) {
  vosEnterIsr();
  _timIrq_wrapper(3);
  vosExitIsr();
}
void vhalIRQ_TIM3(void) {
  vosEnterIsr();
  _timIrq_wrapper(2);
  vosExitIsr();
}
void vhalIRQ_TIM2(void) {
  vosEnterIsr();
  _timIrq_wrapper(1);
  vosExitIsr();
}
void vhalIRQ_TIM1(void) {
  //shared with TIM10
  vosEnterIsr();
  _timIrq_wrapper(0);
  _timIrq_wrapper(9);
  vosExitIsr();
}

void vhalIRQ_TIM1_CC(void) {
  vosEnterIsr();
  _timIrq_wrapper(0);
  vosExitIsr();
}



/* ========================================================================
    INITIALIZERS
   ======================================================================== */

extern uint8_t tim_s_initialized;
extern void init_timer_structs(void);

int vhalInitTIM(void *data) {
(void)data;
  if (!tim_s_initialized) {
    init_timer_structs();    
    vosInstallHandler(tim_irqs[0], vhalIRQ_TIM1);
    vosInstallHandler(TIM1_CC_IRQn, vhalIRQ_TIM1_CC);
    vosInstallHandler(tim_irqs[1], vhalIRQ_TIM2);
    vosInstallHandler(tim_irqs[2], vhalIRQ_TIM3);
    vosInstallHandler(tim_irqs[3], vhalIRQ_TIM4);
    vosInstallHandler(tim_irqs[4], vhalIRQ_TIM5);
    vosInstallHandler(tim_irqs[8], vhalIRQ_TIM9);
    vosInstallHandler(tim_irqs[10], vhalIRQ_TIM11);
    tim_s_initialized = 1;
  }
  return 0;
}



#endif

/* END PRIVATE fun*/




/* ========================================================================
    PWM
   ======================================================================== */

#if VHAL_PWM


const struct _pwmconfig const pwmch_reg[]  = {
  { ~(TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NP | TIM_CCER_CC1NE),
    ~(TIM_CCMR1_OC1M | TIM_CCMR1_CC1S | TIM_CCMR1_OC1PE),
    ~(TIM_CR2_OIS1 | TIM_CR2_OIS1N)
  },
  { ~(TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC2NP | TIM_CCER_CC2NE),
    ~(TIM_CCMR1_OC2M | TIM_CCMR1_CC2S | TIM_CCMR1_OC2PE),
    ~(TIM_CR2_OIS2 | TIM_CR2_OIS2N)
  },
  { ~(TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC3NP | TIM_CCER_CC3NE),
    ~(TIM_CCMR2_OC3M | TIM_CCMR2_CC3S | TIM_CCMR2_OC3PE),
    ~(TIM_CR2_OIS3 | TIM_CR2_OIS3N)
  },
  { ~(TIM_CCER_CC4E | TIM_CCER_CC4P),
    ~(TIM_CCMR2_OC4M | TIM_CCMR2_CC4S | TIM_CCMR2_OC4PE),
    ~(TIM_CR2_OIS4)
  },
};


#endif

/* ========================================================================
    ICU
   ======================================================================== */

#if VHAL_ICU


const struct _icuconfig const icuregs[]  = {
  {(uint16_t)~(TIM_CCER_CC1P | TIM_CCER_CC1NP), (uint16_t)~(TIM_CCMR1_CC1S | TIM_CCMR1_IC1F)},
  {(uint16_t)~(TIM_CCER_CC2P | TIM_CCER_CC2NP), (uint16_t)~(TIM_CCMR1_CC2S | TIM_CCMR1_IC2F)},
  {(uint16_t)~(TIM_CCER_CC3P | TIM_CCER_CC3NP), (uint16_t)~(TIM_CCMR2_CC3S | TIM_CCMR2_IC3F)},
  {(uint16_t)~(TIM_CCER_CC4P | TIM_CCER_CC4NP), (uint16_t)~(TIM_CCMR2_CC4S | TIM_CCMR2_IC4F)},
};
#endif



#else

#endif