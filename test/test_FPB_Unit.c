#include "unity.h"
#include <stdint.h>
#include "FPB_Unit.h"
#include "FPB_Utilities.h"
#include "Delay.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "Emulator.h"
#include "MemoryReadWrite.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"
#include "mock_CodeStepping.h"
void setUp(void)
{
}

void tearDown(void)
{
  instructionComparatorReady[0] = COMP_READY ;
  instructionComparatorReady[1] = COMP_READY ;
  instructionComparatorReady[2] = COMP_READY ;
  instructionComparatorReady[3] = COMP_READY ;
  instructionComparatorReady[4] = COMP_READY ;
  instructionComparatorReady[5] = COMP_READY ;
  
  literalComparatorReady[0] = COMP_READY ;
  literalComparatorReady[1] = COMP_READY ;
}

/*-------------------------enableFPBUnit-----------------------*/
void test_enableFPBUnit_should_write_ENABLE_FPB_to_FP_CTRL()
{
  //Faking CSW to Word Size
  cswDataSize = CSW_WORD_SIZE ;
  
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);

  enableFPBUnit();
}

/*-------------------------disableFPBUnit-----------------------*/
void test_disablePBUnit_should_write_DISABLE_FPB_to_FP_CTRL()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  disableFPBUnit();
}

/*-------------------------manualSetInstructionBreakpoint-----------------------*/
//MATCH_LOWERHALFWORD
void test_manualSetInstructionBreakpoint_givenINSTRUCTION_COMP1_address_0xFFFFFFFF_MATCH_LOWERHALFWORD()
{
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x5FFFFFFD);
  
  TEST_ASSERT_EQUAL(0,manualSetInstructionBreakpoint(INSTRUCTION_COMP1,0xFFFFFFFF,MATCH_LOWERHALFWORD));
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[1]);
}

//MATCH_UPPERHALFWORD
void test_manualSetInstructionBreakpoint_givenINSTRUCTION_COMP2_address_0xA55AA55A_MATCH_UPPERHALFWORD()
{
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x855AA559);
  
  TEST_ASSERT_EQUAL(0,manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0xA55AA55A,MATCH_UPPERHALFWORD));
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[2]);
}

//MATCH_WORD
void test_manualSetInstructionBreakpoint_givenINSTRUCTION_COMP3_address_0x02345670_MATCH_WORD()
{
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xC2345671);
  
  TEST_ASSERT_EQUAL(0,manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x02345670,MATCH_WORD));
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[3]);
}

void test_manualSetInstructionBreakpoint_given_out_of_range_instruction_comp_should_return_negative_1()
{
  TEST_ASSERT_EQUAL(-1,manualSetInstructionBreakpoint(10,0x02345670,MATCH_WORD));
}

/*-------------------------manualSetInstructionRemapping-----------------------*/
void test_manualSetInstructionRemapping_should_program_FP_REMAP_and_comparator()
{
  //Program FP_REMAP
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_REMAP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x02222220);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x02345671);
   
  TEST_ASSERT_EQUAL(0,manualSetInstructionRemapping(INSTRUCTION_COMP0,0x02345670,0x22222222));
  TEST_ASSERT_EQUAL(COMP_REMAP,instructionComparatorReady[0]);
}
/*-------------------------manualSetLiteralRemapping-----------------------*/
void test_manualSetLiteralRemapping_should_program_FP_REMAP_and_literal_Comparator()
{
  //Program FP_REMAP
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_REMAP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x14567880);

  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x12345679);
  
  TEST_ASSERT_EQUAL(0,manualSetLiteralRemapping(LITERAL_COMP1,0x12345678,0x3456789A));
  TEST_ASSERT_EQUAL(COMP_REMAP,literalComparatorReady[1]);
}

/*-------------------------autoSetInstructionBreakpoint-----------------------*/
void test_autoSetInstructionBreakpoint_given_INSTRUCTION_COMP5_free_should_use_INSTRUCTION_COMP5()
{
  //Read INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x5FFFFFFD);
  
  TEST_ASSERT_EQUAL(5,autoSetInstructionBreakpoint(0xFFFFFFFF,MATCH_LOWERHALFWORD));
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[5]);
  
}

