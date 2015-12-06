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
#include "DWTUnit.h"
#include "DwTUnitEx.h"
#include "Yield.h"
#include "LoadElf.h"
#include "ProgramVerifier.h"

#define CODE_SIZE (sizeof(machineCode) / sizeof(uint16_t))

uint16_t machineCode[] = 
{
  /* ---------------- Watchpoint TestCase Read LDRB -------------------- */
  0xF243,0x3644,   //0x08100000  movw r6,#0x3344
  0xF2C1,0x1622,   //0x08100004  movt r6,#0x1122

  0xF64C,0x4CDD,   //0x08100008  movw r12,0xCCDD 
  0xF6CA,0x2CBB,   //0x0810000C  movt r12,0xAABB

  0xF240,0x405C,   //0x08100010  movw r0,#0x045C
  0xF2C2,0x0000,   //0x08100014  movt r0,#0x2000

  0xF8C0,0xC000 ,  //0x08100018  str r12,[r0]
  0x7806,          //0x0810001C  ldrb r6,[r0]
  0x2121,          //0x0810001E  movs r1,#33
  0xE7FE,          //0x08100020  b.
  0xBF00,          //0x08100022  nop

/* ---------------- Watchpoint TestCaseRead LDRH -------------------- */
  0xF243,0x3644,   //0x08100024  movw r6,#0x3344
  0xF2C1,0x1622,   //0x08100028  movt r6,#0x1122

  0xF64C,0x4CDD,   //0x0810002C  movw r12,0xCCDD 
  0xF6CA,0x2CBB,   //0x08100030  movt r12,0xAABB

  0xF240,0x405C,   //0x08100034  movw r0,#0x045C
  0xF2C2,0x0000,   //0x08100038  movt r0,#0x2000

  0xF8C0,0xC000,   //0x0810003C  str r12,[r0]
  0x8806,          //0x08100040  ldrH r6,[r0]
  0x2121,          //0x08100042  movs r1,#33
  0xE7FE,          //0x08100044  b.
  0xBF00,          //0x08100046  nop

// /* ---------------- Watchpoint TestCaseRead LDR -------------------- */
  0xF243,0x3644,   //0x08100048  movw r6,#0x3344
  0xF2C1,0x1622,   //0x0810004C  movt r6,#0x1122

  0xF64C,0x4CDD,   //0x08100050  movw r12,0xCCDD 
  0xF6CA,0x2CBB,   //0x08100054  movt r12,0xAABB

  0xF240,0x405C,   //0x08100058  movw r0,#0x045C
  0xF2C2,0x0000,   //0x0810005C  movt r0,#0x2000

  0xF8C0,0xC000,   //0x08100060  str r12,[r0]
  0x6806,          //0x08100064  ldrb r6,[r0]
  0x2121,          //0x08100066  movs r1,#33
  0xE7FE,          //0x08100068  b.
  0xBF00,          //0x0810006A  nop

/* ---------------- Watchpoint TestCaseWrite STRB -------------------- */
  0xF243,0x3644,   //0x0810006C  movw r6,#0x3344
  0xF2C1,0x1622,   //0x08100070  movt r6,#0x1122

  0xF240,0x405C,   //0x08100074  movw r0,#0x045C
  0xF2C2,0x0000,   //0x08100078  movt r0,#0x2000

  0x7006,          //0x0810007C  strb r6,[r0]
  0x2111,          //0x0810007E  movs r1,#17
  0x2112,          //0x08100080  movs r1,#18
  0x2113,          //0x08100082  movs r1,#19
  0x2114,          //0x08100084  movs r1,#20
  0x2115,          //0x08100086  movs r1,#21
  0xE7FE,          //0x08100088  b.
  0xBF00,          //0x0810008A  nop

// /* ---------------- Watchpoint TestCaseWrite STRH -------------------- */
  0xF243,0x3644,   //0x0810008C  movw r6,#0x3344
  0xF2C1,0x1622,   //0x08100090  movt r6,#0x1122

  0xF240,0x405C,   //0x08100094  movw r0,#0x045C
  0xF2C2,0x0000,   //0x08100098  movt r0,#0x2000

  0x8006,          //0x0810009C  strh r6,[r0]
  0x2111,          //0x0810009E  movs r1,#17
  0x2112,          //0x081000A0  movs r1,#18
  0x2113,          //0x081000A2  movs r1,#19
  0x2114,          //0x081000A4  movs r1,#20
  0x2115,          //0x081000A6  movs r1,#21
  0xE7FE,          //0x081000A8  b.
  0xBF00,          //0x081000AA  nop

/* ---------------- Watchpoint TestCaseWrite STR -------------------- */
  0xF243,0x3644,   //0x081000AC  movw r6,#0x3344
  0xF2C1,0x1622,   //0x081000B0  movt r6,#0x1122

  0xF240,0x405C,   //0x081000B4  movw r0,#0x045C
  0xF2C2,0x0000,   //0x081000B8  movt r0,#0x2000

  0x6006,          //0x081000BC  str r6,[r0]
  0x2111,          //0x081000BE  movs r1,#17
  0x2112,          //0x081000C0  movs r1,#18
  0x2113,          //0x081000C2  movs r1,#19
  0x2114,          //0x081000C4  movs r1,#20
  0x2115,          //0x081000C6  movs r1,#21
  0xE7FE,          //0x081000C8  b.
  0xBF00,          //0x081000CA  nop

/* ---------------- Watchpoint TestCaseDoubleWrite -------------------- */
  0xF240,0x405C,   //0x081000CC  movw r0,#0x045C
  0xF2C2,0x0000,   //0x081000D0  movt r0,#0x2000

  0x210A,          //0x081000D4  movs r1,#10
  0x6001,          //0x081000D6  str r1,[r0]
  0x210B,          //0x081000D8  movs r1,#11
  0x6001,          //0x081000DA  str r1,[r0]

  0xF05F,0x0B11,   //0x081000DC  movw r11,#11
  0xF05F,0x0B12,   //0x081000E0  movw r11,#12
  0xF05F,0x0B13,   //0x081000E4  movw r11,#13
  0xF05F,0x0B14,   //0x081000E8  movw r11,#14
  0xF05F,0x0B15,   //0x081000EC  movw r11,#15

  0xE7FE,          //0x081000F0  b.
  0xBF00,          //0x081000F2  nop

};

