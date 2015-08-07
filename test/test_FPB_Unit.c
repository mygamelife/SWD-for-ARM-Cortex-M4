#include "unity.h"
#include <stdint.h>
#include "FPB_Unit.h"
#include "FPB_Utilities.h"
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

/*---------------------------------------control_FPB-----------------------------------*/
//Enable mode
void test_control_FPB_should_write_and_read_FP_CTRL_given_0x1_should_return_ERR_NOERROR()
{
	FPBInfo fpbInfo ;
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Write(DRW_REG,AP,4,ENABLE_FPB);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x00000001));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,control_FPB(&fpbInfo,Enable));
	TEST_ASSERT_EQUAL(Enable,fpbInfo.EnableDisable);
}

void test_control_FPB_should_write_and_read_FP_CTRL_given_0x0_should_return_ERR_FPB_NOTENABLED()
{
	FPBInfo fpbInfo ;
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Write(DRW_REG,AP,4,ENABLE_FPB);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x00000000));
	
	TEST_ASSERT_EQUAL(ERR_FPB_NOTENABLED,control_FPB(&fpbInfo,Enable));
	TEST_ASSERT_EQUAL(Disable,fpbInfo.EnableDisable);
}

//Disable mode
void test_control_FPB_should_write_and_read_FP_CTRL_given_0x0_should_return_ERR_NOERROR()
{
	FPBInfo fpbInfo ;
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Write(DRW_REG,AP,4,DISABLE_FPB);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x00000000));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,control_FPB(&fpbInfo,Disable));
	TEST_ASSERT_EQUAL(Disable,fpbInfo.EnableDisable);
}

void test_control_FPB_should_write_and_read_FP_CTRL_given_0x1_should_return_ERR_FPB_NOTDISABLED()
{
	FPBInfo fpbInfo ;
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Write(DRW_REG,AP,4,DISABLE_FPB);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x00000001));
	
	TEST_ASSERT_EQUAL(ERR_FPB_NOTDISABLED,control_FPB(&fpbInfo,Disable));
	TEST_ASSERT_EQUAL(Enable,fpbInfo.EnableDisable);
}

/*--------------------------------------------read_FPBControl-----------------------------------------*/
void test_read_FPBControl_should_read_FP_CTRL_and_update_FPBInfo_and_return_ERR_NOERROR()
{
	FPBInfo fpbInfo ;
	
	fpbInfo.EnableDisable = Disable;
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x00000001));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_FPBControl(&fpbInfo));
	TEST_ASSERT_EQUAL(Enable,fpbInfo.EnableDisable);
}

void test_read_FPBControl_should_read_FP_CTRL_will_return_ERR_INVALID_PARITY_RECEIVED_if_wrong_parity_is_received_and_will_not_update_FPBInfo()
{
	FPBInfo fpbInfo ;
	
	fpbInfo.EnableDisable = Disable;
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x00000001));
	
	TEST_ASSERT_EQUAL(ERR_INVALID_PARITY_RECEIVED,read_FPBControl(&fpbInfo));
	TEST_ASSERT_EQUAL(Disable,fpbInfo.EnableDisable);

}
/*--------------------------------------------configure_FP_COMP-----------------------------------------*/
//Enable
void test_configure_FP_COMP_given_FP_COMP2_Match_32bits_should_write_and_read_FPCOMP2_and_return_ERR_NOERROR_if_data_is_the_same_and_updateFPBInfo()
{
	FPBInfo fpbInfo ;
	
	init_FPBInfo(&fpbInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_COMP2);
	emulateSWDRegister_Write(DRW_REG,AP,4,get_FP_COMP_WriteValue(0x10000000,Match_32bits,Enable));
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_COMP2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(get_FP_COMP_WriteValue(0x10000000,Match_32bits,Enable)));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_FP_COMP(&fpbInfo,FP_COMP2,0x10000000,Match_32bits,Enable));
	TEST_ASSERT_EQUAL(0x10000000,fpbInfo.compInfo[2]->address);
	TEST_ASSERT_EQUAL(Enable,fpbInfo.compInfo[2]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_32bits,fpbInfo.compInfo[2]->matchingMode);
}

