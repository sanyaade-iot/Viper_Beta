#include "viper_test.h"

#define RANDOM(x) (vhalRngGenerate()%(x))

TEST(dict_init) {
	PDict *dict = pdict_new(10);
	PObject *ee,*vv;
	int i;
	printf(">>");
	for (i = 0; i < 10; i++){
		ee = PSMALLINT_NEW(3 * i);
		vv = PSMALLINT_NEW(2 * i);
		pdict_put(dict, ee, vv);
		printf(" %i:%i",ee,vv);
	}
	printf("\n");
	TEST_ASSERT("Must have 10 elements", dict->elements == 10);
	printf("<<");
	for (i = 0; i < 10; i++) {
		ee = PSMALLINT_NEW(i * 3);
		vv = PSMALLINT_NEW(i * 2);		
		TEST_ASSERT("Doesn't get correct element!", pdict_get(dict, ee) == vv);
		printf(" %i:%i",ee,vv);
	}
	printf("\n");
	pdict_print(dict);
	return TEST_OK();
}

TEST(dict_rehash) {
	PDict *dict = pdict_new(10);
	int i;
	for (i = 0; i < 20; i++)
		pdict_put(dict, PSMALLINT_NEW(3 * i), PSMALLINT_NEW(i * 2));
	TEST_ASSERT("Must have 20 elements", dict->elements == 20);
	for (i = 0; i < 20; i++) {
		TEST_ASSERT("Doesn't get correct element!", pdict_get(dict, PSMALLINT_NEW(i * 3)) == PSMALLINT_NEW(i * 2));
	}
	pdict_print(dict);
	return TEST_OK();
}

TEST(dict_delete) {
	PDict *dict = pdict_new(10);
	int i;
	for (i = 0; i < 20; i++)
		pdict_put(dict, PSMALLINT_NEW(3 * i), PSMALLINT_NEW(i * 2));
	TEST_ASSERT("Must have 20 elements", dict->elements == 20);
	for (i = 19; i >= 0; i--) {
		TEST_ASSERT("Doesn't delete correct element!", pdict_del(dict, PSMALLINT_NEW(i * 3)) == PSMALLINT_NEW(i * 2));
	}
	TEST_ASSERT("Dict is not empty",dict->elements==0);
	pdict_print(dict);
	return TEST_OK();
}


TEST(set_init) {
	PSet *set = pset_new(PSET,10);
	int i;
	PObject *ee;
	printf(">>");
	for (i = 0; i < 10; i++){
		ee = PSMALLINT_NEW(3 * i);
		pset_put(set, ee);
		printf(" %i",ee);
	}
	printf("\n");
	TEST_ASSERT("Must have 10 elements", set->elements == 10);
	pset_print(set);
	printf("<<");
	for (i = 0; i < 10; i++) {
		ee = pset_get(set, PSMALLINT_NEW(i * 3));
		printf(" %i",ee);
		TEST_ASSERT("Doesn't get correct element!",  ee == PSMALLINT_NEW(i * 3));
	}
	printf("\n");
	pset_print(set);
	return TEST_OK();
}

TEST(set_rehash) {
	PSet *set = pset_new(PSET,10);
	int i;
	for (i = 0; i < 20; i++)
		pset_put(set, PSMALLINT_NEW(3 * i));
	TEST_ASSERT("Must have 20 elements", set->elements == 20);
	for (i = 0; i < 20; i++) {
		TEST_ASSERT("Doesn't get correct element!", pset_get(set, PSMALLINT_NEW(i * 3)) == PSMALLINT_NEW(i * 3));
	}
	pset_print(set);
	return TEST_OK();
}


TEST(set_delete) {
	PSet *set = pset_new(PSET,10);
	int i;
	for (i = 0; i < 20; i++)
		pset_put(set, PSMALLINT_NEW(3 * i));
	TEST_ASSERT("Must have 20 elements", set->elements == 20);
	for (i = 19; i >= 0; i--) {
		TEST_ASSERT("Doesn't delete correct element!", pset_del(set, PSMALLINT_NEW(i * 3)) == PSMALLINT_NEW(i * 3));
	}
	TEST_ASSERT("Set is not empty",set->elements==0);
	pset_print(set);
	return TEST_OK();
}


BEGIN_TEST("DICT & SET TEST"),
           ADD_TEST(dict_init, "Dict Constructor"),
           ADD_TEST(dict_rehash, "Dict Rehashing"),
           ADD_TEST(dict_delete, "Dict Deletions"),
           ADD_TEST(set_init, "Set Constructor"),
           ADD_TEST(set_rehash, "Set Rehashing"),
           ADD_TEST(set_delete, "Set Deletions"),
           END_TEST();