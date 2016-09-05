#include "viper_test.h"




//SET VM_STDOUT!=0

TEST(ser) {
	int cloop=0;
	cloop=vhalSerialInit(0,115200,SERIAL_PARITY_NONE, SERIAL_STOP_ONE, SERIAL_BITS_8, VM_RXPIN(0),VM_TXPIN(0));	
	TEST_ASSERT("init should return 0",cloop==0);
	
	while(cloop<5){
		vhalSerialWrite(0,"Hello!\n",7);
		cloop++;
	}
	vhalSerialDone(0);
	return TEST_OK();
}


TEST(ser2) {
	int cloop=0;
	cloop =vhalSerialInit(0,115200,SERIAL_PARITY_NONE, SERIAL_STOP_ONE, SERIAL_BITS_8, VM_RXPIN(0),VM_TXPIN(0));
	TEST_ASSERT("init should return 0",cloop==0);
	
	printf("type something\n");

	while(cloop!='q'){
		vhalSerialRead(0,&cloop,1);
		printf("%i\n",cloop);
		vhalSerialWrite(0,&cloop,1);
	}

	return TEST_OK();
}





BEGIN_TEST("SER_TEST"),
           ADD_TEST(ser, "print  something on RX0,TX0"),
           ADD_TEST(ser2, "read something RX0,TX0"),
           END_TEST();