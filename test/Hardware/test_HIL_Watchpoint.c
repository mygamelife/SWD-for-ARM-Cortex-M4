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
#include "DWTUnit.h"
#include "DwTUnitEx.h"

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

  0xF64C,0x4CDD,   //0x0810003C  movw r12,0xCCDD 
  0xF6CA,0x2CBB,   //0x08100040  movt r12,0xAABB

  0xF240,0x405C,   //0x08100044  movw r0,#0x045C
  0xF2C2,0x0000,   //0x08100048  movt r0,#0x2000

  0xF8C0,0xC000,   //0x0810005C  str r12,[r0]
  0x8806,          //0x08100060  ldrH r6,[r0]
  0x2121,          //0x08100062  movs r1,#33
  0xE7FE,          //0x08100064  b.
  0xBF00,          //0x08100066  nop

// /* ---------------- Watchpoint TestCaseRead LDR -------------------- */
  0xF243,0x3644,   //0x08100068  movw r6,#0x3344
  0xF2C1,0x1622,   //0x0810007C  movt r6,#0x1122

  0xF64C,0x4CDD,   //0x08100080  movw r12,0xCCDD 
  0xF6CA,0x2CBB,   //0x08100084  movt r12,0xAABB

  0xF240,0x405C,   //0x08100088  movw r0,#0x045C
  0xF2C2,0x0000,   //0x0810009C  movt r0,#0x2000

  0xF8C0,0xC000,   //0x08100100  str r12,[r0]
  0x6806,          //0x08100104  ldrb r6,[r0]
  0x2121,          //0x08100106  movs r1,#33
  0xE7FE,          //0x08100108  b.
  0xBF00,          //0x0810010A  nop

/* ---------------- Watchpoint TestCaseWrite STRB -------------------- */
  0xF243,0x3644,   //0x0810010E  movw r6,#0x3344
  0xF2C1,0x1622,   //0x08100112  movt r6,#0x1122

  0xF240,0x405C,   //0x08100116  movw r0,#0x045C
  0xF2C2,0x0000,   //0x0810011A  movt r0,#0x2000

  0x7006,          //0x0810011E  strb r6,[r0]
  0x2111,          //0x08100120  movs r1,#17
  0x2112,          //0x08100122  movs r1,#18
  0x2113,          //0x08100124  movs r1,#19
  0x2114,          //0x08100126  movs r1,#20
  0x2115,          //0x08100128  movs r1,#21
  0xE7FE,          //0x0810012A  b.
  0xBF00,          //0x0810012C  nop

// /* ---------------- Watchpoint TestCaseWrite STRH -------------------- */
  0xF243,0x3644,   //0x0810012E  movw r6,#0x3344
  0xF2C1,0x1622,   //0x08100132  movt r6,#0x1122

  0xF240,0x405C,   //0x08100136  movw r0,#0x045C
  0xF2C2,0x0000,   //0x0810013A  movt r0,#0x2000

  0x8006,          //0x0810013C  strh r6,[r0]
  0x2111,          //0x0810013E  movs r1,#17
  0x2112,          //0x08100140  movs r1,#18
  0x2113,          //0x08100142  movs r1,#19
  0x2114,          //0x08100144  movs r1,#20
  0x2115,          //0x08100146  movs r1,#21
  0xE7FE,          //0x08100148  b.
  0xBF00,          //0x0810014A  nop

/* ---------------- Watchpoint TestCaseWrite STR -------------------- */
  0xF243,0x3644,   //0x0810014C  movw r6,#0x3344
  0xF2C1,0x1622,   //0x08100150  movt r6,#0x1122

  0xF240,0x405C,   //0x08100154  movw r0,#0x045C
  0xF2C2,0x0000,   //0x08100158  movt r0,#0x2000

  0x6006,          //0x0810015C  str r6,[r0]
  0x2111,          //0x0810015E  movs r1,#17
  0x2112,          //0x08100160  movs r1,#18
  0x2113,          //0x08100162  movs r1,#19
  0x2114,          //0x08100164  movs r1,#20
  0x2115,          //0x08100168  movs r1,#21
  0xE7FE,          //0x0810016A  b.
  0xBF00,          //0x0810016C  nop