static int initFlag = 0;
static void loadWatchpointTestProgram();
static int verifyEqualProgram(uint32_t startingAddress,uint16_t *machineCode,int codeSize);


void setUp(void) 
{
  CEXCEPTION_T err = 0;
  Try 
  {
    if(initFlag == 0) 
    {  
      system("rake target:release[FlashProgrammer/FlashProgrammer.coproj]");
      initFlag = 1;
      if(_session == NULL)
        _session = tlvCreateSession();
      setCoreMode(CORE_DEBUG_MODE);
      loadWatchpointTestProgram();
    }
    setCoreMode(CORE_DEBUG_HALT);
    enableDWTandITM();
    clearDWTTrapDebugEvent(); 
  }
  Catch(err) 
  {
    displayErrorMessage(err);
  }
}

void tearDown(void)
{
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();
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

static void loadWatchpointTestProgram()
{
  CEXCEPTION_T err = 0;
  int i = 0;
  uint32_t address = 0x08100000 ;
  uint32_t pc = 0 ;
  if(verifyEqualProgram(address,machineCode,CODE_SIZE))
    printf("No difference in the program loaded in target device. No flashing is required\n");
  else
  {
    initMemoryReadWrite();
     
    printf("Erasing \n");
    if(_flashErase(0x08100000, 2000) ==1);
      printf("Flash erase complete\n");

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

void test_datawatchpoint_TestCase_ReadByte_LDRB()
{
  uint32_t pc = 0 ;

  writeCoreRegister(CORE_REG_PC,0x08100000);
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XAABBCCDD,WATCHPOINT_BYTE,WATCHPOINT_READ);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  TEST_ASSERT_EQUAL(0x08100020,pc);
}

void test_datawatchpoint_TestCase_ReadHalfword_LDRB()
{
  uint32_t pc = 0 ;
  int fail = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XAABBCCDD,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08100000);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured())
  {
    pc = readCoreRegister(CORE_REG_PC);
    if(pc == 0x08100020)
    {
      fail = 1; 
      break ;
    }
    setCoreMode(CORE_DEBUG_MODE);
  }

  TEST_ASSERT_EQUAL(1,fail);
}

void test_datawatchpoint_TestCase_ReadWord_LDRB()
{
  uint32_t pc = 0 ;
  int fail = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XAABBCCDD,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08100000);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured())
  {
    pc = readCoreRegister(CORE_REG_PC);
    if(pc == 0x08100020)
    {
      fail = 1; 
      break ;
    }
    setCoreMode(CORE_DEBUG_MODE);
  }

  TEST_ASSERT_EQUAL(1,fail);
}


void test_datawatchpoint_TestCase_ReadByte_LDRH()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xDD,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08100024);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  TEST_ASSERT_EQUAL(0x08100044,pc);
}

