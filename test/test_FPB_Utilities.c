#include "unity.h"
#include "FPB_Utilities.h"
#include "Misc_Utilities.h"
#include <stdint.h>
#include <string.h>
void setUp(void)
{
}

void tearDown(void)
{
}

/*---------------------------------------init_FPBInfo-----------------------------------*/

void test_init_FPBInfo_should_initialise_FPBInfo_and_CompInfo()
{
	FPBInfo fpbInfo ;
	
	init_FPBInfo(&fpbInfo);
	
	TEST_ASSERT_EQUAL(2,fpbInfo.numberLiteralCOMP);
	TEST_ASSERT_EQUAL(6,fpbInfo.numberInstructionCOMP);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.EnableDisable);
	
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[0]->comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Instruction",fpbInfo.compInfo[0]->comparatorType);
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[0]->address);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.compInfo[0]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,fpbInfo.compInfo[0]->matchingMode);
	
	TEST_ASSERT_EQUAL(1,fpbInfo.compInfo[1]->comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Instruction",fpbInfo.compInfo[1]->comparatorType);
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[1]->address);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.compInfo[1]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,fpbInfo.compInfo[1]->matchingMode);
	
	TEST_ASSERT_EQUAL(2,fpbInfo.compInfo[2]->comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Instruction",fpbInfo.compInfo[2]->comparatorType);
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[2]->address);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.compInfo[2]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,fpbInfo.compInfo[2]->matchingMode);
	
	TEST_ASSERT_EQUAL(3,fpbInfo.compInfo[3]->comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Instruction",fpbInfo.compInfo[3]->comparatorType);
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[3]->address);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.compInfo[3]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,fpbInfo.compInfo[3]->matchingMode);
	
	TEST_ASSERT_EQUAL(4,fpbInfo.compInfo[4]->comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Instruction",fpbInfo.compInfo[4]->comparatorType);
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[4]->address);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.compInfo[4]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,fpbInfo.compInfo[4]->matchingMode);
	
	TEST_ASSERT_EQUAL(5,fpbInfo.compInfo[5]->comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Instruction",fpbInfo.compInfo[5]->comparatorType);
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[5]->address);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.compInfo[5]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,fpbInfo.compInfo[5]->matchingMode);
	
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[6]->comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Literal",fpbInfo.compInfo[6]->comparatorType);
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[6]->address);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.compInfo[6]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,fpbInfo.compInfo[6]->matchingMode);
	
	TEST_ASSERT_EQUAL(1,fpbInfo.compInfo[7]->comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Literal",fpbInfo.compInfo[7]->comparatorType);
	TEST_ASSERT_EQUAL(0,fpbInfo.compInfo[7]->address);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.compInfo[7]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,fpbInfo.compInfo[7]->matchingMode);
}

/*---------------------------------------init_ComparatorInfo-----------------------------------*/
void test_init_ComparatorInfo_given_compNo_10_Literal_address_0x1234_Enable_Match_32bits_should_init_comparatorInfo()
{
	ComparatorInfo compInfo ;
	
	init_ComparatorInfo(&compInfo,10,"Literal",0x1234,Enable,Match_32bits);
	
	TEST_ASSERT_EQUAL(10,compInfo.comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Literal",compInfo.comparatorType);
	TEST_ASSERT_EQUAL(0x1234,compInfo.address);
	TEST_ASSERT_EQUAL(Enable,compInfo.EnableDisable);
	TEST_ASSERT_EQUAL(Match_32bits,compInfo.matchingMode);
}

void test_init_ComparatorInfo_given_compNo_100_Instruction_address_0xFFFFFFFF_Disable_Match_REMAP_should_init_comparatorInfo()
{
	ComparatorInfo compInfo ;
	
	init_ComparatorInfo(&compInfo,100,"Instruction",0xFFFFFFFF,Disable,Match_REMAP);
	
	TEST_ASSERT_EQUAL(100,compInfo.comparatorNumber);
	TEST_ASSERT_EQUAL_STRING("Instruction",compInfo.comparatorType);
	TEST_ASSERT_EQUAL(0xFFFFFFFF,compInfo.address);
	TEST_ASSERT_EQUAL(Disable,compInfo.EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,compInfo.matchingMode);
}

/*---------------------------------------process_FPControlData-----------------------------------*/

void test_process_FPControlData_given_0x341_should_set_FPBInfo_numLit_to_3_numInstruction_to_4_and_enable_to_Enable()
{
	FPBInfo fpbInfo ;
	
	process_FPControlData(&fpbInfo,0x341);
	
	TEST_ASSERT_EQUAL(3,fpbInfo.numberLiteralCOMP);
	TEST_ASSERT_EQUAL(4,fpbInfo.numberInstructionCOMP);
	TEST_ASSERT_EQUAL(Enable,fpbInfo.EnableDisable);
}

void test_process_FPControlData_given_0x340_should_set_FPBInfo_numLit_to_3_numInstruction_to_4_and_enable_to_Disable()
{
	FPBInfo fpbInfo ;
	
	process_FPControlData(&fpbInfo,0x340);
	
	TEST_ASSERT_EQUAL(3,fpbInfo.numberLiteralCOMP);
	TEST_ASSERT_EQUAL(4,fpbInfo.numberInstructionCOMP);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.EnableDisable);
}

