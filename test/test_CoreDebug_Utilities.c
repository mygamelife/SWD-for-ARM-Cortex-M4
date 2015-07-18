#include "unity.h"
#include "CoreDebug_Utilities.h"
#include "Misc_Utilities.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/*-----------------------------------isCore-----------------------------------------*/
//CORE_DEBUG_MODE
void test_isCore_given_CORE_DEBUG_MODE_given_C_DEBUGEN_1_should_return_ERR_NOERROR()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1;
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,isCore(CORE_DEBUG_MODE,&coreStatus));
}

void test_isCore_given_CORE_DEBUG_MODE_data_C_DEBUGEN_0_should_return_FALSE()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_DEBUG_MODE,&coreStatus));
}

//CORE_DEBUG_HALT
void test_isCore_given_CORE_DEBUG_HALT_C_DEBUGEN_1_C_HALT_1_S_HALT_1_should_return_ERR_NOERROR()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1;
	coreStatus.C_HALT = 1;
	coreStatus.S_HALT = 1;
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,isCore(CORE_DEBUG_HALT,&coreStatus));
}

void test_isCore_given_CORE_DEBUG_HALT_C_DEBUGEN_0_C_HALT_0_S_HALT_0_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 0;
	coreStatus.C_HALT = 0;
	coreStatus.S_HALT = 0;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_DEBUG_HALT,&coreStatus));
}

void test_isCore_given_CORE_DEBUG_HALT_C_DEBUGEN_0_C_HALT_0_S_HALT_1_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 0;
	coreStatus.C_HALT = 0;
	coreStatus.S_HALT = 1;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_DEBUG_HALT,&coreStatus));
}

void test_isCore_given_CORE_DEBUG_HALT_C_DEBUGEN_0_C_HALT_1_S_HALT_0_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 0;
	coreStatus.C_HALT = 1;
	coreStatus.S_HALT = 0;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_DEBUG_HALT,&coreStatus));
}

void test_isCore_given_CORE_DEBUG_HALT_C_DEBUGEN_1_C_HALT_0_S_HALT_1_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1;
	coreStatus.C_HALT = 0;
	coreStatus.S_HALT = 1;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_DEBUG_HALT,&coreStatus));
}

//CORE_SINGLE_STEP
void test_isCore_given_CORE_SINGLE_STEP_C_DEBUGEN_1_C_HALT_1_C_STEP_1_S_HALT_1_should_return_ERR_NOERROR()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1;
	coreStatus.C_HALT = 1;
	coreStatus.S_HALT = 1;
	coreStatus.C_STEP = 1 ;
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,isCore(CORE_SINGLE_STEP,&coreStatus));
}

void test_isCore_given_CORE_SINGLE_STEP_C_DEBUGEN_0_C_HALT_0_C_STEP_0_S_HALT_0_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 0;
	coreStatus.C_HALT = 0;
	coreStatus.S_HALT = 0;
	coreStatus.C_STEP = 0 ;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_SINGLE_STEP,&coreStatus));
}

void test_isCore_given_CORE_SINGLE_STEP_C_DEBUGEN_0_C_HALT_0_C_STEP_1_S_HALT_0_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 0;
	coreStatus.C_HALT = 0;
	coreStatus.S_HALT = 0;
	coreStatus.C_STEP = 1 ;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_SINGLE_STEP,&coreStatus));
}

void test_isCore_given_CORE_SINGLE_STEP_C_DEBUGEN_1_C_HALT_1_C_STEP_1_S_HALT_0_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1;
	coreStatus.C_HALT = 1;
	coreStatus.S_HALT = 0;
	coreStatus.C_STEP = 1 ;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_SINGLE_STEP,&coreStatus));
}

//CORE_MASK_INTERRUPT
void test_isCore_given_CORE_MASK_INTERRUPT_C_DEBUGEN_1_C_HALT_1_C_MASKINT_1_S_HALT_1_should_return_ERR_NOERROR()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1;
	coreStatus.C_HALT = 1;
	coreStatus.S_HALT = 1;
	coreStatus.C_MASKINTS = 1 ;
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,isCore(CORE_MASK_INTERRUPT,&coreStatus));
}

