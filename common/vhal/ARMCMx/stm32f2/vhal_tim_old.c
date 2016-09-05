#if VHAL_PWM || VHAL_ICU || VHAL_HTM

#include "vhal.h"
#include "vhal_gpio.h"
#include "vhal_tim.h"
#include "vhal_irqs.h"
#include "vhal_common.h"



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

#define TIM(x) tim_l[x]
#define TIM_IRQ(x) tim_irqs[x]

uint8_t tim_s_initialized = 0;
uint8_t tim_irq_initialized = 0;
uint8_t tim_s[TIMNUM];
void _timICUIrq(uint8_t tm, uint32_t sr);


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



//extern void testdebug(const char *fmt, ...);
//#define printf(...) testdebug(__VA_ARGS__)
//#define printf(...)

//extern void vbl_printf_stdout(uint8_t *fmt, ...);
//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)
//#define printf(...) vbl_printf_stdout(__VA_ARGS__)



typedef struct _htm_struct{
  htmFn fn;
  void *args;
} HTM_TypeDef;


typedef struct _icu_struct {
  uint32_t time_window;
  uint32_t cfg;
  uint32_t *buffer;
  uint32_t *bufsize;
  uint16_t count;
  uint16_t last;
  uint16_t updated;
  uint8_t started;
  uint8_t pad;
  uint32_t has_capture;
  uint32_t start_capture;
  GPIO_TypeDef *port;
  VThread thread;
} ICU_TypeDef;

typedef struct _pwm_struct {
  VThread *thread;
  uint32_t npulses;
  uint16_t vpin;
} PWM_TypeDef;



/* ========================================================================
    TIMER HANDLING
   ======================================================================== */



void _timFree_timer(uint8_t tm) {
  rcc_disable_tim(tm);
  vhalIrqDisable(TIM_IRQ(tm));
  if (TIM(tm) == TIM1) {
    vhalIrqDisable(TIM1_CC_IRQn);
  } /*else if (TIM(tm)==TIM8){
    vhalIrqDisable(TIM8_CC_IRQn);
  }*/
  TIMER_SET_STATUS(tm, TIM_STATUS_USABLE);
}


void _timFree_channel(uint8_t tm, uint8_t ch) {
  TIMER_STATUS_CH_OFF(tm, ch);
  if (!TIMER_STATUS_CH(tm)) {
    _timFree_timer(tm);
  }
}


void _timReserveAndInit(uint32_t tm, uint8_t status) {
  rcc_enable_tim(tm);
  TIM(tm)->SR = (uint16_t)~TIM_IT_Update;
  TIMER_SET_STATUS(tm, status);
}

#define TICKS_PER_MUSEC ((_system_frequency)/1000000)

int _timeCalcRegisters(uint32_t time, uint32_t *psc, uint32_t *prd, uint32_t desired_psc) {
  uint32_t ticks = GET_TIME_MICROS(time) * TICKS_PER_MUSEC;
  *psc = desired_psc;
  *prd = ticks / (*psc + 1);

  if (*prd <= 0xffff) {
    return 1;
  }
  *psc = ticks / (65536); //minimum required psc
  *prd = ticks / (*psc + 1); //corresponding prd
  if (*prd <= 0xffff)
    return 0;
  return VHAL_GENERIC_ERROR;
}


void _set_timer_base(TIM_TypeDef *timx, uint16_t TIM_Prescaler, uint32_t TIM_Period, uint8_t TIM_RepetitionCounter) {
  uint16_t tmpcr1 = 0;

  tmpcr1 = timx->CR1;



  if ((timx == TIM1) || (timx == TIM8) ||
      (timx == TIM2) || (timx == TIM3) ||
      (timx == TIM4) || (timx == TIM5)) {
    // Select the Counter Mode
    tmpcr1 &= (uint16_t)(~(TIM_CR1_DIR | TIM_CR1_CMS));
    tmpcr1 |= (uint32_t)TIM_CounterMode_Up;
  }

  if ((timx != TIM6) && (timx != TIM7)) {
    // Set the clock division 
    tmpcr1 &=  (uint16_t)(~TIM_CR1_CKD);
    //set to 0
    //tmpcr1 |= (uint32_t)TIM_ClockDivision;
  }


  timx->CR1 = tmpcr1;

  // Set the Autoreload value 
  timx->ARR = TIM_Period ;

  // Set the Prescaler value 
  timx->PSC = TIM_Prescaler;

  if ((timx == TIM1) || (timx == TIM8)) {
    // Set the Repetition Counter value
    timx->RCR = TIM_RepetitionCounter;
  }

  //Generate an update event to reload the Prescaler
  //and the repetition counter(only for TIM1 and TIM8) value immediately 
  timx->CR1|=TIM_CR1_UDIS;
  timx->EGR = TIM_PSCReloadMode_Immediate;
  timx->CR1&=~TIM_CR1_UDIS;
}




