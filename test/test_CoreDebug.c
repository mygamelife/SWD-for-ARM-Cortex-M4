#include "unity.h"
#include <stdint.h>
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "Misc_Utilities.h"
#include "Clock.h"
#include "Emulator.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "mock_configurePort.h"
#include "mock_IO_Operations.h"



void setUp(void)
{
}

void tearDown(void)
{
}

/*--------------------------------setCoreMode---------------------------------------*/
//CORE_NORMAL_MODE
void test_setCoreMode_CORE_NORMAL_MODE_should_write_SET_CORE_NORMAL_to_DHCSR()
{
  cswDataSize = CSW_WORD_SIZE ;
  
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_NORMAL);
	
	setCoreMode(CORE_NORMAL_MODE);
}

//CORE_NORMAL_MASKINT
void test_setCoreMode_CORE_NORMAL_MASKINT_will_setCoreMode_to_CORE_DEBUG_HALT_first_and_write_SET_CORE_NORMAL_MASKINT_to_DHCSR()
{
	//Set to CORE_DEBUG_HALT first
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);

	//Then only set to CORE_NORMAL_MASKINT
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_NORMAL_MASKINT);
  
  setCoreMode(CORE_NORMAL_MASKINT);
}


//CORE_DEBUG_MODE
void test_setCoreMode_CORE_DEBUG_MODE_should_write_SET_CORE_DEBUG_to_DHCSR()
{
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG);

	setCoreMode(CORE_DEBUG_MODE);
}

//CORE_DEBUG_HALT
void test_setCore_CORE_DEBUG_HALT_should_write_SET_CORE_DEBUG_HALT_to_DHCSR()
{
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
  setCoreMode(CORE_DEBUG_HALT);
}


//CORE_SINGLE_STEP
void test_setCoreMode_CORE_SINGLE_STEP_will_setCoreMode_to_CORE_DEBUG_HALT_first_and_write_SET_CORE_STEP_to_DHCSR()
{
	//Set to CORE_DEBUG_HALT first
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);

	//Then only set to CORE_SINGLE_STEP
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_STEP);
  
  setCoreMode(CORE_SINGLE_STEP);
}

//CORE_SINGLE_STEP_MASKINT
void test_setCoreMode_SET_CORE_STEP_MASKINT_will_setCoreMode_to_CORE_DEBUG_HALT_first_and_write_SET_CORE_STEP_MASKINT_to_DHCSR()
{
	//Set to CORE_DEBUG_HALT first
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);

	//Then only set to CORE_SINGLE_STEP
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_STEP_MASKINT);
  
  setCoreMode(CORE_SINGLE_STEP_MASKINT);
}

/*--------------------------------getCoreMode---------------------------------------*/
void test_getCoreMode_given_0x0_should_return_CORE_NORMAL_MODE()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,0,0);
  
  TEST_ASSERT_EQUAL(CORE_NORMAL_MODE,getCoreMode());
}

void test_getCoreMode_given_0x8_should_return_CORE_NORMAL_MASKINT()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x8));
  
  TEST_ASSERT_EQUAL(CORE_NORMAL_MASKINT,getCoreMode());
}

void test_getCoreMode_given_0x1_should_return_CORE_DEBUG_MODE()
{
   emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x1));
  
  TEST_ASSERT_EQUAL(CORE_DEBUG_MODE,getCoreMode());
}

void test_getCoreMode_given_0x20003_should_return_CORE_DEBUG_HALT_MODE()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x20003));
  
  TEST_ASSERT_EQUAL(CORE_DEBUG_HALT,getCoreMode());
}

/*--------------------------------writeCoreRegister---------------------------------------*/
void test_writeCoreRegister_given_0x52_CORE_REG_R1_should_write_0x52_to_DCRDR_and_0x10001_to_DCRSR()
{
  //SET_CORE_DEBUG_HALT
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
  
  //Write data to DCRDR_REG
  emulateswdRegisterWrite(TAR_REG,AP,4,DCRDR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x52);
  
  //Select CORE_REG_R1 and write mode in DCRSR_REG
  emulateswdRegisterWrite(TAR_REG,AP,4,DCRSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x10001);
  
  //wait for transaction to complete
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x10000));
  
  writeCoreRegister(CORE_REG_R1,0x52);
}

void test_writeCoreRegister_given_0xFFFFFFFF_CORE_REG_FPREGS31_should_write_0xFFFFFFFF_to_DCRDR_and_0x1005F_to_DCRSR()
{
  //SET_CORE_DEBUG_HALT
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
  
  //Write data to DCRDR_REG
  emulateswdRegisterWrite(TAR_REG,AP,4,DCRDR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,0xFFFFFFFF);
  
  //Select CORE_REG_FPREGS31 and write mode in DCRSR_REG
  emulateswdRegisterWrite(TAR_REG,AP,4,DCRSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,0x1005F);
  
  //wait for transaction to complete
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x10000));
  
  writeCoreRegister(CORE_REG_FPREGS31,0xFFFFFFFF);
}

