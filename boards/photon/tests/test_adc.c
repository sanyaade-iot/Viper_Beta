#include "viper_test.h"



TEST(adc_wrong_pin) {
	TEST_EQUAL("vhalAdcGetPeripheralForPin(D0)!=-1", vhalAdcGetPeripheralForPin(D0), -1);
	return TEST_OK();
}

TEST(adc_good_pin) {
	int i;
	for (i = 0; i < PIN_CLASS_NUM(PINCLASS_ANALOG); i++) {
		printf("pin A%i...", i);
		TEST_ASSERT("vhalAdcGetPeripheralForPin(Ax)<0", vhalAdcGetPeripheralForPin(MAKE_VPIN(PINCLASS_ANALOG, i)) >= 0);
		printf("OK\n");
	}
	return TEST_OK();
}


TEST(adc_init) {
	vosThSleep(TIME_U(2000,MILLIS));
	vhalAdcConf conf;
	int i, cnt = 0;
	int adc = vhalAdcGetPeripheralForPin(A0);
	for (i = 0; i < 2000000; i += 100000) {
		conf.samples_per_second = i;
		int ret = vhalAdcInit(adc, &conf);
		vhalAdcDone(adc);
		if (ret < 0) {
			printf("%i Hz, unsupported\n", i);
		} else {
			printf("requested %i, got %i\n", i, conf.samples_per_second);
			cnt++;
		}

	}
	TEST_ASSERT("Should be 7 accepted frequencies", cnt == 7);
	return TEST_OK();
}

vhalAdcCaptureInfo nfo;
uint16_t buffer[10][16];
uint16_t npins[] = {A0, A1, A2, A3, A4, A5};
uint16_t mpins[] = {A0, A1, A2, A3, A4, A5, A5, A4, A3, A2, A1,A0,A1,A0,A2,A2};

TEST(adc_prepare) {
	int adc = vhalAdcGetPeripheralForPin(A0);
	nfo.npins = 0;
	TEST_ASSERT("no capture should return < 0", vhalAdcPrepareCapture(adc, &nfo) < 0);
	nfo.npins = 17;
	TEST_ASSERT(">16 capture should return < 0", vhalAdcPrepareCapture(adc, &nfo) < 0);
	nfo.npins = 1;
	uint16_t pin = A0;
	nfo.pins = &pin;
	nfo.samples = 1;
	TEST_ASSERT("single capture should return == 2", vhalAdcPrepareCapture(adc, &nfo) == 2);

	int i;
	for (i = 0; i < 6; i++)
		printf("A%i has class %i ch %i pad %i id %i\n", i, PIN_CLASS(npins[i]),PIN_CLASS_DATA0(npins[i]),PIN_PAD(npins[i]),PIN_CLASS_ID(npins[i]));
	nfo.npins = 6;
	nfo.pins = &npins;
	nfo.samples = 10;
	TEST_ASSERT("multiple capture should return sample_size*samples*npins == 120", vhalAdcPrepareCapture(adc, &nfo) == 120);
	return TEST_OK();
}


TEST(adc_capture_single) {
	blink(LED2,5);
	int adc = vhalAdcGetPeripheralForPin(A0);
	printf("* %i\n",adc);
	int ret = vhalAdcInit(adc, NULL);
	printf("** %i\n",ret);
	vosThSleep(TIME_U(1000,MILLIS));
	
	TEST_ASSERT("vhalAdcInit with NULL conf should return 0", ret == 0);
	
	nfo.npins = 1;
	uint16_t dummy = 0xffff;
	nfo.samples = 1;
	nfo.buffer = &dummy;
	nfo.capture_mode = ADC_CAPTURE_SINGLE;
	int i;
	int apins;
	for (apins = 0; apins < 6; apins++) {
		nfo.pins = &npins[apins];
		TEST_ASSERT("vhalAdcPrepareCapture should return 2", vhalAdcPrepareCapture(adc, &nfo) == 2);
		for (i = 0; i < 4; i++) {
			vhalAdcRead(adc, &nfo);
			printf("A%i read %i\n", apins, *((uint16_t *)nfo.buffer));
			TEST_ASSERT("adc value should be [0,4095]", (dummy <= 4095));
		}
		printf("\n");
	}
	vhalAdcDone(adc);
	return TEST_OK();
}