#if VHAL_ICU || VHAL_HTM || VHAL_PWM


/* ========================================================================
    IRQ HANDLERS
   ======================================================================== */

#define vhalIrqClearPending(IRQn) NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F))


void _timIrq_wrapper(uint8_t tm);

void vhalIRQ_TIM11(void) {
  _timIrq_wrapper(10);
}

void vhalIRQ_TIM9(void) {
  _timIrq_wrapper(8);
}

void vhalIRQ_TIM5(void) {
  _timIrq_wrapper(4);
}
void vhalIRQ_TIM4(void) {
  _timIrq_wrapper(3);
}
void vhalIRQ_TIM3(void) {
  _timIrq_wrapper(2);
}
void vhalIRQ_TIM2(void) {
  _timIrq_wrapper(1);
}
void vhalIRQ_TIM1(void) {
  //shared with TIM10
  _timIrq_wrapper(0);
  _timIrq_wrapper(9);
}

void vhalIRQ_TIM1_CC(void) {
  _timIrq_wrapper(0);
}

/*
void vhalIrq_TIM8_CC(void){
  _timIrq_wrapper(0);
}
*/



#define SR_CAPTURE (TIM_SR_CC1IF|TIM_SR_CC2IF|TIM_SR_CC3IF|TIM_SR_CC4IF)
#define SR_UPDATE (TIM_SR_UIF)

volatile int icuflag = 0;
void _timIrq_wrapper(uint8_t tm) {
  //get status masked
  uint32_t sr = TIM(tm)->SR & TIM(tm)->DIER;

  vosSysLockIsr();


#if VHAL_PWM
if (TIMER_STATUS(tm) == TIM_STATUS_PWM  && (sr & (SR_UPDATE | SR_CAPTURE))) {
    _timPWMIrq(tm, sr);
  }
#endif
#if VHAL_ICU
#if VHAL_PWM
  else
#endif
  if (TIMER_STATUS(tm) == TIM_STATUS_ICU  && (sr & (SR_UPDATE | SR_CAPTURE))) {
    _timICUIrq(tm, sr);
  }
#endif
#if VHAL_HTM
#if VHAL_ICU || VHAL_PWM
  else
#endif
    if (sr & SR_UPDATE) {
      if (TIMER_STATUS(tm) == TIM_HTM_ONE_SHOT || TIMER_STATUS(tm) == TIM_HTM_RECURRENT) {
        if (*tim_htm[tm].fn) {
          vosSysUnlockIsr();
          (*tim_htm[tm].fn)(tm, tim_htm[tm].args);  /* execute callback */
          vosSysLockIsr();
        }
        if (TIMER_STATUS(tm) == TIM_HTM_ONE_SHOT) { /* free TIMx */
          _timFree_timer(tm);
        }
      }
    }
#endif
  TIM(tm)->SR = ~sr;
  vosSysUnlockIsr();
}


/* ========================================================================
    INITIALIZERS
   ======================================================================== */


#if VHAL_ICU
ICU_TypeDef *tim_icu[TIMNUM][4];
VSemaphore icu_sem;
int vhalInitICU(void *data) {
  return vhalInitTIM(data); 
}
#endif

#if VHAL_HTM
HTM_TypeDef tim_htm[TIMNUM];
int vhalInitHTM(void *data) {
  return vhalInitTIM(data);
}
#endif


