#if defined(VHAL_PWM) || defined(VHAL_ICU) || defined(VHAL_HTM)

#include "vhal.h"
#include "vhal_gpio.h"
#include "vhal_irqs.h"
#include "vhal_common.h"
#include "vhal_tim.h"

//#include "sam3x8e.h"

#define vhalIrqClearPending(IRQn) NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F))

#define TICKS_PER_MUSEC ((_system_frequency)/1000000)

TIM_CHAN_TypeDef *const tim_l[TIMNUM * CHANNUM] = {&TIM1, &TIM2, &TIM3, &TIM4, &TIM5, &TIM6, &TIM7, &TIM8, &TIM9};

#define TIM(id)  ((id < TIMNUM * CHANNUM) ? tim_l[id] : NULL)


#define TC_FREE     0
#define TC_TO_ICU   1
#define TC_TO_HTM   2
#define TC_TO_PWM   3
uint8_t tcstatus[TIMNUM * CHANNUM];

typedef struct _icu_struct {
    VThread thread;
    uint32_t* buffer;
    uint32_t* bufsize;
    uint8_t unused;
    uint8_t started;
    uint16_t events;
    uint32_t last;
    uint32_t twrc;

} ICU_TypeDef;

typedef struct {
    union {
        ICU_TypeDef icu;
        struct {
            void *args;
            htmFn fn;
            VThread thread;
            uint32_t oneshot;
        } htm;
        struct {
            VThread thread[2];
            uint32_t npulses[2];
            uint16_t vpin[2];
        } pwm;
    }
} tc_TypeDef;

#if defined(VHAL_HTM) || defined(VHAL_ICU) || defined(VHAL_PWM)
tc_TypeDef tim_c[TIMNUM * CHANNUM];
#endif

typedef struct _pwm_struct {
    VThread *thread;
    uint32_t npulses;
    uint16_t vpin;
    uint16_t ch;
} PWM_TypeDef;




void pin_to_periph(int port, int pad, int pio_prph) {
    switch (port) {
    case PORT_A:
        PIOA->PIO_PDR |= (0x1 << pad);
        PIOA->PIO_ABSR = (PIOA->PIO_ABSR & ~(1 << pad)) | (pio_prph << pad);
        break;
    case PORT_B:
        PIOB->PIO_PDR |= (0x1 << pad);
        PIOB->PIO_ABSR |= (PIOB->PIO_ABSR & ~(1 << pad)) | (pio_prph << pad);
        break;
    case PORT_C:
        PIOC->PIO_PDR |= (0x1 << pad);
        PIOC->PIO_ABSR |= (PIOC->PIO_ABSR & ~(1 << pad)) | (pio_prph << pad);
        break;
    case PORT_D:
        PIOD->PIO_PDR |= (0x1 << pad);
        PIOD->PIO_ABSR |= (PIOD->PIO_ABSR & ~(1 << pad)) | (pio_prph << pad);
        break;
    }
}

void pin_to_pio(int port, int pad) {
    switch (port) {
    case PORT_A:
        PIOA->PIO_PER |= (0x1 << pad);
        break;
    case PORT_B:
        PIOB->PIO_PER |= (0x1 << pad);
        break;
    case PORT_C:
        PIOC->PIO_PER |= (0x1 << pad);
        break;
    case PORT_D:
        PIOD->PIO_PER |= (0x1 << pad);
        break;
    }
}

uint32_t _timeCalcRegisters(uint32_t time, uint32_t *clock_selection, uint32_t *register_c, uint32_t desired_clks) {
    // Delay in ticks
    int32_t ticks = GET_TIME_MICROS(time) * TICKS_PER_MUSEC;
    *clock_selection = desired_clks;
    uint32_t prescale_factor = 2 << (2 * (*clock_selection));
    *register_c = ticks / prescale_factor;

    return VHAL_OK;
}

uint32_t _timEnablePeriphClock(uint32_t tm, uint32_t enable) {
    switch (tm) {
    case 0:
        if (enable)
            PMC->PMC_PCER0 |= PMC_PCER0_PID27;
        else
            PMC->PMC_PCER0 &= PMC_PCER0_PID27;
        break;
    case 1:
        if (enable)
            PMC->PMC_PCER0 |= PMC_PCER0_PID28;
        else
            PMC->PMC_PCER0 &= PMC_PCER0_PID28;
        break;
    case 2:
        if (enable)
            PMC->PMC_PCER0 |= PMC_PCER0_PID29;
        else
            PMC->PMC_PCER0 &= PMC_PCER0_PID29;
        break;
    case 3:
        if (enable)
            PMC->PMC_PCER0 |= PMC_PCER0_PID30;
        else
            PMC->PMC_PCER0 &= PMC_PCER0_PID30;
        break;
    case 4:
        if (enable)
            PMC->PMC_PCER0 |= PMC_PCER0_PID31;
        else
            PMC->PMC_PCER0 &= PMC_PCER0_PID31;
        break;
    case 5:
        if (enable)
            PMC->PMC_PCER1 |= PMC_PCER1_PID32;
        else
            PMC->PMC_PCER1 &= PMC_PCER1_PID32;
        break;
    case 6:
        if (enable)
            PMC->PMC_PCER1 |= PMC_PCER1_PID33;
        else
            PMC->PMC_PCER1 &= PMC_PCER1_PID33;
        break;
    case 7:
        if (enable)
            PMC->PMC_PCER1 |= PMC_PCER1_PID34;
        else
            PMC->PMC_PCER1 &= PMC_PCER1_PID34;
        break;
    case 8:
        if (enable)
            PMC->PMC_PCER1 |= PMC_PCER1_PID35;
        else
            PMC->PMC_PCER1 &= PMC_PCER1_PID35;
        break;
    default:
        return VHAL_GENERIC_ERROR;
    }
    return VHAL_OK;
}

