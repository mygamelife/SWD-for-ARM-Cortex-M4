#include "WatchpointTest.h"

/*-------------------------------------------------*/
void datawatchpoint_TestCase_ReadByte_LDRB(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XDD,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080003F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_ReadByte_LDRH(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XDD,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000410);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_ReadByte_LDR(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0XDD,WATCHPOINT_BYTE,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000430);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}


/*-------------------------------------------------*/

/*
 * Result: Watchpoint event not occur
 */
void datawatchpoint_TestCase_ReadHalfword_LDRB(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0X6677,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000450);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_ReadHalfword_LDRH(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0X6677,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000470);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_ReadHalfword_LDR(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0X6677,WATCHPOINT_HALFWORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000490);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

/*-------------------------------------------------*/
void datawatchpoint_TestCase_ReadWord_LDRB(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080004B0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_ReadWord_LDRH(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080004D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_ReadWord_LDR(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080005b0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

/*-------------------------------------------------*/
void datawatchpoint_TestCase_WriteByte_STRB(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xCA,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000510);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_WriteByte_STRH(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xCA,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000520);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_WriteByte_STR(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xCA,WATCHPOINT_BYTE,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000530);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());
  
  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

/*-------------------------------------------------*/
void datawatchpoint_TestCase_WriteHalfword_STRB(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000540);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_WriteHalfword_STRH(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000550);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_WriteHalfword_STR(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x3344,WATCHPOINT_HALFWORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000560);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

/*-------------------------------------------------*/
void datawatchpoint_TestCase_WriteWord_STRB(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x13579BDF,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000570);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_WriteWord_STRH(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x13579BDF,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000580);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_WriteWord_STR(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x13579BDF,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x08000590);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}
/*-------------------------------------------------*/


void datawatchpoint_TestCase_WriteWord_2bytes(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  uint32_t r1 = 0 ;
  int i = 0 ;
  writeCoreRegister(CORE_REG_PC,0x080005B0);
  setCoreMode(CORE_NORMAL_MODE);
  while(i < 10)
	  i++;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x10,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005C0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  readCoreRegister(CORE_REG_R1,&r1);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_WriteWord_4bytes(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  uint32_t r11= 0 ;

  int i = 0 ;
   writeCoreRegister(CORE_REG_PC,0x080005B0);
   setCoreMode(CORE_NORMAL_MODE);
   while(i < 10)
 	  i++;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x10,WATCHPOINT_WORD,WATCHPOINT_WRITE);

  writeCoreRegister(CORE_REG_PC,0x080005D0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());

  readCoreRegister(CORE_REG_PC,&pc);
  readCoreRegister(CORE_REG_R11,&r11);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);

}

void datawatchpoint_TestCase_ReadWord_2bytes(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  uint32_t r1 = 0 ;

  int i = 0 ;
   writeCoreRegister(CORE_REG_PC,0x080005B0);
   setCoreMode(CORE_NORMAL_MODE);
   while(i < 10)
 	  i++;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x080005F0);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  readCoreRegister(CORE_REG_R1,&r1);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_ReadWord_4bytes(Tlv_Session *session)
{
  uint32_t pc = 0 ;
  uint32_t r11 = 0 ;

  int i = 0 ;
   writeCoreRegister(CORE_REG_PC,0x080005B0);
   setCoreMode(CORE_NORMAL_MODE);
   while(i < 10)
 	  i++;

  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0x12345678,WATCHPOINT_WORD,WATCHPOINT_READ);

  writeCoreRegister(CORE_REG_PC,0x08000610);
  setCoreMode(CORE_DEBUG_MODE);

  while(!hasDWTTrapDebugEventOccured());


  readCoreRegister(CORE_REG_PC,&pc);
  readCoreRegister(CORE_REG_R11,&r11);
  disableDWTComparator(COMPARATOR_1);
  clearDWTTrapDebugEvent();
  disableDWTandITM();

  setCoreMode(CORE_NORMAL_MODE);
}

void datawatchpoint_TestCase_DoubleWrite(Tlv_Session *session)
{
	  uint32_t pc = 0 ;
	  uint32_t r1 = 0 ;

	  int i = 0 ;
	   writeCoreRegister(CORE_REG_PC,0x080005B0);
	   setCoreMode(CORE_NORMAL_MODE);
	   while(i < 10)
	 	  i++;

	  setDataWatchpoint_MatchingOneComparator(COMPARATOR_0,0x2000045C,WATCHPOINT_MASK_NOTHING,0xA,WATCHPOINT_WORD,WATCHPOINT_WRITE);

	  writeCoreRegister(CORE_REG_PC,0x08000640);
	  setCoreMode(CORE_DEBUG_MODE);

	  while(!hasDWTTrapDebugEventOccured());


	  readCoreRegister(CORE_REG_PC,&pc);
	  readCoreRegister(CORE_REG_R1,&r1);
	  disableDWTComparator(COMPARATOR_1);
	  clearDWTTrapDebugEvent();
	  disableDWTandITM();

	  setCoreMode(CORE_NORMAL_MODE);
}