void test_isCore_given_CORE_MASK_INTERRUPT_C_DEBUGEN_0_C_HALT_0_C_MASKINT_0_S_HALT_0_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 0;
	coreStatus.C_HALT = 0;
	coreStatus.S_HALT = 0;
	coreStatus.C_MASKINTS = 0 ;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_MASK_INTERRUPT,&coreStatus));
}

void test_isCore_given_CORE_MASK_INTERRUPT_C_DEBUGEN_1_C_HALT_1_C_MASKINT_1_S_HALT_0_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1;
	coreStatus.C_HALT = 1;
	coreStatus.S_HALT = 0;
	coreStatus.C_MASKINTS = 1 ;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_MASK_INTERRUPT,&coreStatus));
}

void test_isCore_given_CORE_MASK_INTERRUPT_C_DEBUGEN_1_C_HALT_1_C_MASKINT_0_S_HALT_1_should_return_ERR_CORECONTROL_FAILED()
{
	CoreStatus coreStatus ;
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1;
	coreStatus.C_HALT = 1;
	coreStatus.S_HALT = 1;
	coreStatus.C_MASKINTS = 0 ;
	
	TEST_ASSERT_EQUAL(ERR_CORECONTROL_FAILED,isCore(CORE_MASK_INTERRUPT,&coreStatus));
}

/*----------------------inti_XXXXXXX--------------------------------*/


void test_init_CoreStatus_should_set_all_data_to_0()
{
	CoreStatus coreStatus ;
	
	init_CoreStatus(&coreStatus);

	TEST_ASSERT_EQUAL(0,coreStatus.S_RESET);
	TEST_ASSERT_EQUAL(0,coreStatus.S_RETIRE);
	TEST_ASSERT_EQUAL(0,coreStatus.S_LOCKUP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_SLEEP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_REGRDY);
	TEST_ASSERT_EQUAL(0,coreStatus.C_SNAPSTALL);
	TEST_ASSERT_EQUAL(0,coreStatus.C_MASKINTS);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
}

void test_init_DebugEvent_should_set_all_data_to_0()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(0,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(0,debugEvent.HALTED);
}

void test_init_DebugTrap_should_set_all_data_to_0()
{
	DebugTrap debugTrap ;
	
	init_DebugTrap(&debugTrap);
	
	TEST_ASSERT_EQUAL(0,debugTrap.VC_HARDERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_INTERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_BUSERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_STATERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_CHKERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_NOCPERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_MMERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_CORERESET);
}


/*--------------------------------update_CoreStatus------------------------------------------*/
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
//Testing Bit[0]
void test_update_CoreStatus_should_assert_C_DEBUGEN_if_bit0_is_1()
{
	CoreStatus coreStatus ;
	
	init_CoreStatus(&coreStatus);
	
	update_CoreStatus(&coreStatus,0x1);
	
	TEST_ASSERT_EQUAL(0,coreStatus.S_RESET);
	TEST_ASSERT_EQUAL(0,coreStatus.S_RETIRE);
	TEST_ASSERT_EQUAL(0,coreStatus.S_LOCKUP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_SLEEP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_REGRDY);
	TEST_ASSERT_EQUAL(0,coreStatus.C_SNAPSTALL);
	TEST_ASSERT_EQUAL(0,coreStatus.C_MASKINTS);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
}
 
void test_update_CoreStatus_should_deassert_C_DEBUGEN_if_bit0_is_0()
{
	CoreStatus coreStatus ;
	
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_DEBUGEN = 1 ;
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	
	update_CoreStatus(&coreStatus,0);
	
	TEST_ASSERT_EQUAL(0,coreStatus.S_RESET);
	TEST_ASSERT_EQUAL(0,coreStatus.S_RETIRE);
	TEST_ASSERT_EQUAL(0,coreStatus.S_LOCKUP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_SLEEP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_REGRDY);
	TEST_ASSERT_EQUAL(0,coreStatus.C_SNAPSTALL);
	TEST_ASSERT_EQUAL(0,coreStatus.C_MASKINTS);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
}
  