TIM_TypeDef *_correspondingTimerModule(uint32_t tm) {
    switch (tm / 3) {
    case 0:
        return MODULE_TIM1;
    case 1:
        return MODULE_TIM2;
    case 2:
        return MODULE_TIM3;
    default:
        return NULL;
    }
}

int _enableCorrespondingTimerIrq(uint32_t tm, uint32_t enable) {
    vhalIrqClearPending(TC0_IRQn + tm);
    if (enable)
        vhalIrqEnable(TC0_IRQn + tm);
    else
        vhalIrqDisable(TC0_IRQn + tm);
    return VHAL_OK;
}

#if defined(VHAL_HTM)
uint32_t _timSetSimple(uint32_t tm, uint32_t delay, uint8_t oneshot, htmFn fn, void *args) {
    TIM_CHAN_TypeDef *timx = TIM(tm);
    TIM_TypeDef *tim;

    if (timx == NULL)
        return VHAL_GENERIC_ERROR;

    // Enable Peripheral Clock
    if (_timEnablePeriphClock(tm, 1) != VHAL_OK)
        return VHAL_GENERIC_ERROR;

    tim = _correspondingTimerModule(tm);

    tim_c[tm].htm.fn = fn;
    tim_c[tm].htm.args = args;
    tim_c[tm].htm.oneshot = oneshot;
    tcstatus[tm] = TC_TO_HTM;

    uint32_t prescaler;
    uint32_t register_c;
    _timeCalcRegisters(delay, &prescaler, &register_c, TC_CMR_TIMER_CLOCK1);

    timx->TC_CCR.fields.CLKDIS = 1;

    timx->TC_CMR.fields.capture.TCCLKS = prescaler;
    timx->TC_CMR.fields.capture.CLKI = 0;
    timx->TC_CMR.fields.capture.BURST = 0;
    timx->TC_CMR.fields.capture.LDBSTOP = 0;
    timx->TC_CMR.fields.capture.LDBDIS = 0;
    timx->TC_CMR.fields.capture.ETRGEDG = 0;
    timx->TC_CMR.fields.capture.ABETRG = 0;
    timx->TC_CMR.fields.capture.CPCTRG = 1;
    timx->TC_CMR.fields.capture.LDRA = 0;
    timx->TC_CMR.fields.capture.LDRB = 0;

    timx->TC_CMR.fields.capture.WAVE = 0;

    _enableCorrespondingTimerIrq(tm, 1);
    
    timx->TC_IER.fields.CPCS = 1;
    timx->TC_IER.fields.LDRAS = 0;
    timx->TC_IER.fields.LDRBS = 0;
    timx->TC_IER.fields.COVFS = 0;

    // Disable Write protection
    tim->TC_WPMR.fields.WPKEY = 0x54494D;
    tim->TC_WPMR.fields.WPEN = 0;

    // Set Register RC
    timx->TC_RC = register_c;

    // Fire up the timer
    //timx->TC_CCR.fields.CLKEN = 1;
    //timx->TC_CCR.fields.SWTRG = 1;
    timx->TC_CCR.value = 0b101;

    return VHAL_OK;
}
#endif

#if defined(VHAL_ICU)
uint32_t _timReset(uint32_t tm) {
    TIM_CHAN_TypeDef *timx = TIM(tm);

    // Disable Timer
    timx->TC_CCR.fields.CLKDIS = 1;

    // Disable Peripheral Clock
    _timEnablePeriphClock(tm, 0);

    tcstatus[tm] = TC_FREE;

    timx->TC_IDR.fields.CPCS = 1;
    _enableCorrespondingTimerIrq(tm, 0);

    return VHAL_OK;
}

#endif
/* ========================================================================
    IRQ HANDLERS
   ======================================================================== */

#if defined(VHAL_ICU) || defined(VHAL_HTM) || defined(VHAL_PWM)


void _timIrq_wrapper(uint32_t channel);

void vhalIRQ_TC0(void) {
    vosEnterIsr();
    _timIrq_wrapper(0);
    vosExitIsr();
}

void vhalIRQ_TC1(void) {
    vosEnterIsr();
    _timIrq_wrapper(1);
    vosExitIsr();
}

void vhalIRQ_TC2(void) {
    vosEnterIsr();
    _timIrq_wrapper(2);
    vosExitIsr();
}

