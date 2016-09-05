#include "viper_test.h"


// /* PWM0 */ MAKE_PIN_CLASS(0, 2, 4, 0),
// /* PWM1 */ MAKE_PIN_CLASS(1, 2, 3, 0),
// /* PWM2 */ MAKE_PIN_CLASS(2, 1, 3, 0),
// /* PWM3 */ MAKE_PIN_CLASS(3, 2, 2, 0),
// /* PWM4 */ MAKE_PIN_CLASS(4, 3, 2, 0),
// /* PWM5 */ MAKE_PIN_CLASS(5, 3, 1, 0),
// /* PWM6 */ MAKE_PIN_CLASS(6, 2, 3, 0),
// /* PWM7 */ MAKE_PIN_CLASS(7, 1, 1, 0),
// /* PWM8 */ MAKE_PIN_CLASS(8, 1, 2, 0),
// /* PWM9 */ MAKE_PIN_CLASS(9, 3, 2, 0),
// /* PWM10 */ MAKE_PIN_CLASS(10, 4, 1, 0),
// /* PWM11 */ MAKE_PIN_CLASS(11, 1, 1, 1),
// /* PWM12 */ MAKE_PIN_CLASS(12, 3, 1, 0),
// /* PWM13 */ MAKE_PIN_CLASS(13, 2, 1, 0),
// /* PWM14 */ MAKE_PIN_CLASS(14, 4, 4, 0),
// /* PWM15 */ MAKE_PIN_CLASS(15, 4, 3, 0),
// /* PWM16 */ MAKE_PIN_CLASS(17, 2, 1, 0),
// /* PWM17 */ MAKE_PIN_CLASS(18, 2, 2, 0),
// /* PWM18 */ MAKE_PIN_CLASS(20, 1, 2, 1),



TEST(test_pwm) {
  int i;
  //0,1 are serial pins
  for (i = 2; i <= 18; i++) {
    printf("Starting pwm%i\n", i);
    vhalPwmStart(MAKE_VPIN(PINCLASS_PWM, i), TIME_U(1000, MICROS), TIME_U(50 * (i + 1), MICROS));
  }
  vosThSleep(TIME_U(500, MILLIS));
  /*for (i = 2; i <= 18; i++) {
    printf("Stopping pwm%i\n", i);
    vhalPwmStart(MAKE_VPIN(PINCLASS_PWM, i), 0, TIME_U(50 * (i + 1), MICROS));
  }*/
  return TEST_OK();
}


TEST(test_pwm2) {
  int i;
  for (i = 0; i < 100; i++) {
    vhalPwmStart(PWM2, TIME_U(1000, MICROS), TIME_U(50 + 9 * i, MICROS));
    vosThSleep(TIME_U(100, MILLIS));
  }
  vhalPwmStart(PWM2, 0, 0);
  return TEST_OK();
}

TEST(test_pwm3) {
  int i, j;

  for (j = 0; j < 5; j++) {
    for (i = 0; i < 100; i++) {
      vhalPwmStart(PWM13, TIME_U(1000, MICROS), TIME_U(1000-10*i, MICROS));
      vosThSleep(TIME_U(10, MILLIS));
    }
    for (i = 0; i < 100; i++) {
      vhalPwmStart(PWM13, TIME_U(1000, MICROS), TIME_U(10*(i+1), MICROS));
      vosThSleep(TIME_U(10, MILLIS));
    }
  }
  vhalPwmStart(PWM13, 0, 0);
  return TEST_OK();
}


TEST(test_pwm4) {
  int i;
  for (i = 0; i < 100; i++) {
    vhalPwmStart(PWM11, TIME_U(1000, MICROS), TIME_U(50 + 9 * i, MICROS));
    vosThSleep(TIME_U(100, MILLIS));
  }
  vhalPwmStart(PWM11, 0, 0);
  return TEST_OK();
}



/* TEST ARRAY */


BEGIN_TEST("Testing Timers"),
           ADD_TEST(test_pwm, "PWM"),
           //ADD_TEST(test_pwm2, "PWM RAMP"),
          //ADD_TEST(test_pwm3, "PWM LED"),
          //ADD_TEST(test_pwm4, "PWM COMPLEMENTARY"),
           //ADD_TEST(test_htm_rec, "Recurrent Timers"),
           //ADD_TEST(test_calc_time, "Calc Time"),
           END_TEST();
