#include "unity.h"
#include <stdint.h>
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "Emulator.h"
#include "MemoryReadWrite.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "Delay.h"
#include "core_cm4.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/*-------------------------------doesCoreModeRequiresHaltedAndDebug---------------------------------*/
void test_isCoreModeRequiresHaltedAndDebug_given_CORE_NORMAL_MASKINT_should_return_1()
{
  TEST_ASSERT_EQUAL(1,doesCoreModeRequiresHaltedAndDebug(CORE_NORMAL_MASKINT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SINGLE_STEP_should_return_1()
{
  TEST_ASSERT_EQUAL(1,doesCoreModeRequiresHaltedAndDebug(CORE_SINGLE_STEP));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SINGLE_STEP_MASKINT_should_return_1()
{
  TEST_ASSERT_EQUAL(1,doesCoreModeRequiresHaltedAndDebug(CORE_SINGLE_STEP_MASKINT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_NORMAL_MODE_should_return_0()
{
  TEST_ASSERT_EQUAL(0,doesCoreModeRequiresHaltedAndDebug(CORE_NORMAL_MODE));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_DEBUG_MODE_should_return_0()
{
  TEST_ASSERT_EQUAL(0,doesCoreModeRequiresHaltedAndDebug(CORE_DEBUG_MODE));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_DEBUG_HALT_should_return_0()
{
  TEST_ASSERT_EQUAL(0,doesCoreModeRequiresHaltedAndDebug(CORE_DEBUG_HALT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SNAPSTALL_should_return_0()
{
  TEST_ASSERT_EQUAL(0,doesCoreModeRequiresHaltedAndDebug(CORE_SNAPSTALL));
}


/*--------------------------------setCoreMode---------------------------------------*/
//CORE_NORMAL_MODE
void test_setCoreMode_CORE_NORMAL_MODE_should_write_0xA05F0000_to_DHCSR()
{ 
  cswDataSize = CSW_WORD_SIZE ;
  
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0000);
	
	setCoreMode(CORE_NORMAL_MODE);
}

//CORE_NORMAL_MASKINT
void test_setCoreMode_CORE_NORMAL_MASKINT_will_setCoreMode_to_CORE_DEBUG_HALT_first_and_write_0xA05F0008_to_DHCSR()
{
	//Set to CORE_DEBUG_HALT first
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);

	//Then only set to CORE_NORMAL_MASKINT
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0008);
  
  setCoreMode(CORE_NORMAL_MASKINT);
}


//CORE_DEBUG_MODE
void test_setCoreMode_CORE_DEBUG_MODE_should_write_0xA05F0001_to_DHCSR()
{
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);

	setCoreMode(CORE_DEBUG_MODE);
}

//CORE_DEBUG_HALT
void test_setCore_CORE_DEBUG_HALT_should_write_0xA05F0003_to_DHCSR()
{
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);
	
  setCoreMode(CORE_DEBUG_HALT);
}


//CORE_SINGLE_STEP
void test_setCoreMode_CORE_SINGLE_STEP_will_setCoreMode_to_CORE_DEBUG_HALT_first_and_write_0xA05F0005_to_DHCSR()
{
	//Set to CORE_DEBUG_HALT first
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);

	//Then only set to CORE_SINGLE_STEP
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0005);
  
  setCoreMode(CORE_SINGLE_STEP);
}

//CORE_SINGLE_STEP_MASKINT
void test_setCoreMode_SET_CORE_STEP_MASKINT_will_setCoreMode_to_CORE_DEBUG_HALT_first_and_write_0xA05F000D_to_DHCSR()
{
	//Set to CORE_DEBUG_HALT first
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);

	//Then only set to CORE_SINGLE_STEP
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F000D);
  
  setCoreMode(CORE_SINGLE_STEP_MASKINT);
}

/*-------------------------------determineCoreModeFromDataRead---------------------------------*/
void test_determineCoreModeFromDataRead_given_0x0_should_return_CORE_NORMAL_MODE()
{
  TEST_ASSERT_EQUAL(CORE_NORMAL_MODE, determineCoreModeFromDataRead(0));
}

void test_determineCoreModeFromDataRead_given_0x8_should_return_CORE_NORMAL_MASKINT()
{
  TEST_ASSERT_EQUAL(CORE_NORMAL_MASKINT,determineCoreModeFromDataRead(0x8));
}

void test_determineCoreModeFromDataRead_given_0x1_should_return_CORE_DEBUG_MODE()
{
  TEST_ASSERT_EQUAL(CORE_DEBUG_MODE,determineCoreModeFromDataRead(1));
}

void test_determineCoreModeFromDataRead_given_0x1_should_return_CORE_DEBUG_HALT()
{
  TEST_ASSERT_EQUAL(CORE_DEBUG_HALT,determineCoreModeFromDataRead(0x20003));
}

/*--------------------------------getCoreMode---------------------------------------*/
void test_getCoreMode_given_0x0_should_return_CORE_NORMAL_MODE()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,0,0);
  
  TEST_ASSERT_EQUAL(CORE_NORMAL_MODE,getCoreMode());
}

void test_getCoreMode_given_0x8_should_return_CORE_NORMAL_MASKINT()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x8));
  
  TEST_ASSERT_EQUAL(CORE_NORMAL_MASKINT,getCoreMode());
}

