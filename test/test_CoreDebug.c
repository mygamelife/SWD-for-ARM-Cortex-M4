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

/*--------------------------------setCore---------------------------------------*/
//CORE_NORMAL_MODE
void test_setCore_CORE_NORMAL_MODE_should_write_0xA05F0000_to_DHCSR_and_return_ERR_NOERROR_if_successful()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_NORMAL);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	TEST_ASSERT_EQUAL(ERR_NOERROR,setCore(CORE_NORMAL_MODE,&coreStatus));
	
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	
}

//CORE_DEBUG_MODE
void test_setCore_CORE_DEBUG_MODE_should_write_0xA05F0001_to_DHCSR_and_return_ERR_NOERROR_if_successful()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,0xA05F0001);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x1));
	TEST_ASSERT_EQUAL(ERR_NOERROR,setCore(CORE_DEBUG_MODE,&coreStatus));
	
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
}

//CORE_DEBUG_HALT
void test_setCore_CORE_DEBUG_HALT_should_write_0xA05F0003_to_DHCSR_and_return_ERR_NOERROR_if_successful()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	TEST_ASSERT_EQUAL(ERR_NOERROR,setCore(CORE_DEBUG_HALT,&coreStatus));
	
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(1,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);
	
}

void test_setCore_CORE_DEBUG_HALT_should_write_0xA05F0003_to_DHCSR_and_return_false_if_fail()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030000));
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,setCore(CORE_DEBUG_HALT,&coreStatus));
	
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);	
}

//CORE_SINGLE_STEP
void test_setCore_CORE_SINGLE_STEPwill_setCore_to_CORE_DEBUG_HALT_first_and_write_SET_CORE_STEP_to_DHCSR_and_return_ERR_NOERROR_if_successful()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	//Set to CORE_DEBUG_HALT first
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	//Then only set to CORE_SINGLE_STEP
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_STEP);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x03030007));
	TEST_ASSERT_EQUAL(TRUE,setCore(CORE_SINGLE_STEP,&coreStatus));
	
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(1,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(1,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);
}

void test_setCore_CORE_SINGLE_STEP_will_setCore_to_CORE_DEBUG_HALT_first_and_write_SET_CORE_STEP_to_DHCSR_and_return_ERR_CORECONTROL_FAILED_if_fail()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	//Set to CORE_DEBUG_HALT first
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	//Then only set to CORE_SINGLE_STEP
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_STEP);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030000));
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,setCore(CORE_SINGLE_STEP,&coreStatus));
	
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);
}

void test_setCore_CORE_SINGLE_STEP_but_setCore_to_CORE_DEBUG_HALT_failed_will_not_setCore_to_CORE_SINGLE_STEP_NOMASK()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	//Set to CORE_DEBUG_HALT first
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030000));
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,setCore(CORE_SINGLE_STEP,&coreStatus));
	
	
}

/*--------------------------------setCore_Exception-----------------------------------------*/

void test_setCore_Exception_given_CORE_SINGLE_STEP_will_setCore_to_CORE_DEBUG_HALT_and_return_ERR_NOERROR_if_successful()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setCore_Exception(CORE_SINGLE_STEP,&coreStatus));
}

void test_setCore_Exception_given_CORE_SINGLE_STEP_MASKINT_will_setCore_to_CORE_DEBUG_HALT_and_return_ERR_NOERROR_if_successful()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setCore_Exception(CORE_SINGLE_STEP_MASKINT,&coreStatus));
}

void test_setCore_Exception_given_CORE_NORMAL_MASKINT_will_setCore_to_CORE_DEBUG_HALT_and_return_ERR_NOERROR_if_successful()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setCore_Exception(CORE_NORMAL_MASKINT,&coreStatus));
}

void test_setCore_Exception_will_return_ERR_NOERROR_and_do_nothing_for_other_coreControl_mode()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setCore_Exception(CORE_DEBUG_HALT,&coreStatus));
}

