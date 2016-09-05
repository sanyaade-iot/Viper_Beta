#include "viper_test.h"
#include "hal.h"
#include "vhal.h"
#include "vhal_gpio.h"
#include "vhal_tim.h"

volatile uint32_t triggered = 0;

void test_callback(uint32_t tm, void *args) {
    (void) tm;
    (void) args;
    vosSysLockIsr();
    triggered = 1;
    vosSysUnlockIsr();
}

TEST(tim_oneshot) {
    vhalInitTIM(NULL);
    printf("System Frequency: %d Hz\n", _system_frequency);
    
    // Resetting trigger flag
    vosSysLockIsr();
    triggered = 0;
    vosSysUnlockIsr();
    
    // Obtain a free timer first
    printf("Obtaining Free timer\n");
    int handle = vhalHtmGetFreeTimer();
    printf("Timer obtained: %d\n", handle);
    printf("Executing One Shot test...\n");
    TEST_ASSERT("Cannot set timer for one-shot", vhalHtmOneShot(handle, TIME_U(4, SECONDS), test_callback, NULL) == VHAL_OK);
    printf("One shot set. Waiting 4 seconds...\n");
    
    // Check status
    uint32_t *tc_cmr0_0_dump = (uint32_t *) (0x40080004);
    volatile uint32_t *tc_cv0_0_dump = (uint32_t *) (0x40080010);
    uint32_t *tc_rc0_0_dump = (uint32_t *) (0x4008001C);
    uint32_t *tc_sr0_0_dump = (uint32_t *) (0x40080020);
    uint32_t *tc_wpmr_dump = (uint32_t *) (0x400800E4);
    printf("TC_CMR_0_0 dump: %X\n", *tc_cmr0_0_dump);
    printf("TC_CV_0_0 dump: %X\n", *tc_cv0_0_dump);
    printf("TC_RC_0_0 dump: %X\n", *tc_rc0_0_dump);
    printf("TC_SR_0_0 dump: %X\n", *tc_sr0_0_dump);
    printf("TC_WPMR_0 dump: %X\n", *tc_wpmr_dump);
    
    while (1) {
        vosSysLock();
        if (triggered)
            break;
        vosSysUnlock();
    };
    vosSysUnlock();
    
    printf("Timer triggered!\n");
    
    return TEST_OK();
}

TEST(tim_recurrent) {
    vhalInitTIM(NULL);
    // Obtain a free timer first
    printf("Obtaining Free timer\n");
    int handle = vhalHtmGetFreeTimer();
    printf("Timer obtained: %d\n", handle);
    
    // Resetting Trigger Flag
    vosSysLockIsr();
    triggered = 0;
    vosSysUnlockIsr();
    
    printf("Executing test...\n");
    TEST_ASSERT("Cannot set timer for recurrent operation", vhalHtmRecurrent(handle, TIME_U(1, SECONDS), test_callback, NULL) == VHAL_OK);
    printf("Timer started\n");
    
    int i = 0;
    for (i = 0; i < 10; ++i) {
        while (1) {
            vosSysLock();
            if (triggered)
                break;
            vosSysUnlock();
        }
        triggered = 0;
        vosSysUnlock();
        printf("%d seconds elapsed\n", i+1);
    }
    
    printf("Stopping the timer...\n");
    vhalHtmRecurrent(handle, 0, NULL, NULL);
    
    return TEST_OK();
}

TEST(pwm_setup) {
    printf("Testing PWM setup on pin PWM8\n");
    int vpin = PWM8;
    vhalPinSetToPeripheral(vpin, PRPH_PWM, SAM3X_PIN_PR(vpin) | PAL_MODE_OUTPUT_PUSHPULL);
    uint32_t classid = PIN_CLASS_ID(vpin);
    
    int port = PIN_PORT_NUMBER(vpin);
    int pad = PIN_PAD(vpin);
    uint32_t channel = PIN_CLASS_DATA0(vpin);
    
    printf("Pin: %d\n", classid);
    printf("Port Number: %d\n", port);
    printf("Pad: %d\n", pad);
    printf("PWM Channel: %d\n", channel);
    return TEST_OK();
}

TEST(pwm_simple) {
    printf("Turning On PWM on Pin 20\n");
    
    int i =0;
    for(i=2;i<=8;i++){
        //vhalPinSetToPeripheral(MAKE_VPIN(PINCLASS_PWM,i), PRPH_PWM, SAM3X_PIN_PR(MAKE_VPIN(PINCLASS_PWM,i)) | PAL_MODE_OUTPUT_PUSHPULL);
        TEST_ASSERT("Cannot start PWM on pin 20", vhalPwmStart(MAKE_VPIN(PINCLASS_PWM,i), TIME_U(1000*i, MICROS), TIME_U(350, MICROS)) == VHAL_OK);
    }
    /*TEST_ASSERT("Cannot start PWM on pin 20", vhalPwmStart(PWM4, TIME_U(100, MICROS), TIME_U(25, MICROS)) == VHAL_OK);
    TEST_ASSERT("Cannot start PWM on pin 20", vhalPwmStart(PWM5, TIME_U(100, MICROS), TIME_U(35, MICROS)) == VHAL_OK);
    TEST_ASSERT("Cannot start PWM on pin 20", vhalPwmStart(PWM6, TIME_U(100, MICROS), TIME_U(45, MICROS)) == VHAL_OK);
    TEST_ASSERT("Cannot start PWM on pin 20", vhalPwmStart(PWM7, TIME_U(100, MICROS), TIME_U(55, MICROS)) == VHAL_OK);
    TEST_ASSERT("Cannot start PWM on pin 20", vhalPwmStart(PWM8, TIME_U(100, MICROS), TIME_U(65, MICROS)) == VHAL_OK);
    */
    uint32_t *x = (uint32_t *) (0x40094200);
    printf("PWM_CMR_0 dump: %X\n", *x);
    x = (uint32_t *) (0x40094204);
    printf("PWM_CDTY_0 dump: %X\n", *x);
    x = (uint32_t *) (0x4009420C);
    printf("PWM_CPRD_0 dump: %X\n", *x);
    x = (uint32_t *) (0x400940E8);
    printf("PWM_WPSR_0 dump: %X\n", *x);
    x = (uint32_t *) (0x400E1008);
    printf("PIOB_PSR dump: %X\n", *x);
    x = (uint32_t *) (0x400E1070);
    printf("PIOB_ABSR dump: %X\n", *x);
    
    /*vosSysLockIsr();
    triggered = 0;
    vosSysUnlockIsr();
    int handle = vhalHtmGetFreeTimer();
    printf("Waiting 4 seconds...\n");
    int result = vhalHtmOneShot(handle, TIME_U(4, SECONDS), test_callback, NULL);
    
    while (1) {
        vosSysLock();
        if (triggered)
            break;
        vosSysUnlock();
    };
    vosSysUnlock();
    */
    while(1);

    printf("Turning Off PWM on Pin 20\n");
    TEST_ASSERT("Cannot turning off PWM on pin 20", vhalPwmStart(PWM8, 0, 0) == VHAL_OK);
        
    return TEST_OK();
}