/*--------------------------------readCoreRegister---------------------------------------*/
void test_readCoreRegister_given_CORE_REG_PC_should_write_0xF_to_DCRSR_and_read_data_from_DCRDR()
{
  uint32_t dataRead = 0 ;
  
  //SET_CORE_DEBUG_HALT
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
  
  //Select CORE_REG_PC and read mode in DCRSR_REG
  emulateswdRegisterWrite(TAR_REG,AP,4,DCRSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,0xF);
  
  //wait for transaction to complete
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x10000));
  
  //Read data from DCRDR_REG
  emulateswdRegisterWrite(TAR_REG,AP,4,DCRDR_REG);
	emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x1234567));
  
  readCoreRegister(CORE_REG_PC,&dataRead);
  
  TEST_ASSERT_EQUAL(0x1234567,dataRead);
}

/*------------------------------waitForCoreRegisterTransactionToComplete------------------------------------*/
//S_REGRDY @ bit 16
void test_waitForCoreRegisterTransactionToComplete_should_read_DHCSR_until_S_REGRDY_bit_is_1()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,0,0);
  
  emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x10000));
  waitForCoreRegisterTransactionToComplete();
}

/*------------------------------isSelectedDebugEventOccured------------------------------------*/
void test_isSelectedDebugEventOccured_given_HALTED_DEBUGEVENT_should_read_DFSR_dataRead0x1_should_return_1()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(1));
  
  TEST_ASSERT_EQUAL(1,isSelectedDebugEventOccured(HALTED_DEBUGEVENT));
}

void test_isSelectedDebugEventOccured_given_HALTED_DEBUGEVENT_should_read_DFSR_dataRead0x_should_return_0()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0));
  
  TEST_ASSERT_EQUAL(0,isSelectedDebugEventOccured(HALTED_DEBUGEVENT));
}

void test_isSelectedDebugEventOccured_given_VCATCH_DEBUGEVENT_should_read_DFSR_dataRead0x8_should_return_1()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,1,convertMSB_LSB(0x8));
  
  TEST_ASSERT_EQUAL(1,isSelectedDebugEventOccured(VCATCH_DEBUGEVENT));
}

void test_isSelectedDebugEventOccured_given_VCATCH_DEBUGEVENT_should_read_DFSR_dataRead0xC_should_return_1()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateswdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateswdRegisterRead(DRW_REG,AP,4,0,convertMSB_LSB(0xC));
  
  TEST_ASSERT_EQUAL(1,isSelectedDebugEventOccured(VCATCH_DEBUGEVENT));
}

/*------------------------------enableSelectedVectorCatch------------------------------------*/
void test_enableSelectedVectorCatch_given_VC_HARDERR_should_writeHalfWord_0x400_to_DEMCR()
{
  
  //Write BANK_0 to select register
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	//Write CSW_HALFWORD_SIZE to csw register
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
  
  emulateswdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateswdRegisterWrite(DRW_REG,AP,4,0x400);

  enableSelectedVectorCatch(VC_HARDERR);
}

void test_enableSelectedVectorCatch_given_VC_CORERESET_should_writeHalfWord_0x1_to_DEMCR()
{
  cswDataSize = CSW_BYTE_SIZE ;
  //Write BANK_0 to select register
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	//Write CSW_HALFWORD_SIZE to csw register
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
  
  emulateswdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateswdRegisterWrite(DRW_REG,AP,4,0x1);

  enableSelectedVectorCatch(VC_CORERESET);
}

void test_enableSelectedVectorCatch_given_VC_DISABLEALL_should_writeHalfWord_0_to_DEMCR()
{
  /*cswDataSize was previously CSW_HALFWORD_SIZE do not require to set it again */

  emulateswdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateswdRegisterWrite(DRW_REG,AP,4,0);

  enableSelectedVectorCatch(VC_DISABLEALL);
}

/*------------------------------performHaltOnReset------------------------------------*/
void test_performHaltOnReset_should_setCoreMode_CORE_DEBUG_HALT_enable_VC_CORERESET_and_write_REQUEST_SYTEM_RESET_to_ARICR_REG()
{
  //Set CSW to Word Size
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Set to CORE_DEBUG_HALT 
	emulateswdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateswdRegisterWrite(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
  
  
  //Set CSW to Halfword Size
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
  
  //Enable VC_CORERESET
  emulateswdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateswdRegisterWrite(DRW_REG,AP,4,0x1);
  
  //Set CSW to Word Size
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Write REQUEST_SYSTEM_RESET to AIRCR_REG
  emulateswdRegisterWrite(TAR_REG,AP,4,AIRCR_REG);
  emulateswdRegisterWrite(DRW_REG,AP,4,REQUEST_SYSTEM_RESET);
  
  performHaltOnReset();
}


/*------------------------------enableDWTandITM------------------------------------*/
void test_enableDWTandITM_should_writeByte_0xE000EDFF_to_TAR_and_write_1_to_DRW()
{
  //Set CSW to Byte Size
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
  
  emulateswdRegisterWrite(TAR_REG,AP,4,0xE000EDFF);
  emulateswdRegisterWrite(DRW_REG,AP,4,1);
  
  enableDWTandITM();
}

/*------------------------------disableDWTandITM------------------------------------*/
void test_disableDWTandITM_should_writeByte_0xE000EDFF_to_TAR_and_write_0_to_DRW()
{
  emulateswdRegisterWrite(TAR_REG,AP,4,0xE000EDFF);
  emulateswdRegisterWrite(DRW_REG,AP,4,0);
  
  disableDWTandITM();
}