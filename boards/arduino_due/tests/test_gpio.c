#include "viper_test.h"





TEST(blink) {
	int cloop;
	printf("Please connect D5 to D0: class: %i@%i port %x@%i pad %i\n",PIN_CLASS(D5),PIN_CLASS_ID(D5),PIN_PORT(D5),PIN_PORT_NUMBER(D5),PIN_PAD(D5));
	vhalPinSetMode(D5, PINMODE_OUTPUT_PUSHPULL);
	vhalPinSetMode(D0, PINMODE_INPUT_PULLDOWN);
	vhalPinWrite(D5, 1);
	TEST_ASSERT("D5 HI so D0 HI", vhalPinRead(D5) > 0);
	vosThSleep(TIME_U(1000,MILLIS));
	vhalPinWrite(D5, 0);
	TEST_ASSERT("D5 LO so D0 LO", vhalPinRead(D5) == 0);
	return TEST_OK();
}







BEGIN_TEST("GPIO_TEST"),
           ADD_TEST(blink, "D5 blink"),
           END_TEST();