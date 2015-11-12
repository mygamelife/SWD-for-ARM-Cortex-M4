#include "unity.h"
#include <stdint.h>
#include "mock_FPBUnit.h"
#include "FPBUnitEx.h"
#include "CodeStepping.h"
#include "mock_CoreDebug.h"
#include "CoreDebugEx.h"
#include "Emulator.h"
#include "Swd.h"
#include "IoOperations.h"
#include "MemoryReadWrite.h"
#include "mock_IoOperationsEx.h"
#include "mock_configurePort.h"

void setUp(void) {}

void tearDown(void) {}

/*-------------------------isStackPointerPointingToDefaultLocation---------------------------*/
void test_isStackPointerPointingToDefaultLocation_given_0x2002ffe8_should_return_1()
{
  readCoreRegister_ExpectAndReturn(CORE_REG_SP,0x2002FFE8);
  
  TEST_ASSERT_EQUAL(1,isStackPointerPointingToDefaultLocation());
}

void test_isStackPointerPointingToDefaultLocation_given_false_should_return_0()
{
  readCoreRegister_ExpectAndReturn(CORE_REG_SP,0x2002FFFF);
  
  TEST_ASSERT_EQUAL(0,isStackPointerPointingToDefaultLocation());
}
/*-------------------------isSelectedAddressContains32bitsInstructionExtended---------------------------*/

void test_isSelectedAddressContains32bitsInstructionExtended_given_0xE800_should_return_1()
{
  cswDataSize = CSW_HALFWORD_SIZE ;
  
  uint32_t machineCode = 0 ;
  
	// Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xE800));
  
  TEST_ASSERT_EQUAL(1,isSelectedAddressContains32bitsInstructionExtended(0x10000000, &machineCode));
  TEST_ASSERT_EQUAL(0xE800, machineCode);
}

void test_isSelectedAddressContains32bitsInstructionExtended_given_0xF000_should_return_1()
{
  uint32_t machineCode = 0 ;
  
	//Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF000));
  
  TEST_ASSERT_EQUAL(1,isSelectedAddressContains32bitsInstructionExtended(0x10000000,&machineCode));
  TEST_ASSERT_EQUAL(0xF000,machineCode);
}

void test_isSelectedAddressContains32bitsInstructionExtended_given_0xF800_should_return_1()
{
  uint32_t machineCode = 0 ;
  
	//Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF800));
  
  TEST_ASSERT_EQUAL(1,isSelectedAddressContains32bitsInstructionExtended(0x10000000,&machineCode));
  TEST_ASSERT_EQUAL(0xF800,machineCode);
}

void test_isSelectedAddressContains32bitsInstructionExtended_given_0xFF00_should_return_1()
{
  uint32_t machineCode = 0 ;
  
	//Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xFF00));
  
  TEST_ASSERT_EQUAL(1,isSelectedAddressContains32bitsInstructionExtended(0x10000000,&machineCode));;
  TEST_ASSERT_EQUAL(0xFF00,machineCode);
}

void test_isSelectedAddressContains32bitsInstructionExtended_given_0xE700_should_return_0()
{
  uint32_t machineCode = 0 ;
  
	//Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xE700));
  
  TEST_ASSERT_EQUAL(0,isSelectedAddressContains32bitsInstructionExtended(0x10000000,&machineCode));
  TEST_ASSERT_EQUAL(0xE700,machineCode);
}

/*-------------------------isSelectedAddressContainsCallingSubroutineInstruction---------------------------*/
void test_isSelectedAddressContainsCallingSubroutineInstruction_given_0xF000D000_should_return_1()
{
  cswDataSize = CSW_WORD_SIZE ;
  
  //Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF000D000));
  
  TEST_ASSERT_EQUAL(1,isSelectedAddressContainsCallingSubroutineInstruction(0x10000000));
}

