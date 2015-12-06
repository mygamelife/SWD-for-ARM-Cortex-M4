#include "unity.h"
#include "Tlv.h"
#include "Uart.h"
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
#include "Yield.h"
#include "LoadElf.h"
#include "ProgramVerifier.h"

#define CODE_SIZE (sizeof(machineCode) / sizeof(uint16_t))

uint16_t machineCode[] = 
{
  0xBF00,         //0x080003BE  nop
  0x2117,         //0x080003C0  r1,23
  0xE7FE,         //0x080003C2  b.
  0xF3AF, 0x8000, //0x080003C4  nop.w
  0xF04F, 0x010F, //0x080003C8  mov r1,#0x0F
  0xE7FE,         //0x080003CC  b.
  0xF3AF, 0x8000, //0x080003CE  nop.w
  0xF04F, 0x011F, //0x080003D2  mov r1,#0x1F
  0xF04F, 0x012F, //0x080003D6  mov r1,#0x2F
  0xE7FE,         //0x080003DA  b.
  0xF3AF, 0x8000, //0x080003CC  nop.w
  0x210D,         //0x080003E0  movs r1,#13
  0xF04F, 0x013F, //0x080003E2  mov r1,#0x3F
  0x2117,         //0x080003E6  movs r1,#23
  0xE7FE,         //0x080003E8  b.
  0xBF00,         //0x080003EA  nop.
};

static int initFlag = 0;
static int verifyEqualProgram(uint32_t startingAddress,uint16_t *machineCode,int codeSize);
static void loadBreakpointTestProgram();

void setUp(void) 
{
  CEXCEPTION_T err = 0;
  
  Try 
  {
    if(initFlag == 0) 
    {  
      initFlag = 1;
      initMemoryReadWrite();
      
      setCoreMode(CORE_DEBUG_MODE);
      loadBreakpointTestProgram();
    }
    setCoreMode(CORE_DEBUG_HALT);
    clearBreakpointDebugEvent();
    enableFPBUnit();    
  }
  Catch(err) 
  {
    displayErrorMessage(err);
  }
}

void tearDown(void) 
{
  clearBreakpointDebugEvent();
  disableFPBUnit();
}

static int verifyEqualProgram(uint32_t startingAddress,uint16_t *machineCode,int codeSize)
{
  int i = 0;
  uint32_t machineCodeReadFromTarget[codeSize] ;
  
  for(i = 0 ; i < codeSize ; i++, startingAddress+=2)
  {  
    memoryReadHalfword(startingAddress,&machineCodeReadFromTarget[i]);
    
    if(machineCodeReadFromTarget[i] != machineCode[i])
    {
      printf("Difference in program found !\n");
      printf("Address : %x \tMachineCodeRead %x vs MachineCode %x\n",startingAddress,machineCodeReadFromTarget[i],machineCode[i]);
      return 0 ;
    }
  }
  
  return 1 ;
}

static void loadBreakpointTestProgram()
{
  CEXCEPTION_T err = 0;
  int i = 0;
  uint32_t address = 0x080003BE ;
  
  uint32_t pc = 0 ;
  if(verifyEqualProgram(address,machineCode,CODE_SIZE))
    printf("No difference in the program loaded in target device. No flashing is required\n");
  else
  {
    printf("Flashing \n");
    Try {
      _flashWrite(address,(uint8_t *)machineCode,CODE_SIZE*2);
    }
    Catch(err) {
      displayErrorMessage(err);
    }
  }
}

/**
 * 0x080003C0	 ________   <- set lower halfword breakpoint at 0x080003C0
 * 0x080003C2 	|________|
 * 0x080003C4 	|________|
 *
 * Result : Breakpoint at 0x080003C0
 */
void test_instructionBreakPointTestCase_2bytes_LowerHalfWord()
{ 
  uint32_t PC = 0 ;
  int i = 0 ;
  
  writeCoreRegister(CORE_REG_PC,0x080003BE);
  manualSetInstructionBreakpoint(INSTRUCTION_COMP0,0x080003C0,MATCH_LOWERHALFWORD);
  setCoreMode(CORE_DEBUG_MODE);
  
  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFlashPatchInstructionComparator(INSTRUCTION_COMP0);

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

  writeCoreRegister(CORE_REG_PC,0x080003BE);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFlashPatchInstructionComparator(INSTRUCTION_COMP1);

  TEST_ASSERT_EQUAL(0x080003C2,PC);
}

/**
 * 0x080003C0	   ________   <- set word breakpoint at 0x080003C0
 * 0x080003C2 	|________|
 * 0x080003C4 	|________|
 *
 * Result : Breakpoint at 0x080003C2
 */
