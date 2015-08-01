#include "unity.h"
#include "Misc_Utilities.h"
#include <stdint.h>
void setUp(void)
{
}

void tearDown(void)
{
}

void test_check_BitSetWithMask_given_0x1_Mask_0x1_should_return_TRUE()
{
	TEST_ASSERT_EQUAL(ERR_NOERROR,check_BitSetWithMask(0x1,0x1));
}

void test_check_BitSetWithMask_given_0x1_Mask_0x0_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(ERR_BITS_SPECIFIED_NOT_SET,check_BitSetWithMask(0x1,0x0));
}

void test_check_BitSetWithMask_given_0x10_Mask_0x1_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(ERR_BITS_SPECIFIED_NOT_SET,check_BitSetWithMask(0x10,0x1));
}

void test_check_dataWrote_dataRead_given_0xFFFFFFFF_and_0xFFFFFFFF_should_return_TRUE()
{
	TEST_ASSERT_EQUAL(ERR_NOERROR,check_dataWrote_dataRead(0xFFFFFFFF,0xFFFFFFFF));
}

void test_check_dataWrote_dataRead_given_0xFFFFFFFF_and_0xFFFAFFFF_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(ERR_DATARW_NOT_MATCH,check_dataWrote_dataRead(0xFFFFFFFF,0xFFFAFFFF));
}