#if VHAL_PWM
PWM_TypeDef tim_pwm[TIMNUM][4];
int vhalInitPWM(void *data) {
  return vhalInitTIM(data);
}
#endif




int vhalInitTIM(void *data) {
(void)data;
  if (!tim_s_initialized) {
    memset(tim_s, TIM_STATUS_USABLE, sizeof(tim_s));
    #if VHAL_ICU
    icu_sem = vosSemCreate(1);
    memset(tim_icu,0,sizeof(tim_icu));
    #endif
    #if VHAL_PWM
    memset(tim_pwm,0,sizeof(tim_pwm));
    #endif
    #if VHAL_HTM
    memset(tim_htm,0,sizeof(tim_htm));
    #endif
    tim_s_initialized = 1;
  }
  if (!tim_irq_initialized) {
    vosInstallHandler(tim_irqs[0], vhalIRQ_TIM1);
    vosInstallHandler(TIM1_CC_IRQn, vhalIRQ_TIM1_CC);
    vosInstallHandler(tim_irqs[1], vhalIRQ_TIM2);
    vosInstallHandler(tim_irqs[2], vhalIRQ_TIM3);
    vosInstallHandler(tim_irqs[3], vhalIRQ_TIM4);
    vosInstallHandler(tim_irqs[4], vhalIRQ_TIM5);
    vosInstallHandler(tim_irqs[8], vhalIRQ_TIM9);
    vosInstallHandler(tim_irqs[10], vhalIRQ_TIM11);
    tim_irq_initialized = 1;
  }
  
  return 0;
}


#endif

/* END PRIVATE fun*/




/* ========================================================================
    PWM
   ======================================================================== */

#if VHAL_PWM



struct _pwmconfig {
  uint16_t ccer;
  uint16_t ccmr;
  uint16_t cr2;
};


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

void _timOCxInit(uint8_t ch, TIM_TypeDef *timx, uint32_t period) {
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
  tmpccer = timx->CCER;
  tmpcr2 =  timx->CR2;
  tmpccmrx = (ch < 2) ? timx->CCMR1 : timx->CCMR2;

  tmpccer &= pwmch_reg[ch].ccer;
  tmpccmrx &= pwmch_reg[ch].ccmr;
  tmpccer |= (TIM_OutputNState_Enable | TIM_OutputState_Enable) << (4 * ch); //can be or'ed with OCPOLARITY
  tmpccmrx |= (TIM_OCMode_PWM1 /*| TIM_OCPreload_Enable*/) << (8 * (ch % 2));
  timx->CCR[ch] = period;

  if ((timx == TIM1) || (timx == TIM8)) {
    tmpcr2 &= pwmch_reg[ch].cr2;
    tmpcr2 |= (TIM_OCIdleState_Set) << (2 * ch);
  }

  timx->CR2 = tmpcr2;
  if (ch < 2)
    timx->CCMR1 = tmpccmrx;
  else
    timx->CCMR2 = tmpccmrx;
  timx->CCER = tmpccer;
  //timx->EGR |= ((uint8_t)0x01);
}