void test_instructionBreakPointTestCase_2bytes_Word()
{
  uint32_t PC = 0 ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003C0,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003BE);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFlashPatchInstructionComparator(INSTRUCTION_COMP2);

  TEST_ASSERT_EQUAL(0x080003C0,PC);
}

/**
 * 0x080003C8	   ________    <---set upper halfword breakpoint at 0x080003C8
 *  		        |        |
 * 0x080003CC   |________|
 * 0x080003CE	  |________|
 *
 * Result : Breakpoint will never occur
 */
void test_instructionBreakPointTestCase_4bytes_UpperHalfWord()
{
  uint32_t PC = 0 , data = 0;
  int fail = 0 ;
    
  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003C8,MATCH_UPPERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003C4);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured())
  {
    PC = readCoreRegister(CORE_REG_PC);
    if(PC > 0x080003C8)
    {
      fail = 1;
      break;
    }
    setCoreMode(CORE_DEBUG_MODE);
  }
  disableFlashPatchInstructionComparator(INSTRUCTION_COMP2);

  TEST_ASSERT_EQUAL(1,fail); 
}

/**
 * 0x080003C8	 ________    <---set word breakpoint at 0x080003C8
 *  		    |        |
 * 0x080003CC   |________|
 * 0x080003CE	|________|
 *
 * Result : Breakpoint at 0x080003C8
 */
void test_instructionBreakPointTestCase_4bytes_Word()
{
  uint32_t PC = 0 ;
  uint32_t dataRead = 0 ;
  manualSetInstructionBreakpoint(INSTRUCTION_COMP2,0x080003C8,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003C4);
  memoryReadWord(DFSR_REG,&dataRead);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured());

  PC = readCoreRegister(CORE_REG_PC);
  disableFlashPatchInstructionComparator(INSTRUCTION_COMP2);

  TEST_ASSERT_EQUAL(0x080003C8,PC);
}

/**
 * 0x080003D2	 ________
 * 0x080003D4 	|________|
 * 		 	    |        | <----set lower halfword breakpoint at 0x080003D6
 * 0x080003D8   |________|
 *              |        |
 * 0x080003DC	|________|
 *
 * Result : Breakpoint will never occur
 */
void test_instructionBreakPointTestCase_2bytes_4bytes_4bytes_LowerHalfword()
{
  uint32_t PC = 0 ;
  int fail ;

  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003D6,MATCH_LOWERHALFWORD);

  writeCoreRegister(CORE_REG_PC,0x080003D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured())
    {
      PC = readCoreRegister(CORE_REG_PC);
      if(PC > 0x080003D6)
      {
        fail = 1;
        break ;
      }
      setCoreMode(CORE_DEBUG_MODE);
    }

  PC = readCoreRegister(CORE_REG_PC);
  disableFlashPatchInstructionComparator(INSTRUCTION_COMP3);

  TEST_ASSERT_EQUAL(1,fail);
}

/**
 * 0x080003D0	  ________
 * 0x080003D2 	|________|
 * 		 	    |        | <----set word breakpoint at 0x080003D4
 * 0x080003D6   |________|
 *              |        |
 * 0x080003DA	|________|
 * 0x080003DC
 *
 * Result : Breakpoint will never occur
 */
void test_instructionBreakPointTestCase_2bytes_4bytes_4bytes_Word()
{
  uint32_t PC = 0 ;
  int fail = 0 ;
  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003D4,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003D2);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured())
    {
      PC = readCoreRegister(CORE_REG_PC);
      if(PC > 0x080003D4)
      {
        fail = 1;
        break ;
      }
      setCoreMode(CORE_DEBUG_MODE);
    }

  disableFlashPatchInstructionComparator(INSTRUCTION_COMP3);

  TEST_ASSERT_EQUAL(1,fail);
}

/**
 * 0x080003E0	   ________
 * 0x080003E2 	  |________|
 * 		 	      | 	   | <----set word breakpoint at 0x080003E4
 * 0x080003E6 	  |________|
 * 0x080003E8	  |________|
 *
 * Result : Breakpoint will never occur
 */
void test_instructionBreakPointTestCase_2bytes_4bytes_2bytes_Word()
{
  uint32_t PC = 0 ;
  int fail = 0 ;
  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003E6,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003E0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured())
    {
      PC = readCoreRegister(CORE_REG_PC);
      if(PC > 0x080003E0)
      {
		fail = 1;
		break ;
	  }
      setCoreMode(CORE_DEBUG_MODE);
    }

  disableFlashPatchInstructionComparator(INSTRUCTION_COMP3);

  TEST_ASSERT_EQUAL(1,fail);
}