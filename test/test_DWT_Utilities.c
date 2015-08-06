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
	TEST_ASSERT_EQUAL(Disable,event.FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(Disable,event.LSUCountEvent);
	TEST_ASSERT_EQUAL(Disable,event.SleepCountEvent);
	TEST_ASSERT_EQUAL(Disable,event.ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(Disable,event.CPICountEvent);
	TEST_ASSERT_EQUAL(Disable,event.InterruptTracingEvent);
	TEST_ASSERT_EQUAL(Disable,event.PCSamplingEvent);
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
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->LSUCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->SleepCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->CPICountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->InterruptTracingEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->PCSamplingEvent);
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
	
	TEST_ASSERT_EQUAL(0,compInfo.data);
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
	DWTInfo dwtInfo;
	
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
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->data);
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
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->data);
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
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->data);
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
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->data);
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

/*------------------configure_DWTEventGeneration--------------*/
void test_configure_DWTEventGeneration_given_all_disable_should_set_all_members_to_Disable()
{
  EventStatus eventStatus;

  configure_DWTEventGeneration(&eventStatus,Disable,Disable,Disable,Disable,Disable,Disable,Disable,Disable);
  
  TEST_ASSERT_EQUAL(Disable,eventStatus.CycleCountEvent);
  TEST_ASSERT_EQUAL(Disable,eventStatus.FoldedInstructionCountEvent);
  TEST_ASSERT_EQUAL(Disable,eventStatus.LSUCountEvent);
  TEST_ASSERT_EQUAL(Disable,eventStatus.SleepCountEvent);
  TEST_ASSERT_EQUAL(Disable,eventStatus.ExceptionOverheadCountEvent);
  TEST_ASSERT_EQUAL(Disable,eventStatus.CPICountEvent);
  TEST_ASSERT_EQUAL(Disable,eventStatus.InterruptTracingEvent);
  TEST_ASSERT_EQUAL(Disable,eventStatus.PCSamplingEvent);
}

void test_configure_DWTEventGeneration_given_all_enable_should_set_all_members_to_enable()
{
  EventStatus eventStatus;
  
  configure_DWTEventGeneration(&eventStatus,Enable,Enable,Enable,Enable,Enable,Enable,Enable,Enable);
  
  TEST_ASSERT_EQUAL(Enable,eventStatus.CycleCountEvent);
  TEST_ASSERT_EQUAL(Enable,eventStatus.FoldedInstructionCountEvent);
  TEST_ASSERT_EQUAL(Enable,eventStatus.LSUCountEvent);
  TEST_ASSERT_EQUAL(Enable,eventStatus.SleepCountEvent);
  TEST_ASSERT_EQUAL(Enable,eventStatus.ExceptionOverheadCountEvent);
  TEST_ASSERT_EQUAL(Enable,eventStatus.CPICountEvent);
  TEST_ASSERT_EQUAL(Enable,eventStatus.InterruptTracingEvent);
  TEST_ASSERT_EQUAL(Enable,eventStatus.PCSamplingEvent);
}