int vhalPwmStart(int vpin, uint32_t period, uint32_t pulse, uint32_t npulses) {
  
  if (!PIN_HAS_PRPH(vpin, PRPH_PWM)) {
    printf("NOT A PWM VPIN\n");
    return VHAL_INVALID_PIN;
  }

  uint32_t tm = VP_TOTIM(vpin);
  uint32_t ch = VP_TOCHA (vpin);
  TIM_TypeDef *timx = TIM(tm);

  if ((TIME_IS_ZERO(period) || TIME_IS_ZERO(pulse) || period < pulse)) {
    vhalPinSetMode(vpin, PINMODE_OUTPUT_PUSHPULL);
    printf("Disabling\n");
    if (TIMER_STATUS_HAS_CH(tm, ch)) {
      timx->CCER &= ~(TIM_CCER_CC1E << (4 * ch)); //disable channel
      _timFree_channel(tm, ch);
    }
    return VHAL_OK;
  }


  if (!(TIMER_STATUS(tm) == TIM_STATUS_PWM  || TIMER_STATUS(tm) == TIM_STATUS_USABLE)
      || !PIN_FREE_OR_ASSIGNED_TO(vpin, PRPH_PWM)) {
    printf("NOT USABLE AS PWM %x %i\n", vpin, TIMER_STATUS(tm));
    return VHAL_INVALID_PIN;
  }


  uint32_t tim_period = 0;
  uint32_t pwm_period = 0;
  uint32_t prescaler = 0;
  _timeCalcRegisters(period, &prescaler, &tim_period, 0);
  _timeCalcRegisters(pulse, &prescaler, &pwm_period, prescaler);
  //the prescaler is the same because pwm_period is pulse_ticks/(prescaler+1)+1
  //printf("PSC %i, PRD %i\n",prescaler,tim_period);

  if (!(TIMER_STATUS_HAS_ONLY_CH(tm, ch))) {
    if (TIMER_STATUS_CH(tm) && timx->ARR != tim_period) {
      printf("CURRENT PWM HAS DIFFERENT PERIOD\n");
      return VHAL_HARDWARE_STATUS_ERROR;
    }
  } else if (timx->ARR == tim_period && timx->CCR[ch] == pwm_period) {
    //already set, return
    return VHAL_OK;
  }

  //printf("PSC %i, PRD %i\n",prescaler,pwm_period);

  //pwm_periods[tm] = period;


  if (!TIMER_STATUS_CH(tm) || TIMER_STATUS_HAS_ONLY_CH(tm, ch)) {
    //first init
    _timReserveAndInit(tm, TIM_STATUS_PWM);
    //timx->CNT = 0;
    timx->SR = 0xffff; //disable everything
    timx->DIER=0;
    _set_timer_base(timx, prescaler, tim_period, 0);
  }


  TIMER_STATUS_CH_ON(tm, ch);
  //set output latch to 0
  vhalPinWrite(vpin,0);
  vhalPinSetToPeripheral(vpin, PRPH_PWM, ALTERNATE_FN(VP_TOAF(vpin)) | STM32_PUDR_NOPULL | STM32_OSPEED_HIGHEST | STM32_OTYPE_PUSHPULL);

  timx->CR1 |= TIM_CR1_ARPE;

  vosSysLock();
  _timOCxInit(ch, timx, pwm_period);
  vosSysUnlock();

  if (timx == TIM1 || timx == TIM8) {
    timx->BDTR |= TIM_BDTR_MOE;  
    //Enable the TIM Main Output (TIM_CtrlPWMOutputs in ST stdlib)
  }
  
  if (npulses) {
    //block
    printf("PWM with npulses %i %i %i\n",npulses,tm,ch);
    vosSysLock();
    tim_pwm[tm][ch].thread = vosThCurrent();
    tim_pwm[tm][ch].vpin = vpin;
    tim_pwm[tm][ch].npulses = npulses+1; //first pulse is triggered by channel activation
    if (timx == TIM1) {
      vhalIrqEnable(TIM1_CC_IRQn);
    } 
    vhalIrqEnable(TIM_IRQ(tm));
    timx->DIER|=(TIM_DIER_CC1IE<<ch); //enable interrupt on capture for ch
    timx->CR1 |= TIM_CR1_CEN| TIM_CR1_URS; //start timer
    vosThSuspend();   
    vosSysUnlock();
  } else {
      printf("PWM without npulses\n");
      timx->CR1 |= TIM_CR1_CEN | TIM_CR1_URS;
       //Enable the TIM Counter
  }
  
  return VHAL_OK;
}

void _timPWMIrq(uint8_t tm, uint32_t sr) {
  TIM_TypeDef *timx = TIM(tm);
  uint16_t ch;

  for (ch = 0; ch < 4; ch++) {
      if (!(sr&(TIM_SR_CC1IF<<ch)))
        continue;
      if (!TIMER_STATUS_HAS_CH(tm, ch))
        continue;
      PWM_TypeDef *pwm = &tim_pwm[tm][ch];
      pwm->npulses--;
      
      if(!pwm->npulses){
        _timFree_channel(tm, ch);
        timx->CCER &= ~(TIM_CCER_CC1E << (4 * ch)); //disable channel
        timx->DIER &= ~(TIM_DIER_CC1IE << ch);
        vhalPinSetMode(pwm->vpin, PINMODE_OUTPUT_PUSHPULL); //remove from PWM control
        if (pwm->thread){
          vosThResumeIsr(pwm->thread);
          pwm->thread=NULL;  
        }
      }
  }
}

