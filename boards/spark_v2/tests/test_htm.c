#include "viper_test.h"

volatile int flag = 0;
volatile int count = 0;
uint8_t timers[] = {0, 1, 2, 3};

void htm_cbk(uint32_t tm, void *args) {
  flag = tm + 1;
}

void htm2_cbk(uint32_t tm, void *args) {
  count++;
  if (count == 1000)
    flag = tm + 1;
}


TEST(test_htm) {
  vosThSleep(TIME_U(2000,MILLIS));
  int i;
  for (i = 0; i < sizeof(timers); i++) {
    flag = 0;
    printf("One Shot Timer in 1 sec on timer %i\n", timers[i] + 1);
    vhalHtmOneShot(timers[i], TIME_U(1, SECONDS), &htm_cbk, NULL);
    while (!flag);
    printf("Ok!\n");
  }
  return TEST_OK();
}

TEST(test_htm_rec) {
  int i;
  for (i = 0; i < sizeof(timers); i++) {
    flag = 0;
    count = 0;
    printf("Recurrent Timer in 1 msec on timer %i x1000\n", timers[i] + 1);
    vhalHtmRecurrent(timers[i], TIME_U(1, MILLIS), &htm2_cbk, NULL);
    while (!flag);
    vhalHtmRecurrent(timers[i], 0,NULL,NULL);
    printf("Ok!\n");
  }
  return TEST_OK();
}


/* TEST ARRAY */


BEGIN_TEST("Testing Timers"),
           ADD_TEST(test_htm, "One Shot Timers"),
           ADD_TEST(test_htm_rec, "Recurrent Timers"),
          //ADD_TEST(test_calc_time, "Calc Time"),
           END_TEST();