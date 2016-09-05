#include "viper_test.h"


#include "hal.h"

//SET VM_STDOUT!=0

extern void blink(int vpin, int n);
extern void blink_off(int vpin);

TEST(ser) {
	int cloop=0;
	cloop=vhalSerialInit(0,115200,SERIAL_PARITY_NONE, SERIAL_STOP_ONE, SERIAL_BITS_8, RX2,TX2);
	cloop=vhalSerialInit(1,115200,SERIAL_PARITY_NONE, SERIAL_STOP_ONE, SERIAL_BITS_8, RX1,TX1);
	cloop=vhalSerialInit(2,115200,SERIAL_PARITY_NONE, SERIAL_STOP_ONE, SERIAL_BITS_8, RX0,TX0);
	vosThSleep(TIME_U(2000,MILLIS));

	printf("%x\n",AFIO->MAPR);

	TEST_ASSERT("init should return 0",cloop==0);
	
	printf("%x\n",(uint8_t *) &__codemem__);
	while(cloop<5){
		vhalSerialWrite(2,"Hello!\n",7);
		cloop++;
	}

	//vhalSerialDone(0);
	return TEST_OK();
}


TEST(ser2) {
	int cloop=0;
	cloop =vhalSerialInit(2,115200,SERIAL_PARITY_NONE, SERIAL_STOP_ONE, SERIAL_BITS_8, VM_RXPIN(0),VM_TXPIN(0));
	TEST_ASSERT("init should return 0",cloop==0);
	
	printf("USB!\n");

	while(cloop!='q'){
		vhalSerialRead(2,&cloop,1);
		printf("%i\n",cloop);
		vhalSerialWrite(2,&cloop,1);
	}

	return TEST_OK();
}





BEGIN_TEST("SER_TEST"),
           ADD_TEST(ser, "print  something on RX0,TX0"),
           ADD_TEST(ser2, "read something RX0,TX0"),
           END_TEST();