void test_getCoreMode_given_0x1_should_return_CORE_DEBUG_MODE()
{
   emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x1));
  
  TEST_ASSERT_EQUAL(CORE_DEBUG_MODE,getCoreMode());
}

void test_getCoreMode_given_0x20003_should_return_CORE_DEBUG_HALT_MODE()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x20003));
  
  TEST_ASSERT_EQUAL(CORE_DEBUG_HALT,getCoreMode());
}


/*--------------------------------stepOnly---------------------------------------*/
void test_stepOnly_given_1_should_step_1_times()
{
  //Set to CORE_DEBUG_HALT first
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);

	//Then only set to CORE_SINGLE_STEP
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0005);
  
  stepOnly(1);
}

void test_stepOnly_given_3_should_step_3_times()
{
  //Set to CORE_DEBUG_HALT first
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);

	//Then only set to CORE_SINGLE_STEP
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0005);
  
  //Set to CORE_DEBUG_HALT first
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);

	//Then only set to CORE_SINGLE_STEP
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0005);
  
  //Set to CORE_DEBUG_HALT first
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);

	//Then only set to CORE_SINGLE_STEP
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0005);
  
  stepOnly(3);
}

/*--------------------------------writeCoreRegister---------------------------------------*/
void test_writeCoreRegister_given_0x52_CORE_REG_R1_should_write_0x52_to_DCRDR_and_0x10001_to_DCRSR()
{
  //0xA05F0003
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);
  
  //Write data to DCRDR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,DCRDR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x52);
  
  //Select CORE_REG_R1 and write mode in DCRSR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,DCRSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x10001);
  
  //wait for transaction to complete
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x10000));
  
  writeCoreRegister(CORE_REG_R1,0x52);
}

void test_writeCoreRegister_given_0xFFFFFFFF_CORE_REG_FPREGS31_should_write_0xFFFFFFFF_to_DCRDR_and_0x1005F_to_DCRSR()
{
  //0xA05F0003
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);
  
  //Write data to DCRDR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,DCRDR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xFFFFFFFF);
  
  //Select CORE_REG_FPREGS31 and write mode in DCRSR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,DCRSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x1005F);
  
  //wait for transaction to complete
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x10000));
  
  writeCoreRegister(CORE_REG_FPREGS31,0xFFFFFFFF);
}

/*--------------------------------readCoreRegister---------------------------------------*/
void test_readCoreRegister_given_CORE_REG_PC_should_write_0xF_to_DCRSR_and_read_data_from_DCRDR()
{
  uint32_t dataRead = 0 ;
  
  //Set to CORE_DEBUG_HALT
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);
  
  //Select CORE_REG_PC and read mode in DCRSR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,DCRSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,CORE_REG_PC);
  
  //wait for transaction to complete
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x10000));
  
  //Read data from DCRDR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,DCRDR_REG);
	emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x1234567));
 
  TEST_ASSERT_EQUAL(0x1234567,readCoreRegister(CORE_REG_PC));
}

/*------------------------------waitForCoreRegisterTransactionToComplete------------------------------------*/
//S_REGRDY @ bit 16
void test_waitForCoreRegisterTransactionToComplete_should_read_DHCSR_until_S_REGRDY_bit_is_1()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,0,0);
  
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x10000));
  waitForCoreRegisterTransactionToComplete();
}

/*------------------------------readDebugEventRegister------------------------------------*/
void test_readDebugEventRegister_will_read_DFSR_and_return_the_dataRead()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x10000));
  
  TEST_ASSERT_EQUAL(0x10000,readDebugEventRegister());
}



