#include "viper_test.h"


volatile uint32_t flag = 0;
volatile int idir = 0;
int btn = D2;

void callback(int slot, int dir) {
	idir = dir;
	flag = 1;
	printf("cbk %i %i\n",slot,dir);
}

TEST(ext_falling) {
	int cloop = 0;
	flag = 0;
	idir = 2;
	int slot = vhalPinAttachInterrupt(btn, PINMODE_EXT_FALLING|PINMODE_INPUT_PULLDOWN, callback);
	TEST_ASSERT("vhalPinAttachInterrupt(btn)<0", slot >= 0);
	printf("Please generate a falling interrupt on btn\n");
	while (!flag) {
		vosThSleep(TIME_U(500, MILLIS));
		cloop++;
		if (cloop > 20) {
			return TEST_KO("Interrupt not generated :(");
		}
	}
	vhalPinAttachInterrupt(btn, 0, NULL);
	TEST_ASSERT("Interrupt direction should be 0", idir == 0);
	return TEST_OK();
}


TEST(ext_rising) {
	int cloop = 0;
	flag = 0;
	idir = 2;
	int slot = vhalPinAttachInterrupt(btn, PINMODE_EXT_RISING|PINMODE_INPUT_PULLDOWN, callback);
	TEST_ASSERT("vhalPinAttachInterrupt(btn)<0", slot >= 0);
	printf("Please generate a rising interrupt on btn\n");
	while (!flag) {
		vosThSleep(TIME_U(500, MILLIS));
		cloop++;
		if (cloop > 20) {
			return TEST_KO("Interrupt not generated :(");
		}
	}
	vhalPinAttachInterrupt(btn, 0, NULL);
	TEST_ASSERT("Interrupt direction should be 1", idir == 1);
	return TEST_OK();
}


TEST(ext_both) {
	int cloop = 0;
	flag = 0;
	idir = 2;
	int slot = vhalPinAttachInterrupt(btn, PINMODE_EXT_BOTH|PINMODE_INPUT_PULLDOWN, callback);
	TEST_ASSERT("vhalPinAttachInterrupt(btn)<0", slot >= 0);
	printf("Please generate an interrupt on btn\n");
	while (!flag) {
		vosThSleep(TIME_U(500, MILLIS));
		cloop++;
		if (cloop > 20) {
			return TEST_KO("Interrupt not generated :(");
		}
	}
	vhalPinAttachInterrupt(btn, 0, NULL);
	if (idir==0) printf("Falling interrupt!");
	else if(idir==1) printf("Rising interrupt!");
	else return TEST_KO("Unknown interrupt!");
	return TEST_OK();
}


TEST(ext_both2) {
	int cloop = 0;
	flag = 0;
	idir = 2;
	int slot = vhalPinAttachInterrupt(btn, PINMODE_EXT_FALLING|PINMODE_INPUT_PULLDOWN, callback);
	TEST_ASSERT("vhalPinAttachInterrupt(btn)<0", slot >= 0);
	slot = vhalPinAttachInterrupt(btn, PINMODE_EXT_RISING|PINMODE_INPUT_PULLDOWN, callback);
	TEST_ASSERT("vhalPinAttachInterrupt(btn)<0", slot >= 0);
	printf("Please generate an interrupt on btn\n");
	while (!flag) {
		vosThSleep(TIME_U(500, MILLIS));
		cloop++;
		if (cloop > 20) {
			return TEST_KO("Interrupt not generated :(");
		}
	}
	vhalPinAttachInterrupt(btn, 0, NULL);
	if (idir==0) printf("Falling interrupt!");
	else if(idir==1) printf("Rising interrupt!");
	else return TEST_KO("Unknown interrupt!");
	return TEST_OK();
}


BEGIN_TEST("EXT TEST"),
           ADD_TEST(ext_rising, "Rising Interrupt on btn"),
           ADD_TEST(ext_falling, "Falling Interrupt on btn"),
           ADD_TEST(ext_both, "Both Edges Interrupt on btn"),
           ADD_TEST(ext_both, "Both Edges Interrupt on btn"),
           ADD_TEST(ext_both, "Both Edges (R+F) Interrupt on btn"),
           ADD_TEST(ext_both, "Both Edges (R+F) Interrupt on btn"),
           END_TEST();