/*------------------process_DWTControlData--------------*/
void test_process_DWTControlData_given_0xFFFFFFFF_should_update_DWTControl()
{
	DWT_Control dwtControl;
	
	init_DWTControl(&dwtControl);
	process_DWTControlData(&dwtControl,0xFFFFFFFF);
	
	TEST_ASSERT_EQUAL(0xF,dwtControl.numberComparator);
	TEST_ASSERT_EQUAL(NOT_SUPPORTED,dwtControl.support_Tracing);
	TEST_ASSERT_EQUAL(NOT_SUPPORTED,dwtControl.support_ExternalTrigger);
	TEST_ASSERT_EQUAL(NOT_SUPPORTED,dwtControl.support_CycleCount);
	TEST_ASSERT_EQUAL(NOT_SUPPORTED,dwtControl.support_ProfilingCount);
	TEST_ASSERT_EQUAL(Tap_CYCCNTbit28,dwtControl.syncTap);
	TEST_ASSERT_EQUAL(1,dwtControl.cycTap);
	TEST_ASSERT_EQUAL(0xF,dwtControl.postCnt);
	TEST_ASSERT_EQUAL(0xF,dwtControl.postReset);
	TEST_ASSERT_EQUAL(Enable,dwtControl.EnableDisable_CycleCountCounter);
	
	TEST_ASSERT_EQUAL(Enable,dwtControl.eventStatus->CycleCountEvent);
	TEST_ASSERT_EQUAL(Enable,dwtControl.eventStatus->FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(Enable,dwtControl.eventStatus->LSUCountEvent);
	TEST_ASSERT_EQUAL(Enable,dwtControl.eventStatus->SleepCountEvent);
	TEST_ASSERT_EQUAL(Enable,dwtControl.eventStatus->ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(Enable,dwtControl.eventStatus->CPICountEvent);
	TEST_ASSERT_EQUAL(Enable,dwtControl.eventStatus->InterruptTracingEvent);
	TEST_ASSERT_EQUAL(Enable,dwtControl.eventStatus->PCSamplingEvent);
}

void test_process_DWTControlData_given_0x4000EEAE_should_update_DWTControl()
{
	DWT_Control dwtControl;
	
	init_DWTControl(&dwtControl);
	process_DWTControlData(&dwtControl,0x4000EEAE);
	
	TEST_ASSERT_EQUAL(0x4,dwtControl.numberComparator);
	TEST_ASSERT_EQUAL(SUPPORTED,dwtControl.support_Tracing);
	TEST_ASSERT_EQUAL(SUPPORTED,dwtControl.support_ExternalTrigger);
	TEST_ASSERT_EQUAL(SUPPORTED,dwtControl.support_CycleCount);
	TEST_ASSERT_EQUAL(SUPPORTED,dwtControl.support_ProfilingCount);
	TEST_ASSERT_EQUAL(Tap_CYCCNTbit28,dwtControl.syncTap);
	TEST_ASSERT_EQUAL(1,dwtControl.cycTap);
	TEST_ASSERT_EQUAL(0x5,dwtControl.postCnt);
	TEST_ASSERT_EQUAL(0x7,dwtControl.postReset);
	TEST_ASSERT_EQUAL(Disable,dwtControl.EnableDisable_CycleCountCounter);
	
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->CycleCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->LSUCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->SleepCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->CPICountEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->InterruptTracingEvent);
	TEST_ASSERT_EQUAL(Disable,dwtControl.eventStatus->PCSamplingEvent);
}


/*------------------process_EventStatusData--------------*/
void test_process_EventStatusData_given_0x7F1000_should_enable_all_event()
{
	EventStatus eventStatus ;
	
	process_EventStatusData(&eventStatus,0x7F1000);
	
	TEST_ASSERT_EQUAL(Enable,eventStatus.CycleCountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.LSUCountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.SleepCountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.CPICountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.InterruptTracingEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.PCSamplingEvent);
}

void test_process_EventStatusData_given_0xA51000_should_update_eventStatus()
{
	EventStatus eventStatus ;
	
	process_EventStatusData(&eventStatus,0xA51000);
	
	TEST_ASSERT_EQUAL(Disable,eventStatus.CycleCountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(Disable,eventStatus.LSUCountEvent);
	TEST_ASSERT_EQUAL(Disable,eventStatus.SleepCountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(Disable,eventStatus.CPICountEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.InterruptTracingEvent);
	TEST_ASSERT_EQUAL(Enable,eventStatus.PCSamplingEvent);
}

/*------------------process_8bitCounterData--------------*/
void test_process_8bitCounterData_given_0xA2_should_store_to_currentValue_previousValue_0()
{
	DWT_8bitCounter counter ;
	
	init_DWT8bitCounter(&counter);
	process_8bitCounterData(&counter,0xA2,CPI);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0xA2,counter.currentValue);
}

void test_process_8bitCounterData_given_0xA2_0xFF_should_store_to_currentValue_previousValue_0xA2()
{
	DWT_8bitCounter counter ;
	
	init_DWT8bitCounter(&counter);
	
	process_8bitCounterData(&counter,0xA2,CPI);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0xA2,counter.currentValue);
	
	process_8bitCounterData(&counter,0xFF,CPI);
	
	TEST_ASSERT_EQUAL(0xA2,counter.previousValue);
	TEST_ASSERT_EQUAL(0xFF,counter.currentValue);
}

/*------------------process_32bitCounterData--------------*/
void test_process_32bitCounterData_given_0x12345678_should_store_to_currentValue_previousValue_0()
{
	DWT_32bitCounter counter ;
	
	init_DWT32bitCounter(&counter);
	process_32bitCounterData(&counter,0x12345678,CPI);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0x12345678,counter.currentValue);
}