/*-------------------------autoSetInstructionRemapping-----------------------*/
void test_autoSetInstructionRemapping_should_program_machineCode_to_REMAP_BASE_given_COMP0_free()
{
  //Read INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0));
  
  //Read INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
  
  //Program machine code
  emulateSwdRegisterWrite(TAR_REG,AP,4,REMAP_BASE);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x56781234);
  
  //Program FP_REMAP
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_REMAP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,REMAP_BASE & FP_REMAP_ADDRESS_MASK);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x08000001);

  TEST_ASSERT_EQUAL(INSTRUCTION_COMP0,autoSetInstructionRemapping(0x08000000,0x12345678));
  TEST_ASSERT_EQUAL(COMP_REMAP,instructionComparatorReady[0]);
}

void test_autoSetInstructionRemapping_16bit_machineCode_case()
{
  //Read INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0));
  
  //Read INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
  
  //Set CSW to Halfword Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
  
  //Read Instruction Address + 2
  emulateSwdRegisterWrite(TAR_REG, AP, OK, 0x08000002);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0xBBBB));

  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Program machine code
  emulateSwdRegisterWrite(TAR_REG,AP,4,REMAP_BASE);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xAAAABBBB);
  
  //Program FP_REMAP
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_REMAP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,REMAP_BASE & FP_REMAP_ADDRESS_MASK);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x08000001);

  TEST_ASSERT_EQUAL(INSTRUCTION_COMP0,autoSetInstructionRemapping(0x08000000,0xAAAA));
  TEST_ASSERT_EQUAL(COMP_REMAP,instructionComparatorReady[0]);
}
/*-------------------------autoSetSoftwareBreakpoint-----------------------*/
//32bit case
void test_autoSetSoftwareBreakpoint_should_replace_the_address_with_bkpt_and_nop_instruction_and_return_machineCode()
{
  cswDataSize = CSW_WORD_SIZE ;
  
  //Check is it 32 bit instruction
  isSelectedAddressContains32bitsInstruction_ExpectAndReturn(0x08001000,1);
  
  //Read the machince code
  emulateSwdRegisterWrite(TAR_REG, AP, OK, 0x08001000);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0xA5A5A5A));
  
  //Write bkpt and nop instruction into the address
  emulateSwdRegisterWrite(TAR_REG,AP,4,0x08001000);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xBE00BF00);
  
  TEST_ASSERT_EQUAL(0xA5A5A5A,autoSetSoftwareBreakpoint(0x08001000));
  
}
//16bit case
void test_autoSetSoftwareBreakpoint_should_replace_the_address_with_bkpt_instruction_and_return_machineCode()
{
  //Check is it 32 bit instruction
  isSelectedAddressContains32bitsInstruction_ExpectAndReturn(0x08001000,0);
  
  //Write SELECT_BANK0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
  //Write CSW_HALFWORD_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
  
  //Read 16bit machince code
  emulateSwdRegisterWrite(TAR_REG, AP, OK, 0x08001000);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x1234));
  
  //Write bkpt and nop instruction into the address
  emulateSwdRegisterWrite(TAR_REG,AP,4,0x08001000);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xBE00);
  
  
  TEST_ASSERT_EQUAL(0x1234,autoSetSoftwareBreakpoint(0x08001000));
}

/*-------------------------restoreSoftwareBreakpointOriginalInstruction-----------------------*/
void test_restoreSoftwareBreakpointOriginalInstruction_given_32bit_machineCode_should_write_word()
{
  cswDataSize = CSW_WORD_SIZE ;
  
  emulateSwdRegisterWrite(TAR_REG,AP,4,0x08000000);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x1234ABCD);
  
  restoreSoftwareBreakpointOriginalInstruction(0x08000000,0x1234ABCD);
}

void test_restoreSoftwareBreakpointOriginalInstruction_given_16bit_machineCode_should_write_halfword()
{
  cswDataSize = CSW_HALFWORD_SIZE ;
  
  emulateSwdRegisterWrite(TAR_REG,AP,4,0x08000000);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x1234);
  
  restoreSoftwareBreakpointOriginalInstruction(0x08000000,0x1234);
}


/*-------------------------disableInstructionComparator-----------------------*/
void test_disableInstructionComparator_should_write_FP_COMP_DISABLE_to_the_selected_instruction_comparator()
{
  cswDataSize = CSW_WORD_SIZE ;
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  TEST_ASSERT_EQUAL(0,disableInstructionComparator(INSTRUCTION_COMP0));
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[0]);
}

void test_disableInstructionComparator_should_return_negative_1_given_unkwown_comp()
{
  TEST_ASSERT_EQUAL(-1,disableInstructionComparator(100));
}


