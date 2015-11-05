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

int initFlag = 0;

void setUp(void) 
{
  if(initFlag == 0) 
    initFlag = 1;
  initMemoryReadWrite();
  /* Erase flash space according to size */
  _flashErase(0x08000000, 2000);
  enableDWTandITM();
}

void tearDown(void)
{
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();
}

void test_loadWatchpointTestProgram()
{
/* ---------------- Watchpoint TestCase Read LDRB -------------------- */
// 0x080003F0    F2433644   movw r6,#0x3344
// 0x080003F4    F2C11622   movt r6,#0x1122

// 0x080003F8    F64C4CDD   movw r12,0xCCDD 
// 0x080003FC    F6CA2CBB   movt r12,0xAABB

// 0x08000400    F240405C   movw r0,#0x045C
// 0x08000404    F2C20000   movt r0,#0x2000

// 0x08000408    F8C0C000   str r12,[r0]
// 0x0800040C    7806       ldrb r6,[r0]
// 0x0800040E    2121       movs r1,#33
// 0x08000410    E7FE       b.n	8000410

  _flashWrite(0x080003F0,0xF2433644,WORD_SIZE);
  _flashWrite(0x080003F4,0xF2C11622,WORD_SIZE);
  
  _flashWrite(0x080003F8,0xF64C4CDD,WORD_SIZE);
  _flashWrite(0x080003FC,0xF6CA2CBB,WORD_SIZE);
  
  _flashWrite(0x08000400,0xF240405C,WORD_SIZE);
  _flashWrite(0x08000404,0xF2C20000,WORD_SIZE);
  
  _flashWrite(0x08000408,0xF8C0C000,WORD_SIZE);
  _flashWrite(0x0800040C,0x7806,HALFWORD_SIZE);
  _flashWrite(0x0800040E,0x2121,HALFWORD_SIZE);
  _flashWrite(0x08000410,0xE7FE,HALFWORD_SIZE);
  
  
/* ---------------- Watchpoint TestCaseRead LDRH -------------------- */
// 0x080004F0    F2433644   movw r6,#0x3344
// 0x080004F4    F2C11622   movt r6,#0x1122

// 0x080004F8    F64C4CDD   movw r12,0xCCDD 
// 0x080004FC    F6CA2CBB   movt r12,0xAABB

// 0x08000500    F240405C   movw r0,#0x045C
// 0x08000504    F2C20000   movt r0,#0x2000

// 0x08000508    F8C0C000   str r12,[r0]
// 0x0800050C    8806       ldrH r6,[r0]
// 0x0800050E    2121       movs r1,#33
// 0x08000510    E7FE       b.n	8000410

  _flashWrite(0x080004F0,0xF2433644,WORD_SIZE);
  _flashWrite(0x080004F4,0xF2C11622,WORD_SIZE);
  
  _flashWrite(0x080004F8,0xF64C4CDD,WORD_SIZE);
  _flashWrite(0x080004FC,0xF6CA2CBB,WORD_SIZE);
  
  _flashWrite(0x08000500,0xF240405C,WORD_SIZE);
  _flashWrite(0x08000504,0xF2C20000,WORD_SIZE);
  
  _flashWrite(0x08000508,0xF8C0C000,WORD_SIZE);
  _flashWrite(0x0800050C,0x8806,HALFWORD_SIZE);
  _flashWrite(0x0800050E,0x2121,HALFWORD_SIZE);
  _flashWrite(0x08000510,0xE7FE,HALFWORD_SIZE);

  
/* ---------------- Watchpoint TestCaseRead LDR -------------------- */
// 0x080005F0    F2433644   movw r6,#0x3344
// 0x080005F4    F2C11622   movt r6,#0x1122

// 0x080005F8    F64C4CDD   movw r12,0xCCDD 
// 0x080005FC    F6CA2CBB   movt r12,0xAABB

// 0x08000600    F240405C   movw r0,#0x045C
// 0x08000604    F2C20000   movt r0,#0x2000

// 0x08000608    F8C0C000   str r12,[r0]
// 0x0800060C    6806       ldrb r6,[r0]
// 0x0800060E    2121       movs r1,#33
// 0x08000610    E7FE       b.n	8000410

  _flashWrite(0x080005F0,0xF2433644,WORD_SIZE);
  _flashWrite(0x080005F4,0xF2C11622,WORD_SIZE);
  
  _flashWrite(0x080005F8,0xF64C4CDD,WORD_SIZE);
  _flashWrite(0x080005FC,0xF6CA2CBB,WORD_SIZE);
  
  _flashWrite(0x08000600,0xF240405C,WORD_SIZE);
  _flashWrite(0x08000564,0xF2C20000,WORD_SIZE);
  
  _flashWrite(0x08000608,0xF8C0C000,WORD_SIZE);
  _flashWrite(0x0800060C,0x6806,HALFWORD_SIZE);
  _flashWrite(0x0800060E,0x2121,HALFWORD_SIZE);
  _flashWrite(0x08000610,0xE7FE,HALFWORD_SIZE);
  
  
/* ---------------- Watchpoint TestCaseWrite STRB -------------------- */
// 0x080006A0    F2433644   movw r6,#0x3344
// 0x080006A4    F2C11622   movt r6,#0x1122

// 0x080006A8    F240405C   movw r0,#0x045C
// 0x080006AC    F2C20000   movt r0,#0x2000

// 0x080006B0    7006       strb r6,[r0]
// 0x080006B2    2111       movs r1,#17
// 0x080006B4    2112       movs r1,#18
// 0x080006B6    2113       movs r1,#19
// 0x080006B8    2114       movs r1,#20
// 0x080006BA    2115       movs r1,#21
// 0x080006BC    E7FE       b.n	80005bc

  _flashWrite(0x080006A0,0xF2433644,WORD_SIZE);
  _flashWrite(0x080006A4,0xF2C11622,WORD_SIZE);
  
  _flashWrite(0x080006A8,0xF64C4CDD,WORD_SIZE);
  _flashWrite(0x080006AC,0xF6CA2CBB,WORD_SIZE);

  _flashWrite(0x080006B0,0x7006,HALFWORD_SIZE);
  _flashWrite(0x080006B2,0x2111,HALFWORD_SIZE);
  _flashWrite(0x080006B4,0x2112,HALFWORD_SIZE);
  _flashWrite(0x080006B6,0x2113,HALFWORD_SIZE);
  _flashWrite(0x080006B8,0x2114,HALFWORD_SIZE);
  _flashWrite(0x080006BA,0x2115,HALFWORD_SIZE);
  _flashWrite(0x080006BC,0xE7FE,HALFWORD_SIZE);

/* ---------------- Watchpoint TestCaseWrite STRH -------------------- */
// 0x080007A0    F2433644   movw r6,#0x3344
// 0x080007A4    F2C11622   movt r6,#0x1122

// 0x080007A8    F240405C   movw r0,#0x045C
// 0x080007AC    F2C20000   movt r0,#0x2000

// 0x080007B0    8006       strh r6,[r0]
// 0x080007B2    2111       movs r1,#17
// 0x080007B4    2112       movs r1,#18
// 0x080007B6    2113       movs r1,#19
// 0x080007B8    2114       movs r1,#20
// 0x080007BA    2115       movs r1,#21
// 0x080007BC    E7FE       b.n	80005bc

  _flashWrite(0x080007A0,0xF2433644,WORD_SIZE);
  _flashWrite(0x080007A4,0xF2C11622,WORD_SIZE);
  
  _flashWrite(0x080007A8,0xF64C4CDD,WORD_SIZE);
  _flashWrite(0x080007AC,0xF6CA2CBB,WORD_SIZE);

  _flashWrite(0x080007B0,0x8006,HALFWORD_SIZE);
  _flashWrite(0x080007B2,0x2111,HALFWORD_SIZE);
  _flashWrite(0x080007B4,0x2112,HALFWORD_SIZE);
  _flashWrite(0x080007B6,0x2113,HALFWORD_SIZE);
  _flashWrite(0x080007B8,0x2114,HALFWORD_SIZE);
  _flashWrite(0x080007BA,0x2115,HALFWORD_SIZE);
  _flashWrite(0x080007BC,0xE7FE,HALFWORD_SIZE);
  
/* ---------------- Watchpoint TestCaseWrite STR -------------------- */
// 0x080008A0    F2433644   movw r6,#0x3344
// 0x080008A4    F2C11622   movt r6,#0x1122

// 0x080008A8    F240405C   movw r0,#0x045C
// 0x080008AC    F2C20000   movt r0,#0x2000

// 0x080008B0    6006       str r6,[r0]
// 0x080008B2    2111       movs r1,#17
// 0x080008B4    2112       movs r1,#18
// 0x080008B6    2113       movs r1,#19
// 0x080008B8    2114       movs r1,#20
// 0x080008BA    2115       movs r1,#21
// 0x080008BC    E7FE       b.n	80005bc

  _flashWrite(0x080008A0,0xF2433644,WORD_SIZE);
  _flashWrite(0x080008A4,0xF2C11622,WORD_SIZE);
  
  _flashWrite(0x080008A8,0xF64C4CDD,WORD_SIZE);
  _flashWrite(0x080008AC,0xF6CA2CBB,WORD_SIZE);

  _flashWrite(0x080008B0,0x6006,HALFWORD_SIZE);
  _flashWrite(0x080008B2,0x2111,HALFWORD_SIZE);
  _flashWrite(0x080008B4,0x2112,HALFWORD_SIZE);
  _flashWrite(0x080008B6,0x2113,HALFWORD_SIZE);
  _flashWrite(0x080008B8,0x2114,HALFWORD_SIZE);
  _flashWrite(0x080008BA,0x2115,HALFWORD_SIZE);
  _flashWrite(0x080008BC,0xE7FE,HALFWORD_SIZE);
  
  /* ---------------- Watchpoint TestCaseDoubleWrite -------------------- */
// 0x080009D0    F240405C   movw r0,#0x045C
// 0x080009D4    F2C20000   movt r0,#0x2000

// 0x080009D8    210A       movs r1,#10
// 0x080009DA    6001       str r1,[r0]
// 0x080009DC    210B       movs r1,#11
// 0x080009DE    6001       str r1,[r0]

// 0x080009E0    F05F0B11   movw r11,#11
// 0x080009E4    F05F0B12   movw r11,#12
// 0x080009E8    F05F0B13   movw r11,#13
// 0x080009EC    F05F0B14   movw r11,#14
// 0x080009F0    F05F0B15   movw r11,#15

// 0x080009F4    E7FE       b.n	80006F4

  _flashWrite(0x080009D0,0xF240405C,WORD_SIZE);
  _flashWrite(0x080009D4,0xF2C20000,WORD_SIZE);
  
  _flashWrite(0x080009D8,0x210A,HALFWORD_SIZE);
  _flashWrite(0x080009DA,0x6001,HALFWORD_SIZE);
  _flashWrite(0x080009DC,0x210B,HALFWORD_SIZE);
  _flashWrite(0x080009DE,0x6001,HALFWORD_SIZE);
  
  _flashWrite(0x080009E0,0xF05F0B11,WORD_SIZE);
  _flashWrite(0x080009E4,0xF05F0B12,WORD_SIZE);
  _flashWrite(0x080009E8,0xF05F0B13,WORD_SIZE);
  _flashWrite(0x080009EC,0xF05F0B14,WORD_SIZE);
  _flashWrite(0x080009F0,0xF05F0B15,WORD_SIZE);
  

  _flashWrite(0x080009F4,0xE7FE,HALFWORD_SIZE);
  
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