void test_process_FPControlData_given_0xFA4_should_set_FPBInfo_numLit_to_15_numInstruction_to_10_and_enable_to_Disable()
{
	FPBInfo fpbInfo ;
	
	process_FPControlData(&fpbInfo,0xFA4);
	
	TEST_ASSERT_EQUAL(15,fpbInfo.numberLiteralCOMP);
	TEST_ASSERT_EQUAL(10,fpbInfo.numberInstructionCOMP);
	TEST_ASSERT_EQUAL(Disable,fpbInfo.EnableDisable);
}

/*---------------------------------------process_FPComparatorData-----------------------------------*/

void test_process_FPComparatorData_given_0x01234561_should_store_compInfo_address_0x1234560_enable_1_and_matchRemap()
{
	ComparatorInfo compInfo;
	
	process_FPComparatorData(&compInfo,0x01234561);
	
	TEST_ASSERT_EQUAL(0x01234560,compInfo.address);
	TEST_ASSERT_EQUAL(Enable,compInfo.EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,compInfo.matchingMode);
}

void test_process_FPComparatorData_given_0x41234561_should_store_compInfo_address_0x1234561_enable_1_and_Match_Lower16bits()
{
	ComparatorInfo compInfo;
	
	process_FPComparatorData(&compInfo,0x41234561);
	
	TEST_ASSERT_EQUAL(0x01234560,compInfo.address);
	TEST_ASSERT_EQUAL(Enable,compInfo.EnableDisable);
	TEST_ASSERT_EQUAL(Match_Lower16bits,compInfo.matchingMode);
}

void test_process_FPComparatorData_given_0x81234561_should_store_compInfo_address_0x1234562_enable_1_and_Match_Upper16bits()
{
	ComparatorInfo compInfo;
	
	process_FPComparatorData(&compInfo,0x81234561);
	
	TEST_ASSERT_EQUAL(0x01234562,compInfo.address);
	TEST_ASSERT_EQUAL(Enable,compInfo.EnableDisable);
	TEST_ASSERT_EQUAL(Match_Upper16bits,compInfo.matchingMode);
}


void test_process_FPComparatorData_given_0xC1234561_should_store_compInfo_address_0x1234563_enable_1_and_match32bits()
{
	ComparatorInfo compInfo;
	
	process_FPComparatorData(&compInfo,0xC1234561);
	
	TEST_ASSERT_EQUAL(0x01234560,compInfo.address);
	TEST_ASSERT_EQUAL(Enable,compInfo.EnableDisable);
	TEST_ASSERT_EQUAL(Match_32bits,compInfo.matchingMode);
}

void test_process_FPComparatorData_given_0x01234560_should_store_compInfo_address_0x1234560_enable_0_and_matchRemap()
{
	ComparatorInfo compInfo;
	
	process_FPComparatorData(&compInfo,0x01234560);
	
	TEST_ASSERT_EQUAL(0x01234560,compInfo.address);
	TEST_ASSERT_EQUAL(Disable,compInfo.EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,compInfo.matchingMode);
}