/* ---------------- Watchpoint TestCaseDoubleWrite -------------------- */
  0xF240,0x405C,   //0x0810016E  movw r0,#0x045C
  0xF2C2,0x0000,   //0x08100172  movt r0,#0x2000

  0x210A,          //0x08100176  movs r1,#10
  0x6001,          //0x08100178  str r1,[r0]
  0x210B,          //0x0810017A  movs r1,#11
  0x6001,          //0x0810017C  str r1,[r0]

  0xF05F,0x0B11,   //0x0810017E  movw r11,#11
  0xF05F,0x0B12,   //0x08100182  movw r11,#12
  0xF05F,0x0B13,   //0x08100186  movw r11,#13
  0xF05F,0x0B14,   //0x0810018A  movw r11,#14
  0xF05F,0x0B15,   //0x0810018C  movw r11,#15

  0xE7FE,          //0x08100190  b.
  0xBF00,          //0x08100192  nop

};

int initFlag = 0;
static void loadWatchpointTestProgram();

void setUp(void) 
{
  if(initFlag == 0) 
  {
    initFlag = 1;
    initMemoryReadWrite();
    /* Erase flash space according to size */
    // _flashErase(0x08000000, 2000);
    loadWatchpointTestProgram();
  }
  enableDWTandITM();
}

void tearDown(void)
{
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();
}

static void loadWatchpointTestProgram()
{
  int i , isIdenticalProgram = 1 ;
  
  uint32_t machineCodeReadFromTarget[CODE_SIZE] ;
  uint32_t address = 0x08100000 ;
  
  for(i = 0 ; i <CODE_SIZE ; i++, address+=2)
  {  
    memoryReadHalfword(address,&machineCodeReadFromTarget[i]);
    printf("Address : %x \tMachineCodeRead %x vs MachineCode %x\n",address,machineCodeReadFromTarget[i],machineCode[i]);
    if(machineCodeReadFromTarget[i] != machineCode[i])
    {
      isIdenticalProgram = 0 ;
      break ;
    }
  }
  
  if(isIdenticalProgram == 0)
  {
    address = 0x08100000 ;
    printf("Flashing program\n");
    for(i = 0 ; i < CODE_SIZE ; i++,address +=2)
      _flashWrite(address,machineCode[i],HALFWORD_SIZE);
  }
  else
    printf("No Flashing is required\n");
}

void test_datawatchpoint_TestCase_ReadByte_LDRB()
{
  uint32_t pc = 0 ;
  
  writeCoreRegister(CORE_REG_PC,0x080003F0);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_BYTE,WATCHPOINT_READ);

  setCoreMode(CORE_DEBUG_MODE);

  pc = readCoreRegister(CORE_REG_PC);

  //TEST_ASSERT_EQUAL(0x080003FC,pc);
}

/*
 * Result: Watchpoint event not occur
 */
void xtest_datawatchpoint_TestCase_ReadHalfword_LDRB()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_ReadWord_LDRB()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}



void xtest_datawatchpoint_TestCase_ReadByte_LDRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080004F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}


void xtest_datawatchpoint_TestCase_ReadHalfword_LDRH()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080004F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_ReadWord_LDRH()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080004F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}




void xtest_datawatchpoint_TestCase_ReadByte_LDR()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080005F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_ReadHalfword_LDR()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080005F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_ReadWord_LDR()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080005F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}


void xtest_datawatchpoint_TestCase_WriteByte_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080006A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteHalfword_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080006A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteWord_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080006A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteByte_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080007A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteHalfword_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080007A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteWord_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080007A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}




void xtest_datawatchpoint_TestCase_WriteByte_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080008A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());
  
  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteHalfword_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080008A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteWord_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080008A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_DoubleWrite()
{
	  uint32_t pc = 0 ;
	  uint32_t r1 = 0 ;

	  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xA,WATCHPOINT_WORD,WATCHPOINT_WRITE);

	  writeCoreRegister(CORE_REG_PC,0x080009D0);
	  setCoreMode(CORE_DEBUG_MODE);

	  while(!hasDWTTrapDebugEventOccured());


	  pc = readCoreRegister(CORE_REG_PC);
	  r1 = readCoreRegister(CORE_REG_R1);

	  // TEST_ASSERT_EQUAL(,pc);
}
