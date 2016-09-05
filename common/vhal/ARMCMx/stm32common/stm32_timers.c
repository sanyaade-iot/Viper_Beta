//GENERIC TIMER DRIVERS
#if defined(VHAL_PWM) || defined(VHAL_ICU) || defined(VHAL_HTM)

#include "vhal.h"
#include "vhal_gpio.h"
#include "vhal_tim.h"
#include "vhal_irqs.h"
#include "vhal_common.h"
#include "stm32_timers.h"


//RCC
extern void rcc_enable_tim(int x);
extern void rcc_disable_tim(int x);



/* ========================================================================
    TIMER CONSTANTS
   ======================================================================== */



//CONFIG STRUCTURES
extern const uint8_t const tim_irqs[TIMNUM];
extern TIM_TypeDef *const tim_l[TIMNUM];

#define TIM(x) tim_l[x]
#define TIM_IRQ(x) tim_irqs[x]

//STATUS STRUCTURES
uint8_t tim_s_initialized = 0;
uint8_t tim_s[TIMNUM];
#if defined(VHAL_PWM)
PWM_TypeDef tim_pwm[TIMNUM][4];
#endif

#if defined(VHAL_HTM)
HTM_TypeDef tim_htm[TIMNUM];
#endif

#if defined(VHAL_ICU)
ICU_TypeDef *tim_icu[TIMNUM][4];
VSemaphore icu_sem;
#endif


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


//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)




#define vhalIrqClearPending(IRQn) NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F))



/* ========================================================================
    TIMER HANDLING
   ======================================================================== */



void _timFree_timer(uint32_t tm) {
  rcc_disable_tim(tm);
  vhalIrqDisable(TIM_IRQ(tm));
  vhalIrqClearPending(TIM_IRQ(tm));
  if (TIM(tm) == TIM1) {
    vhalIrqDisable(TIM1_CC_IRQn);
    vhalIrqClearPending(TIM1_CC_IRQn);
  } /*else if (TIM(tm)==TIM8){
    vhalIrqDisable(TIM8_CC_IRQn);
  }*/
  TIMER_SET_STATUS(tm, TIM_STATUS_USABLE);
}


void _timFree_channel(uint32_t tm, uint32_t ch) {
  TIMER_STATUS_CH_OFF(tm, ch);
  if (!TIMER_STATUS_CH(tm)) {
    _timFree_timer(tm);
  }
}


void _timReserveAndInit(uint32_t tm, uint32_t status) {
  rcc_enable_tim(tm);
  TIM(tm)->SR = (uint16_t)~TIM_IT_Update;
  TIMER_SET_STATUS(tm, status);
}


int _timeCalcRegisters(uint32_t apb, uint32_t time, uint32_t *psc, uint32_t *prd, uint32_t desired_psc) {
  uint32_t ticks = GET_TIME_MICROS(time) * TICKS_PER_MUSEC(apb);
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
  timx->ARR = TIM_Period;

  // Set the Prescaler value
  timx->PSC = TIM_Prescaler;

  if ((timx == TIM1) || (timx == TIM8)) {
    // Set the Repetition Counter value
    timx->RCR = TIM_RepetitionCounter;
  }

  //Generate an update event to reload the Prescaler
  //and the repetition counter(only for TIM1 and TIM8) value immediately
  timx->EGR = TIM_PSCReloadMode_Immediate;
}




#if defined(VHAL_ICU) || defined(VHAL_HTM) || defined(VHAL_PWM)


/* ========================================================================
    IRQ HANDLERS
   ======================================================================== */

#define SR_CAPTURE (TIM_SR_CC1IF|TIM_SR_CC2IF|TIM_SR_CC3IF|TIM_SR_CC4IF)
#define SR_UPDATE (TIM_SR_UIF)

#if defined(VHAL_ICU)
void _timICUIrq(uint32_t tm, uint32_t sr);
#endif

#if defined(VHAL_ICU)
void _timPWMIrq(uint32_t tm, uint32_t sr);
#endif