void test_isSelectedAddressContainsCallingSubroutineInstruction_given_0xF700D000_should_return_1()
{  
  //Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF700D000));
  
  TEST_ASSERT_EQUAL(1,isSelectedAddressContainsCallingSubroutineInstruction(0x10000000));
}
void test_isSelectedAddressContainsCallingSubroutineInstruction_given_0xF000F000_should_return_1()
{
  //Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF000F000));  

  TEST_ASSERT_EQUAL(1,isSelectedAddressContainsCallingSubroutineInstruction(0x10000000));
}

void test_isSelectedAddressContainsCallingSubroutineInstruction_given_0xF0000000_should_return_0()
{
  //Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF0000000));
  
  TEST_ASSERT_EQUAL(0,isSelectedAddressContainsCallingSubroutineInstruction(0x10000000));
}

void test_isSelectedAddressContainsCallingSubroutineInstruction_given_0xF800D000_should_return_0()
{
  //Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF800D000));
  
  TEST_ASSERT_EQUAL(0,isSelectedAddressContainsCallingSubroutineInstruction(0x10000000));
}

void test_isSelectedAddressContainsCallingSubroutineInstruction_given_0xFF00D000_should_return_0()
{
  //Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xFF00D000));
  
  TEST_ASSERT_EQUAL(0,isSelectedAddressContainsCallingSubroutineInstruction(0x10000000));
}

void test_isSelectedAddressContainsCallingSubroutineInstruction_given_0xE000D000_should_return_0()
{ 
  //Retrieve machine code
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x10000000);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xE000D000));
  
  TEST_ASSERT_EQUAL(0,isSelectedAddressContainsCallingSubroutineInstruction(0x10000000));
}

/*-------------------------isSelectedMachineCodeMatchesReturnFromSubroutineInstruction---------------------------*/
void test_isSelectedMachineCodeMatchesReturnFromSubroutineInstruction_given_match_should_return_1()
{
  TEST_ASSERT_EQUAL(1,isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(0xBD80));
  TEST_ASSERT_EQUAL(1,isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(0x4770));
  TEST_ASSERT_EQUAL(1,isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(0x46F7));
}

void test_isSelectedAddressContainsReturnFromSubroutineInstruction_given_unmatch_should_return_0()
{
  TEST_ASSERT_EQUAL(0,isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(0xBD81));
  TEST_ASSERT_EQUAL(0,isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(0x4771));
  TEST_ASSERT_EQUAL(0,isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(0x56F7));
}

/*-------------------------stepIntoOnce---------------------------*/
void test_stepIntoOnce_should_step_and_read_and_return_PC()
{
  stepOnly_Expect(1);
  
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x22334450);
  
  TEST_ASSERT_EQUAL(0x22334450,stepIntoOnce());
}

/*-------------------------stepOver---------------------------*/
void test_stepOver_should_do_stepInto_for_16bit_Instruction_and_return_PC()
{
  cswDataSize = CSW_HALFWORD_SIZE ;
  
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x22334450);
  
  //Check for 32bit instruction and get 16 bit instruction
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x22334450);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0x1000));
  
  stepOnly_Expect(1);
  
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x22334452);
  
  TEST_ASSERT_EQUAL(0x22334452,stepOver());
}

void test_stepOver_should_do_stepInto_for_32bit_normal_Instruction_and_return_PC()
{ 
  cswDataSize = CSW_HALFWORD_SIZE ;
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x22334450);
  
  //Check for 32bit instruction and true
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x22334450);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF800));
  
  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, SWD_DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, SWD_AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE | CSW_ENABLE_ADDR_INC_PACKED));
  
  //Read word at the address but get not match
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x22334450);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF8000000));
  
  stepOnly_Expect(1);
  
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x22334452);
  
  TEST_ASSERT_EQUAL(0x22334452,stepOver());
}

