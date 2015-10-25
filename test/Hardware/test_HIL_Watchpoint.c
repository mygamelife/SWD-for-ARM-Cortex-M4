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

void setUp(void) 
{
  initMemoryReadWrite();
  setCoreMode(CORE_DEBUG_HALT);
  enableDWTandITM();
}

void tearDown(void)
{
  disableDWTandITM();
  setCoreMode(CORE_NORMAL_MODE);
}


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


void test_programWatchpoint_TestCaseRead_LDRB()
{
  memoryWriteWord(0x080003F0,0xF2433644);
  memoryWriteWord(0x080003F4,0xF2C11622);
  
  memoryWriteWord(0x080003F8,0xF64C4CDD);
  memoryWriteWord(0x080003FC,0xF6CA2CBB);
  
  memoryWriteWord(0x08000400,0xF240405C);
  memoryWriteWord(0x08000404,0xF2C20000);
  
  memoryWriteWord(0x08000408,0xF8C0C000);
  memoryWriteHalfword(0x0800040C,0x7806);
  memoryWriteHalfword(0x0800040E,0x2121);
  memoryWriteHalfword(0x08000410,0xE7FE);
}

void test_datawatchpoint_TestCase_ReadByte_LDRB()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  printf("\n\n PC : %x",pc);
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
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

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
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

/* ---------------- Watchpoint TestCaseRead LDRH -------------------- */
// 0x080003F0    F2433644   movw r6,#0x3344
// 0x080003F4    F2C11622   movt r6,#0x1122

// 0x080003F8    F64C4CDD   movw r12,0xCCDD 
// 0x080003FC    F6CA2CBB   movt r12,0xAABB

// 0x08000400    F240405C   movw r0,#0x045C
// 0x08000404    F2C20000   movt r0,#0x2000

// 0x08000408    F8C0C000   str r12,[r0]
// 0x0800040C    8806       ldrb r6,[r0]
// 0x0800040E    2121       movs r1,#33
// 0x08000410    E7FE       b.n	8000410


void xtest_programWatchpoint_TestCaseRead_LDRH()
{
  memoryWriteWord(0x080003F0,0xF2433644);
  memoryWriteWord(0x080003F4,0xF2C11622);
  
  memoryWriteWord(0x080003F8,0xF64C4CDD);
  memoryWriteWord(0x080003FC,0xF6CA2CBB);
  
  memoryWriteWord(0x08000400,0xF240405C);
  memoryWriteWord(0x08000404,0xF2C20000);
  
  memoryWriteWord(0x08000408,0xF8C0C000);
  memoryWriteHalfword(0x0800040C,0x8806);
  memoryWriteHalfword(0x0800040E,0x2121);
  memoryWriteHalfword(0x08000410,0xE7FE);
}


void xtest_datawatchpoint_TestCase_ReadByte_LDRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}


void xtest_datawatchpoint_TestCase_ReadHalfword_LDRH()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_ReadWord_LDRH()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

/* ---------------- Watchpoint TestCaseRead LDR -------------------- */
// 0x080003F0    F2433644   movw r6,#0x3344
// 0x080003F4    F2C11622   movt r6,#0x1122

// 0x080003F8    F64C4CDD   movw r12,0xCCDD 
// 0x080003FC    F6CA2CBB   movt r12,0xAABB

// 0x08000400    F240405C   movw r0,#0x045C
// 0x08000404    F2C20000   movt r0,#0x2000

// 0x08000408    F8C0C000   str r12,[r0]
// 0x0800040C    6806       ldrb r6,[r0]
// 0x0800040E    2121       movs r1,#33
// 0x08000410    E7FE       b.n	8000410