void _timIrq_wrapper(uint32_t tm) {
  uint32_t sr;

  vosSysLockIsr();
  sr = TIM(tm)->SR;

  //TIM(tm)->SR = 0;

#if defined(VHAL_PWM)
  if (TIMER_STATUS(tm) == TIM_STATUS_PWM  && (sr & (SR_UPDATE | SR_CAPTURE))) {
    _timPWMIrq(tm, sr);
  }
#endif
#if defined(VHAL_ICU)
#if defined(VHAL_PWM)
  else
#endif
    if (TIMER_STATUS(tm) == TIM_STATUS_ICU  && (sr & (SR_UPDATE | SR_CAPTURE))) {
      _timICUIrq(tm, sr);
    }
#endif
#if defined(VHAL_HTM)
#if defined(VHAL_ICU) || defined(VHAL_PWM)
    else
#endif
      if (sr & SR_UPDATE) {
        if (TIMER_STATUS(tm) == TIM_HTM_ONE_SHOT || TIMER_STATUS(tm) == TIM_HTM_RECURRENT) {
          if (*tim_htm[tm].fn) {
            vosSysUnlockIsr();
            (*tim_htm[tm].fn)(tm, tim_htm[tm].args);  // execute callback
            vosSysLockIsr();
          }
          if (TIMER_STATUS(tm) == TIM_HTM_ONE_SHOT) { // free TIMx
            _timFree_timer(tm);
            if (tim_htm[tm].thread) {
              vosThResumeIsr(tim_htm[tm].thread);
              tim_htm[tm].thread = NULL;
            }
          }
        }
      }
#endif
  TIM(tm)->SR = ~sr;
  vosSysUnlockIsr();
}

#endif
/* ========================================================================
    INITIALIZERS
   ======================================================================== */


#if defined(VHAL_ICU)
int vhalInitICU(void *data) {
  return vhalInitTIM(data);
}
#endif

#if defined(VHAL_HTM)
int vhalInitHTM(void *data) {
  return vhalInitTIM(data);
}
#endif


#if defined(VHAL_PWM)
int vhalInitPWM(void *data) {
  return vhalInitTIM(data);
}
#endif

void init_timer_structs() {
  memset(tim_s, TIM_STATUS_USABLE, sizeof(tim_s));
#if defined(VHAL_ICU)
  icu_sem = vosSemCreate(1);
  memset(tim_icu, 0, sizeof(tim_icu));
#endif
#if defined(VHAL_PWM)
  memset(tim_pwm, 0, sizeof(tim_pwm));
#endif
#if defined(VHAL_HTM)
  memset(tim_htm, 0, sizeof(tim_htm));
#endif
}


/* ========================================================================
    PWM
   ======================================================================== */

#if defined(VHAL_PWM)


//registers
extern const struct _pwmconfig const pwmch_reg[];

void _timOCxInit(uint32_t ch, TIM_TypeDef *timx, uint32_t period) {
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
  tmpccer = timx->CCER;
  tmpcr2 =  timx->CR2;
  tmpccmrx = (ch < 2) ? timx->CCMR1 : timx->CCMR2;

  //timx->CCER &= ~(TIM_CCER_CC1E << (4 * ch)); //disable channel
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
  //timx->CCER |= (TIM_CCER_CC1E << (4 * ch)); //disable channel
  //TODO: only for stm32f1
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
  _timeCalcRegisters(TIM_APB(tm), period, &prescaler, &tim_period, 0);
  _timeCalcRegisters(TIM_APB(tm), pulse, &prescaler, &pwm_period, prescaler);
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

  printf("PSC %i, PRD %i, PLS %i\n",prescaler,tim_period,pwm_period);

  //pwm_periods[tm] = period;


  if (!TIMER_STATUS_CH(tm) || TIMER_STATUS_HAS_ONLY_CH(tm, ch)) {
    //first init
    _timReserveAndInit(tm, TIM_STATUS_PWM);
    //timx->CNT = 0;
    timx->SR = 0; //disable everything
    timx->DIER = 0;
    timx->CR1 |= TIM_CR1_UDIS;
    _set_timer_base(timx, prescaler, tim_period, 0);
    timx->CR1 &= ~TIM_CR1_UDIS;
  }


  TIMER_STATUS_CH_ON(tm, ch);
  //set output latch to 0
  vhalPinWrite(vpin, 0);
  //TODO: make a macro for each platform
  PIN_TO_PWM(vpin);
  //vhalPinSetToPeripheral(vpin, PRPH_PWM, ALTERNATE_FN(VP_TOAF(vpin)) | STM32_PUDR_NOPULL | STM32_OSPEED_HIGHEST | STM32_OTYPE_PUSHPULL);

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
    //vhalPinSetMode(D5,PINMODE_OUTPUT_PUSHPULL);
    /*vhalPinSetMode(D0, PINMODE_OUTPUT_PUSHPULL);
    vhalPinSetMode(D1, PINMODE_OUTPUT_PUSHPULL);
    vhalPinSetMode(D2, PINMODE_OUTPUT_PUSHPULL);
    vhalPinSetMode(D3, PINMODE_OUTPUT_PUSHPULL);
    vhalPinSetMode(D4, PINMODE_OUTPUT_PUSHPULL);
    vhalPinSetMode(D5, PINMODE_OUTPUT_PUSHPULL);
    vhalPinSetMode(D6, PINMODE_OUTPUT_PUSHPULL);
    */
    tim_pwm[tm][ch].thread = vosThCurrent();
    printf("PWM with npulses %i %i %i %x\n", npulses, tm, ch, tim_pwm[tm][ch].thread);
    vosSysLock();
    tim_pwm[tm][ch].vpin = vpin;
    tim_pwm[tm][ch].npulses = npulses;
    if (timx == TIM1) {
      vhalIrqEnable(TIM1_CC_IRQn);
    }
    vhalIrqEnable(TIM_IRQ(tm));
    timx->DIER |= (TIM_DIER_CC1IE << ch); //enable interrupt on capture for ch
    timx->CR1 |= TIM_CR1_CEN | TIM_CR1_URS; //start timer
    vosThSuspend();
    vosSysUnlock();
  } else {
    printf("PWM without npulses\n");
    timx->CR1 |= TIM_CR1_CEN | TIM_CR1_URS;
    //Enable the TIM Counter
  }

  return VHAL_OK;
}