void test_process_32bitCounterData_given_0x12345678_0x13579BDF_should_store_to_currentValue_previousValue_0x12345678()
{
	DWT_32bitCounter counter ;
	
	init_DWT32bitCounter(&counter);
	
	process_32bitCounterData(&counter,0x12345678,PC);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0x12345678,counter.currentValue);
	
	process_32bitCounterData(&counter,0x13579BDF,PC);
	
	TEST_ASSERT_EQUAL(0x12345678,counter.previousValue);
	TEST_ASSERT_EQUAL(0x13579BDF,counter.currentValue);
}

/*------------------process_DWTComparatorData--------------*/
void test_process_DWTComparatorData_given_0x12345678_should_update_address_in_DWTComparatorInfo()
{
	DWT_ComparatorInfo dwtCompInfo;
	
	process_DWTComparatorData(&dwtCompInfo,0x12345678);
	
	TEST_ASSERT_EQUAL(0x12345678,dwtCompInfo.data);
	
}

void test_process_DWTComparatorData_given_0xFFFFFFFF_should_update_addressn_DWTComparatorInfo()
{
	DWT_ComparatorInfo dwtCompInfo;
	
	process_DWTComparatorData(&dwtCompInfo,0xFFFFFFFF);
	
	TEST_ASSERT_EQUAL(0xFFFFFFFF,dwtCompInfo.data);
}

/*------------------process_DWTMaskData--------------*/
void test_process_DWTMaskData_given_0x1should_update_ignoreMask_in_DWTComparatorInfo()
{
	DWT_ComparatorInfo dwtCompInfo;
	
	process_DWTMaskData(&dwtCompInfo,0x1);
	
	TEST_ASSERT_EQUAL(0x1,dwtCompInfo.ignoreMask);
}

void test_process_DWTMaskData_given_0xF_should_update_ignoreMask_in_DWTComparatorInfo()
{
	DWT_ComparatorInfo dwtCompInfo;
	
	process_DWTMaskData(&dwtCompInfo,0xF);
	
	TEST_ASSERT_EQUAL(0xF,dwtCompInfo.ignoreMask);
}


/*------------------process_DWTFunctionData--------------*/
void test_process_DWTFunctionData_given_0xFFFFFFFF_should_update_DWTFunction()
{
	DWT_FunctionInfo dwtFunctionInfo ;
	init_DWTFunctionInfo(&dwtFunctionInfo);
	
	process_DWTFunctionData(&dwtFunctionInfo,0xFFFFFFFF);
	
	TEST_ASSERT_EQUAL(MATCH,dwtFunctionInfo.MATCHED);
	TEST_ASSERT_EQUAL(0xF,dwtFunctionInfo.DATAVADDR1);
	TEST_ASSERT_EQUAL(0xF,dwtFunctionInfo.DATAVADDR0);
	TEST_ASSERT_EQUAL(3,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.LNK1ENA);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.CYCMATCH);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.EMITRANGE);
	TEST_ASSERT_EQUAL(Sample_DataAddressOffset_Data_Write,dwtFunctionInfo.dwtFunction);
}

void test_process_DWTFunctionData_given_0xFEFAB8F0_should_update_DWTFunction()
{
	DWT_FunctionInfo dwtFunctionInfo ;
	init_DWTFunctionInfo(&dwtFunctionInfo);
	
	process_DWTFunctionData(&dwtFunctionInfo,0xFEFAB8F0);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtFunctionInfo.MATCHED);
	TEST_ASSERT_EQUAL(0xA,dwtFunctionInfo.DATAVADDR1);
	TEST_ASSERT_EQUAL(0xB,dwtFunctionInfo.DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.CYCMATCH);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtFunctionInfo.dwtFunction);
}

