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