void _timPWMIrq(uint32_t tm, uint32_t sr) {
  TIM_TypeDef *timx = TIM(tm);
  uint32_t ch;

  for (ch = 0; ch <= 4; ch++) {
    if (!(sr & (TIM_SR_CC1IF << ch)))
      continue;
    if (!TIMER_STATUS_HAS_CH(tm, ch))
      continue;
    PWM_TypeDef *pwm = &tim_pwm[tm][ch];
    pwm->npulses--;

    if (pwm->npulses <= 0) {
      timx->CCER &= ~(TIM_CCER_CC1E << (4 * ch)); //disable channel
      timx->DIER &= ~(TIM_DIER_CC1IE << ch);
      _timFree_channel(tm, ch);
      vhalPinSetMode(pwm->vpin, PINMODE_OUTPUT_PUSHPULL); //remove from PWM control
      if (pwm->thread) {
        vosThResumeIsr(pwm->thread);
        pwm->thread = NULL;
      }
    }
  }
}

#endif

/* ========================================================================
    ICU
   ======================================================================== */

#if defined(VHAL_ICU)

//registers
extern const struct _icuconfig const icuregs[];

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
    timx->CCER &= ~(TIM_CCER_CC1E << (4 * ch)); \
    timx->DIER &= ~(TIM_IT_CC1 << ch); \
    _timFree_channel(tm, ch); \
    *icu->bufsize = icu->count; /*save read timings*/  \
    /*args is used to store current thread for blocking calls*/ \
    vosThResumeIsr(icu->thread); \
  }

#define IS_ELAPSED(icu)   ((!icu->has_capture) ? (timx->CNT-icu->start_capture>icu->time_window):((icu->has_capture-icu->start_capture)>icu->time_window))