/***************************************check_CoreStatus***************************************************/
/********************************************************
	Debug Halting and Control Status Register , DHCSR
 
	Bit[26]		--- S_RESET_ST
	Bit[25]		--- S_RETIRE_ST
	
	Bit[19]		--- S_LOCKUP
	Bit[18]		--- S_SLEEP
	Bit[17]		--- S_HALT
	Bit[16]		--- S_REGRDY
	
	Bit[5]		--- C_SNAPSTALL
	
	Bit[3]		--- C_MASKINTS
	Bit[2]		--- C_STEP
	Bit[1]		--- C_HALT
	Bit[0]		--- C_DEBUGEN
	
 ************************************************************/

void test_check_CoreStatus_should_read_DHCSR_and_process_CoreStatusData()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	check_CoreStatus(&coreStatus);
	
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(1,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);
}

/***************************************check_DebugEvent***************************************************/
/******************************************************************************************************
	Debug Fault Status Register , DFSR
 
	Bits[31:5] --- RESERVED

	Bit[4]		--- EXTERNAL
	Bit[3]		--- VCATCH
	Bit[2]		--- DWTTRAP
	Bit[1]		--- BKPT
	Bit[0]		--- HALTED
	
 ******************************************************************************************************/

void test_check_DebugEvent_should_read_DFSR_and_update_DebugEvent()
{
	DebugEvent debugEvent ;
	init_DebugEvent(&debugEvent);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DFSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x2345671F));
	
	check_DebugEvent(&debugEvent);
	
	TEST_ASSERT_EQUAL(1,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(1,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(1,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(1,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(1,debugEvent.HALTED);
}

/***************************************clear_DebugEvent***************************************************/

void test_clear_DebugEvent_should_write_to_DFSR_to_clear_and_update_DebugEvent()
{
	DebugEvent debugEvent ;
	init_DebugEvent(&debugEvent);
	
	debugEvent.EXTERNAL = 1 ;
	debugEvent.VCATCH = 1 ;
	debugEvent.DWTTRAP = 1 ;
	debugEvent.BKPT = 1 ;
	debugEvent.HALTED = 1 ;
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DFSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,0X1F);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DFSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x0));
	
	clear_DebugEvent(&debugEvent);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(0,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(0,debugEvent.HALTED);
}

/***************************************check_DebugTrapStatus***************************************************/
/******************************************************************************************************
	Debug Exception and Monitor Control Register, DEMCR
 
	Bit[10]	--- VC_HARDERR
	Bit[9]	--- VC_INTERR
	Bit[8]	--- VC_BUSERR
	Bit[7]	--- VC_STATERR
	Bit[6]	--- VC_CHKERR
	Bit[5]	--- VC_NOCPERR
	Bit[4]	--- VC_MMERR

	Bit[0]	--- VC_CORERESET
	
 ******************************************************************************************************/
 
void test_check_DebugTrapStatus_should_read_DEMCR_and_update_DebugTrap()
{
	DebugTrap debugTrap ;
	init_DebugTrap(&debugTrap);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x7A1));
	
	check_DebugTrapStatus(&debugTrap);
	
	TEST_ASSERT_EQUAL(1,debugTrap.VC_HARDERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_INTERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_BUSERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_STATERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_CHKERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_NOCPERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_MMERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_CORERESET);

}

/***************************************clear_DebugTrap***************************************************/
void test_clear_DebugTrap_should_write_to_DEMCR_to_clear_and_update_DebugTrap()
{
	DebugTrap debugTrap ;
	init_DebugTrap(&debugTrap);
	
	debugTrap.VC_HARDERR = 1; 
	debugTrap.VC_INTERR = 1; 
	debugTrap.VC_BUSERR = 1; 
	debugTrap.VC_STATERR = 1; 
	debugTrap.VC_CHKERR = 1; 
	debugTrap.VC_NOCPERR = 1; 
	debugTrap.VC_MMERR = 1; 
	debugTrap.VC_CORERESET = 1; 
	
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	
	clear_DebugTrap(&debugTrap);
	
	TEST_ASSERT_EQUAL(0,debugTrap.VC_HARDERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_INTERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_BUSERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_STATERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_CHKERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_NOCPERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_MMERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_CORERESET);
	
}