/*-------------------------disableLiteralComparator-----------------------*/
void test_disableLiteralComparator_should_write_FP_COMP_DISABLE_to_the_selected_literal_comparator()
{
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  TEST_ASSERT_EQUAL(0,disableLiteralComparator(LITERAL_COMP1));
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[1]);
}

void test_disableLiteralComparator_should_return_negative_1_given_unkwown_comp()
{
  TEST_ASSERT_EQUAL(-1,disableLiteralComparator(100));
}

/*-------------------------disableFPComparatorLoadedWithAddress-----------------------*/
//INSTRUCTION_TYPE
void test_disableFPComparatorLoadedWithAddress_given_found_comp3_should_disable_comp3()
{
  instructionComparatorReady[0] = COMP_BUSY ;
  instructionComparatorReady[1] = COMP_BUSY ;
  instructionComparatorReady[2] = COMP_BUSY ;
  instructionComparatorReady[3] = COMP_BUSY ;
  instructionComparatorReady[4] = COMP_BUSY ;
  instructionComparatorReady[5] = COMP_BUSY ;
  
  //Read INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x88884444));
  
  //Read INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0xBEEFBEEF));
  
  //Read INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x23456789));
  
  //Read INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x12345678));
  
  //Disable INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Read INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0xBEEFBEEF));
  
  //Read INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0xBEEFBEEF));
  
  disableFPComparatorLoadedWithAddress(0x12345678,INSTRUCTION_TYPE);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[3]);
  
}

void test_disableFPComparatorLoadedWithAddress_given_all_loaded_with_same_address_should_disable_all()
{
  instructionComparatorReady[0] = COMP_BUSY ;
  instructionComparatorReady[1] = COMP_BUSY ;
  instructionComparatorReady[2] = COMP_BUSY ;
  instructionComparatorReady[3] = COMP_BUSY ;
  instructionComparatorReady[4] = COMP_BUSY ;
  instructionComparatorReady[5] = COMP_BUSY ;
  
  //Read INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x12345678));
  
  //Disable INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Read INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x12345678));
  
  //Disable INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Read INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x12345678));
  
  //Disable INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Read INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x12345678));
  
  //Disable INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Read INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x12345678));
  
  //Disable INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Read INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x12345678));
  
  //Disable INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  disableFPComparatorLoadedWithAddress(0x12345678,INSTRUCTION_TYPE);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[1]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[2]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[3]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[4]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[5]);
}

void test_disableFPComparatorLoadedWithAddress_given_all_comparator_free_should_do_nothing()
{
  instructionComparatorReady[0] = COMP_READY ;
  instructionComparatorReady[1] = COMP_READY ;
  instructionComparatorReady[2] = COMP_READY ;
  instructionComparatorReady[3] = COMP_READY ;
  instructionComparatorReady[4] = COMP_READY ;
  instructionComparatorReady[5] = COMP_READY ;
  
  disableFPComparatorLoadedWithAddress(0,INSTRUCTION_TYPE);
}

void test_disableFPComparatorLoadedWithAddress_given_non_match_should_do_nothing()
{
  instructionComparatorReady[0] = COMP_BUSY ;
  instructionComparatorReady[1] = COMP_BUSY ;
  instructionComparatorReady[2] = COMP_BUSY ;
  instructionComparatorReady[3] = COMP_BUSY ;
  instructionComparatorReady[4] = COMP_BUSY ;
  instructionComparatorReady[5] = COMP_BUSY ;
  
  //Read INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  disableFPComparatorLoadedWithAddress(0xFFFFFFFF,INSTRUCTION_TYPE);
  
}

//LITERAL_TYPE
void test_disableFPComparatorLoadedWithAddress_given_found_literal_comp1_should_disable_comp1()
{

  literalComparatorReady[0] = COMP_BUSY ;
  literalComparatorReady[1] = COMP_BUSY ;
  
  //Read LITERAL_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(LITERAL_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x88884444));
  
  //Read LITERAL_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x12345678));
  
  //Disable LITERAL_COMP1
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  disableFPComparatorLoadedWithAddress(0x12345678,LITERAL_TYPE);
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[1]);
  
}