//Testing Bit[1] 
void test_update_CoreStatus_should_assert_C_HALT_if_bit1_is_1()
{
	CoreStatus coreStatus ;
	
	init_CoreStatus(&coreStatus);
	
	update_CoreStatus(&coreStatus,0x2);
	
	TEST_ASSERT_EQUAL(0,coreStatus.S_RESET);
	TEST_ASSERT_EQUAL(0,coreStatus.S_RETIRE);
	TEST_ASSERT_EQUAL(0,coreStatus.S_LOCKUP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_SLEEP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_REGRDY);
	TEST_ASSERT_EQUAL(0,coreStatus.C_SNAPSTALL);
	TEST_ASSERT_EQUAL(0,coreStatus.C_MASKINTS);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(1,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
} 
 
void test_update_CoreStatus_should_deassert_C_HALT_if_bit1_is_0()
{
	CoreStatus coreStatus ;
	
	init_CoreStatus(&coreStatus);
	
	coreStatus.C_HALT = 1 ;
	TEST_ASSERT_EQUAL(1,coreStatus.C_HALT);
	
	update_CoreStatus(&coreStatus,0);
	
	TEST_ASSERT_EQUAL(0,coreStatus.S_RESET);
	TEST_ASSERT_EQUAL(0,coreStatus.S_RETIRE);
	TEST_ASSERT_EQUAL(0,coreStatus.S_LOCKUP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_SLEEP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_REGRDY);
	TEST_ASSERT_EQUAL(0,coreStatus.C_SNAPSTALL);
	TEST_ASSERT_EQUAL(0,coreStatus.C_MASKINTS);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
}  
 
//Testing Bit[17]
void test_update_CoreStatus_should_assert_S_HALT_if_bit17_is_1()
{
	CoreStatus coreStatus ;
	
	init_CoreStatus(&coreStatus);
	
	update_CoreStatus(&coreStatus,0x20000);
	
	TEST_ASSERT_EQUAL(0,coreStatus.S_RESET);
	TEST_ASSERT_EQUAL(0,coreStatus.S_RETIRE);
	TEST_ASSERT_EQUAL(0,coreStatus.S_LOCKUP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_SLEEP);
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_REGRDY);
	TEST_ASSERT_EQUAL(0,coreStatus.C_SNAPSTALL);
	TEST_ASSERT_EQUAL(0,coreStatus.C_MASKINTS);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
	
}
 
void test_update_CoreStatus_should_deassert_S_HALT_if_bit17_is_0()
{
	CoreStatus coreStatus ;
	
	init_CoreStatus(&coreStatus);
	
	coreStatus.S_HALT = 1 ;
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);
	
	update_CoreStatus(&coreStatus,0);
	
	TEST_ASSERT_EQUAL(0,coreStatus.S_RESET);
	TEST_ASSERT_EQUAL(0,coreStatus.S_RETIRE);
	TEST_ASSERT_EQUAL(0,coreStatus.S_LOCKUP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_SLEEP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_REGRDY);
	TEST_ASSERT_EQUAL(0,coreStatus.C_SNAPSTALL);
	TEST_ASSERT_EQUAL(0,coreStatus.C_MASKINTS);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
	
}

