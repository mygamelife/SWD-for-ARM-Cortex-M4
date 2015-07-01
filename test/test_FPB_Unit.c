#include "unity.h"
#include <stdint.h>
#include "FPB_Unit.h"
#include "Clock.h"
#include "Emulator.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "mock_configurePort.h"
#include "mock_IO_Operations.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_isFPB_Enabled_should_return_TRUE_if_bit_1_is_1()
{
	TEST_ASSERT_EQUAL(TRUE,isFPB_Enabled(0x21));
}
void test_isFPB_Enabled_should_return_FALSE_if_bit_1_is_0()
{
	TEST_ASSERT_EQUAL(FALSE,isFPB_Enabled(0x12));
}

void test_get_FP_COMP_WriteValue_given_address_0x1FFFFFFF_MatchingMode_REMAP_should_return_0x1FFFFFFD()
{
	TEST_ASSERT_EQUAL(0x1FFFFFFD,get_FP_COMP_WriteValue(0x1FFFFFFF,Match_REMAP));
}

void test_get_FP_COMP_WriteValue_given_address_0x0A1B2C37_MatchingMode_Match_32bits_should_return_0xCA1B2C35()
{
	TEST_ASSERT_EQUAL(0xCA1B2C35,get_FP_COMP_WriteValue(0xCA1B2C35,Match_32bits));
}