#include "unity.h"
#include <stdint.h>
#include "FPBUnit.h"
#include "FPBUnitEx.h"
#include "Delay.h"
#include "CoreDebug.h"
#include "CoreDebugEx.h"
#include "CodeStepping.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "configurePort.h"
#include "LowLevelIO.h"
#include <string.h>
#include "Tlv.h"
#include "Tlv_ex.h"
#include <malloc.h>
#include "Read_File.h"
#include "GetHeaders.h"
#include "ProgramElf.h"
#include "uart.h"
#include "ProgramLoader.h"
#include "ErrorCode.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "Interface.h"
#include "GetTime.h"

void setUp(void) 
{
  initialiseFPBUnit();
  setCoreMode(CORE_DEBUG_HALT);
}

void tearDown(void) {}
{
  disableAllFlashPatchComparator();
  setCoreMode(CORE_NORMAL_MODE);
}

void test_loadProgram()
{
  tlvLoadProgram(Tlv_Session *session, char *file, TLV_WRITE_FLASH);
}

/**
 * 0x500	 ________   <- set lower halfword breakpoint at 0x500
 * 0x502 	|________|
 * 0x504 	|________|
 *
 * Result : Breakpoint at 0x500
 */
void test_instructionBreakPointTestCase_2bytes_LowerHalfWord()
{
  uint32_t PC = 0  ;
  
  manualSetInstructionBreakpoint(INSTRUCTION_COMP0,0x080003C0,MATCH_LOWERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFPComparator(INSTRUCTION_COMP0);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003C0,PC);
}

/**
 * 0x500	 ________   <- set upper halfword breakpoint at 0x500
 * 0x502 	|________|
 * 0x504 	|________|
 *
 * Result : Breakpoint at 0x502
 */
void test_instructionBreakPointTestCase_2bytes_UpperHalfWord()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP1,0x080003C0,MATCH_UPPERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFPComparator(INSTRUCTION_COMP1);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003C0,PC);
}

/**
 * 0x500	 ________   <- set word breakpoint at 0x500
 * 0x502 	|________|
 * 0x504 	|________|
 *
 * Result : Breakpoint at 0x500
 */
void test_instructionBreakPointTestCase_2bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003C0,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFPComparator(INSTRUCTION_COMP2);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003C0,PC);
}

/**
 * 0x500	 ________    <---set upper halfword breakpoint at 0x500
 *  		  |        |
 * 0x504 	|________|
 * 0x506 	|________|
 *
 * Result : Breakpoint will never occur
 */
void test_instructionBreakPointTestCase_4bytes_UpperHalfWord()
{
	  uint32_t PC = 0 , data = 0, fail = 0;

	  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003B0,MATCH_UPPERHALFWORD);

	  writeCoreRegister(CORE_REG_PC,0x080003B0);
	  setCoreMode(CORE_DEBUG_MODE);

	  while(!hasBreakpointDebugEventOccured()
    {
      PC = readCoreRegister(CORE_REG_PC);
      memoryReadHalfWord(PC,&data);
      if(data == 0xE7FE)
      {
        fail = 1 ;
        break ;
      }
      setCoreMode(CORE_DEBUG_MODE);
    }

	  PC = readCoreRegister(CORE_REG_PC);
	  disableFPComparator(INSTRUCTION_COMP2);
	  clearBreakpointDebugEvent();

    if(fail)
      TEST_ASSERT_EQUAL(0x80003b6,PC);
}


/**
 * 0x500	 ________    <---set word breakpoint at 0x500
 *  		  |        |
 * 0x504 	|________|
 * 0x506 	|________|
 *
 * Result : Breakpoint at 0x500
 */
void test_instructionBreakPointTestCase_4bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003B0,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003B0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFPComparator(INSTRUCTION_COMP2);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003B0,PC);
}

/**
 * 0x500	 ________
 * 0x502 	|________|
 * 		 	  |        | <----set lower halfword breakpoint at 0x504
 * 0x506 	|________|
 *        |        |
 * 0x50A	|________|
 *
 * Result : Breakpoint will never occur
 */
void test_instructionBreakPointTestCase_2bytes_4bytes_4bytes_LowerHalfword()
{
  uint32_t PC = 0, data = 0 ,fail= 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003D4,MATCH_LOWERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured())
    {
      PC = readCoreRegister(CORE_REG_PC);
      memoryReadHalfWord(PC,&data);
      if(data == 0xE7FE)
      {
        fail = 1 ;
        break ;
      }
      setCoreMode(CORE_DEBUG_MODE);
    }

  PC = readCoreRegister(CORE_REG_PC);
  disableFPComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  if(fail)
    TEST_ASSERT_EQUAL(0x80003da,PC);
}


/**
 * 0x500	 ________
 * 0x502 	|________|
 * 		 	  | 		   | <----set word breakpoint at 0x504
 * 0x506 	|________|
 *        |        |
 * 0x50A	|________|
 *
 * Result : Breakpoint at 0x506
 */
void test_instructionBreakPointTestCase_2bytes_4bytes_4bytes_Word()
{
  uint32_t PC = 0 ;
  
  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003D4,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFPComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003D6,PC);
}

/**
 * 0x500	 ________
 * 0x502 	|________|
 * 		 	  | 		   | <----set word breakpoint at 0x504
 * 0x506 	|________|
 * 0x508	|________|
 *
 * Result : Breakpoint at 0x506
 */
void test_instructionBreakPointTestCase_2bytes_4bytes_2bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003E4,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003E0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFPComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003E6,PC);
}