void vhalIRQ_TC3(void) {
    vosEnterIsr();
    _timIrq_wrapper(3);
    vosExitIsr();
}

void vhalIRQ_TC4(void) {
    vosEnterIsr();
    _timIrq_wrapper(4);
    vosExitIsr();
}

void vhalIRQ_TC5(void) {
    vosEnterIsr();
    _timIrq_wrapper(5);
    vosExitIsr();
}

void vhalIRQ_TC6(void) {
    vosEnterIsr();
    _timIrq_wrapper(6);
    vosExitIsr();
}

void vhalIRQ_TC7(void) {
    vosEnterIsr();
    _timIrq_wrapper(7);
    vosExitIsr();
}

void vhalIRQ_TC8(void) {
    vosEnterIsr();
    _timIrq_wrapper(8);
    vosExitIsr();
}

#if !defined(VHAL_PWM)
void vhalIRQ_PWM(void) {

}
#else

void vhalIRQ_PWM(void);

#endif


uint8_t irq_initialized = 0;

#if defined(VHAL_HTM)
void vhalInitHTM(void *data) {
    return vhalInitTIM(data);
}
#endif

#if defined(VHAL_ICU)
VSemaphore icu_sem;
void vhalInitICU(void *data) {
    return vhalInitTIM(data);
}
#endif

#if defined(VHAL_PWM) || defined(VHAL_ICU)
uint8_t tioa[9];
uint8_t tiob[9];
uint8_t *tioc[] = {tioa, tiob};
#endif

#if defined(VHAL_PWM)
PWM_TypeDef pwm_pwm;
int vhalInitPWM(void *data) {
    return vhalInitTIM(data);
}

#endif

int vhalInitTIM(void *data) {
    (void)data;
    if (!irq_initialized) {
        vosInstallHandler(TC0_IRQn, vhalIRQ_TC0);
        vosInstallHandler(TC1_IRQn, vhalIRQ_TC1);
        vosInstallHandler(TC2_IRQn, vhalIRQ_TC2);
        vosInstallHandler(TC3_IRQn, vhalIRQ_TC3);
        vosInstallHandler(TC4_IRQn, vhalIRQ_TC4);
        vosInstallHandler(TC5_IRQn, vhalIRQ_TC5);
        vosInstallHandler(TC6_IRQn, vhalIRQ_TC6);
        vosInstallHandler(TC7_IRQn, vhalIRQ_TC7);
        vosInstallHandler(TC8_IRQn, vhalIRQ_TC8);
        vosInstallHandler(PWM_IRQn, vhalIRQ_PWM);

#if defined(VHAL_PWM)
        memset(tioa, 0, sizeof(tioa));
        memset(tiob, 0, sizeof(tiob));
#endif

#if defined(VHAL_ICU)
        icu_sem = vosSemCreate(1);
#endif

        memset(tim_c, 0, sizeof(tim_c));

        irq_initialized = 1;
    }
    return 0;
}

