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

void test_isFP_COMP_Updated_will_compare_and_return_TRUE_if_2_pass_in_values_are_the_same()
{
	TEST_ASSERT_EQUAL(TRUE,isFP_COMP_Updated(0x12345678,0x12345678));
}

void test_isFP_COMP_Updated_will_compare_and_return_FALSE_if_2_pass_in_values_are_different()
{
	TEST_ASSERT_EQUAL(FALSE,isFP_COMP_Updated(0x12345678,0x12345677));
}

void test_get_FP_COMP_WriteValue_given_address_0x1FFFFFFF_MatchingMode_REMAP_should_return_0x1FFFFFFD()
{
	TEST_ASSERT_EQUAL(0x1FFFFFFD,get_FP_COMP_WriteValue(0x1FFFFFFF,Match_REMAP));
}

void test_get_FP_COMP_WriteValue_given_address_0x0A1B2C37_MatchingMode_Match_32bits_should_return_0xCA1B2C35()
{
	TEST_ASSERT_EQUAL(0xCA1B2C35,get_FP_COMP_WriteValue(0xCA1B2C35,Match_32bits));
}

void test_configure_FP_COMP_given_FP_COMP2_0x10000000_Match_32bits_should_write_0xD0000001_and_read_FPCOMP2_and_return_TRUE_if_data_is_the_same()
{
	printf("Here\n");
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_COMP2);
	emulateSWDRegister_Write(DRW_REG,AP,4,0xD0000001);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_COMP2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0xD0000001));
	
	TEST_ASSERT_EQUAL(TRUE,configure_FP_COMP(FP_COMP2,0x10000000,Match_32bits));
	printf("End\n");
}

void test_configure_FP_COMP_given_FP_COMP1_0x10000000_Match_32bits_should_write_0xD0000001_and_read_FPCOMP1_and_return_FALSE_if_data_is_different()
{
	printf("Next\n");
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_COMP1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0xD0000001);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_COMP1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0xD0000001));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));

	
	TEST_ASSERT_EQUAL(FALSE,configure_FP_COMP(FP_COMP1,0x10000000,Match_32bits));
}