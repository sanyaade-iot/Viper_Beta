
#include "viper_test.h"


extern vos_vectors *ram_vectors;

TEST(test1) {
    printf("ram_vectors %x\n",ram_vectors);
    printf("__ramvectors__ %x\n",&__ramvectors__);
    printf("*ram_vectors %x\n",ram_vectors->vectors);
    
    return TEST_OK();
}





/* TEST ARRAY */


BEGIN_TEST("Testing the test suite"),
           ADD_TEST(test1, "Description of Test 1"),
           END_TEST();