void test_stepOver_should_stepOver_by_setting_breakpoint_when_branch_is_met_and_return_PC()
{
  cswDataSize = CSW_HALFWORD_SIZE ;
  
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x22334450);
  
  //Check for 32bit instruction and true
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x22334450);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF800));
  
  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, SWD_DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, SWD_AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE | CSW_ENABLE_ADDR_INC_PACKED));
  
  //Read word at the address and match
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x22334450);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF000D000));
  
  //Set breakpoint
  autoSetInstructionBreakpoint_ExpectAndReturn(0x22334454,1);
  //Wait for breakpoint event
  readDebugEventRegister_ExpectAndReturn(0x2);
  //Disable breakpoint 
  disableFlashPatchInstructionComparator_ExpectAndReturn(1,0);
  //Clear breakpoint event
  emulateSwdRegisterWrite(TAR_REG,SWD_AP,4,DFSR_REG);
	emulateSwdRegisterWrite(DRW_REG,SWD_AP,4,BKPT_DEBUGEVENT);
  
  TEST_ASSERT_EQUAL(0x22334454,stepOver());
}

void test_stepOver_will_return_0_if_fail_to_stepOver_branch_instruction()
{
  cswDataSize = CSW_HALFWORD_SIZE ;
  
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x22334450);
  
  //Check for 32bit instruction and true
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x22334450);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF800));
  
  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, SWD_DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, SWD_AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE | CSW_ENABLE_ADDR_INC_PACKED));
  
  //Read word at the address and match
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x22334450);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xF000D000));
  
  //Set breakpoint and failed
  autoSetInstructionBreakpoint_ExpectAndReturn(0x22334454,-1);
  
  TEST_ASSERT_EQUAL(0,stepOver());
}

/*-------------------------stepOut---------------------------*/
void test_stepOut_given_outside_of_subroutine_should_return_0()
{
  readCoreRegister_ExpectAndReturn(CORE_REG_SP,INITIALMAINCODE_SP);
  
  TEST_ASSERT_EQUAL(0,stepOut());
}

void test_stepOut_given_inside_Subroutine_should_searchforReturnsubroutineInstruction_stepOnce_and_return_PC()
{
  cswDataSize = CSW_HALFWORD_SIZE ;
  readCoreRegister_ExpectAndReturn(CORE_REG_SP,0x2002FFE0);
  
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x11223330);
  
  //Selected addresss contains 32bit machine code ,next call pc = pc + 4
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x11223330);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xFF00));
  
  //Selected addresss contains 32bit machine code ,next call pc = pc + 4
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x11223334);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xFF00));
  
  //Selected addresss contains 32bit machine code ,next call pc = pc + 4
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x11223338);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xFF00));
  
  //Selected addresss contains 16bit machine code but didnt match any return from subroutine instruction , next call pc = pc + 2
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x1122333C);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xBD00));
  
  //Selected addresss contains 32bit machine code ,next call pc = pc + 4
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x1122333E);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xABCD));

  //Selected addresss contains 16bit machine code and matches return from subroutine instruction
  emulateSwdRegisterWrite(TAR_REG, SWD_AP, OK, 0x11223340);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, 0);
	emulateSwdRegisterRead(DRW_REG, SWD_AP, OK, 1, interconvertMSBandLSB(0xBD80));
  
  // Set breakpoint
  autoSetInstructionBreakpoint_ExpectAndReturn(0x11223340,1);
  // Wait for breakpoint event
  readDebugEventRegister_ExpectAndReturn(0x2);
  // Disable breakpoint 
  disableFlashPatchInstructionComparator_ExpectAndReturn(1,0);
  
  // Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, SWD_DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, SWD_AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE) );
  
  //Clear breakpoint event
  emulateSwdRegisterWrite(TAR_REG,SWD_AP,4,DFSR_REG);
	emulateSwdRegisterWrite(DRW_REG,SWD_AP,4,BKPT_DEBUGEVENT);
  
  //Step once to execute return from subroutine instruction
  stepOnly_Expect(1);
  readCoreRegister_ExpectAndReturn(CORE_REG_PC,0x11223340);
  
  TEST_ASSERT_EQUAL(0x11223340,stepOut());
}