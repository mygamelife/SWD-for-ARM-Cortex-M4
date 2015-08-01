#include "unity.h"
#include <stdint.h>
#include "DWT_Unit.h"
#include "DWT_Utilities.h"
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

/*------------------read_DWTControl--------------*/
void test_read_DWTControl_should_read_DWTCTRL_and_Update_DWTInfo()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_CTRL);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x4000EEAE));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTControl(&dwtInfo));
	
	TEST_ASSERT_EQUAL(0x4,dwtInfo.dwtControl->numberComparator);
	TEST_ASSERT_EQUAL(SUPPORTED,dwtInfo.dwtControl->support_Tracing);
	TEST_ASSERT_EQUAL(SUPPORTED,dwtInfo.dwtControl->support_ExternalTrigger);
	TEST_ASSERT_EQUAL(SUPPORTED,dwtInfo.dwtControl->support_CycleCount);
	TEST_ASSERT_EQUAL(SUPPORTED,dwtInfo.dwtControl->support_ProfilingCount);
	TEST_ASSERT_EQUAL(Tap_CYCCNTbit28,dwtInfo.dwtControl->syncTap);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtControl->cycTap);
	TEST_ASSERT_EQUAL(0x5,dwtInfo.dwtControl->postCnt);
	TEST_ASSERT_EQUAL(0x7,dwtInfo.dwtControl->postReset);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->EnableDisable_CycleCountCounter);
	
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->eventStatus->CycleCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->eventStatus->FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->eventStatus->LSUCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->eventStatus->SleepCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->eventStatus->ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->eventStatus->CPICountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->eventStatus->InterruptTracingEvent);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->eventStatus->PCSamplingEvent);
	
}

/*------------------read_DWTComaprator--------------*/
void test_read_DWTComparator_given_DWTCOMP1_should_read_DWTCOMP1_and_update_address_in_DWTCOMP1()
{
	DWTInfo dwtInfo ;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x4000EEAE));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTComparator(&dwtInfo,DWT_COMP1));
	
	TEST_ASSERT_EQUAL(0x4000EEAE,dwtInfo.dwtCompInfo[1]->address);
}

void test_read_DWTComparator_given_DWTCOMP3_should_read_DWTCOMP3_and_update_address_in_DWTCOMP3()
{
	DWTInfo dwtInfo ;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0xABCDEF));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTComparator(&dwtInfo,DWT_COMP3));
	
	TEST_ASSERT_EQUAL(0xABCDEF,dwtInfo.dwtCompInfo[3]->address);
}

/*------------------read_DWTMask--------------*/
void test_read_DWTMask_given_DWTMASK0_should_read_DWTMASK0_and_update_ignoreMask_in_DWTCOMP0()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0xF));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTMask(&dwtInfo,DWT_MASK0));
	
	TEST_ASSERT_EQUAL(0xF,dwtInfo.dwtCompInfo[0]->ignoreMask);
}

void test_read_DWTMask_given_DWTMASK2_should_read_DWTMASK2_and_update_ignoreMask_in_DWTCOMP2()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x2));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTMask(&dwtInfo,DWT_MASK2));
	
	TEST_ASSERT_EQUAL(0x2,dwtInfo.dwtCompInfo[2]->ignoreMask);
}

/*------------------read_DWTFunction--------------*/
void test_read_DWTFunction_given_DWTFUN0_should_read_DWTFUNC0_and_update_DWTFunction_in_DWTCOMP0()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0xFEFAB8F0));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTFunction(&dwtInfo,DWT_FUNC0));
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dwtFunction);
}

/*------------------disable_DWTComparator--------------*/
void xtest_disable_DWTComparator_given_DWTCOMP2_should_write_to_DWTFUNC2_to_disable_and_update_DWTCOMP2()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x0));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,disable_DWTComparator(&dwtInfo,DWT_COMP2));
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
}

void xtest_disable_DWTComparator_should_retain_the_previous_settings_of_DWTFunction_but_just_disable_the_comparator()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	process_DWTFunctionData(dwtInfo.dwtCompInfo[2]->dwtFunctionInfo,0xFEFAB8F1);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(ITM_SampleEmit_PC,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,disable_DWTComparator(&dwtInfo,DWT_COMP2));
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
}