#endif

/* ========================================================================
    ICU
   ======================================================================== */

#if VHAL_ICU

struct _icuconfig {
  uint16_t ccer;
  uint16_t ccmr;
};

const struct _icuconfig const icuregs[]  = {
  {(uint16_t)~(TIM_CCER_CC1P | TIM_CCER_CC1NP), (uint16_t)~(TIM_CCMR1_CC1S | TIM_CCMR1_IC1F)},
  {(uint16_t)~(TIM_CCER_CC2P | TIM_CCER_CC2NP), (uint16_t)~(TIM_CCMR1_CC2S | TIM_CCMR1_IC2F)},
  {(uint16_t)~(TIM_CCER_CC3P | TIM_CCER_CC3NP), (uint16_t)~(TIM_CCMR2_CC3S | TIM_CCMR2_IC3F)},
  {(uint16_t)~(TIM_CCER_CC4P | TIM_CCER_CC4NP), (uint16_t)~(TIM_CCMR2_CC4S | TIM_CCMR2_IC4F)},
};


void TIM_ICInit(TIM_TypeDef *timx, uint32_t ch, uint32_t polarity) {

  uint16_t ccmr, ccer;

  ccmr = (ch < 2) ? timx->CCMR1 : timx->CCMR2;
  timx->CCER &= ~(TIM_CCER_CC1E << (4 * ch)); //disable channel
  ccer = timx->CCER;
  ccer &= icuregs[ch].ccer;
  ccmr &= icuregs[ch].ccmr;
  ccmr |= (TIM_ICSelection_DirectTI | (/*filter*/0 << 4) | TIM_ICPSC_DIV1) << (8 * (ch % 2));
  ccer |= (TIM_CCER_CC1E << (4 * ch)) | (polarity << (4 * ch));
  if (ch < 2) {
    timx->CCMR1 = ccmr;
  } else {
    timx->CCMR2 = ccmr;
  }
  timx->CCER = ccer;

}


#define ICU_END_CAPTURE() {\
    _timFree_channel(tm, ch);\
    timx->DIER &= ~(TIM_IT_CC1 << ch);\
    *icu->bufsize = icu->count; /*save read timings*/  \
    /*args is used to store current thread for blocking calls*/ \
    vosThResumeIsr(icu->thread); \
  }

#define IS_ELAPSED(icu)   ((!icu->has_capture) ? (timx->CNT-icu->start_capture>icu->time_window):((icu->has_capture-icu->start_capture)>icu->time_window))

void _timICUIrq(uint8_t tm, uint32_t sr) {
  TIM_TypeDef *timx = TIM(tm);
  uint16_t ch;
  uint16_t now;
  uint32_t val = 0;


  if (sr & SR_UPDATE) {
    //printf("tm %i sr %x\n",tm,timx->DIER);
    for (ch = 0; ch < 4; ch++) {
      if (!TIMER_STATUS_HAS_CH(tm, ch))
        continue;
      ICU_TypeDef *icu = tim_icu[tm][ch];
      if (icu->started) {
        icu->updated++;
        icu->has_capture += timx->ARR;
        if (IS_ELAPSED(icu)) {
          //time_window elapsed, end capture
          ICU_END_CAPTURE();
          continue;
        }
      }
    }
  }

  if (sr & SR_CAPTURE) {
    //printf("tm %i cp %x\n",tm,sr);
    for (ch = 0; ch < 4; ch++) {
      if ( (sr & (TIM_SR_CC1IF << ch)) && TIMER_STATUS_HAS_CH(tm, ch)) {
        ICU_TypeDef *icu = tim_icu[tm][ch];
        now = timx->CCR[ch];
        if (icu->started) {
          if (icu->updated) {
            val = ((now + timx->ARR * (icu->updated) - icu->last));
          } else {
            val = ((uint16_t)(now - icu->last));
          }
          val = (val * (timx->PSC + 1)) / (_system_frequency / 1000000);//micros
          if (icu->buffer && icu->count < *icu->bufsize) {
            icu->buffer[icu->count] = val;
          }
          if ( (icu->buffer && icu->count >= *icu->bufsize) || IS_ELAPSED(icu)) {
            ICU_END_CAPTURE();
            continue;
          }
          icu->count++;
          icu->last = now;
        } else if ( ((icu->port->IDR >> icu->pad) & 1) == ICU_CFG_GET_TRIGGER(icu->cfg)) {
          //start!
          icu->last = now;
          icu->started = 1;
          icu->has_capture = 0;
          icu->start_capture = timx->CNT;
        }
        icu->updated = 0;
      }
    }
  }
}