void test_configure_FP_COMP_given_FP_COMP2_Match_Upper16bits_should_write_and_read_FPCOMP2_and_return_ERR_NOERROR_if_data_is_the_same_and_updateFPBInfo()
{
	FPBInfo fpbInfo ;
	
	init_FPBInfo(&fpbInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_COMP2);
	emulateSWDRegister_Write(DRW_REG,AP,4,get_FP_COMP_WriteValue(0x10000000,Match_Upper16bits,Enable));
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_COMP2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(get_FP_COMP_WriteValue(0x10000000,Match_Upper16bits,Enable)));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_FP_COMP(&fpbInfo,FP_COMP2,0x10000000,Match_Upper16bits,Enable));
	TEST_ASSERT_EQUAL(0x10000002,fpbInfo.compInfo[2]->address);
	TEST_ASSERT_EQUAL(Enable,fpbInfo.compInfo[2]->EnableDisable);
	TEST_ASSERT_EQUAL(Match_Upper16bits,fpbInfo.compInfo[2]->matchingMode);
}

void test_configure_FP_COMP_given_FP_COMP100_should_return_ERR_INVALID_COMPARATOR()
{
	FPBInfo fpbInfo ;
	
	init_FPBInfo(&fpbInfo);

	uint32_t FP_COMP100 = 100;
	
	TEST_ASSERT_EQUAL(ERR_INVALID_COMPARATOR,configure_FP_COMP(&fpbInfo,FP_COMP100,0x10000000,Match_Upper16bits,Enable));

}

/*--------------------------------------------configure_FP_REMAP-----------------------------------------*/
void test_configure_FP_REMAP_should_write_and_read_FP_REMAP_and_return_ERR_NOERROR_If_data_is_the_same_and_update_FPBInfo()
{
	uint32_t data = 0 ;
	FPBInfo fpbInfo ;
	
	init_FPBInfo(&fpbInfo);
	
	data = get_FP_REMAP_WriteValue(0x34567890) ;
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_REMAP);
	emulateSWDRegister_Write(DRW_REG,AP,4,data);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_REMAP);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(data));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_FP_REMAP(&fpbInfo,0x34567890));
	TEST_ASSERT_EQUAL((data & FP_REMAP_ReadAddress_MASK),fpbInfo.SRAM_REMAP_address);
}

/*--------------------------------------------prepare_FPBOperations-----------------------------------------*/
//DEBUGMONITOR_DISABLE
void test_prepare_FPBOperations_will_enable_FPB_if_it_is_disable_and_will_setCore_to_CORE_DEBUG_MODE_if_not_in_that_mode_and_return_ERR_NOERROR_if_successful()
{
	FPBInfo fpbInfo ;
	CoreStatus coreStatus ;
	DebugExceptionMonitor debugExceptionMonitor;
	DebugTrap debugTrap;
	init_FPBInfo(&fpbInfo);
	init_CoreStatus(&coreStatus);
	init_DebugTrap(&debugTrap);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
	
	//Enable FPB Unit
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Write(DRW_REG,AP,4,ENABLE_FPB);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x00000001));
	
	//Set Core to CORE_DEBUG_MODE
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,0xA05F0001);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x1));
		
	TEST_ASSERT_EQUAL(ERR_NOERROR,prepare_FPBOperations(&fpbInfo,&coreStatus,&debugExceptionMonitor,&debugTrap,DEBUGMONITOR_DISABLE));
	
	TEST_ASSERT_EQUAL(Enable,fpbInfo.EnableDisable);
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.enableDWT_ITM);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_REQ);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_STEP);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_PEND);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_EN);
}

//DEBUGMONITOR_ENABLED
void test_prepare_FPBOperations_will_enable_FPB_if_it_is_disable_and_will_enable_debugMonitor_return_ERR_NOERROR_if_successful()
{
	uint32_t dataToWrite = 0 ;
	
	FPBInfo fpbInfo ;
	CoreStatus coreStatus ;
	DebugExceptionMonitor debugExceptionMonitor;
	DebugTrap debugTrap;
	init_FPBInfo(&fpbInfo);
	init_CoreStatus(&coreStatus);
	init_DebugTrap(&debugTrap);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
	
	dataToWrite = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_ENABLE,&debugTrap,ENABLE_DWT_ITM);
	
	
	//Enable FPB Unit
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Write(DRW_REG,AP,4,ENABLE_FPB);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,FP_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x00000001));
	
	//Enable debugMonitor
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(dataToWrite));
		
	TEST_ASSERT_EQUAL(ERR_NOERROR,prepare_FPBOperations(&fpbInfo,&coreStatus,&debugExceptionMonitor,&debugTrap,DEBUGMONITOR_ENABLE));
	
	TEST_ASSERT_EQUAL(Enable,fpbInfo.EnableDisable);
	
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	
	TEST_ASSERT_EQUAL(1,debugExceptionMonitor.enableDWT_ITM);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_REQ);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_STEP);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_PEND);
	TEST_ASSERT_EQUAL(1,debugExceptionMonitor.debugMonitor->MON_EN);
}