void _timICUIrq(uint32_t tm, uint32_t sr) {
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
          //no need to convert, val is already in micros since clock is configured to
          //tick every micro in vhalIcuStart
          //val = (val * (timx->PSC + 1)) / (_system_frequency / 1000000);//micros
          if (icu->buffer && icu->count < *icu->bufsize) {
            icu->buffer[icu->count] = val;
          }
          if ( (icu->buffer && icu->count >= *icu->bufsize) || IS_ELAPSED(icu)) {
            ICU_END_CAPTURE();
            continue;
          }
          icu->count++;
          icu->last = now;
        } else {
          //start!
          icu->last = now;
          icu->started = 1;
          icu->has_capture = 0;
          icu->start_capture = timx->CNT;
        }
        icu->updated = 0;
        timx->CCER ^= (2 << (ch * 4)); //no bothedge mode, flip the CC(ch)P bit in CCER
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
  uint32_t prescaler = TICKS_PER_MUSEC(TIM_APB(tm)) -1;// _system_frequency / 1000000 - 1
  uint32_t period = 65535;
  //_timeCalcRegisters(conf->time_window, &prescaler, &period, 0);

  if (!TIMER_STATUS_CH(tm)) {
    //first init
    _timReserveAndInit(tm, TIM_STATUS_ICU);
    timx->CR1 &= ~TIM_CR1_UDIS;
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

  //TODO: set to macro
  PIN_TO_ICU(vpin, cfg);

  icu_s->time_window = GET_TIME_MICROS(time_window);
  icu_s->started = 0;
  icu_s->has_capture = 0;
  icu_s->buffer = buffer;
  icu_s->cfg = cfg;
  icu_s->bufsize = bufsize;
  icu_s->thread = vosThCurrent();



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
  TIM_ICInit(timx, ch, (ICU_CFG_GET_TRIGGER(cfg) == 1) ? TIM_ICPolarity_Rising : TIM_ICPolarity_Falling);
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

#if defined(VHAL_HTM)

int vhalHtmGetFreeTimer() {
  int prphs = PERIPHERAL_NUM(htm);
  int idx;
  int tmh;
  for (idx = 0; idx < prphs; idx++) {
    tmh = GET_PERIPHERAL_ID(htm, idx);
    if (TIMER_STATUS(tmh) == TIM_STATUS_USABLE) {
      return tmh;
    }
  }
  return VHAL_GENERIC_ERROR;
}


void _timSetSimple(uint32_t tm, uint32_t delay, htmFn fn, void *args) {
  TIM_TypeDef *timx = TIM(tm);

  tim_htm[tm].fn = fn;
  tim_htm[tm].args = args;
  tim_htm[tm].thread = NULL;

  uint32_t prescaler;
  uint32_t period;
  _timeCalcRegisters(TIM_APB(tm), delay, &prescaler, &period, 0);
  _set_timer_base(timx, prescaler, period, 0);
  timx->CNT = 0;
  timx->SR = 0;

  vhalIrqClearPending(TIM_IRQ(tm));
  vhalIrqEnable(TIM_IRQ(tm));

  timx->DIER |= TIM_IT_Update; /* Enable the Interrupt sources (TIM_ITConfig in ST stdlib) */
  timx->CR1 |= TIM_CR1_CEN | TIM_CR1_URS;    /* Enable the TIM Counter  (TIM_Cmd in ST stdlib) */
}


int _set_htm_timer(uint32_t tm, uint32_t delay, htmFn fn, void *args, uint32_t status) {
  if (TIMER_STATUS(tm) == status && TIME_IS_ZERO(delay)) {
    _timFree_timer(tm);
    return 0;
  }
  if ( TIMER_STATUS(tm) == TIM_STATUS_USABLE) {
    _timReserveAndInit(tm, status);
    _timSetSimple(tm, delay, fn, args);
    return 0;
  }
  return VHAL_GENERIC_ERROR;
}

int vhalHtmOneShot(uint32_t tm, uint32_t delay, htmFn fn, void *args, uint32_t blocking) {
  int ret = VHAL_OK;
  vosSysLock();
  ret = _set_htm_timer(tm, delay, fn, args, TIM_HTM_ONE_SHOT);
  if (blocking && !ret){
    tim_htm[tm].thread = vosThCurrent();
    vosThSuspend();
  }
  vosSysUnlock();
  return ret;
}

int vhalSleepMicros(uint32_t tm, uint32_t micros) {
  if (TIMER_STATUS(tm) != TIM_STATUS_USABLE) {
    return VHAL_GENERIC_ERROR;
  }
  vosSysLock();
  _timReserveAndInit(tm, TIM_HTM_ONE_SHOT);
  _timSetSimple(tm, TIME_U(micros,MICROS), NULL, NULL);
  tim_htm[tm].thread = vosThCurrent();
  vosThSuspend();
  vosSysUnlock();
  //vhalPinToggle(D5);
  return VHAL_OK;
}

int vhalHtmRecurrent(uint32_t tm, uint32_t delay, htmFn fn, void *args) {
  return _set_htm_timer(tm, delay, fn, args, TIM_HTM_RECURRENT);
}

#endif


#else

#endif