/*-------------------------disableAllFPBComparator-----------------------*/
void test_disableAllFPBComparator_should_disable_all_comparator_and_set_all_ready_flag_to_COMP_READY()
{
  instructionComparatorReady[0] = COMP_BUSY ;
  instructionComparatorReady[1] = COMP_BUSY ;
  instructionComparatorReady[2] = COMP_BUSY ;
  instructionComparatorReady[3] = COMP_BUSY ;
  instructionComparatorReady[4] = COMP_BUSY ;
  instructionComparatorReady[5] = COMP_BUSY ;
  
  literalComparatorReady[0] = COMP_BUSY ;
  literalComparatorReady[1] = COMP_BUSY ;
  
  //Disable INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable LITERAL_COMP0
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable LITERAL_COMP1
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  disableAllFPComparator();
  
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[1]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[2]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[3]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[4]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[5]);
  
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[1]);
}

/*-------------------------readAndUpdateComparatorReadyFlag-----------------------*/
//INSTRUCTION_TYPE
void test_readAndUpdateComparatorReadyFlag_should_read_all_Instruction_Comparator_and_update_readyFlag()
{
  instructionComparatorReady[0] = COMP_READY ;
  instructionComparatorReady[1] = COMP_READY ;
  instructionComparatorReady[2] = COMP_READY ;
  instructionComparatorReady[3] = COMP_READY ;
  instructionComparatorReady[4] = COMP_READY ;
  instructionComparatorReady[5] = COMP_READY ;
  
  //Read INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
  
  //Read INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x10000001));
  
  //Read INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
  
  //Read INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  //Read INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
  
  //Read INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0xF0000001));

  readAndUpdateComparatorReadyFlag(INSTRUCTION_TYPE);
  
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[1]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[2]);
  TEST_ASSERT_EQUAL(COMP_REMAP,instructionComparatorReady[3]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[4]);
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[5]);
}

//Literal_Type
void test_readAndUpdateComparatorReadyFlag_should_read_all_Literal_Comparator_and_update_readyFlag()
{
  literalComparatorReady[0] = COMP_READY ;
  literalComparatorReady[1] = COMP_READY ;
  
  //Read LITERAL_COMP0
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(LITERAL_COMP[0]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
  
  //Read LITERAL_COMP1
  emulateSwdRegisterWrite(TAR_REG, AP, OK, (uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(0x20000001));
  
  readAndUpdateComparatorReadyFlag(LITERAL_TYPE);
  
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_BUSY,literalComparatorReady[1]);
}

/*-------------------------initialiseFPBUnit-----------------------*/
void test_initialiseFPBUnit_should_disable_all_comparator_and_enable_FPB_unit()
{
  //Disable INSTRUCTION_COMP0
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP4
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[4]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable LITERAL_COMP0
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable LITERAL_COMP1
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Enable FPB Unit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  initialiseFPBUnit();
  
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[1]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[2]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[3]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[4]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[5]);
  
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[1]);
}

/*-------------------------removeAllFPComparatorSetToBreakpoint-----------------------*/
void test_removeAllFPComparatorSetToBreakpoint_should_only_disable_comparator_set_to_COMP_BUSY()
{
  instructionComparatorReady[0] = COMP_READY ;
  instructionComparatorReady[1] = COMP_BUSY ;
  instructionComparatorReady[2] = COMP_REMAP ;
  instructionComparatorReady[3] = COMP_BUSY ;
  instructionComparatorReady[4] = COMP_READY ;
  instructionComparatorReady[5] = COMP_BUSY ;
  
  //Disable INSTRUCTION_COMP1
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable INSTRUCTION_COMP3
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
 
  //Disable INSTRUCTION_COMP5
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  removeAllFPComparatorSetToBreakpoint();
  
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[1]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[3]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[5]);
}

/*-------------------------stopAllFPRemapping-----------------------*/
void test_stopAllFlashPatchRemapping_should_only_disable_comparator_set_to_COMP_REMAP()
{
  instructionComparatorReady[0] = COMP_READY ;
  instructionComparatorReady[1] = COMP_BUSY ;
  instructionComparatorReady[2] = COMP_REMAP ;
  instructionComparatorReady[3] = COMP_BUSY ;
  instructionComparatorReady[4] = COMP_READY ;
  instructionComparatorReady[5] = COMP_BUSY ;
  
  literalComparatorReady[0] = COMP_READY ;
  literalComparatorReady[1] = COMP_REMAP ;
  
  //Disable INSTRUCTION_COMP2
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  //Disable LITERAL_COMP1
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  stopAllFPRemapping();
  
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[2]);
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[1]);
}