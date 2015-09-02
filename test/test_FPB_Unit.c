#include "unity.h"
#include <stdint.h>
#include "FPB_Unit.h"
#include "FPB_Utilities.h"
#include "Delay.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "Emulator.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"

void setUp(void)
{
}

void tearDown(void)
{
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
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x5FFFFFFD);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,manualSetInstructionBreakpoint(INSTRUCTION_COMP1,0xFFFFFFFF,MATCH_LOWERHALFWORD));
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[1]);
}

//MATCH_UPPERHALFWORD
void test_manualSetInstructionBreakpoint_givenINSTRUCTION_COMP2_address_0xA55AA55A_MATCH_UPPERHALFWORD()
{
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[2]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x855AA559);

  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0xA55AA55A,MATCH_UPPERHALFWORD));
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[2]);
}

//MATCH_WORD
void test_manualSetInstructionBreakpoint_givenINSTRUCTION_COMP3_address_0x02345670_MATCH_WORD()
{
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[3]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xC2345671);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
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
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program FP_REMAP
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_REMAP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x02222220);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[0]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x02345671);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,manualSetInstructionRemapping(INSTRUCTION_COMP0,0x02345670,0x22222222));
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[0]);
}
/*-------------------------manualSetLiteralRemapping-----------------------*/
void test_manualSetLiteralRemapping_should_program_FP_REMAP_and_literal_Comparator()
{
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program FP_REMAP
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_REMAP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x14567880);

  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(LITERAL_COMP[1]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x12345679);

  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,manualSetLiteralRemapping(LITERAL_COMP1,0x12345678,0x3456789A));
  TEST_ASSERT_EQUAL(COMP_BUSY,literalComparatorReady[1]);
}

/*-------------------------disableInstructionComparator-----------------------*/
void test_disableInstructionComparator_should_write_FP_COMP_DISABLE_to_the_selected_instruction_comparator()
{
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
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
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
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));

  readAndUpdateComparatorReadyFlag(INSTRUCTION_TYPE);
  
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[1]);
  TEST_ASSERT_EQUAL(COMP_READY,instructionComparatorReady[2]);
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[3]);
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
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, interconvertMSBandLSB(1));
  
  readAndUpdateComparatorReadyFlag(LITERAL_TYPE);
  
  TEST_ASSERT_EQUAL(COMP_READY,literalComparatorReady[0]);
  TEST_ASSERT_EQUAL(COMP_BUSY,literalComparatorReady[1]);
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
  
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(INSTRUCTION_COMP[5]));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x5FFFFFFD);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)&(FPB->FP_CTRL));
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,autoSetInstructionBreakpoint(0xFFFFFFFF,MATCH_LOWERHALFWORD));
  TEST_ASSERT_EQUAL(COMP_BUSY,instructionComparatorReady[5]);
  
}