int vhalIcuStart(int vpin, uint32_t cfg, uint32_t time_window, uint32_t *buffer, uint32_t *bufsize) {
  int ret = VHAL_GENERIC_ERROR;
  vosSemWait(icu_sem);

  if (!PIN_HAS_PRPH(vpin, PRPH_ICU)) {
    printf("NOT A ICU VPIN\n");
    ret = VHAL_INVALID_PIN;
    goto _ret;
  }

  uint32_t tm = VP_TOTIM(vpin);
  uint32_t ch = VP_TOCHA (vpin);

  if (!(TIMER_STATUS(tm) == TIM_STATUS_ICU  || TIMER_STATUS(tm) == TIM_STATUS_USABLE)
      || !PIN_FREE_OR_ASSIGNED_TO(vpin, PRPH_ICU)) {
    printf("NOT USABLE AS ICU %x %i\n", vpin, TIMER_STATUS(tm));
    ret = VHAL_INVALID_PIN;
    goto _ret;
  }

  if (TIMER_STATUS_HAS_CH(tm, ch)) {
    printf("ALREADY CAPTURING\n");
    ret = VHAL_HARDWARE_STATUS_ERROR;
    goto _ret;
  }

  TIM_TypeDef *timx = TIM(tm);
  uint32_t prescaler = _system_frequency / 1000000 - 1, period = 65535;
  //_timeCalcRegisters(conf->time_window, &prescaler, &period, 0);

  if (!TIMER_STATUS_CH(tm)) {
    //first init
    _timReserveAndInit(tm, TIM_STATUS_ICU);
    _set_timer_base(timx, prescaler, period, 0);
    printf("PSC %i, PRD %i\n", prescaler, period);
    timx->CNT = 0;
    timx->SR = 0;
  } else {
    if (prescaler != timx->PSC || period != timx->ARR) {
      printf("CURRENT ICU HAS DIFFERENT PERIOD\n");
      ret = VHAL_HARDWARE_STATUS_ERROR;
      goto _ret;
    }
  }

  TIMER_STATUS_CH_ON(tm, ch);

  //cleanup leftovers and alloc as needed
  tim_icu[tm][ch] = (ICU_TypeDef *)gc_malloc(sizeof(ICU_TypeDef));

  //new channel: since this function is blocking we alloc memory everytime
  ICU_TypeDef *icu_s = tim_icu[tm][ch];
  //tim_icu[tm][ch] = icu_s;

  vhalPinSetToPeripheral(vpin, PRPH_ICU, ALTERNATE_FN(VP_TOAF(vpin)) | ((ICU_CFG_GET_INPUT(cfg) == ICU_INPUT_PULLUP) ? STM32_PUDR_PULLUP : STM32_PUDR_PULLDOWN));

  //icu_s->ithread = vosThCurrent();
  icu_s->port = (GPIO_TypeDef *)PIN_PORT(vpin);
  icu_s->pad = PIN_PAD(vpin);
  icu_s->time_window = GET_TIME_MICROS(time_window);
  icu_s->started = 0;
  icu_s->has_capture = 0;
  icu_s->buffer = buffer;
  icu_s->cfg = cfg;
  icu_s->bufsize = bufsize;
  icu_s->thread = vosThCurrent();
  
  TIM_ICInit(timx, ch, TIM_ICPolarity_BothEdge);


  printf("ARR: %i, PSC: %i\n", timx->ARR, timx->PSC);

  if (timx == TIM1) {
    printf("Enabling CC\n");
    vhalIrqClearPending(TIM1_CC_IRQn);
    vhalIrqEnable(TIM1_CC_IRQn);
  } /*else if (timx.TIMx==TIM8){
    vhalIrqClearPending(TIM8_CC_IRQn);
    vhalIrqEnable(TIM8_CC_IRQn);
  }*/
  vhalIrqClearPending(TIM_IRQ(tm));
  vhalIrqEnable(TIM_IRQ(tm));

  ret = VHAL_OK;

  //blocking
    vosSemSignal(icu_sem);
    vosSysLock();
    timx->DIER |= TIM_IT_Update | (TIM_IT_CC1 << ch); /* Enable the CCx Interrupt sources (TIM_ITConfig in ST stdlib) */
    timx->CR1  |= TIM_CR1_CEN | TIM_CR1_URS;   /* Enable the TIM Counter  (TIM_Cmd in ST stdlib) */
    vosThSuspend();
    vosSysUnlock();
    vosSemWait(icu_sem);
    if (tim_icu[tm][ch]) {
      gc_free(icu_s);
      tim_icu[tm][ch] = NULL;
    }
  
_ret:
  vosSemSignal(icu_sem);
  return ret;
}
#endif