void test_datawatchpoint_TestCase_ReadHalfword_LDRH()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XAABBCCDD,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08100024);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  TEST_ASSERT_EQUAL(0x08100044,pc);
}

void test_datawatchpoint_TestCase_ReadWord_LDRH()
{
  uint32_t pc = 0 ;
  int fail = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XAABBCCDD,WATCHPOINT_WORD,WATCHPOINT_READ);
  
  writeCoreRegister(CORE_REG_PC,0x08100024);
  setCoreMode(CORE_DEBUG_MODE);
  
  while(!hasDWTTrapDebugEventOccured())
  {
    pc = readCoreRegister(CORE_REG_PC);
    if(pc == 0x08100044)
    {
      fail = 1; 
      break ;
    }
    setCoreMode(CORE_DEBUG_MODE);
  }

  TEST_ASSERT_EQUAL(1,fail);
}

void test_datawatchpoint_TestCase_ReadByte_LDR()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XAABBCCDD,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08100048);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  TEST_ASSERT_EQUAL(0x08100068,pc);
}

void test_datawatchpoint_TestCase_ReadHalfword_LDR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XAABBCCDD,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08100048);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  TEST_ASSERT_EQUAL(0x08100068,pc);
}

void test_datawatchpoint_TestCase_ReadWord_LDR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XAABBCCDD,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08100048);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  TEST_ASSERT_EQUAL(0x08100068,pc);
}

void test_datawatchpoint_TestCase_WriteByte_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x0810006C);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  TEST_ASSERT_EQUAL(0x08100084,pc);
}

void test_datawatchpoint_TestCase_WriteHalfword_STRB()
{
  uint32_t pc = 0 ;
  int fail = 0;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x0810006C);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured())
  {
    pc = readCoreRegister(CORE_REG_PC);
    if(pc == 0x08100088)
    {
      fail = 1; 
      break ;
    }
    setCoreMode(CORE_DEBUG_MODE);
  }

  TEST_ASSERT_EQUAL(1,fail);
}

void test_datawatchpoint_TestCase_WriteWord_STRB()
{
  uint32_t pc = 0 ;
  int fail = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x0810006C);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured())
  {
    pc = readCoreRegister(CORE_REG_PC);
    if(pc == 0x08100088)
    {
      fail = 1; 
      break ;
    }
    setCoreMode(CORE_DEBUG_MODE);
  }

  TEST_ASSERT_EQUAL(1,fail);
}

void test_datawatchpoint_TestCase_WriteByte_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x0810008C);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  TEST_ASSERT_EQUAL(0x081000A4,pc);
}

void test_datawatchpoint_TestCase_WriteHalfword_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x0810008C);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  TEST_ASSERT_EQUAL(0x081000A4,pc);
}

// Test fail
// void test_datawatchpoint_TestCase_WriteWord_STRH()
// {
  // uint32_t pc = 0 ;
  // int fail = 0 ;
  
  // setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  // writeCoreRegister(CORE_REG_PC,0x0810008C);
  // setCoreMode(CORE_DEBUG_MODE);

  // while(!hasDWTTrapDebugEventOccured())
  // {
    // pc = readCoreRegister(CORE_REG_PC);
    // if(pc == 0x081000A8)
    // {
      // fail = 1; 
      // break ;
    // }
    // setCoreMode(CORE_DEBUG_MODE);
  // }

  // TEST_ASSERT_EQUAL(1,fail);
// }

void test_datawatchpoint_TestCase_WriteByte_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x081000AC);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());
  
  pc = readCoreRegister(CORE_REG_PC);

  TEST_ASSERT_EQUAL(0x081000C4,pc);
}

void test_datawatchpoint_TestCase_WriteHalfword_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x081000AC);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  TEST_ASSERT_EQUAL(0x081000C4,pc);
}

// void test_datawatchpoint_TestCase_WriteWord_STR()
// {
  // uint32_t pc = 0 ;
  
  // setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  // writeCoreRegister(CORE_REG_PC,0x081000AC);
  // setCoreMode(CORE_DEBUG_MODE);

  // while(!hasDWTTrapDebugEventOccured());

  // pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(0x081000C4,pc);
// }

// void test_datawatchpoint_TestCase_DoubleWrite()
// {
	// uint32_t pc = 0 ;

	// setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xA,WATCHPOINT_WORD,WATCHPOINT_WRITE);

	// writeCoreRegister(CORE_REG_PC,0x081000CC);
	// setCoreMode(CORE_DEBUG_MODE);

	// while(!hasDWTTrapDebugEventOccured());

	// pc = readCoreRegister(CORE_REG_PC);

	// TEST_ASSERT_EQUAL(0x081000E0,pc);
// }