void xtest_programWatchpoint_TestCaseRead_LDR()
{
  memoryWriteWord(0x080003F0,0xF2433644);
  memoryWriteWord(0x080003F4,0xF2C11622);
  
  memoryWriteWord(0x080003F8,0xF64C4CDD);
  memoryWriteWord(0x080003FC,0xF6CA2CBB);
  
  memoryWriteWord(0x08000400,0xF240405C);
  memoryWriteWord(0x08000404,0xF2C20000);
  
  memoryWriteWord(0x08000408,0xF8C0C000);
  memoryWriteHalfword(0x0800040C,0x6806);
  memoryWriteHalfword(0x0800040E,0x2121);
  memoryWriteHalfword(0x08000410,0xE7FE);
}


void xtest_datawatchpoint_TestCase_ReadByte_LDR()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_ReadHalfword_LDR()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_ReadWord_LDR()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}


/* ---------------- Watchpoint TestCaseWrite STRB -------------------- */
// 0x080005A0    F2433644   movw r6,#0x3344
// 0x080005A4    F2C11622   movt r6,#0x1122

// 0x080005A8    F240405C   movw r0,#0x045C
// 0x080005AC    F2C20000   movt r0,#0x2000

// 0x080005B0    7006       strb r6,[r0]
// 0x080005B2    2111       movs r1,#17
// 0x080005B4    2112       movs r1,#18
// 0x080005B6    2113       movs r1,#19
// 0x080005B8    2114       movs r1,#20
// 0x080005BA    2115       movs r1,#21
// 0x080005BC    E7FE       b.n	80005bc

void xtest_programWatchpoint_TestCaseWrite_STRB()
{
  memoryWriteWord(0x080005A0,0xF2433644);
  memoryWriteWord(0x080005A4,0xF2C11622);
  
  memoryWriteWord(0x080005A8,0xF64C4CDD);
  memoryWriteWord(0x080005AC,0xF6CA2CBB);

  memoryWriteHalfword(0x080005B0,0x7006);
  memoryWriteHalfword(0x080005B2,0x2111);
  memoryWriteHalfword(0x080005B4,0x2112);
  memoryWriteHalfword(0x080005B6,0x2113);
  memoryWriteHalfword(0x080005B8,0x2114);
  memoryWriteHalfword(0x080005BA,0x2115);
  memoryWriteHalfword(0x080005bc,0xE7FE);
}


void xtest_datawatchpoint_TestCase_WriteByte_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteHalfword_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteWord_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

/* ---------------- Watchpoint TestCaseWrite STRH -------------------- */
// 0x080005A0    F2433644   movw r6,#0x3344
// 0x080005A4    F2C11622   movt r6,#0x1122

// 0x080005A8    F240405C   movw r0,#0x045C
// 0x080005AC    F2C20000   movt r0,#0x2000

// 0x080005B0    8006       strb r6,[r0]
// 0x080005B2    2111       movs r1,#17
// 0x080005B4    2112       movs r1,#18
// 0x080005B6    2113       movs r1,#19
// 0x080005B8    2114       movs r1,#20
// 0x080005BA    2115       movs r1,#21
// 0x080005BC    E7FE       b.n	80005bc

void xtest_programWatchpoint_TestCaseWrite_STRH()
{
  memoryWriteWord(0x080005A0,0xF2433644);
  memoryWriteWord(0x080005A4,0xF2C11622);
  
  memoryWriteWord(0x080005A8,0xF64C4CDD);
  memoryWriteWord(0x080005AC,0xF6CA2CBB);

  memoryWriteHalfword(0x080005B0,0x8006);
  memoryWriteHalfword(0x080005B2,0x2111);
  memoryWriteHalfword(0x080005B4,0x2112);
  memoryWriteHalfword(0x080005B6,0x2113);
  memoryWriteHalfword(0x080005B8,0x2114);
  memoryWriteHalfword(0x080005BA,0x2115);
  memoryWriteHalfword(0x080005bc,0xE7FE);
}



void xtest_datawatchpoint_TestCase_WriteByte_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteHalfword_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteWord_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

/* ---------------- Watchpoint TestCaseWrite STR -------------------- */
// 0x080005A0    F2433644   movw r6,#0x3344
// 0x080005A4    F2C11622   movt r6,#0x1122