uint32_t icu_buffer[32];
vhalIcuConf conf;
volatile uint32_t icu_triggered = 0;
volatile uint32_t icu_finished = 0;

int icu_cb(int dir, uint32_t time, int pos) {
    vosSysLockIsr();
    icu_triggered = 1;
    vosSysUnlockIsr();
    return VHAL_OK;
}

void icu_end_cb(void *conf) {
    vosSysLockIsr();
    icu_finished = 1;
    vosSysUnlockIsr();
}

TEST(icu_simple) {
    printf("Turning On ICU on Pin 2\n");
    conf.args = NULL;
    conf.buffer = icu_buffer;
    conf.bufsize = 32;
    conf.time_window = TIME_U(5000, MILLIS);
    conf.callback = NULL;//icu_cb;
    conf.endcbk = NULL;//icu_end_cb;
    conf.cfg = ICU_CFG(ICU_MODE_BOTH, ICU_TRIGGER_HIGH, 0, ICU_INPUT_PULLUP);
    
    int vpin = ICU0;
    uint32_t classid = PIN_CLASS_ID(vpin);
    
    int port = PIN_PORT_NUMBER(vpin);
    int pad = PIN_PAD(vpin);
    uint32_t channel = PIN_CLASS_DATA0(vpin);
    
    printf("Pin: %d\n", classid);
    printf("Port Number: %d\n", port);
    printf("Pad: %d\n", pad);
    printf("TC Channel: %d\n", channel);
    
    vosSysLock();
    icu_finished = 0;
    vosSysUnlock();
    
    TEST_ASSERT("Cannot turn on PWM2 ",vhalPwmStart(PWM2,TIME_U(1000,MICROS),TIME_U(200,MICROS))==VHAL_OK);
    int result = vhalIcuStart(ICU0, &conf);
    printf("Starting ICU result: %d\n", result);
    TEST_ASSERT("Cannot turn on ICU on pin 2", result == VHAL_OK);
    
    // Check status
    volatile uint32_t *tc_cmr0_0_dump = (uint32_t *) (0x40080004);
    volatile uint32_t *tc_cv0_0_dump = (uint32_t *) (0x40080010);
    volatile uint32_t *tc_rc0_0_dump = (uint32_t *) (0x4008001C);
    volatile uint32_t *tc_sr0_0_dump = (uint32_t *) (0x40080020);
    volatile uint32_t *tc_wpmr_dump = (uint32_t *) (0x400800E4);
    printf("TC_CMR_0_0 dump: %X\n", *tc_cmr0_0_dump);
    printf("TC_CV_0_0 dump: %X\n", *tc_cv0_0_dump);
    printf("TC_RC_0_0 dump: %X\n", *tc_rc0_0_dump);
    printf("TC_SR_0_0 dump: %X\n", *tc_sr0_0_dump);
    printf("TC_WPMR_0 dump: %X\n", *tc_wpmr_dump);
    
    uint32_t events = 0;
    /*
    while (1) {
        vosSysLock();
        if (icu_finished)
            break;
        vosSysUnlock();
        
        vosSysLock();
        if (icu_triggered) {
            vosSysUnlock();
            //printf("ICU shot!\n");
            events++;
            vosSysLock();
            icu_triggered = 0;
        }
        vosSysUnlock();
    };
    vosSysUnlock();
    */    
    uint8_t i;
    for (i = 0; i < conf.bufsize; ++i)
        printf("Result: %d - %d\n", i, conf.buffer[i]);
    
    printf("Icu operation finished. # of events: %d\n", events);
    
    while(1);
    return TEST_OK();
}

BEGIN_TEST("Testing the test suite"),
    //ADD_TEST(tim_oneshot, "Simple One Shot Timer Test"),
    //ADD_TEST(tim_recurrent, "Simple Recurrent Timer Test - Iteration 1"),
    //ADD_TEST(tim_recurrent, "Simple Recurrent Timer Test - Iteration 2"),
    //ADD_TEST(pwm_setup, "PWM Set up test"),
    //ADD_TEST(pwm_simple, "Simple PWM Test"),
    ADD_TEST(icu_simple, "Simple ICU Test"),
END_TEST();