void test_process_FPComparatorData_given_0x0123456D_should_store_compInfo_address_0x123456C_enable_1_and_matchRemap()
{
	ComparatorInfo compInfo;
	
	process_FPComparatorData(&compInfo,0x0123456D);
	
	TEST_ASSERT_EQUAL(0x0123456C,compInfo.address);
	TEST_ASSERT_EQUAL(Enable,compInfo.EnableDisable);
	TEST_ASSERT_EQUAL(Match_REMAP,compInfo.matchingMode);
}

/*--------------------------------------process_FPRemapData-----------------------------------*/

void test_process_FPRemapData_given_0xFFFFFFFF_should_store_0x3FFFFFE0_into_fpbInfo_SRAM_REMAP_address()
{
	FPBInfo fpbInfo;
	
	process_FPRemapData(&fpbInfo,0xFFFFFFFF);
	
	TEST_ASSERT_EQUAL(0x3FFFFFE0,fpbInfo.SRAM_REMAP_address);
}

void test_process_FPRemapData_given_0x23456789_should_store_0x23456780_into_fpbInfo_SRAM_REMAP_address()
{
	FPBInfo fpbInfo;
	
	process_FPRemapData(&fpbInfo,0x23456789);
	
	TEST_ASSERT_EQUAL(0x23456780,fpbInfo.SRAM_REMAP_address);
}

void test_process_FPRemapData_given_0x34567890_should_store_0x34567880_into_fpbInfo_SRAM_REMAP_address()
{
	FPBInfo fpbInfo;
	
	process_FPRemapData(&fpbInfo,0x34567890);
	
	TEST_ASSERT_EQUAL(0x34567880,fpbInfo.SRAM_REMAP_address);
}

/*--------------------------------------isFPB_EnabledDisabled-----------------------------------*/
//Enable mode
void test_isFPB_EnabledDisabled_should_return_ERR_NOERROR_if_FPBInfo_enable_is_Enable()
{
	FPBInfo fpbInfo ;
	
	fpbInfo.EnableDisable = Enable ;
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,isFPB_EnabledDisabled(&fpbInfo,Enable));
}
void test_iisFPB_EnabledDisabled_should_return_ERR_FPB_NOTENABLED_if_FPBInfo_enable_is_Disable()
{
	FPBInfo fpbInfo ;
	
	fpbInfo.EnableDisable = Disable ;
	
	TEST_ASSERT_EQUAL(ERR_FPB_NOTENABLED,isFPB_EnabledDisabled(&fpbInfo,Enable));
}

//Disable mode
void test_isFPB_EnabledDisabled_should_return_ERR_NOERROR_if_FPBInfo_enable_is_Disable()
{
	FPBInfo fpbInfo ;
	
	fpbInfo.EnableDisable = Disable ;
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,isFPB_EnabledDisabled(&fpbInfo,Disable));
}
void test_isFPB_EnabledDisabled_should_return_ERR_FPB_NOTDISABLED_if_FPBInfo_enable_is_Enable()
{
	FPBInfo fpbInfo ;
	
	fpbInfo.EnableDisable = Enable ;
	
	TEST_ASSERT_EQUAL(ERR_FPB_NOTDISABLED,isFPB_EnabledDisabled(&fpbInfo,Disable));
}

/*---------------------------------------isFP_COMP_Updated-----------------------------------*/

void test_isFP_COMP_Updated_will_compare_and_return_ERR_NOERROR_if_2_pass_in_values_are_the_same()
{
	TEST_ASSERT_EQUAL(ERR_NOERROR,isFP_COMP_Updated(0x12345678,0x12345678));
}

void test_isFP_COMP_Updated_will_compare_and_return_ERR_DATARW_NOT_MATCH_if_2_pass_in_values_are_different()
{
	TEST_ASSERT_EQUAL(ERR_DATARW_NOT_MATCH,isFP_COMP_Updated(0x12345678,0x12345677));
}

/*---------------------------------------get_FP_CTRL_WriteValue-----------------------------------*/
void test_get_FP_CTRL_WriteValue_given_Enable_should_return_ENABLE_FPB()
{
	TEST_ASSERT_EQUAL(ENABLE_FPB,get_FP_CTRL_WriteValue(Enable));
}