// 0x080005A8    F240405C   movw r0,#0x045C
// 0x080005AC    F2C20000   movt r0,#0x2000

// 0x080005B0    8006       strb r6,[r0]
// 0x080005B2    2111       movs r1,#17
// 0x080005B4    2112       movs r1,#18
// 0x080005B6    2113       movs r1,#19
// 0x080005B8    2114       movs r1,#20
// 0x080005BA    2115       movs r1,#21
// 0x080005BC    E7FE       b.n	80005bc

void xtest_programWatchpoint_TestCaseWrite_STR()
{
  memoryWriteWord(0x080005A0,0xF2433644);
  memoryWriteWord(0x080005A4,0xF2C11622);
  
  memoryWriteWord(0x080005A8,0xF64C4CDD);
  memoryWriteWord(0x080005AC,0xF6CA2CBB);

  memoryWriteHalfword(0x080005B0,0x8006);
  memoryWriteHalfword(0x080005B2,0x2111);
  memoryWriteHalfword(0x080005B4,0x2112);
  memoryWriteHalfword(0x080005B6,0x2113);
  memoryWriteHalfword(0x080005B8,0x2114);
  memoryWriteHalfword(0x080005BA,0x2115);
  memoryWriteHalfword(0x080005bc,0xE7FE);
}


void xtest_datawatchpoint_TestCase_WriteByte_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x44,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());
  
  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteHalfword_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}

void xtest_datawatchpoint_TestCase_WriteWord_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x11223344,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005A0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  // TEST_ASSERT_EQUAL(,pc);
}


/* ---------------- Watchpoint TestCaseDoubleWrite -------------------- */
// 0x080006D0    F240405C   movw r0,#0x045C
// 0x080006D4    F2C20000   movt r0,#0x2000

// 0x080006D8    210A       movs r1,#10
// 0x080006DA    6001       str r1,[r0]
// 0x080006DC    210B       movs r1,#11
// 0x080006DE    6001       str r1,[r0]

// 0x080006E0    F05F0B11   movw r11,#11
// 0x080006E4    F05F0B12   movw r11,#12
// 0x080006E8    F05F0B13   movw r11,#13
// 0x080006EC    F05F0B14   movw r11,#14
// 0x080006F0    F05F0B15   movw r11,#15

// 0x080006F4    E7FE       b.n	80006F4

void xtest_programWatchpoint_TestCaseDoubleWrite()
{
  memoryWriteWord(0x080005A0,0xF2433644);
  memoryWriteWord(0x080005A4,0xF2C11622);
  
  memoryWriteWord(0x080005A8,0xF64C4CDD);
  memoryWriteWord(0x080005AC,0xF6CA2CBB);

  memoryWriteHalfword(0x080005B0,0x8006);
  memoryWriteHalfword(0x080005B2,0x2111);
  memoryWriteHalfword(0x080005B4,0x2112);
  memoryWriteHalfword(0x080005B6,0x2113);
  memoryWriteHalfword(0x080005B8,0x2114);
  memoryWriteHalfword(0x080005BA,0x2115);
  memoryWriteHalfword(0x080005bc,0xE7FE);
}

void xtest_datawatchpoint_TestCase_DoubleWrite()
{
	  uint32_t pc = 0 ;
	  uint32_t r1 = 0 ;

	  int i = 0 ;
	   writeCoreRegister(CORE_REG_PC,0x080005A0);
	   
	   while(i < 10)
	 	  i++;

	  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xA,WATCHPOINT_WORD,WATCHPOINT_WRITE);

	  writeCoreRegister(CORE_REG_PC,0x08000640);
	  setCoreMode(CORE_DEBUG_MODE);

	  while(!hasDWTTrapDebugEventOccured());


	  pc = readCoreRegister(CORE_REG_PC);
	  r1 = readCoreRegister(CORE_REG_R1);
	  disableDWTComparator(COMPARATOR_1);
	  clearDWTTrapDebugEvent();
	  disableDWTandITM();

	  // TEST_ASSERT_EQUAL(,pc);
}
