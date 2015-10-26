#include "unity.h"
#include "Tlv.h"
#include "uart.h"
#include "TlvEx.h"
#include "SystemTime.h"
#include "ErrorCode.h"
#include "Read_File.h"
#include "GetHeaders.h"
#include "Interface.h"
#include "ProgramElf.h"
#include "ProgramLoader.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "StringObject.h"
#include "Token.h"
#include "IdentifierToken.h"
#include "NumberToken.h"
#include "OperatorToken.h"
#include "FileToken.h"
#include "MemoryReadWrite.h"
#include "CoreDebug.h"
#include "CoreDebugEx.h"
#include "FPBUnit.h"
#include "FPBUnitEx.h"
#include "CodeStepping.h"

void setUp(void) 
{
  initMemoryReadWrite();
  initialiseFPBUnit();
  setCoreMode(CORE_DEBUG_HALT);
}

void tearDown(void) 
{
  disableAllFPComparator();
  setCoreMode(CORE_NORMAL_MODE);
}

/* ---------------- Breakpoint Test Case 2 Bytes -------------------- */
// 0x080003C0    2117    movs r1,23
// 0x080003C2    E7FE    b.n	80003c2 
// 0x080003C4


void test_programBreakpointTestCase2Bytes()
{
  memoryWriteHalfword(0x080003C0,0x2117);
  memoryWriteHalfword(0x080003C2,0xE7FE);
}

/**
 * 0x080003C0	   ________   <- set lower halfword breakpoint at 0x080003C0
 * 0x080003C2 	|________|
 * 0x080003C4 	|________|
 *
 * Result : Breakpoint at 0x080003C0
 */
void test_instructionBreakPointTestCase_2bytes_LowerHalfWord()
{
  uint32_t PC = 0  ;
  
  manualSetInstructionBreakpoint(INSTRUCTION_COMP0,0x080003C0,MATCH_LOWERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableInstructionComparator(INSTRUCTION_COMP0);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003C0,PC);
}

/**
 * 0x080003C0	   ________   <- set upper halfword breakpoint at 0x080003C0
 * 0x080003C2 	|________|
 * 0x080003C4 	|________|
 *
 * Result : Breakpoint at 0x080003C2
 */
void test_instructionBreakPointTestCase_2bytes_UpperHalfWord()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP1,0x080003C0,MATCH_UPPERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableInstructionComparator(INSTRUCTION_COMP1);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003C2,PC);
}

/**
 * 0x500	 ________   <- set word breakpoint at 0x080003C0
 * 0x502 	|________|
 * 0x504 	|________|
 *
 * Result : Breakpoint at 0x080003C2
 */
void test_instructionBreakPointTestCase_2bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003C0,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableInstructionComparator(INSTRUCTION_COMP2);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003C0,PC);
}

/* ---------------- Breakpoint Test Case 4 Bytes -------------------- */
// 0x080003B0    F04F01FF    mov r1,#0xFF
// 0x080003B4    E7FE        b.n	80003b4
// 0x080003B6


void test_programBreakpointTestCase4Bytes()
{
  memoryWriteWord(0x080003B0,0xF04F01FF);
  memoryWriteHalfword(0x080003B4,0xE7FE);
}



/**
 * 0x080003B0	   ________    <---set upper halfword breakpoint at 0x080003B0
 *  		        |        |
 * 0x080003B4   |________|
 * 0x080003B6	  |________|
 *
 * Result : Breakpoint will never occur
 */
void test_instructionBreakPointTestCase_4bytes_UpperHalfWord()
{
	  uint32_t PC = 0 , data = 0;
    int fail = 0 ;
    
	  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003B0,MATCH_UPPERHALFWORD);

	  writeCoreRegister(CORE_REG_PC,0x080003B0);
	  setCoreMode(CORE_DEBUG_MODE);

	  while(!hasBreakpointDebugEventOccured())
    {
      PC = readCoreRegister(CORE_REG_PC);
      if(PC == 0x080003B4)
      {
        fail = 1;
        break;
      }
      setCoreMode(CORE_DEBUG_MODE);
    }


  disableInstructionComparator(INSTRUCTION_COMP2);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(1,fail); 
  TEST_ASSERT_EQUAL(0x080003B4,PC);
}