void _timIrq_wrapper(uint32_t channel) {
    //uint8_t tm = index * 3;
    //uint8_t channel = index;
    TIM_CHAN_TypeDef *timx = TIM(channel);
    uint32_t sr = timx->TC_SR.value & timx->TC_IMR.value;
    vosSysLockIsr();

#if defined(VHAL_PWM)
    if (tcstatus[channel] == TC_TO_PWM) {
        int tio;
        for (tio = 0; tio <= 1; tio++) {
            if (((tio == 0) && (sr & TC_SR_CPAS) && (tioa[channel] == 2)) || ((tio == 1) && (sr & TC_SR_CPBS) && (tiob[channel] == 2))) {
                tim_c[channel].pwm.npulses[tio]--;
            }
            if (!tim_c[channel].pwm.npulses[tio] && tioc[tio][channel] == 2) {
                //TODO: disable tio and interrupt, eventually disable tc channel
                vhalPinSetMode(tim_c[channel].pwm.vpin[tio], PINMODE_OUTPUT_PUSHPULL);
                if (!tio) timx->TC_IDR.fields.CPAS = 1;
                else timx->TC_IDR.fields.CPBS = 1;
                tioc[tio][channel] = 0;
                if (!tioc[(tio + 1) % 2][channel]) {
                    //disable channel
                    timx->TC_CCR.fields.CLKDIS = 1;
                    tcstatus[channel] = TC_FREE;
                }
                if (tim_c[channel].pwm.thread[tio]) {
                    vosThResumeIsr(tim_c[channel].pwm.thread[tio]);
                    tim_c[channel].pwm.thread[tio] = NULL;
                }
            }
        }
    }
#endif

#if defined(VHAL_HTM)
#if defined(VHAL_PWM)
    else
#endif
        if (tcstatus[channel] == TC_TO_HTM) {
            // Timer elapsed
            if (tim_c[channel].htm.oneshot) {
                timx->TC_CCR.fields.CLKDIS = 1;
            }
            if (tim_c[channel].htm.fn) {
                vosSysUnlockIsr();
                if (tim_c[channel].htm.fn != NULL)
                    (*tim_c[channel].htm.fn)(channel, tim_c[channel].htm.args);
                vosSysLockIsr();
            }
            if (tim_c[channel].htm.thread) {
                    vosThResumeIsr(tim_c[channel].htm.thread);
                    tim_c[channel].htm.thread = NULL;
            }
        }
#endif
#if defined(VHAL_ICU)
#if defined(VHAL_HTM) || defined(VHAL_PWM)
        else
#endif
        {
            // ICU event
            if (tcstatus[channel] == TC_TO_ICU) {
                ICU_TypeDef *icu = &tim_c[channel].icu;
                if (sr & TC_SR_LDRAS) {
                    // Register A was loaded -> New pulse started
                    if (!icu->started) {
                        timx->TC_RC = icu->twrc;
                        icu->started = TRUE;
                    } else {
                        uint32_t val = timx->TC_RA;
                        val -= icu->last;
                        val /= (_system_frequency / (2 * 1000000)); //TODO: remove 2 and multiply by prescaler
                        if (icu->events < *icu->bufsize && icu->buffer)
                            icu->buffer[icu->events] = val;
                        else goto end_icu;
                        icu->events++;
                    }
                    icu->last = timx->TC_RA;
                } else if (sr & TC_SR_LDRBS) {
                    // Register B was loaded -> Pulse finished
                    uint32_t val = timx->TC_RB;
                    val -= icu->last;
                    val /= (_system_frequency / (2 * 1000000)); //TODO: remove 2 and multiply by prescaler
                    if (icu->events < *icu->bufsize && icu->buffer)
                        icu->buffer[icu->events] = val;
                    else goto end_icu;
                    icu->last = timx->TC_RB;
                    icu->events++;
                }
                if ( (sr & TC_SR_CPCS) && icu->started) {
                    // Time window expired
end_icu:
                    timx->TC_CCR.fields.CLKDIS = 1;
                    timx->TC_IDR.value = 0xff;
                    *icu->bufsize = icu->events;
                    if (icu->thread) {
                        vosThResumeIsr(icu->thread);
                        icu->thread = NULL;
                    }
                    tcstatus[channel] = TC_FREE;
                    tioc[0][channel] = 0; //tiob can't be used as icu
                }
            }
        }
#endif
end_fn:
    vosSysUnlockIsr();
}

#endif



//extern void testdebug(const char *fmt, ...);
//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)



#if defined(VHAL_PWM)



int _enablePWMPin(int vpin) {
    int port = PIN_PORT_NUMBER(vpin);
    int pad = PIN_PAD(vpin);
    //mode is not important, just set status to PRPH_PWM
    vhalPinSetToPeripheral(vpin, PRPH_PWM, PINMODE_OUTPUT_PUSHPULL);
    //All Arduino PWM pins go to B-->1
    //TODO: make it generic
    pin_to_periph(port, pad, 1);
    return VHAL_OK;
}

int _setPWMPin(int vpin, uint32_t clk_src, uint32_t cprd, uint32_t cdty) {
    uint32_t channel = PIN_CLASS_DATA0(vpin);

    // Disable the channel first
    PWM->PWM_DIS = (0x1 << channel);
    // Disable Write Protection for PWM
    PWM->PWM_WPCR = 0x50574DFC;
    // CLOCK SELECTION
    PWM->PWM_CH_NUM[channel].PWM_CMR = clk_src;
    // PERIOD
    PWM->PWM_CH_NUM[channel].PWM_CPRD = cprd;
    // DUTY CYCLE
    PWM->PWM_CH_NUM[channel].PWM_CDTY = cdty;

    // Enable Channel
    PWM->PWM_ENA = (0x1 << channel);

    return VHAL_OK;
}

int _setPWMPinSynch(int vpin, uint32_t clk_src, uint32_t cprd, uint32_t cdty) {
    uint32_t channel = PIN_CLASS_DATA0(vpin);

    // Disable the channel first
    PWM->PWM_DIS = (0x1 << channel);
    PWM->PWM_DIS = (0x1); //ch 0
    // Disable Write Protection for PWM
    PWM->PWM_WPCR = 0x50574DFC;
    // CLOCK SELECTION
    PWM->PWM_CH_NUM[0].PWM_CMR = clk_src;
    // PERIOD
    PWM->PWM_CH_NUM[0].PWM_CPRD = cprd;
    // DUTY CYCLE
    PWM->PWM_CH_NUM[channel].PWM_CDTY = cdty;
    // COMPARATORS
    PWM->PWM_CMP[channel].PWM_CMPV = cdty;
    PWM->PWM_CMP[channel].PWM_CMPM = 1;

    //SYNCH
    PWM->PWM_SCM |= 1 | (1 << channel);

    // Enable Channel
    //PWM->PWM_ENA = (0x1 << channel);
    PWM->PWM_ENA = (0x1);

    return VHAL_OK;
}

