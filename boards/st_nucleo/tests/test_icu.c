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

vhalIcuConf conf;
vhalIcuConf conf2;
uint32_t buffer[128];

extern volatile int icuflag;

TEST(test_icu) {
	conf.time_window = TIME_U(2000, MILLIS);
	conf.cfg = ICU_CFG(ICU_MODE_BOTH, ICU_TRIGGER_HIGH, 0,ICU_INPUT_PULLUP);
	conf.bufsize = 16;
	conf.buffer = buffer;
	conf.callback = NULL;
	conf.endcbk=NULL; //blocking
	vhalPwmStart(PWM6,TIME_U(1000,MILLIS),TIME_U(10,MILLIS));
	vhalIcuStart(PWM2,&conf);
	printf("Done! %i\n",icuflag);
	int i;
	for(i=0;i<4*conf.bufsize;i++){
		printf("%i. %i\n",i,conf.buffer[i]);
		//printf("%i. m %i\n",i,conf.buffer[i]);
	}
	return TEST_OK();
}


volatile int captures=0;
int end1(void *args){
	(void)args;
	captures++;
	return 0;
}


/*
TEST(test_icu2) {
	conf.time_window = TIME_U(4000, MILLIS);
	conf.cfg = ICU_CFG(ICU_MODE_BOTH, ICU_TRIGGER_HIGH, 0);
	conf.bufsize = 16;
	conf.buffer = buffer;
	conf.callback = NULL;
	conf.endcbk=end1; //non-blocking
	conf2.time_window = TIME_U(4000, MILLIS);
	conf2.cfg = ICU_CFG(ICU_MODE_BOTH, ICU_TRIGGER_HIGH, 0);
	conf2.bufsize = 16;
	conf2.buffer = (buffer+32);
	conf2.callback = NULL;
	conf2.endcbk=end1; //non-blocking

	vhalPwmStart(PWM6,TIME_U(1500,MILLIS),TIME_U(600,MILLIS));
	vhalPwmStart(PWM2,TIME_U(1500,MILLIS),TIME_U(200,MILLIS));
	
	//vhalIcuStart(PWM7,&conf2);
	vhalIcuStart(PWM4,&conf);
	while(captures<1);
	printf("Done!\n");
	int i;
	for(i=0;i<48;i++){
		printf("%i. %i\n",i,conf.buffer[i]);
		//printf("%i. m %i\n",i,GET_TIME_MICROS(conf.buffer[i]));
	}
	return TEST_OK();
}
*/

/* TEST ARRAY */


BEGIN_TEST("Testing Timers"),
           ADD_TEST(test_icu, "ICU"),
           //ADD_TEST(test_icu2, "ICU2"),
           //ADD_TEST(test_pwm2, "PWM RAMP"),
           //ADD_TEST(test_pwm3, "PWM LED"),
           //ADD_TEST(test_pwm4, "PWM COMPLEMENTARY"),
           //ADD_TEST(test_htm_rec, "Recurrent Timers"),
           //ADD_TEST(test_calc_time, "Calc Time"),
           END_TEST();