/**
 * 0x080003B0	   ________    <---set word breakpoint at 0x080003B0
 *  		        |        |
 * 0x080003B4   |________|
 * 0x080003B6	  |________|
 *
 * Result : Breakpoint at 0x080003B0
 */
void test_instructionBreakPointTestCase_4bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003B0,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003B0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableInstructionComparator(INSTRUCTION_COMP2);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003B0,PC);
}

/* ---------------- Breakpoint Test Case 2 Bytes 4 Bytes 4 Bytes -------------------- */
// 0x080003D0    210D        movs r1,#13
// 0x080003D2    F04F01FF    mov r1,#0xFF
// 0x080003D6    F04F02FF    mov r2,#0xFF
// 0x080003DA    E7FE        b.n	80003da
// 0x080003DC


void test_programBreakpointTestCase2Bytes_4bytes_4bytes()
{
  memoryWriteHalfword(0x080003D0,0x210D);
  memoryWriteWord(0x080003D2,0xF04F01FF);
  memoryWriteWord(0x080003D6,0xF04F02FF);
  memoryWriteHalfword(0x080003DA,0xE7FE);
}


/**
 * 0x080003D0	   ________
 * 0x080003D2 	|________|
 * 		 	        |        | <----set lower halfword breakpoint at 0x080003D4
 * 0x080003D6   |________|
 *              |        |
 * 0x080003DA	  |________|
 * 0x080003DC
 *
 * Result : Breakpoint will never occur
 */
void test_instructionBreakPointTestCase_2bytes_4bytes_4bytes_LowerHalfword()
{
  uint32_t PC = 0 ;
  int fail ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003D4,MATCH_LOWERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured())
    {
      PC = readCoreRegister(CORE_REG_PC);
      if(PC == 0x080003DA)
      {
        fail = 1;
        break ;
      }
      setCoreMode(CORE_DEBUG_MODE);
    }

  PC = readCoreRegister(CORE_REG_PC);
  disableInstructionComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003DA,PC);
  TEST_ASSERT_EQUAL(1,fail);
}


/**
 * 0x080003D0	   ________
 * 0x080003D2 	|________|
 * 		 	        |        | <----set word breakpoint at 0x080003D4
 * 0x080003D6   |________|
 *              |        |
 * 0x080003DA	  |________|
 * 0x080003DC
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
  disableInstructionComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003D6,PC);
}

/* ---------------- Breakpoint Test Case 2 Bytes 4 Bytes 2 Bytes -------------------- */
// 0x080003E0    210D        movs r1,#13
// 0x080003E2    F04F01FF    mov r1,#0xFF
// 0x080003E6    2117        movs r1,#23
// 0x080003E8    E7FE        b.n	80003E8 <breakpoint_TestCase_4bytes+0x4>
// 0x080003EA


void test_programBreakpointTestCase2Bytes_4bytes_2bytes()
{
  memoryWriteHalfword(0x080003E0,0x210D);
  memoryWriteWord(0x080003E2,0xF04F01FF);
  memoryWriteHalfword(0x080003E6,0x2117);
  memoryWriteHalfword(0x080003E8,0xE7FE);
}

/**
 * 0x080003E0	   ________
 * 0x080003E2 	|________|
 * 		 	        | 		   | <----set word breakpoint at 0x080003E4
 * 0x080003E6 	|________|
 * 0x080003E8	  |________|
 *
 * Result : Breakpoint at 0x080003E6
 */
void test_instructionBreakPointTestCase_2bytes_4bytes_2bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003E4,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003E0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableInstructionComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  TEST_ASSERT_EQUAL(0x080003E6,PC);
}