int _calcPWMParams(uint32_t period, uint32_t pulse, uint32_t *clk_src, uint32_t *cprd, uint32_t *cdty) {
    if (period <= pulse)
        return VHAL_GENERIC_ERROR;

    uint32_t period_in_us = GET_TIME_MICROS(period);
    uint32_t pulse_in_us = GET_TIME_MICROS(pulse);
    //minimum needed prescaler
    uint32_t psc = (period_in_us * TICKS_PER_MUSEC) / 65536;
    uint32_t mainpsc = 0;
    uint32_t prescaler = (0x1 << mainpsc);
    while (psc > 0) {
        psc = psc / 2;
        prescaler = prescaler * 2;
        mainpsc++;
    }

    if ((period_in_us * TICKS_PER_MUSEC) > (prescaler * 65536)) {
        prescaler = prescaler * 2;
        mainpsc++;
    }

    if (mainpsc > 12) //not greater than 4096
        return -1;

    *cprd = (period_in_us * TICKS_PER_MUSEC) / (prescaler);
    *cdty = (pulse_in_us * TICKS_PER_MUSEC) / (prescaler);
    *clk_src = mainpsc;

    return VHAL_OK;
}

void vhalIRQ_PWM(void) {
    uint32_t sr = PWM->PWM_ISR2 & PWM->PWM_IMR2;
    uint32_t ch;

    vosEnterIsr();
    vosSysLockIsr();
    if (pwm_pwm.ch < 8) {
        if (sr & (PWM_ISR2_CMPM0 << pwm_pwm.ch)) {
            //ch is active
            if (pwm_pwm.thread) {
                pwm_pwm.npulses--;
                if (!pwm_pwm.npulses) {
                    PWM->PWM_IDR2 = PWM_IDR2_CMPM0 << pwm_pwm.ch;
                    PWM->PWM_DIS = 0x1 << pwm_pwm.ch;
                    vhalPinSetMode(pwm_pwm.vpin, PINMODE_OUTPUT_PUSHPULL);
                    pwm_pwm.ch = 0xffff;
                    PWM->PWM_DIS = 0x1;
                    PWM->PWM_SCM &= ~(0xff);
                    if ((PWM->PWM_SR & 0xFF) == 0)
                        PMC->PMC_PCER1 &= ~PMC_PCER1_PID36;
                    vosThResumeIsr(pwm_pwm.thread);
                    pwm_pwm.thread = NULL;
                }
            }
        }
    }
    vosSysUnlockIsr();
    vosExitIsr();
}

int _pwm_pwm_start(int vpin, uint32_t period, uint32_t pulse, uint32_t npulses) {
    uint32_t channel = PIN_CLASS_DATA0(vpin);
    int ret = VHAL_OK;

    vosSysLock();
    if ((TIME_IS_ZERO(period) || TIME_IS_ZERO(pulse) || period < pulse)) {
        // Check if PWM channel is active
        if (PWM_CHANNEL_IS_ACTIVE(channel)) {
            PWM->PWM_DIS = 0x1 << channel;
            // Disable Peripheral Clock if every PWM Channel is disabled
            if ((PWM->PWM_SR & 0xFF) == 0)
                PMC->PMC_PCER1 &= ~PMC_PCER1_PID36;
        }
        goto _ret;
    }

    printf("Asked npulses %i and %x on %x\n", npulses, PWM->PWM_SR, vpin);
    if (npulses && PWM->PWM_SR & 1) {
        //to count pulses channel 0 must be free
        ret = VHAL_HARDWARE_STATUS_ERROR;
        goto _ret;
    }

    uint32_t clk_src = PWM_CMR_CPRE_MCK;
    uint32_t cprd;
    uint32_t cdty;

    if (_calcPWMParams(period, pulse, &clk_src, &cprd, &cdty) != VHAL_OK) {
        printf("period %i pulse %i => PSC %i PRD %i DTY %i\n", GET_TIME_MICROS(period), GET_TIME_MICROS(pulse), clk_src, cprd, cdty);
        printf("err\n");
        ret = VHAL_GENERIC_ERROR;
        goto _ret;
    }

    printf("period %i pulse %i => PSC %i PRD %i DTY %i\n", GET_TIME_MICROS(period), GET_TIME_MICROS(pulse), clk_src, cprd, cdty);
    // Enable Peripheral Clock for PWM
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    //DIVA = 2 DIVB =4 PREA = lck/1024 PREB = clk/0124 --> total clk/4096
    PWM->PWM_CLK = (10 << 8) | (10 << 24) | 2 | (4 << 16);

    //vhalPinSetToPeripheral(vpin, PRPH_PWM, SAM3X_PIN_PR(vpin) | PAL_MODE_OUTPUT_PUSHPULL);

    vhalPinWrite(vpin, 0);
    _enablePWMPin(vpin);


    //TODO: handle H channels
    //cdty = (PIN_CLASS_DATA2(vpin)==1) ? cdty:(period-cdty);
    if (npulses) {
        //blocking
        printf("PWM with npulses %i %i\n", npulses, channel);
        pwm_pwm.thread = vosThCurrent();
        pwm_pwm.npulses = npulses;
        pwm_pwm.vpin = vpin;
        pwm_pwm.ch = channel;
        vhalIrqEnable(PWM_IRQn);
        PWM->PWM_IER2 = PWM_IER2_CMPM0 << channel;
        _setPWMPinSynch(vpin, clk_src, cprd, cdty);
        vosThSuspend();
        vosSysUnlock();
        goto _exit;
    } else {
        _setPWMPin(vpin, clk_src, cprd, cdty);
    }

_ret:
    vosSysUnlock();
_exit:
    return ret;
}

