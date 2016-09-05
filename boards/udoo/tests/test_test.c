
#include "viper_test.h"




TEST(test1) {
    TEST_ASSERT("1+1=2?", (1 + 1) == 2);
    printf("Writing something from test1\n");
    return TEST_OK();
}


TEST(test2) {
    TEST_EQUAL("still 1+1=2?",1+1,2);
    return TEST_KO("this test shall never pass!");
}



/* TEST ARRAY */


BEGIN_TEST("Testing the test suite"),
           ADD_TEST(test1, "Description of Test 1"),
           ADD_TEST(test2, "Description of Test 2"),
           END_TEST();