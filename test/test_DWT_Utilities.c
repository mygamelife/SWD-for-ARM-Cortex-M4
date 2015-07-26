#include "unity.h"
#include "DWT_Utilities.h"
#include "Misc_Utilities.h"
void setUp(void)
{
}

void tearDown(void)
{
}

/*------------------init_DWT8bitCounter--------------*/

void test_init_DWT8bitCounter_should_set_all_to_zero()
{
	DWT_8bitCounter counter;
	
	init_DWT8bitCounter(&counter);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0,counter.currentValue);
}

/*------------------init_DWT32bitCounter--------------*/
void test_init_DWT32bitCounter_should_set_all_to_zero()
{
	DWT_32bitCounter counter;
	
	init_DWT32bitCounter(&counter);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0,counter.currentValue);
}

/*------------------init_EventStatus--------------*/
void test_init_EventStatus_should_set_all_to_Disable()
{
	EventStatus event;
	
	init_EventStatus(&event);

	TEST_ASSERT_EQUAL(Disable,event.CycleCountEvent);
	TEST_ASSERT_EQUAL(Disable,event.FoldedInstructionCount);
	TEST_ASSERT_EQUAL(Disable,event.LSUCount);
	TEST_ASSERT_EQUAL(Disable,event.SleepCount);
	TEST_ASSERT_EQUAL(Disable,event.ExceptionOverheadCount);
	TEST_ASSERT_EQUAL(Disable,event.CPICount);
	TEST_ASSERT_EQUAL(Disable,event.InterruptTracing);
	TEST_ASSERT_EQUAL(Disable,event.PCSampling);
}

/*------------------init_DWTControl--------------*/
void test_DWTControl_should_set_initialise_dwtControl()
{
	DWT_Control dwtControl;
	
	init_DWTControl(&dwtControl);
	
	TEST_ASSERT_EQUAL(4,dwtControl.numberComparator);
	TEST_ASSERT_EQUAL(0,dwtControl.support_Tracing);
	TEST_ASSERT_EQUAL(0,dwtControl.support_ExternalTrigger);
	TEST_ASSERT_EQUAL(0,dwtControl.support_CycleCount);
	TEST_ASSERT_EQUAL(0,dwtControl.support_ProfilingCount);
	TEST_ASSERT_EQUAL(Tap_Disabled,dwtControl.syncTap);
	TEST_ASSERT_EQUAL(0,dwtControl.cycTap);
	TEST_ASSERT_EQUAL(0,dwtControl.postCnt);
	TEST_ASSERT_EQUAL(0,dwtControl.postReset);
	TEST_ASSERT_EQUAL(Disable,dwtControl.EnableDisable_CycleCountCounter);
	
	
	
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->CycleCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->FoldedInstructionCount);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->LSUCount);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->SleepCount);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->ExceptionOverheadCount);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->CPICount);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->InterruptTracing);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->PCSampling);
}

/*------------------init_DWTFunctionInfo--------------*/
void test_initDWTFunctionInfo_should_set_all_to_0()
{
	DWT_FunctionInfo  dwtFunctionInfo;
	
	init_DWTFunctionInfo(&dwtFunctionInfo);
	
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.MATCHED);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.EMITRANGE);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.dwtFunction);
	
	TEST_ASSERT_EQUAL(Byte,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtFunctionInfo.dwtFunction);
}

/*------------------init_DWTComparatorInfo--------------*/
void test_init_DWTCompratorInfo_should_initialise_DWTComparatorInfo_and_DWTFunctionInfo_together()
{
	DWT_ComparatorInfo compInfo ;
	DWT_FunctionInfo funcInfo ;
	
	init_DWTComparatorInfo(&compInfo,&funcInfo);
	
	TEST_ASSERT_EQUAL(0,compInfo.address);
	TEST_ASSERT_EQUAL(0,compInfo.ignoreMask);
	TEST_ASSERT_EQUAL(Ignore_Non,compInfo.ignoreMask);
	TEST_ASSERT_EQUAL(&funcInfo,compInfo.dwtFunctionInfo);
	
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->dwtFunction);
}

/*------------------init_DWTInfo--------------*/
void test_init_DWTInfo_should_initialise_all_8bit_and_32bitcounters_DWTControl_and_DWTComparatorInfo_DWTFunctionInfo()
{
	DWT_Info dwtInfo;
	
	init_DWTInfo(&dwtInfo);
	
	TEST_ASSERT_EQUAL(4,dwtInfo.dwtControl->numberComparator);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtControl->support_Tracing);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtControl->support_ExternalTrigger);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtControl->support_CycleCount);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtControl->support_ProfilingCount);
	TEST_ASSERT_EQUAL(Tap_Disabled,dwtInfo.dwtControl->syncTap);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtControl->cycTap);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtControl->postCnt);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtControl->postReset);
	TEST_ASSERT_EQUAL(Disable,dwtInfo.dwtControl->EnableDisable_CycleCountCounter);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.CycleCount->previousValue);
	TEST_ASSERT_EQUAL(0,dwtInfo.CycleCount->currentValue);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.CPICount->previousValue);
	TEST_ASSERT_EQUAL(0,dwtInfo.CPICount->currentValue);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.ExceptionOverheadCount->previousValue);
	TEST_ASSERT_EQUAL(0,dwtInfo.ExceptionOverheadCount->currentValue);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.LSUCount->previousValue);
	TEST_ASSERT_EQUAL(0,dwtInfo.LSUCount->currentValue);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.FoldedInstructionCount->previousValue);
	TEST_ASSERT_EQUAL(0,dwtInfo.FoldedInstructionCount->currentValue);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.PCSample->previousValue);
	TEST_ASSERT_EQUAL(0,dwtInfo.PCSample->currentValue);	
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->address);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->ignoreMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->address);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->ignoreMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->address);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->ignoreMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->address);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->ignoreMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dwtFunction);
}