//Testing Bits[1:0] and Bit[17] 
void test_update_CoreStatus_given_0x20003_should_assert_C_DEBUGEN_C_HALT_S_HALT()
 {
	CoreStatus coreStatus ;
	
	init_CoreStatus(&coreStatus);
	
	update_CoreStatus(&coreStatus,0x20003);
	
	TEST_ASSERT_EQUAL(0,coreStatus.S_RESET);
	TEST_ASSERT_EQUAL(0,coreStatus.S_RETIRE);
	TEST_ASSERT_EQUAL(0,coreStatus.S_LOCKUP);
	TEST_ASSERT_EQUAL(0,coreStatus.S_SLEEP);
	TEST_ASSERT_EQUAL(1,coreStatus.S_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_REGRDY);
	TEST_ASSERT_EQUAL(0,coreStatus.C_SNAPSTALL);
	TEST_ASSERT_EQUAL(0,coreStatus.C_MASKINTS);
	TEST_ASSERT_EQUAL(0,coreStatus.C_STEP);
	TEST_ASSERT_EQUAL(1,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	
}
/*--------------------------------update_DebugEvent------------------------------------------*/

 /******************************************************************************************************
	Debug Fault Status Register , DFSR
 
	Bits[31:5] --- RESERVED

	Bit[4]		--- EXTERNAL
	Bit[3]		--- VCATCH
	Bit[2]		--- DWTTRAP
	Bit[1]		--- BKPT
	Bit[0]		--- HALTED
	
 ******************************************************************************************************/
//Testing Bit[0]
void test_update_DebugEvent_should_assert_HALTED_if_bit0_is_1()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	update_DebugEvent(&debugEvent,0x1);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(0,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(1,debugEvent.HALTED);
}

void test_update_DebugEvent_should_deassert_HALTED_if_bit0_is_0()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	debugEvent.HALTED = 1 ;
	TEST_ASSERT_EQUAL(1,debugEvent.HALTED);
	
	update_DebugEvent(&debugEvent,0);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(0,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(0,debugEvent.HALTED);
}

//Testing Bit[1]
void test_update_DebugEvent_should_assert_BKPT_if_bit1_is_1()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	update_DebugEvent(&debugEvent,0x2);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(1,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(0,debugEvent.HALTED);
}

void test_update_DebugEvent_should_deassert_BKPT_if_bit1_is_0()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	debugEvent.BKPT = 1 ;
	TEST_ASSERT_EQUAL(1,debugEvent.BKPT);
	
	update_DebugEvent(&debugEvent,0x1);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(0,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(1,debugEvent.HALTED);
}

//Testing Bit[2]
void test_update_DebugEvent_should_assert_DWTTRAP_if_bit2_is_1()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	update_DebugEvent(&debugEvent,0x4);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(1,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(0,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(0,debugEvent.HALTED);
}

void test_update_DebugEvent_should_deassert_DWTTRAP_if_bit2_is_0()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	debugEvent.DWTTRAP = 1 ;
	TEST_ASSERT_EQUAL(1,debugEvent.DWTTRAP);
	
	update_DebugEvent(&debugEvent,0x3);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(1,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(1,debugEvent.HALTED);
}

//Testing Bit[3]
void test_update_DebugEvent_should_assert_VCATCH_if_bit3_is_1()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	update_DebugEvent(&debugEvent,0x8);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(1,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(0,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(0,debugEvent.HALTED);
}

void test_update_DebugEvent_should_deassert_VCATCH_if_bit3_is_0()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	debugEvent.VCATCH = 1 ;
	TEST_ASSERT_EQUAL(1,debugEvent.VCATCH);
	
	update_DebugEvent(&debugEvent,0x7);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(1,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(1,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(1,debugEvent.HALTED);
}

//Testing Bit[4]
void test_update_DebugEvent_should_assert_EXTERNAL_if_bit4_is_1()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	update_DebugEvent(&debugEvent,0x10);
	
	TEST_ASSERT_EQUAL(1,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(0,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(0,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(0,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(0,debugEvent.HALTED);
}

void test_update_DebugEvent_should_deassert_EXTERNAL_if_bit4_is_0()
{
	DebugEvent debugEvent ;
	
	init_DebugEvent(&debugEvent);
	
	debugEvent.EXTERNAL = 1 ;
	TEST_ASSERT_EQUAL(1,debugEvent.EXTERNAL);
	
	update_DebugEvent(&debugEvent,0xF);
	
	TEST_ASSERT_EQUAL(0,debugEvent.EXTERNAL);
	TEST_ASSERT_EQUAL(1,debugEvent.VCATCH);
	TEST_ASSERT_EQUAL(1,debugEvent.DWTTRAP);
	TEST_ASSERT_EQUAL(1,debugEvent.BKPT);
	TEST_ASSERT_EQUAL(1,debugEvent.HALTED);
}

/*-------------------------------update_DebugTrapStatus-----------------------------------------*/
 /******************************************************************************************************
	Debug Exception and Monitor Control Register , DEMCR
 
	Bits[10] 	--- VC_HARDERR
	Bits[9] 	--- VC_INTERR
	Bits[8] 	--- VC_BUSERR
	Bits[7] 	--- VC_STATERR
	Bits[6] 	--- VC_CHKERR
	Bits[5] 	--- VC_NOCPERR
	Bits[4] 	--- VC_MMERR
	
	Bits[0] 	--- VC_CORERESET
 ******************************************************************************************************/

void test_update_DebugEventStatus_should_assert_VC_CORERESET_if_bit0_is_1()
{
	DebugTrap debugTrap;
	
	init_DebugTrap(&debugTrap);
	
	update_DebugTrapStatus(&debugTrap,0x1);
	
	TEST_ASSERT_EQUAL(0,debugTrap.VC_HARDERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_INTERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_BUSERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_STATERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_CHKERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_NOCPERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_MMERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_CORERESET);
}

void test_update_DebugEventStatus_should_deassert_VC_CORERESET_if_bit0_is_0()
{
	DebugTrap debugTrap;
	
	init_DebugTrap(&debugTrap);
	
	debugTrap.VC_CORERESET = 1;
	TEST_ASSERT_EQUAL(1,debugTrap.VC_CORERESET);
	
	update_DebugTrapStatus(&debugTrap,0x0);
	
	TEST_ASSERT_EQUAL(0,debugTrap.VC_HARDERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_INTERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_BUSERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_STATERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_CHKERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_NOCPERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_MMERR);
	TEST_ASSERT_EQUAL(0,debugTrap.VC_CORERESET);
}
  
void test_update_DebugEventStatus_should_given_0xFFFFFFFF_should_assert_all_VC()
{
	DebugTrap debugTrap;
	
	init_DebugTrap(&debugTrap);

	update_DebugTrapStatus(&debugTrap,0xFFFFFFFF);
	
	TEST_ASSERT_EQUAL(1,debugTrap.VC_HARDERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_INTERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_BUSERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_STATERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_CHKERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_NOCPERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_MMERR);
	TEST_ASSERT_EQUAL(1,debugTrap.VC_CORERESET);
} 
 
/*-------------------------------get_Core_WriteValue-----------------------------------------*/


void test_get_Core_WriteValue_given_CORE_DEBUG_MODE_should_return_0xA05F0001_or_SET_CORE_DEBUG()
{
	TEST_ASSERT_EQUAL(0xA05F0001,get_Core_WriteValue(CORE_DEBUG_MODE));
	TEST_ASSERT_EQUAL(SET_CORE_DEBUG,get_Core_WriteValue(CORE_DEBUG_MODE));
}

void test_get_Core_WriteValue_given_CORE_DEBUG_HALT_should_return_0xA05F0003_or_SET_CORE_DEBUG_HALT()
{
	TEST_ASSERT_EQUAL(0xA05F0003,get_Core_WriteValue(CORE_DEBUG_HALT));
	TEST_ASSERT_EQUAL(SET_CORE_DEBUG_HALT,get_Core_WriteValue(CORE_DEBUG_HALT));
}

void test_get_Core_WriteValue_given_CORE_SINGLE_STEP_should_return_0xA05F0007_or_SET_CORE_STEP()
{
	TEST_ASSERT_EQUAL(0xA05F0007,get_Core_WriteValue(CORE_SINGLE_STEP));
	TEST_ASSERT_EQUAL(SET_CORE_STEP,get_Core_WriteValue(CORE_SINGLE_STEP));
}

void test_get_Core_WriteValue_given_CORE_SINGLE_STEP_should_return_0xA05F0007_or_SET_CORE_MASKINT()
{
	TEST_ASSERT_EQUAL(0xA05F000B,get_Core_WriteValue(CORE_MASK_INTERRUPT));
	TEST_ASSERT_EQUAL(SET_CORE_MASKINT,get_Core_WriteValue(CORE_MASK_INTERRUPT));
}


void test_get_Core_WriteValue_given_CORE_SNAPSTALL_should_return_0xA05F0007_or_SET_CORE_SNAPSTALL()
{
	TEST_ASSERT_EQUAL(0xA05F0023,get_Core_WriteValue(CORE_SNAPSTALL));
	TEST_ASSERT_EQUAL(SET_CORE_SNAPSTALL,get_Core_WriteValue(CORE_SNAPSTALL));
}

/*----------------------------get_CoreRegisterAccess_WriteValue----------------------------*/

void test_get_CoreRegisterAccess_WriteValue_given_R0_read_should_return_0()
{
	TEST_ASSERT_EQUAL(0,get_CoreRegisterAccess_WriteValue(ARM_R0,CoreRegister_Read));
}

void test_get_CoreRegisterAccess_WriteValue_given_R0_write_should_return_0x10000()
{
	TEST_ASSERT_EQUAL(0x10000,get_CoreRegisterAccess_WriteValue(ARM_R0,CoreRegister_Write));
}

void test_get_CoreRegisterAccess_WriteValue_given_R12_write_should_return_0x1000C()
{
	TEST_ASSERT_EQUAL(0x1000C,get_CoreRegisterAccess_WriteValue(ARM_R12,CoreRegister_Write));
}

void test_get_CoreRegisterAccess_WriteValue_given_FP_REGS31_read_should_return_0x5F()
{
	TEST_ASSERT_EQUAL(0x5F,get_CoreRegisterAccess_WriteValue(FP_REGS31,CoreRegister_Read));

}

/*-------------------------get_DebugExceptionMonitorControl_WriteValue----------------------------*/
void test_get_DebugExceptionMonitorControl_WriteValue_given_DebugMonitor_DISABLED_debugTrap_0_disable_DWT_ITM_should_return_0()
{
	DebugTrap debugTrap ;
	
	init_DebugTrap(&debugTrap);
	
	TEST_ASSERT_EQUAL(0,get_DebugExceptionMonitorControl_WriteValue(DebugMonitor_DISABLE,&debugTrap,DISABLE_DWT_ITM));
}

void test_get_DebugExceptionMonitorControl_WriteValue_given_DebugMonitor_ENABLE_debugTrap_0_disable_DWT_ITEM_should_return_0x1000()
{
	DebugTrap debugTrap ;
	
	init_DebugTrap(&debugTrap);
	
	TEST_ASSERT_EQUAL(0x1000,get_DebugExceptionMonitorControl_WriteValue(DebugMonitor_ENABLE,&debugTrap,DISABLE_DWT_ITM));

}

void test_get_DebugExceptionMonitorControl_WriteValue_given_DebugMonitor_STEP_debugTrap_0_disable_DWT_ITM_should_return_0x5000()
{
	DebugTrap debugTrap ;
	
	init_DebugTrap(&debugTrap);
	
	TEST_ASSERT_EQUAL(0x5000,get_DebugExceptionMonitorControl_WriteValue(DebugMonitor_STEP,&debugTrap,DISABLE_DWT_ITM));
}

void test_get_DebugExceptionMonitorControl_WriteValue_given_DebugMonitor_DISABLED_debugTrap_0_ENABLE_DWT_ITM_should_return_0x01000000()
{
	DebugTrap debugTrap ;
	
	init_DebugTrap(&debugTrap);
	
	TEST_ASSERT_EQUAL(0x01000000,get_DebugExceptionMonitorControl_WriteValue(DebugMonitor_DISABLE,&debugTrap,ENABLE_DWT_ITM));
}

void test_get_DebugExceptionMonitorControl_WriteValue_given_DebugMonitor_STEP_debugTrap_VC_CORERESET_1_VC_MMERR_1_VC_HARDERR_1_ENABLE_DWT_ITM_should_return_0x01005411()
{
	DebugTrap debugTrap ;
	
	init_DebugTrap(&debugTrap);
	
	debugTrap.VC_CORERESET = 1;
	debugTrap.VC_MMERR = 1;
	debugTrap.VC_HARDERR = 1;
	
	TEST_ASSERT_EQUAL(0x01005411,get_DebugExceptionMonitorControl_WriteValue(DebugMonitor_STEP,&debugTrap,ENABLE_DWT_ITM));
}