TEST(adc_capture_single3) {
	int adc = vhalAdcGetPeripheralForPin(A0);
	int ret = vhalAdcInit(adc, NULL);
	TEST_ASSERT("vhalAdcInit with NULL conf should return 0", ret == 0);
	nfo.npins = 1;
	uint16_t dummy[4];
	nfo.samples = 4;
	nfo.buffer = dummy;
	int i;
	int apins;
	for (apins = 0; apins < 6; apins++) {
		nfo.pins = &npins[apins];
		TEST_ASSERT("vhalAdcPrepareCapture should return 8", vhalAdcPrepareCapture(adc, &nfo) == 8);
		memset(dummy,0xff,sizeof(dummy));
		vhalAdcRead(adc, &nfo);
		for (i = 0; i < 4; i++) {
			printf("A%i read %i\n", apins, dummy[i]);
			TEST_ASSERT("adc value should be [0,4095]", (dummy[i] <= 4095));
		}
		printf("\n");
	}
	vhalAdcDone(adc);
	return TEST_OK();
}

TEST(adc_capture_single2) {
	vhalAdcConf conf;
	conf.samples_per_second=0;
	int adc = vhalAdcGetPeripheralForPin(A0);
	int ret = vhalAdcInit(adc, &conf);
	TEST_ASSERT("vhalAdcInit with samples_per_second=0 should return 0", ret == 0);
	TEST_ASSERT("obtained freq should be < 50000",conf.samples_per_second<50000);
	nfo.npins = 1;
	uint16_t dummy = 0xffff;
	nfo.samples = 1;
	nfo.buffer = &dummy;
	int i;
	int apins;
	for (apins = 0; apins < 6; apins++) {
		nfo.pins = &npins[apins];
		TEST_ASSERT("vhalAdcPrepareCapture should return 2", vhalAdcPrepareCapture(adc, &nfo) == 2);
		for (i = 0; i < 4; i++) {
			vhalAdcRead(adc, &nfo);
			printf("A%i read %i\n", apins, *((uint16_t *)nfo.buffer));
			TEST_ASSERT("adc value should be [0,4095]", (dummy <= 4095));
		}
		printf("\n");
	}
	vhalAdcDone(adc);
	return TEST_OK();
}


TEST(adc_capture_multiple) {
	vhalAdcConf conf;
	conf.samples_per_second = 0;
	int adc = vhalAdcGetPeripheralForPin(A0);
	int ret = vhalAdcInit(adc, &conf);
	TEST_ASSERT("vhalAdcInit with NULL conf should return 0", ret == 0);
	nfo.npins = 16;
	nfo.samples = 10;
	nfo.buffer = buffer;
	nfo.pins = mpins;
	TEST_ASSERT("vhalAdcPrepareCapture should return 320", vhalAdcPrepareCapture(adc, &nfo) == 320);
	memset(buffer,0xff,sizeof(buffer));
	int i,j;
	vhalAdcRead(adc, &nfo);
	for(i=0;i<16;i++){
		for(j=0;j<10;j++){
			printf("A%i[%i] = %i\n",PIN_CLASS_PAD(mpins[i]),j,buffer[j][i]);
			TEST_ASSERT("adc value should be [0,4095]",(buffer[j][i]<=4095));
		}
		printf("\n");
	}
	vhalAdcDone(adc);
	return TEST_OK();
}



/* TEST ARRAY */


BEGIN_TEST("ADC TEST"),
           ADD_TEST(adc_wrong_pin, "vhalAdcGetPeripheralForPin Negative Test"),
           ADD_TEST(adc_good_pin, "vhalAdcGetPeripheralForPin Positive Test"),
           ADD_TEST(adc_init, "vhalAdcInit samples per second Test"),
           ADD_TEST(adc_prepare, "vhalAdcPrepare Test"),
           ADD_TEST(adc_capture_single, "vhalAdcRead Single Capture @ Max Freq"),
           ADD_TEST(adc_capture_single3, "vhalAdcRead Single Channel Buffer Capture @ Max Freq"),
           //ADD_TEST(adc_capture_single2, "vhalAdcRead Single Capture @ Min Freq"),
           ADD_TEST(adc_capture_multiple, "vhalAdcRead Multiple Capture @ Max Freq [10 samples in 16 channels]"),
           END_TEST();