/***************************************write_CoreRegister***************************************************/
void test_write_CoreRegister_given_ARM_R0_will_halt_CPU_write_data_to_DCRDR_and_select_R0_in_DCRSR()
{
	CoreStatus coreStatus ;
	init_CoreStatus (&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DCRDR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x100);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DCRSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,get_CoreRegisterAccess_WriteValue(ARM_R0,CoreRegister_Write));

	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x10000));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,write_CoreRegister(ARM_R0,&coreStatus,0x100));
}

/***************************************read_CoreRegister***************************************************/
void test_read_CoreRegister_given_ARM_R1_will_halt_CPU_select_R1_in_DCRSR_and_read_data_from_DCRDR()
{
	uint32_t dataRead ;
	CoreStatus coreStatus ;
	init_CoreStatus (&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
		
	emulateSWDRegister_Write(TAR_REG,AP,4,DCRSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,get_CoreRegisterAccess_WriteValue(ARM_R1,CoreRegister_Read));

	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x10000));
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DCRDR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345678));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_CoreRegister(ARM_R1,&coreStatus,&dataRead));
	TEST_ASSERT_EQUAL(0x12345678,dataRead);
}

/***************************************wait_CoreRegisterTransaction*********************************/
void test_wait_CoreRegisterTransaction_should_read_S_REGRDY_bit_and_return_ERR_NOERR_if_is_1()
{
	CoreStatus coreStatus ;
	init_CoreStatus (&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,wait_CoreRegisterTransaction(&coreStatus,10));
}

void test_wait_CoreRegisterTransaction_should_read_S_REGRDY_bit_and_repeat_until_is_1()
{
	CoreStatus coreStatus ;
	init_CoreStatus (&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x10000));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,wait_CoreRegisterTransaction(&coreStatus,10));
}

void test_wait_CoreRegisterTransaction_should_return_ERR_COREREGRW_FAILED_if_S_REGRDY_is_not_1_after_retry()
{
	CoreStatus coreStatus ;
	init_CoreStatus (&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	
	TEST_ASSERT_EQUAL(ERR_COREREGRW_FAILED,wait_CoreRegisterTransaction(&coreStatus,2));
}

/***************************************configure_DebugExceptionMonitorControl*********************************/
void test_configure_DebugExceptionMonitorControl_should_write_to_DEMCR()
{
	DebugExceptionMonitor debugExceptionMonitor ;
	DebugTrap debugTrap ;
	
	init_DebugExceptionMonitor(&debugExceptionMonitor);
	init_DebugTrap(&debugTrap);
	uint32_t data = 0 ;
	data = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_DISABLE,&debugTrap,DISABLE_DWT_ITM);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,data);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_DebugExceptionMonitorControl(&debugExceptionMonitor,DEBUGMONITOR_DISABLE,&debugTrap,DISABLE_DWT_ITM));
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.enableDWT_ITM);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_REQ);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_STEP);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_PEND);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_EN);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_HARDERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_INTERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_BUSERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_STATERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_CHKERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_NOCPERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_MMERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_CORERESET);
}

/***************************************perform_HaltOnReset*********************************/
void test_perform_HaltOnReset_should_debug_halt_cpu_enable_CoreResetDebugTrap_and_write_to_AIRCR_REG()
{
	DebugExceptionMonitor debugExceptionMonitor ;
	CoreStatus coreStatus ;	
	DebugTrap debugTrap ;
	
	init_CoreStatus(&coreStatus);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
	init_DebugTrap(&debugTrap);
	
	debugTrap.VC_CORERESET = 1 ;
	uint32_t data = 0 ;
	data = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_DISABLE,&debugTrap,DISABLE_DWT_ITM);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,data);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	
	emulateSWDRegister_Write(TAR_REG,AP,4,AIRCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,0xFA050004);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(data));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,perform_HaltOnReset(&coreStatus,&debugExceptionMonitor));
	
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(1,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.enableDWT_ITM);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_REQ);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_STEP);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_PEND);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_EN);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_HARDERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_INTERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_BUSERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_STATERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_CHKERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_NOCPERR);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugTrap->VC_MMERR);
	TEST_ASSERT_EQUAL(1,debugExceptionMonitor.debugTrap->VC_CORERESET);
}