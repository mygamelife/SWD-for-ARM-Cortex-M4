#include "unity.h"
#include <stdint.h>
#include "DWT_Unit.h"
#include "DWT_Utilities.h"
#include "Delay.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "CodeStepping.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "configurePort.h"
#include "LowLevelIO.h"
#include "core_cm4.h"
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
  setCoreMode(CORE_DEBUG_HALT);
  enableDWTandITM()
}

void tearDown(void) {}
{
  setCoreMode(CORE_NORMAL_MODE);
}

void test_loadProgram()
{
  tlvLoadProgram(Tlv_Session *session, char *file, TLV_WRITE_FLASH);
}


/*-------------------------------------------------*/
void test_datawatchpoint_TestCase_ReadByte_LDRB()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XDD,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_ReadByte_LDRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XDD,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000410);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_ReadByte_LDR()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XDD,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000430);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}


/*-------------------------------------------------*/

/*
 * Result: Watchpoint event not occur
 */
void test_datawatchpoint_TestCase_ReadHalfword_LDRB()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0X6677,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000450);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_ReadHalfword_LDRH()
{
  uint32_t pc = 0 ;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0X6677,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000470);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_ReadHalfword_LDR()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0X6677,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000490);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

/*-------------------------------------------------*/
void test_datawatchpoint_TestCase_ReadWord_LDRB()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080004B0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_ReadWord_LDRH()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080004D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_ReadWord_LDR()
{
  uint32_t pc = 0 ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080005b0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

/*-------------------------------------------------*/
void test_datawatchpoint_TestCase_WriteByte_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xCA,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000510);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_WriteByte_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xCA,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000520);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_WriteByte_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xCA,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000530);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());
  
  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

/*-------------------------------------------------*/
void test_datawatchpoint_TestCase_WriteHalfword_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000540);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_WriteHalfword_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000550);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_WriteHalfword_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000560);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

/*-------------------------------------------------*/
void test_datawatchpoint_TestCase_WriteWord_STRB()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x13579BDF,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000570);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_WriteWord_STRH()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x13579BDF,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000580);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_WriteWord_STR()
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x13579BDF,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000590);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}
/*-------------------------------------------------*/


void test_datawatchpoint_TestCase_WriteWord_2bytes()
{
  uint32_t pc = 0 ;
  uint32_t r1 = 0 ;
  int i = 0 ;
  writeCoreRegister(CORE_REG_PC,0x080005B0);
  
  while(i < 10)
	  i++;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x10,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  readCoreRegister(CORE_REG_R1,&r1);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_WriteWord_4bytes()
{
  uint32_t pc = 0 ;
  uint32_t r11= 0 ;

  int i = 0 ;
   writeCoreRegister(CORE_REG_PC,0x080005B0);
   
   while(i < 10)
 	  i++;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x10,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  pc = readCoreRegister(CORE_REG_PC);
  readCoreRegister(CORE_REG_R11,&r11);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  
  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_ReadWord_2bytes()
{
  uint32_t pc = 0 ;
  uint32_t r1 = 0 ;

  int i = 0 ;
   writeCoreRegister(CORE_REG_PC,0x080005B0);
   
   while(i < 10)
 	  i++;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080005F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  readCoreRegister(CORE_REG_R1,&r1);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_ReadWord_4bytes()
{
  uint32_t pc = 0 ;
  uint32_t r11 = 0 ;

  int i = 0 ;
   writeCoreRegister(CORE_REG_PC,0x080005B0);
   
   while(i < 10)
 	  i++;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000610);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  pc = readCoreRegister(CORE_REG_PC);
  readCoreRegister(CORE_REG_R11,&r11);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  TEST_ASSERT_EQUAL(,pc);
}

void test_datawatchpoint_TestCase_DoubleWrite()
{
	  uint32_t pc = 0 ;
	  uint32_t r1 = 0 ;

	  int i = 0 ;
	   writeCoreRegister(CORE_REG_PC,0x080005B0);
	   
	   while(i < 10)
	 	  i++;

	  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xA,WATCHPOINT_WORD,WATCHPOINT_WRITE);

	  writeCoreRegister(CORE_REG_PC,0x08000640);
	  setCoreMode(CORE_DEBUG_MODE);

	  while(!hasDWTTrapDebugEventOccured());


	  pc = readCoreRegister(CORE_REG_PC);
	  readCoreRegister(CORE_REG_R1,&r1);
	  disableDWTComparator(COMPARATOR_1);
	  clearDWTTrapDebugEvent();
	  disableDWTandITM();

	  TEST_ASSERT_EQUAL(,pc);
}