/* ========================================================================
    HTM
   ======================================================================== */

#if VHAL_HTM

int vhalHtmGetFreeTimer() {
  int prphs = PERIPHERAL_NUM(htm);
  int idx;
  int tmh;
  vosSysLock();
  for (idx = 0; idx < prphs; idx++) {
    tmh = GET_PERIPHERAL_ID(htm, idx);
    if (TIMER_STATUS(tmh) == TIM_STATUS_USABLE) {
      goto end_fn;
    }
  }
  tmh = VHAL_GENERIC_ERROR;
end_fn:
  vosSysUnlock();
  return tmh;
}


void _timSetSimple(uint32_t tm, uint32_t delay, htmFn fn, void *args) {
  TIM_TypeDef *timx = TIM(tm);

  tim_htm[tm].fn = fn;
  tim_htm[tm].args = args;

  uint32_t prescaler;
  uint32_t period;
  _timeCalcRegisters(delay, &prescaler, &period, 0);
  _set_timer_base(timx, prescaler, period, 0);
  timx->CNT = 0;
  timx->SR = 0;

  vhalIrqClearPending(TIM_IRQ(tm));
  vhalIrqEnable(TIM_IRQ(tm));

  timx->DIER |= TIM_IT_Update; /* Enable the Interrupt sources (TIM_ITConfig in ST stdlib) */
  timx->CR1 |= TIM_CR1_CEN | TIM_CR1_URS;    /* Enable the TIM Counter  (TIM_Cmd in ST stdlib) */
}


int _set_htm_timer(uint32_t tm, uint32_t delay, htmFn fn, void *args, uint32_t status) {
  int ret = VHAL_OK;
  vosSysLock();
  if (TIMER_STATUS(tm) == status && (delay <= 0 || fn == NULL)) {
    _timFree_timer(tm);
    goto end_fn;
  }
  if ( TIMER_STATUS(tm) == TIM_STATUS_USABLE) {
    _timReserveAndInit(tm, status);
    _timSetSimple(tm, delay, fn, args);
    goto end_fn;
  } else ret = VHAL_GENERIC_ERROR;
end_fn:
  vosSysUnlock();
  return ret;
}

int vhalHtmOneShot(uint32_t tm, uint32_t delay, htmFn fn, void *args) {
  return _set_htm_timer(tm, delay, fn, args, TIM_HTM_ONE_SHOT);
}

int vhalHtmRecurrent(uint32_t tm, uint32_t delay, htmFn fn, void *args) {
  return _set_htm_timer(tm, delay, fn, args, TIM_HTM_RECURRENT);
}

#endif


#else

#endif