int _pwm_tc_start(int vpin, uint32_t period, uint32_t pulse, uint32_t npulses) {
    // PWM Pin associated with TC MODULE
    int port = PIN_PORT_NUMBER(vpin);
    int pad = PIN_PAD(vpin);
    uint8_t periph = PIN_CLASS_DATA2(vpin);
    uint8_t tio = PIN_CLASS_DATA0(vpin);
    uint8_t channel = tio % 9;
    int ret = VHAL_OK;
    TIM_CHAN_TypeDef *timx = TIM(channel);
    tio = tio / 9;


    vosSysLock();
    //occupied channel
    if (tcstatus[channel] != TC_TO_PWM && tcstatus[channel] != TC_FREE) {
        ret = VHAL_INVALID_PIN;
        goto _ret;
    }

    if ((TIME_IS_ZERO(period) || TIME_IS_ZERO(pulse) || period < pulse)) {
        //set pin to pio: TODO, make this a function in vhal_gpio.c
        if (!tio) timx->TC_IDR.fields.CPAS = 1;
        else timx->TC_IDR.fields.CPBS = 1;
        pin_to_pio(port, pad);
        vhalPinSetMode(vpin, PINMODE_OUTPUT_PUSHPULL);
        tioc[tio][channel] = 0;
        if (!tioc[(tio + 1) % 2][channel]) {
            //disable channel
            timx->TC_CCR.fields.CLKDIS = 1;
            tcstatus[channel] = TC_FREE;
        }
        goto _ret;
    }

    /*
    //cant activate npulses on the same tc yet
    if ((npulses && tioc[(tio + 1) % 2][channel])||(tioc[(tio + 1) % 2][channel]==2)){
        ret = VHAL_HARDWARE_STATUS_ERROR;
        goto _ret;
    }*/

    //calc prd-->RC calc cdty--> RA or RB    CLOCK --->  MCK/2
    uint32_t cprd = GET_TIME_MICROS(period) * (TICKS_PER_MUSEC / 2);
    uint32_t cdty = GET_TIME_MICROS(pulse) * (TICKS_PER_MUSEC / 2);

    printf("tc: period %i pulse %i => PRD %i DTY %i\n", GET_TIME_MICROS(period), GET_TIME_MICROS(pulse), cprd, cdty);
    printf("tio: %i ch: %i port:%i pad: %i prph: %i\n", tio, channel, port, pad, periph);
    if (tioc[(tio + 1) % 2][channel] && cprd != timx->TC_RC) {
        //oops, !tio is active with a different period :(
        ret = VHAL_HARDWARE_STATUS_ERROR;
        goto _ret;
    }

    // Enable Peripheral Clock
    _timEnablePeriphClock(channel, 1);

    TIM_TypeDef *tim;
    tim = _correspondingTimerModule(channel);
    // Disable Write protection
    tim->TC_WPMR.fields.WPKEY = 0x54494D;
    tim->TC_WPMR.fields.WPEN = 0;


    timx->TC_CCR.fields.CLKDIS = 1;

    // Set Register RC
    timx->TC_RC = cprd;


    timx->TC_CMR.fields.capture.WAVE = 1;
    timx->TC_CMR.fields.waveform.WAVE = 1;

    timx->TC_CMR.fields.waveform.TCCLKS = 0; //--->MCK/2
    timx->TC_CMR.fields.waveform.CLKI = 0;
    timx->TC_CMR.fields.waveform.BURST = 0;
    timx->TC_CMR.fields.waveform.CPCSTOP = 0;
    timx->TC_CMR.fields.waveform.CPCDIS = 0;

    timx->TC_CMR.fields.waveform.EEVTEDG = 0;
    timx->TC_CMR.fields.waveform.EEVT = 1;
    timx->TC_CMR.fields.waveform.ENTRG = 0;

    timx->TC_CMR.fields.waveform.WAVSEL = 2; // UP Mode with automatic trigger on RC compare

    timx->TC_IDR.fields.CPCS = 1;
    //timx->TC_IDR.fields.CPAS = 1;
    //timx->TC_IDR.fields.CPBS = 1;
    timx->TC_IDR.fields.LDRAS = 1;
    timx->TC_IDR.fields.LDRBS = 1;
    timx->TC_IDR.fields.COVFS = 1;


    if (!tio) {
        //TIOA
        timx->TC_RA = cdty;
        timx->TC_CMR.fields.waveform.ACPA = 2; // When Counter = RA -> CLEAR
        timx->TC_CMR.fields.waveform.ACPC = 1; // When Counter = RC -> SET
        timx->TC_CMR.fields.waveform.AEEVT = 0;
        timx->TC_CMR.fields.waveform.ASWTRG = 1; //When Counter starts -> SET
        if (npulses) {
            timx->TC_IER.fields.CPAS = 1;
        } else {
            timx->TC_IDR.fields.CPAS = 1;
        }
    } else {
        //TIOB
        timx->TC_RB = cdty;
        timx->TC_CMR.fields.waveform.BCPB = 2; // When Counter = RB -> CLEAR
        timx->TC_CMR.fields.waveform.BCPC = 1; // When Counter = RC -> SET
        timx->TC_CMR.fields.waveform.BEEVT = 0;
        timx->TC_CMR.fields.waveform.BSWTRG = 1; //When Counter starts -> SET
        if (npulses) {
            timx->TC_IER.fields.CPBS = 1;
        } else {
            timx->TC_IDR.fields.CPBS = 1;
        }
    }
    pin_to_periph(port, pad, periph);


    //printf("timx %x\n", timx);
    //printf("RC: %i, RA: %i, RB:%i\n", timx->TC_RC, timx->TC_RA, timx->TC_RB);
    printf("TC_IMR: %x %i %i %i\n", timx->TC_IMR, tio, npulses, channel);
    //printf("PMC CSR0: %x CSR1: %x\n", PMC->PMC_PCSR0, PMC->PMC_PCSR1);
    //printf("\n\n");
    // Fire up the timer
    //timx->TC_CCR.fields.CLKEN = 1;
    //timx->TC_CCR.fields.SWTRG = 1;
    tcstatus[channel] = TC_TO_PWM;
    if (npulses) {
        tim_c[channel].pwm.thread[tio] = vosThCurrent();
        tim_c[channel].pwm.npulses[tio] = npulses;
        tim_c[channel].pwm.vpin[tio] = vpin;
        vhalIrqEnable(TC0_IRQn + channel);
        tioc[tio][channel] = 2;
        timx->TC_CCR.value = 0b101;
        vosThSuspend();
        vosSysUnlock();
        goto _exit;
    } else {
        tioc[tio][channel] = 1;
        timx->TC_CCR.value = 0b101;
    }

_ret:
    vosSysUnlock();
_exit:
    return ret;
}

