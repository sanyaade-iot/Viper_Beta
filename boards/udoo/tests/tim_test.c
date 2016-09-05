#include "viper_test.h"
#include "vhal_tim.h"

volatile int sw = 1;
volatile uint32_t t;
uint32_t led = LED0;
volatile int oo =0;

void ledda(uint32_t tm, void *args){
	(void) tm;
	t = *(uint32_t*) args;
	if(sw) vhalPinWrite(t, 1);
    else vhalPinWrite(t, 0);
    sw = !sw;
  oo++;
}


TEST(tim1) {
    int ttt=oo;
    vhalInitTIM(NULL);
    printf("TEST TIM1\n");
    vhalHtmRecurrent(3, TIME_U(4, SECONDS), &ledda, &led);
    while(oo<6){
      vosSysLock();
      if(ttt!=oo){
        printf("tick! %i\n",oo);
        ttt=oo;
      }
      vosSysUnlock();
    }
    printf("#: %i\n", oo);

    return TEST_OK();
}

TEST(timinput){
  vhalInitTIM(NULL);
  printf("Attivo pwm su 1.1 (D7)\n");
  vhalPwmStart(PWM13,  TIME_U(2, SECONDS),TIME_U(1, SECONDS));
  printf("Attivo icu su 4.1 (D10)\n");
  vhalIcuStart(PWM10, NULL);
  printf("Aspetto come uno scemo\n");
  testtest();
  printf("END\n");
  return TEST_OK();
}


TEST(allpwm) {

    vhalInitTIM(NULL);
    //vhalHtmRecurrent(2, TIME_U(2, SECONDS), &ledda, &led);

    printf("TEST PWM TIM1\n");
    vhalPwmStart(PWM7,  TIME_U(6, SECONDS),TIME_U(4, SECONDS));
    vhalPwmStart(PWM8,  TIME_U(6, SECONDS),TIME_U(3, SECONDS));
    vhalPwmStart(PWM18, TIME_U(6, SECONDS),TIME_U(2, SECONDS));
    //vhalPwmStart(PWM2,  TIME_U(6, SECONDS),TIME_U(1, SECONDS)); /* MISS on board */

    printf("TEST PWM TIM2\n");
    vhalPwmStart(PWM13,  TIME_U(6, SECONDS),TIME_U(4, SECONDS));
    vhalPwmStart(PWM3,   TIME_U(6, SECONDS),TIME_U(3, SECONDS));
    //vhalPwmStart(PWM18, TIME_U(6, SECONDS),TIME_U(2, SECONDS)); /* Serial 1!!!!*/
    //vhalPwmStart(PWM2,  TIME_U(6, SECONDS),TIME_U(1, SECONDS));

    printf("TEST PWM TIM3\n");
    vhalPwmStart(PWM12,  TIME_U(6, SECONDS),TIME_U(4, SECONDS));
    vhalPwmStart(PWM9,   TIME_U(6, SECONDS),TIME_U(3, SECONDS));
    vhalPwmStart(PWM5,   TIME_U(6, SECONDS),TIME_U(4, SECONDS));
    vhalPwmStart(PWM4,   TIME_U(6, SECONDS),TIME_U(3, SECONDS));
    //vhalPwmStart(PWM18, TIME_U(6, SECONDS),TIME_U(2, SECONDS)); /* MISS on board */
    //vhalPwmStart(PWM2,  TIME_U(6, SECONDS),TIME_U(1, SECONDS)); /* MISS on board */

    printf("TEST PWM TIM4\n");
    vhalPwmStart(PWM10,  TIME_U(6, SECONDS),TIME_U(4, SECONDS));
    //vhalPwmStart(PWM8,  TIME_U(6, SECONDS),TIME_U(3, SECONDS)); /* MISS on board */
    vhalPwmStart(PWM15,  TIME_U(6, SECONDS),TIME_U(2, SECONDS));
    vhalPwmStart(PWM14,  TIME_U(6, SECONDS),TIME_U(1, SECONDS));


    return TEST_OK();
}


/* TEST ARRAY */


BEGIN_TEST("Testing the test suite"),
           //ADD_TEST(allpwm, "All PWM all CHAN test"),
           //ADD_TEST(tim1, "TIM1 simple test"),
           ADD_TEST(timinput, "ICU test"),
           END_TEST();