void test_process_DWTFunctionData_given_0x11111111_should_update_DWTFunction()
{
	DWT_FunctionInfo dwtFunctionInfo ;
	init_DWTFunctionInfo(&dwtFunctionInfo);
	
	process_DWTFunctionData(&dwtFunctionInfo,0x11111111);
	
	TEST_ASSERT_EQUAL(MATCH,dwtFunctionInfo.MATCHED);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.DATAVADDR1);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.LNK1ENA);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.EMITRANGE);
	TEST_ASSERT_EQUAL(ITM_SampleEmit_PC,dwtFunctionInfo.dwtFunction);
}

/*------------------get_DWTFunction_WriteValue--------------*/
void_test_get_DWTFunction_WriteValue_given_1stLink_0xF_2ndLink_0xA_CycleCountCompare_HalfWord_DWTFunction_DISABLED_should_return0x000FA4A0()
{
	TEST_ASSERT_EQUAL(0x000FA4A0,get_DWTFunction_WriteValue(0xF,0xA,CycleCount_Comparison,Halfword,DWTFunction_Disabled));
}

void_test_get_DWTFunction_WriteValue_given_1stLink_0x3_2ndLink_0x5_DataValueCompare_Word_DWTFunction_DISABLED_should_return0x000FA900()
{
	TEST_ASSERT_EQUAL(0x000FA900,get_DWTFunction_WriteValue(0x3,0x5,DataValue_Comparison,Word,DWTFunction_Disabled));
}

void_test_get_DWTFunction_WriteValue_given_1stLink_0x3_2ndLink_0x5_DataValueCompare_Word_ITM_Emit_AddressOffset_should_return0x000FA921()
{
	TEST_ASSERT_EQUAL(0x000FA921,get_DWTFunction_WriteValue(0x3,0x5,DataValue_Comparison,Word,ITM_Emit_AddressOffset));
}

/*------------------get_DWTControl_WriteValue--------------*/
void test_get_DWTControl_WriteValue_given_enable_all_event_Tap_CYCCNTbit24_cycTap_1_postCnt_0xA_postReset_0xB_enableCycleCount_should_return_0x007F1757()
{
	EventStatus eventStatus ;
	
	process_EventStatusData(&eventStatus,0x7F1000);
	
	TEST_ASSERT_EQUAL(0x007F1757,get_DWTControl_WriteValue(&eventStatus,Tap_CYCCNTbit24,1,0xA,0xB,Enable));
}

/*------------------get_DWTComparatorInfoNumber--------------*/
void test_get_DWTComparatorInfoNumber_given_DWT_COMP0_DWT_MASK0_DWT_FUNC0_should_return_0()
{
	TEST_ASSERT_EQUAL(0,get_DWTComparatorInfoNumber(DWT_COMP0));
	TEST_ASSERT_EQUAL(0,get_DWTComparatorInfoNumber(DWT_MASK0));
	TEST_ASSERT_EQUAL(0,get_DWTComparatorInfoNumber(DWT_FUNC0));
}


void test_get_DWTComparatorInfoNumber_given_DWT_COMP3_DWT_MASK3_DWT_FUNC3_should_return_3()
{
	TEST_ASSERT_EQUAL(3,get_DWTComparatorInfoNumber(DWT_COMP3));
	TEST_ASSERT_EQUAL(3,get_DWTComparatorInfoNumber(DWT_MASK3));
	TEST_ASSERT_EQUAL(3,get_DWTComparatorInfoNumber(DWT_FUNC3));
}
void test_get_DWTComparatorInfoNumber_given_unknown_address_should_return_ERR_INVALID_DWTREGISTER()
{
	TEST_ASSERT_EQUAL(ERR_INVALID_DWTREGISTER,get_DWTComparatorInfoNumber(0x12345678));
}