int vhalPwmStart(int vpin, uint32_t period, uint32_t pulse, uint32_t npulses) {

    int ret = VHAL_OK;
    if (!PIN_HAS_PRPH(vpin, PRPH_PWM)) {
        printf("invalid pin\n");
        return VHAL_INVALID_PIN;
    }

    if (PIN_CLASS_DATA1(vpin)) {
        //tc pwm
        ret = _pwm_tc_start(vpin, period, pulse, npulses);
    } else {
        //pwm pwm
        ret = _pwm_pwm_start(vpin, period, pulse, npulses);
    }

    return ret;
}

#endif

#if defined(VHAL_ICU)

int vhalIcuStart(int vpin, uint32_t cfg, uint32_t time_window, uint32_t *buffer, uint32_t *bufsize) {
    int ret = VHAL_GENERIC_ERROR;

    vosSemWait(icu_sem);
    if (!PIN_HAS_PRPH(vpin, PRPH_ICU)) {
        ret = VHAL_INVALID_PIN;
        goto _ret;
    }

    // Pull-up or Pull-down (Only Pull-up is supported)
    uint32_t icu_input = ICU_CFG_GET_INPUT(cfg);
    uint32_t trigger = ICU_CFG_GET_TRIGGER(cfg);

    // Obtain the corresponding TC channel and peripheral data
    int port = PIN_PORT_NUMBER(vpin);
    int pad = PIN_PAD(vpin);
    uint8_t pio_prph = PIN_CLASS_DATA2(vpin);
    uint8_t tio = PIN_CLASS_DATA0(vpin);
    uint8_t channel = tio % 9;
    int tmh = channel;
    TIM_CHAN_TypeDef *timx = TIM(tmh);
    tio = tio / 9; //Only TIOA can be used as ICU


    if (tcstatus[tmh] != TC_FREE && tcstatus[tmh] != TC_TO_ICU) {
        ret = VHAL_INVALID_PIN;
        goto _ret;
    }

    // Enable Peripheral Clock
    _timEnablePeriphClock(tmh, 1);

    TIM_TypeDef *tim;
    tim = _correspondingTimerModule(tmh);


    // Enable Peripheral A/B on pin
    // TODO: get port register directly from vpin

    if (icu_input == ICU_INPUT_PULLUP)
        vhalPinSetToPeripheral(vpin, PRPH_ICU, SAM3X_PIN_PR(vpin) | TIM_MODE_OUTPUT_PUSHPULL | TIM_MODE_INPUT_PULLUP);
    else
        vhalPinSetToPeripheral(vpin, PRPH_ICU, SAM3X_PIN_PR(vpin) | TIM_MODE_OUTPUT_PUSHPULL);

    pin_to_periph(port, pad, pio_prph);
    uint32_t prescaler = 0;
    uint32_t register_c = 0;
    _timeCalcRegisters(time_window, &prescaler, &register_c, TC_CMR_TIMER_CLOCK1);

    //tim_c[tmh].fn = NULL;
    //tim_c[tmh].args = NULL;
    tcstatus[tmh] = TC_TO_ICU;
    tim_c[tmh].icu.events = 0;
    tim_c[tmh].icu.buffer = buffer;
    tim_c[tmh].icu.bufsize = bufsize;
    tim_c[tmh].icu.started = FALSE;
    tim_c[tmh].icu.thread = (void *)vosThCurrent();
    tim_c[tmh].icu.twrc = register_c;

    // Disable Write protection
    tim->TC_WPMR.fields.WPKEY = 0x54494D;
    tim->TC_WPMR.fields.WPEN = 0;


    timx->TC_CCR.fields.CLKDIS = 1;

    timx->TC_CMR.fields.capture.TCCLKS = prescaler;
    timx->TC_CMR.fields.capture.CLKI = 0;
    timx->TC_CMR.fields.capture.BURST = 0;
    timx->TC_CMR.fields.capture.LDBSTOP = 0;
    timx->TC_CMR.fields.capture.LDBDIS = 0;
    timx->TC_CMR.fields.capture.ETRGEDG = 0;
    timx->TC_CMR.fields.capture.ABETRG = 0;
    timx->TC_CMR.fields.capture.CPCTRG = 1;

    if (trigger == ICU_TRIGGER_HIGH) {
        timx->TC_CMR.fields.capture.LDRA = 1; // RISING EDGE
        timx->TC_CMR.fields.capture.LDRB = 2; // FALLING EDGE
    }
    else {
        timx->TC_CMR.fields.capture.LDRA = 2; // FALLING EDGE
        timx->TC_CMR.fields.capture.LDRB = 1; // RISING EDGE
    }

    timx->TC_CMR.fields.capture.WAVE = 0;
    timx->TC_CMR.fields.waveform.WAVE = 0;

    _enableCorrespondingTimerIrq(tmh, 1);

    timx->TC_IER.fields.CPCS = 1;
    timx->TC_IER.fields.LDRAS = 1;
    timx->TC_IER.fields.LDRBS = 1;
    timx->TC_IER.fields.COVFS = 1;


    // Set Register RC
    timx->TC_RC = register_c;

    printf("timx %x\n", timx);
    printf("tio: %i ch: %i port:%i pad: %i prph: %i\n", tio, channel, port, pad, pio_prph);
    printf("ICU: RC %i CMR: %x\n", timx->TC_RC, timx->TC_CMR);

    // Fire up the timer
    //timx->TC_CCR.fields.CLKEN = 1;
    //timx->TC_CCR.fields.SWTRG = 1;
    tioc[tio][channel] = 1;
    timx->TC_CCR.value = 0b101;

    //blocking
    ret = VHAL_OK;
    vosSemSignal(icu_sem);
    vosSysLock();
    vosThSuspend();
    vosSysUnlock();
    vosSemWait(icu_sem);

_ret:
    vosSemSignal(icu_sem);
    return ret;
}

