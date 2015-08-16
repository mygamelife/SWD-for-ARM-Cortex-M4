#include "unity.h"
#include <stdint.h>
#include "FPB_Unit.h"
#include "FPB_Utilities.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "Misc_Utilities.h"
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

/*-------------------------enableFPBUnit-----------------------*/
void test_enableFPBUnit_should_write_ENABLE_FPB_to_FP_CTRL()
{
  cswDataSize = CSW_WORD_SIZE ;
  
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  enableFPBUnit();
}

/*-------------------------disableFPBUnit-----------------------*/
void test_disablePBUnit_should_write_DISABLE_FPB_to_FP_CTRL()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  disableFPBUnit();
}

/*-------------------------setInstructionBreakpoint-----------------------*/
//MATCH_LOWERHALFWORD
void test_setInstructionBreakpoint_givenINSTRUCTION_COMP1_address_0xFFFFFFFF_MATCH_LOWERHALFWORD()
{
  //disableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateswdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP1);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x5FFFFFFD);
  
  //set to CORE_DEBUG_MODE
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG);
  
  //enableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setInstructionBreakpoint(INSTRUCTION_COMP1,0xFFFFFFFF,MATCH_LOWERHALFWORD));
}

//MATCH_UPPERHALFWORD
void test_setInstructionBreakpoint_givenINSTRUCTION_COMP2_address_0xA55AA55A_MATCH_UPPERHALFWORD()
{
  //disableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateswdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP2);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x855AA559);
  
  //set to CORE_DEBUG_MODE
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG);
  
  //enableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setInstructionBreakpoint(INSTRUCTION_COMP2,0xA55AA55A,MATCH_UPPERHALFWORD));
}

//MATCH_WORD
void test_setInstructionBreakpoint_givenINSTRUCTION_COMP3_address_0x02345670_MATCH_WORD()
{
  //disableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program comparator
  emulateswdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP3);
	emulateswdRegisterWrite(DRW_REG,AP,4,0xC2345671);
  
  //set to CORE_DEBUG_MODE
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG);
  
  //enableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setInstructionBreakpoint(INSTRUCTION_COMP3,0x02345670,MATCH_WORD));
}

/*-------------------------setInstructionRemapping-----------------------*/
void test_setInstructionRemapping_should_program_FP_REMAP_and_comparator()
{
  //disableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program FP_REMAP
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_REMAP);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x02222220);
  
  //Program comparator
  emulateswdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP0);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x02345671);
  
  //set to CORE_DEBUG_MODE
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG);
  
  //enableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setInstructionRemapping(INSTRUCTION_COMP0,0x02345670,0x22222222));
}
/*-------------------------setLiteralRemapping-----------------------*/
void test_setLiteralRemapping_should_program_FP_REMAP_and_literal_Comparator()
{
  //disableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,DISABLE_FPB);
  
  //Program FP_REMAP
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_REMAP);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x14567880);
  
  //Program comparator
  emulateswdRegisterWrite(TAR_REG,AP,4,LITERAL_COMP1);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x12345679);
  
  //set to CORE_DEBUG_MODE
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG);
  
  //enableFPBUnit
  emulateswdRegisterWrite(TAR_REG,AP,4,FP_CTRL);
	emulateswdRegisterWrite(DRW_REG,AP,4,ENABLE_FPB);
  
  TEST_ASSERT_EQUAL(0,setLiteralRemapping(LITERAL_COMP1,0x12345678,0x3456789A));
}

/*-------------------------disableFPComparator-----------------------*/
void test_disableFPComparator_should_write_FP_COMP_DISABLE_to_the_selected_comparator()
{
  //Program comparator
  emulateswdRegisterWrite(TAR_REG,AP,4,INSTRUCTION_COMP0);
	emulateswdRegisterWrite(DRW_REG,AP,4,FP_COMP_DISABLE);
  
  TEST_ASSERT_EQUAL(0,disableFPComparator(INSTRUCTION_COMP0));
}