void test_get_FP_CTRL_WriteValue_given_Disable_should_return_Disable_FPB()
{
	TEST_ASSERT_EQUAL(DISABLE_FPB,get_FP_CTRL_WriteValue(Disable));
}
/*---------------------------------------get_FP_COMP_WriteValue-----------------------------------*/
//Enable
void test_get_FP_COMP_WriteValue_given_address_0x1FFFFFFF_MatchingMode_REMAP_should_return_0x1FFFFFFD()
{
	TEST_ASSERT_EQUAL(0x1FFFFFFD,get_FP_COMP_WriteValue(0x1FFFFFFF,Match_REMAP,Enable));
}

void test_get_FP_COMP_WriteValue_given_address_0xCA1B2C35_MatchingMode_Match_32bits_should_return_0xCA1B2C35()
{
	TEST_ASSERT_EQUAL(0xCA1B2C35,get_FP_COMP_WriteValue(0xCA1B2C35,Match_32bits,Enable));
}

//Disable
void test_get_FP_COMP_WriteValue_given_address_0x1FFFFFFF_MatchingMode_REMAP_should_return_0x1FFFFFFC()
{
	TEST_ASSERT_EQUAL(0x1FFFFFFC,get_FP_COMP_WriteValue(0x1FFFFFFF,Match_REMAP,Disable));
}

void test_get_FP_COMP_WriteValue_given_address_0xCA1B2C35_MatchingMode_Match_32bits_should_return_0xCA1B2C34()
{
	TEST_ASSERT_EQUAL(0xCA1B2C34,get_FP_COMP_WriteValue(0xCA1B2C35,Match_32bits,Disable));
}
/*---------------------------------------get_FP_REMAP_WriteValue-----------------------------------*/

void test_get_FP_REMAP_WriteValue_given_address_0x0A1B2C37_should_clear_bits31_till_29_and_bits4_till_0_and_return_0x0A1B2C20()
{
	TEST_ASSERT_EQUAL(0x0A1B2C20,get_FP_REMAP_WriteValue(0x0A1B2C37));
}

void test_get_FP_REMAP_WriteValue_given_address_0xFA1B2C37_should_clear_bits31_till_29_and_bits4_till_0_and_return_0x1A1B2C20()
{
	TEST_ASSERT_EQUAL(0x1A1B2C20,get_FP_REMAP_WriteValue(0xFA1B2C37));
}

void test_get_FP_REMAP_WriteValue_given_address_0xFFFFFFFF_should_clear_bits31_till_29_and_bits4_till_0_and_return_0x1FFFFFE0()
{
	TEST_ASSERT_EQUAL(0x1FFFFFE0,get_FP_REMAP_WriteValue(0xFFFFFFFF));
}

/*---------------------------------------get_ComparatorInfoNumber-----------------------------------*/
void test_getComparatorInforNumber_should_return_0_for_FPCOMP0_1_for_FPCOMP1_2_for_FPCOMP2_and_so_on_until_FPCOMP7()
{
	 
	TEST_ASSERT_EQUAL(0,get_ComparatorInfoNumber(FP_COMP0));
	TEST_ASSERT_EQUAL(1,get_ComparatorInfoNumber(FP_COMP1));
	TEST_ASSERT_EQUAL(2,get_ComparatorInfoNumber(FP_COMP2));
	TEST_ASSERT_EQUAL(3,get_ComparatorInfoNumber(FP_COMP3));
	TEST_ASSERT_EQUAL(4,get_ComparatorInfoNumber(FP_COMP4));
	TEST_ASSERT_EQUAL(5,get_ComparatorInfoNumber(FP_COMP5));
	TEST_ASSERT_EQUAL(6,get_ComparatorInfoNumber(FP_COMP6));
	TEST_ASSERT_EQUAL(7,get_ComparatorInfoNumber(FP_COMP7));

}

void test_getComparatorInforNumber_should_return_ERR_INVALID_COMPARATOR_for_unknown_value_of_comparator()
{
	TEST_ASSERT_EQUAL(ERR_INVALID_COMPARATOR,get_ComparatorInfoNumber(100));

}