/*------------------------------clearDebugEvent------------------------------------*/
void test_clearDebugEvent_EXTERNAL_should_write_0x10_to_DFSR()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x10);
  
  clearExternalDebugEvent();
}

void test_clearDebugEvent_VCATCH_should_write_0x8_to_DFSR()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x8);
  
  clearVectorCatchDebugEvent();
}

void test_clearDebugEvent_DWTTRAP_should_write_0x4_to_DFSR()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x4);
  
  clearDWTTrapDebugEvent();
}

void test_clearDebugEvent_BKPT_should_write_0x2_to_DFSR()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x2);
  
  clearBreakpointDebugEvent()
}

/*------------------------------enableVectorCatch------------------------------------*/
void test_enableVectorCatchHARDERR_should_writeHalfWord_0x400_to_DEMCR()
{
  
  //Write SELECT_BANK0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	//Write CSW_HALFWORD_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
  
  emulateSwdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateSwdRegisterWrite(DRW_REG,AP,4,0x400);

 enableVectorCatchHARDERR();
}

void test_enableVectorCatchCoreReset_should_writeHalfWord_0x1_to_DEMCR()
{
  cswDataSize = CSW_WORD_SIZE ;
  //Write SELECT_BANK0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	//Write CSW_BYTE_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
  
  emulateSwdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateSwdRegisterWrite(DRW_REG,AP,4,0x1);

  enableVectorCatchCoreReset();
}

void test_disableAllVectorCatch_should_writeHalfWord_0_to_DEMCR()
{
  //Write SELECT_BANK0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
  //Write CSW_HALFWORD_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));

  emulateSwdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateSwdRegisterWrite(DRW_REG,AP,4,0);

  disableAllVectorCatch();
}

/*------------------------------softResetTarget------------------------------------*/
void test_softResetTarget_should_write_REQUEST_SYTEM_RESET_to_AIRCR_REG()
{
  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Write REQUEST_SYSTEM_RESET to AIRCR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,AIRCR_REG);
  emulateSwdRegisterWrite(DRW_REG,AP,4,REQUEST_SYSTEM_RESET);
  softResetTarget();
}

/*------------------------------softResetTarget------------------------------------*/
void test_vectorResetTarget_should_write_REQUEST_VECTOR_RESET_to_AIRCR_REG()
{
  //Write REQUEST_VECTOR_RESET to AIRCR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,AIRCR_REG);
  emulateSwdRegisterWrite(DRW_REG,AP,4,REQUEST_VECTOR_RESET);
  
  vectorResetTarget();
}


/*------------------------------performHaltOnReset------------------------------------*/
void test_performHaltOnReset_should_setCoreMode_CORE_DEBUG_HALT_enable_VC_CORERESET_and_write_REQUEST_SYTEM_RESET_to_AIRCR_REG()
{
  //Set to CORE_DEBUG_HALT 
	emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0003);
  
  //Set CSW to Byte Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
  
  //Enable VC_CORERESET
  emulateSwdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateSwdRegisterWrite(DRW_REG,AP,4,0x1);
  
  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Write REQUEST_SYSTEM_RESET to AIRCR_REG
  emulateSwdRegisterWrite(TAR_REG,AP,4,AIRCR_REG);
  emulateSwdRegisterWrite(DRW_REG,AP,4,REQUEST_SYSTEM_RESET);
  
  //Set CSW to Byte Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
  
  //Disable VC_CORERESET
  emulateSwdRegisterWrite(TAR_REG,AP,4,DEMCR_REG);
  emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  performHaltOnReset();
}


/*------------------------------enableDWTandITM------------------------------------*/
void test_enableDWTandITM_should_writeByte_0xE000EDFF_to_TAR_and_write_1000000_to_DRW()
{
  cswDataSize = CSW_WORD_SIZE;
  
  //Set CSW to Byte Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
  
  emulateSwdRegisterWrite(TAR_REG,AP,4,0xE000EDFF);
  emulateSwdRegisterWrite(DRW_REG,AP,4,0x1000000);
  
  enableDWTandITM();
}

/*------------------------------disableDWTandITM------------------------------------*/
void test_disableDWTandITM_should_writeByte_0xE000EDFF_to_TAR_and_write_0_to_DRW()
{
  emulateSwdRegisterWrite(TAR_REG,AP,4,0xE000EDFF);
  emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  disableDWTandITM();
}