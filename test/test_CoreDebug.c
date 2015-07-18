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
void test_setCore_CORE_SINGLE_STEP_will_setCore_to_CORE_DEBUG_HALT_first_and_write_0xA05F0007_to_DHCSR_and_return_ERR_NOERROR_if_successful()
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

void test_setCore_CORE_SINGLE_STEP_will_setCore_to_CORE_DEBUG_HALT_first_and_write_0xA05F0007_to_DHCSR_and_return_false_if_fail()
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

void test_setCore_CORE_SINGLE_STEP_but_setCore_to_CORE_DEBUG_HALT_failed_will_not_setCore_to_CORE_SINGLE_STEP()
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

void test_setCore_Exception_given_CORE_MASK_INTERRUPT_will_setCore_to_CORE_DEBUG_HALT_and_return_ERR_NOERROR_if_successful()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x03030003));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setCore_Exception(CORE_MASK_INTERRUPT,&coreStatus));
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

void test_check_CoreStatus_should_read_DHCSR_and_update_CoreStatus()
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