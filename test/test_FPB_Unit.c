#include "unity.h"
#include <stdint.h>
#include "FPB_Unit.h"
#include "FPB_Utilities.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "Misc_Utilities.h"
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
  cswDataSize = CSW_WORD_SIZE ;
  
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  enableFPBUnit();
}

/*-------------------------disableFPBUnit-----------------------*/
void test_disablePBUnit_should_write_DISABLE_FPB_to_FP_CTRL()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  disableFPBUnit();
}

/*-------------------------setInstructionBreakpoint-----------------------*/
//MATCH_LOWERHALFWORD
void test_setInstructionBreakpoint_givenINSTRUCTION_COMP1_address_0xFFFFFFFF_MATCH_LOWERHALFWORD()
{
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP1);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x5FFFFFFD);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setInstructionBreakpoint(INSTRUCTION_COMP1,0xFFFFFFFF,MATCH_LOWERHALFWORD));
}

//MATCH_UPPERHALFWORD
void test_setInstructionBreakpoint_givenINSTRUCTION_COMP2_address_0xA55AA55A_MATCH_UPPERHALFWORD()
{
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP2);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x855AA559);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setInstructionBreakpoint(INSTRUCTION_COMP2,0xA55AA55A,MATCH_UPPERHALFWORD));
}

//MATCH_WORD
void test_setInstructionBreakpoint_givenINSTRUCTION_COMP3_address_0x02345670_MATCH_WORD()
{
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP3);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xC2345671);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setInstructionBreakpoint(INSTRUCTION_COMP3,0x02345670,MATCH_WORD));
}

/*-------------------------setInstructionRemapping-----------------------*/
void test_setInstructionRemapping_should_program_FP_REMAP_and_comparator()
{
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program FP_REMAP
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_REMAP);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x02222220);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP0);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x02345671);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setInstructionRemapping(INSTRUCTION_COMP0,0x02345670,0x22222222));
}
/*-------------------------setLiteralRemapping-----------------------*/
void test_setLiteralRemapping_should_program_FP_REMAP_and_literal_Comparator()
{
  //disableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program FP_REMAP
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_REMAP);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x14567880);
  
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,LITERAL_COMP1);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x12345679);
  
  //set to CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //enableFPBUnit
  emulateSwdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setLiteralRemapping(LITERAL_COMP1,0x12345678,0x3456789A));
}

/*-------------------------disableFPComparator-----------------------*/
void test_disableFPComparator_should_write_FP_COMP_DISABLE_to_the_selected_comparator()
{
  //Program comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP0);
	emulateSwdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  TEST_ASSERT_EQUAL(0,disableFPComparator(INSTRUCTION_COMP0));
}