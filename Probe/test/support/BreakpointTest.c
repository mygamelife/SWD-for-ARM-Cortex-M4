#include "BreakpointTest.h"


/**
 * 0x500	 ________   <- set lower halfword breakpoint at 0x500
 * 0x502 	|________|
 * 0x504 	|________|
 *
 * Result : Breakpoint at 0x500
 */
void instructionBreakPointTestCase_2bytes_LowerHalfWord()
{
  uint32_t PC = 0  ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP0,0x080003C0,MATCH_LOWERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&PC);
  disableFPComparator(INSTRUCTION_COMP0);
  clearBreakpointDebugEvent();

  setCoreMode(CORE_NORMAL_MODE);
}

/**
 * 0x500	 ________   <- set upper halfword breakpoint at 0x500
 * 0x502 	|________|
 * 0x504 	|________|
 *
 * Result : Breakpoint at 0x502
 */
void instructionBreakPointTestCase_2bytes_UpperHalfWord()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP1,0x080003C0,MATCH_UPPERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&PC);
  disableFPComparator(INSTRUCTION_COMP1);
  clearBreakpointDebugEvent();

  setCoreMode(CORE_NORMAL_MODE);
}

/**
 * 0x500	 ________   <- set word breakpoint at 0x500
 * 0x502 	|________|
 * 0x504 	|________|
 *
 * Result : Breakpoint at 0x500
 */
void instructionBreakPointTestCase_2bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003C0,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&PC);
  disableFPComparator(INSTRUCTION_COMP2);
  clearBreakpointDebugEvent();

  setCoreMode(CORE_NORMAL_MODE);
}

/**
 * 0x500	 ________    <---set upper halfword breakpoint at 0x500
 *  		  |        |
 * 0x504 	|________|
 * 0x506 	|________|
 *
 * Result : Breakpoint will never occur
 */
void instructionBreakPointTestCase_4bytes_UpperHalfWord()
{
	  uint32_t PC = 0 ;

	  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003B0,MATCH_UPPERHALFWORD);

	  writeCoreRegister(CORE_REG_PC,0x080003B0);
	  setCoreMode(CORE_DEBUG_MODE);

	  while(!hasBreakpointDebugEventOccured());

	  readCoreRegister(CORE_REG_PC,&PC);
	  disableFPComparator(INSTRUCTION_COMP2);
	  clearBreakpointDebugEvent();

	  setCoreMode(CORE_NORMAL_MODE);
}


/**
 * 0x500	 ________    <---set word breakpoint at 0x500
 *  		  |        |
 * 0x504 	|________|
 * 0x506 	|________|
 *
 * Result : Breakpoint at 0x500
 */
void instructionBreakPointTestCase_4bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003B0,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003B0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&PC);
  disableFPComparator(INSTRUCTION_COMP2);
  clearBreakpointDebugEvent();

  setCoreMode(CORE_NORMAL_MODE);
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
void instructionBreakPointTestCase_2bytes_4bytes_4bytes_LowerHalfword()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003D4,MATCH_LOWERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&PC);
  disableFPComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  setCoreMode(CORE_NORMAL_MODE);
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
void instructionBreakPointTestCase_2bytes_4bytes_4bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003D4,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&PC);
  disableFPComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  setCoreMode(CORE_NORMAL_MODE);
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
void instructionBreakPointTestCase_2bytes_4bytes_2bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003E4,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003E0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&PC);
  disableFPComparator(INSTRUCTION_COMP3);
  clearBreakpointDebugEvent();

  setCoreMode(CORE_NORMAL_MODE);
}