#endif


#if defined(VHAL_HTM)

//TODO: let HTM play together with ICU & PWM: tcstatus & co

int vhalHtmGetFreeTimer() {
    int prphs = PERIPHERAL_NUM(htm);
    int idx;
    int tmh;
    vosSysLock();
    for (idx = 0; idx < prphs; idx++) {
        tmh = GET_PERIPHERAL_ID(htm, idx);
        if (tcstatus[tmh] == TC_FREE) {
            goto end_fn;
        }
    }
    tmh = VHAL_GENERIC_ERROR;
end_fn:
    vosSysUnlock();
    return tmh;
}

int _set_htm_timer(uint32_t tm, uint32_t delay, htmFn fn, uint8_t oneshot, void *args) {
    if (TIME_IS_ZERO(delay)) {
        return _timReset(tm);
    }
    else {
        return _timSetSimple(tm, delay, oneshot, fn, args);
    }
}

int vhalHtmOneShot(uint32_t tm, uint32_t delay, htmFn fn, void *args, uint32_t blocking) {
    int ret = VHAL_OK;
    vosSysLock();
    ret = _set_htm_timer(tm, delay, fn, 1,args);
    if (blocking) {
        tim_c[tm].htm.thread = vosThCurrent();
        vosThSuspend();
    }
    vosSysUnlock();
    return ret;
}

int vhalHtmRecurrent(uint32_t tm, uint32_t delay, htmFn fn, void *args) {
    return _set_htm_timer(tm, delay, fn, 0, args);
}


int vhalSleepMicros(uint32_t tm, uint32_t micros) {
    int ret = VHAL_OK;
    vosSysLock();
    ret = _set_htm_timer(tm, TIME_U(micros,MICROS), NULL, 1, NULL);
    tim_c[tm].htm.thread = vosThCurrent();
    if(ret==VHAL_OK)
        vosThSuspend();
    vosSysUnlock();
    return ret;
}


#endif

#else


#endif



