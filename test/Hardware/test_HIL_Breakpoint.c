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
#include "CodeStepping.h"

#define CODE_SIZE (sizeof(machineCode) / sizeof(uint16_t))

uint16_t machineCode[] = 
{
  0xBF00,         //0x080003BE  nop
  // 0x2117,         //0x080003C0  r1,23
  // 0xE7FE,         //0x080003C2  b.
  // 0x8001, 0xF3AF, //0x080003C4  nop.w
  // 0x01FF, 0xF04F, //0x080003C8  mov r1,#0xFF
  // 0xE7FE,         //0x080003CC  b.
  // 0xBF00,         //0x080003CE  nop
  // 0xBF00,         //0x080003D0  nop
  // 0x01FF, 0xF04F, //0x080003D2  mov r1,#0xFF
  // 0xF04F, 0x02FF, //0x080003D6  mov r1,#0xFF
  // 0xE7FE,         //0x080003DA  b.
  // 0x8000, 0xF3AF, //0x080003CC  nop.w
  // 0x210D,         //0x080003E0  movs r1,#13
//  0x01FF, 0xF04F, //0x080003E2  mov r1,#0xFF
//  0x2117,         //0x080003E6  movs r1,#23
//  0xE7FE,         //0x080003E8  b.
//  0xBF00,         //0x080003EA  nop.
  
};

static int initFlag = 0;
static void loadBreakpointTestProgram();

void setUp(void) 
{
  CEXCEPTION_T err = 0;
  
  Try {
    if(initFlag == 0) 
    {  
      system("rake target:release[FlashProgrammer/FlashProgrammer.coproj]");
      initFlag = 1;
      initMemoryReadWrite();
      setCoreMode(CORE_DEBUG_HALT);
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

int verifyEqualProgram(uint32_t startingAddress,uint16_t *machineCode,int codeSize)
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
    printf("Erasing \n");

    /* Erase flash space according to size */
    if(_flashErase(0x08000000, 2000) ==1);
      printf("Complete erasing\n");
    
    // writeCoreRegister(CORE_REG_PC,0x20000000);
    pc = readCoreRegister(CORE_REG_PC);
    printf("PC before flashing %x\n",pc);
    setCoreMode(CORE_DEBUG_MODE);
    
    
    
    printf("Flashing \n");
    Try{
      for(i = 0 ; i < CODE_SIZE ; i++,address +=2)
      _flashWrite(address,machineCode[i],HALFWORD_SIZE);
    }
    Catch(err)
    {
      displayErrorMessage(err);
    }
  }
  

}

void test_nothing()
{
  uint32_t pc = 0 , afterPC = 0 ,data1 = 0,data2 = 0 , dhcsr1 = 0,dhcsr2 = 0, dhcsr3 = 0 ;
  
  // memoryReadWord(DHCSR_REG,&dhcsr1);
  // printf("Inital DHCSR_REG %x\n",dhcsr1);
  
  // writeCoreRegister(CORE_REG_PC,0x080003BE);
  // pc = readCoreRegister(CORE_REG_PC);
  
  // memoryReadHalfword(pc,&data1);
  // printf("Initial PC %x contains %x\n",pc,data1);
  
  // stepOnly(1);
  
  // memoryReadWord(DHCSR_REG,&dhcsr2);
  // printf("Mid DHCSR_REG %x\n",dhcsr2);
  
  // afterPC = readCoreRegister(CORE_REG_PC);
  // memoryReadHalfword(afterPC,&data2);
  
  // printf("After PC %x contains %x\n",afterPC,data2);
  
  // memoryReadWord(DHCSR_REG,&dhcsr3);
  // printf("After DHCSR_REG %x\n",dhcsr3);
}

/**
 * 0x080003C0	   ________   <- set lower halfword breakpoint at 0x080003C0
 * 0x080003C2 	|________|
 * 0x080003C4 	|________|
 *
 * Result : Breakpoint at 0x080003C0
 */
void xtest_instructionBreakPointTestCase_2bytes_LowerHalfWord()
{ 
  uint32_t PC = 0 ;
  int i = 0 ;
  
  writeCoreRegister(CORE_REG_PC,0x080003BE);
  manualSetInstructionBreakpoint(INSTRUCTION_COMP0,0x080003C0,MATCH_LOWERHALFWORD);
  setCoreMode(CORE_DEBUG_MODE);
  
  //while(!hasBreakpointDebugEventOccured());

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
void xtest_instructionBreakPointTestCase_2bytes_UpperHalfWord()
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
void xtest_instructionBreakPointTestCase_2bytes_Word()
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
void xtest_instructionBreakPointTestCase_4bytes_UpperHalfWord()
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
 * 0x080003C8	   ________    <---set word breakpoint at 0x080003C8
 *  		        |        |
 * 0x080003CC   |________|
 * 0x080003CE	  |________|
 *
 * Result : Breakpoint at 0x080003C8
 */
void xtest_instructionBreakPointTestCase_4bytes_Word()
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
 * 0x080003D2	   ________
 * 0x080003D4 	|________|
 * 		 	        |        | <----set lower halfword breakpoint at 0x080003D6
 * 0x080003D8   |________|
 *              |        |
 * 0x080003DC	  |________|
 *
 * Result : Breakpoint will never occur
 */
void xtest_instructionBreakPointTestCase_2bytes_4bytes_4bytes_LowerHalfword()
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
 * 0x080003D0	   ________
 * 0x080003D2 	|________|
 * 		 	        |        | <----set word breakpoint at 0x080003D4
 * 0x080003D6   |________|
 *              |        |
 * 0x080003DA	  |________|
 * 0x080003DC
 *
 * Result : Breakpoint at 0x080003D6
 */
void xtest_instructionBreakPointTestCase_2bytes_4bytes_4bytes_Word()
{
  uint32_t PC = 0 ;
  int fail = 0 ;
  uint32_t data = 0 ;
  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003D4,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003D0);
  PC = readCoreRegister(CORE_REG_PC);
  printf("Initial PC %x\n",PC);
  printf("Data %x\n",data);
  stepOnly(1);
    PC = readCoreRegister(CORE_REG_PC);
  printf("Stepped PC %x\n",PC);
  stepOnly(1);
    memoryReadWord(PC,&data);
  printf("Data %x\n",data);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured())
    {
      PC = readCoreRegister(CORE_REG_PC);
      if(fail < 10)
      {
        printf("PC %x\n",PC);
        fail  ++;
      }
        else
          break ;
      setCoreMode(CORE_DEBUG_MODE);
    }

  PC = readCoreRegister(CORE_REG_PC);
  disableFlashPatchInstructionComparator(INSTRUCTION_COMP3);

  TEST_ASSERT_EQUAL(0x080003D6,PC);
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
void xtest_instructionBreakPointTestCase_2bytes_4bytes_2bytes_Word()
{
  uint32_t PC = 0 ;
  int fail = 0 ;
  manualSetInstructionBreakpoint(INSTRUCTION_COMP3,0x080003E6,MATCH_WORD);

  writeCoreRegister(CORE_REG_PC,0x080003E0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasBreakpointDebugEventOccured())
    {
      PC = readCoreRegister(CORE_REG_PC);
      if(fail < 10)
      {
        printf("PC %x\n",PC);
        fail  ++;
      }
        else
          break ;
      setCoreMode(CORE_DEBUG_MODE);
    }

  PC = readCoreRegister(CORE_REG_PC);
  disableFlashPatchInstructionComparator(INSTRUCTION_COMP3);

  TEST_ASSERT_EQUAL(0x080003E6,PC);
}
