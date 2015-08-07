#include "unity.h"
#include "DWT_Utilities.h"
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

/*------------------init_DWTEventStatus--------------*/
void test_init_DWTEventStatus_should_set_all_to_Disable()
{
	DWT_EventStatus event;
	
	init_DWTEventStatus(&event);

	TEST_ASSERT_EQUAL(DWT_DISABLE,event.CycleCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,event.FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,event.LSUCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,event.SleepCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,event.ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,event.CPICountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,event.InterruptTracingEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,event.PCSamplingEvent);
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
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.enableCycleCount);
	
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->CycleCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->LSUCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->SleepCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->CPICountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->InterruptTracingEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->PCSamplingEvent);
}

/*------------------init_DWTFunctionInfo--------------*/
void test_initDWTFunctionInfo_should_set_all_to_0()
{
	DWT_FunctionInfo  dwtFunctionInfo;
	
	init_DWTFunctionInfo(&dwtFunctionInfo);
	
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.comparatorMatched);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.secondLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.firstLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.dataMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.cycleMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.enableTracePackets);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.dwtFunction);
	
	TEST_ASSERT_EQUAL(WATCHPOINT_BYTE,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(DWTComparator_Disabled,dwtFunctionInfo.dwtFunction);
}

/*------------------init_DWTComparatorInfo--------------*/
void test_init_DWTCompratorInfo_should_initialise_DWTComparatorInfo_and_DWTFunctionInfo_together()
{
	DWT_ComparatorInfo compInfo ;
	DWT_FunctionInfo funcInfo ;
	
	init_DWTComparatorInfo(&compInfo,&funcInfo);
	
	TEST_ASSERT_EQUAL(0,compInfo.data);
	TEST_ASSERT_EQUAL(0,compInfo.addressMask);
	TEST_ASSERT_EQUAL(WATCHPOINT_MASK_NOTHING,compInfo.addressMask);
	TEST_ASSERT_EQUAL(&funcInfo,compInfo.dwtFunctionInfo);
	
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->comparatorMatched);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->secondLinkCompNo);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->firstLinkCompNo);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->dataMatchingEnable);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->cycleMatchingEnable);
	TEST_ASSERT_EQUAL(0,compInfo.dwtFunctionInfo->enableTracePackets);
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
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->enableCycleCount);
	
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
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->addressMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->comparatorMatched);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->secondLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->firstLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->cycleMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->enableTracePackets);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->data);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->addressMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->comparatorMatched);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->secondLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->firstLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dataMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->cycleMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->enableTracePackets);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->data);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->addressMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->comparatorMatched);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->secondLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->firstLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->cycleMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->enableTracePackets);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->data);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->addressMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->comparatorMatched);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->secondLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->firstLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->cycleMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->enableTracePackets);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dwtFunction);
}

/*------------------configure_DWTEventGeneration--------------*/
void test_configure_DWTEventGeneration_given_all_disable_should_set_all_members_to_Disable()
{
  DWT_EventStatus eventStatus;

  configure_DWTEventGeneration(&eventStatus,DWT_DISABLE,DWT_DISABLE,DWT_DISABLE,DWT_DISABLE,DWT_DISABLE,DWT_DISABLE,DWT_DISABLE,DWT_DISABLE);
  
  TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.CycleCountEvent);
  TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.FoldedInstructionCountEvent);
  TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.LSUCountEvent);
  TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.SleepCountEvent);
  TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.ExceptionOverheadCountEvent);
  TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.CPICountEvent);
  TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.InterruptTracingEvent);
  TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.PCSamplingEvent);
}

void test_configure_DWTEventGeneration_given_all_enable_should_set_all_members_to_enable()
{
  DWT_EventStatus eventStatus;
  
  configure_DWTEventGeneration(&eventStatus,DWT_ENABLE,DWT_ENABLE,DWT_ENABLE,DWT_ENABLE,DWT_ENABLE,DWT_ENABLE,DWT_ENABLE,DWT_ENABLE);
  
  TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.CycleCountEvent);
  TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.FoldedInstructionCountEvent);
  TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.LSUCountEvent);
  TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.SleepCountEvent);
  TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.ExceptionOverheadCountEvent);
  TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.CPICountEvent);
  TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.InterruptTracingEvent);
  TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.PCSamplingEvent);
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
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.enableCycleCount);
	
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.dwtEventStatus->CycleCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.dwtEventStatus->FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.dwtEventStatus->LSUCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.dwtEventStatus->SleepCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.dwtEventStatus->ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.dwtEventStatus->CPICountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.dwtEventStatus->InterruptTracingEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtControl.dwtEventStatus->PCSamplingEvent);
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
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.enableCycleCount);
	
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->CycleCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->LSUCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->SleepCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->CPICountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->InterruptTracingEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtControl.dwtEventStatus->PCSamplingEvent);
}


/*------------------process_DWTEventStatusData--------------*/
void test_process_EventStatusData_given_0x7F1000_should_enable_all_event()
{
	DWT_EventStatus eventStatus ;
	
	process_DWTEventStatusData(&eventStatus,0x7F1000);
	
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.CycleCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.LSUCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.SleepCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.CPICountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.InterruptTracingEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.PCSamplingEvent);
}

void test_process_DWTEventStatusData_given_0xA51000_should_update_eventStatus()
{
	DWT_EventStatus eventStatus ;
	
	process_DWTEventStatusData(&eventStatus,0xA51000);
	
	TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.CycleCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.LSUCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.SleepCountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,eventStatus.CPICountEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.InterruptTracingEvent);
	TEST_ASSERT_EQUAL(DWT_ENABLE,eventStatus.PCSamplingEvent);
}

/*------------------process_8bitCounterData--------------*/
void test_process_8bitCounterData_given_0xA2_should_store_to_currentValue_previousValue_0()
{
	DWT_8bitCounter counter ;
	
	init_DWT8bitCounter(&counter);
	process_8bitCounterData(&counter,0xA2,CPICount);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0xA2,counter.currentValue);
}

void test_process_8bitCounterData_given_0xA2_0xFF_should_store_to_currentValue_previousValue_0xA2()
{
	DWT_8bitCounter counter ;
	
	init_DWT8bitCounter(&counter);
	
	process_8bitCounterData(&counter,0xA2,CPICount);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0xA2,counter.currentValue);
	
	process_8bitCounterData(&counter,0xFF,CPICount);
	
	TEST_ASSERT_EQUAL(0xA2,counter.previousValue);
	TEST_ASSERT_EQUAL(0xFF,counter.currentValue);
}

/*------------------process_32bitCounterData--------------*/
void test_process_32bitCounterData_given_0x12345678_should_store_to_currentValue_previousValue_0()
{
	DWT_32bitCounter counter ;
	
	init_DWT32bitCounter(&counter);
	process_32bitCounterData(&counter,0x12345678,CycleCount);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0x12345678,counter.currentValue);
}

void test_process_32bitCounterData_given_0x12345678_0x13579BDF_should_store_to_currentValue_previousValue_0x12345678()
{
	DWT_32bitCounter counter ;
	
	init_DWT32bitCounter(&counter);
	
	process_32bitCounterData(&counter,0x12345678,PCSample);
	
	TEST_ASSERT_EQUAL(0,counter.previousValue);
	TEST_ASSERT_EQUAL(0x12345678,counter.currentValue);
	
	process_32bitCounterData(&counter,0x13579BDF,PCSample);
	
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
void test_process_DWTMaskData_given_0x1should_update_addressMask_in_DWTComparatorInfo()
{
	DWT_ComparatorInfo dwtCompInfo;
	
	process_DWTMaskData(&dwtCompInfo,0x1);
	
	TEST_ASSERT_EQUAL(0x1,dwtCompInfo.addressMask);
}

void test_process_DWTMaskData_given_0xF_should_update_addressMask_in_DWTComparatorInfo()
{
	DWT_ComparatorInfo dwtCompInfo;
	
	process_DWTMaskData(&dwtCompInfo,0xF);
	
	TEST_ASSERT_EQUAL(0xF,dwtCompInfo.addressMask);
}


/*------------------process_DWTFunctionData--------------*/
void test_process_DWTFunctionData_given_0xFFFFFFFF_should_update_DWTFunction()
{
	DWT_FunctionInfo dwtFunctionInfo ;
	init_DWTFunctionInfo(&dwtFunctionInfo);
	
	process_DWTFunctionData(&dwtFunctionInfo,0xFFFFFFFF);
	
	TEST_ASSERT_EQUAL(DWT_MATCH,dwtFunctionInfo.comparatorMatched);
	TEST_ASSERT_EQUAL(0xF,dwtFunctionInfo.secondLinkCompNo);
	TEST_ASSERT_EQUAL(0xF,dwtFunctionInfo.firstLinkCompNo);
	TEST_ASSERT_EQUAL(3,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.secondLinkCompEnable);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.dataMatchingEnable);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.cycleMatchingEnable);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.enableTracePackets);
	TEST_ASSERT_EQUAL(Sample_DataAddressOffset_Data_Write,dwtFunctionInfo.dwtFunction);
}

void test_process_DWTFunctionData_given_0xFEFAB8F0_should_update_DWTFunction()
{
	DWT_FunctionInfo dwtFunctionInfo ;
	init_DWTFunctionInfo(&dwtFunctionInfo);
	
	process_DWTFunctionData(&dwtFunctionInfo,0xFEFAB8F0);
	
	TEST_ASSERT_EQUAL(DWT_NOTMATCH,dwtFunctionInfo.comparatorMatched);
	TEST_ASSERT_EQUAL(0xA,dwtFunctionInfo.secondLinkCompNo);
	TEST_ASSERT_EQUAL(0xB,dwtFunctionInfo.firstLinkCompNo);
	TEST_ASSERT_EQUAL(WATCHPOINT_WORD,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.dataMatchingEnable);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.cycleMatchingEnable);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.enableTracePackets);
	TEST_ASSERT_EQUAL(DWTComparator_Disabled,dwtFunctionInfo.dwtFunction);
}

void test_process_DWTFunctionData_given_0x11111111_should_update_DWTFunction()
{
	DWT_FunctionInfo dwtFunctionInfo ;
	init_DWTFunctionInfo(&dwtFunctionInfo);
	
	process_DWTFunctionData(&dwtFunctionInfo,0x11111111);
	
	TEST_ASSERT_EQUAL(DWT_MATCH,dwtFunctionInfo.comparatorMatched);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.secondLinkCompNo);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.firstLinkCompNo);
	TEST_ASSERT_EQUAL(WATCHPOINT_BYTE,dwtFunctionInfo.dataSize);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.secondLinkCompEnable);
	TEST_ASSERT_EQUAL(1,dwtFunctionInfo.dataMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.cycleMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtFunctionInfo.enableTracePackets);
	TEST_ASSERT_EQUAL(ITM_SampleEmit_PC,dwtFunctionInfo.dwtFunction);
}

/*------------------get_DWTFunction_WriteValue--------------*/
void_test_get_DWTFunction_WriteValue_given_1stLink_0xF_2ndLink_0xA_CycleCountCompare_HalfWord_DWTComparator_Disabled_should_return0x000FA4A0()
{
	TEST_ASSERT_EQUAL(0x000FA4A0,get_DWTFunction_WriteValue(0xF,0xA,CYCLECOUNT_COMPARISON,WATCHPOINT_HALFWORD,DWTComparator_Disabled));
}

void_test_get_DWTFunction_WriteValue_given_1stLink_0x3_2ndLink_0x5_DataValueCompare_Word_DWTComparator_Disabled_should_return0x000FA900()
{
	TEST_ASSERT_EQUAL(0x000FA900,get_DWTFunction_WriteValue(0x3,0x5,DATA_COMPARISON,WATCHPOINT_WORD,DWTComparator_Disabled));
}

void_test_get_DWTFunction_WriteValue_given_1stLink_0x3_2ndLink_0x5_DataValueCompare_Word_ITM_Emit_AddressOffset_should_return0x000FA921()
{
	TEST_ASSERT_EQUAL(0x000FA921,get_DWTFunction_WriteValue(0x3,0x5,DATA_COMPARISON,WATCHPOINT_WORD,ITM_Emit_AddressOffset));
}

/*------------------get_DWTControl_WriteValue--------------*/
void test_get_DWTControl_WriteValue_given_enable_all_event_Tap_CYCCNTbit24_cycTap_1_postCnt_0xA_postReset_0xB_enableCycleCount_should_return_0x007F1757()
{
	DWT_EventStatus eventStatus ;
	
	process_DWTEventStatusData(&eventStatus,0x7F1000);
	
	TEST_ASSERT_EQUAL(0x007F1757,get_DWTControl_WriteValue(&eventStatus,Tap_CYCCNTbit24,1,0xA,0xB,DWT_ENABLE));
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


/*---------------------------------------control_DWT-----------------------------------*/
//Enable mode
void test_control_DWT_should_write_and_read_DEMCR_REG_given_0x1000000_should_return_ERR_NOERROR()
{
	uint32_t dataToWrite = 0 ;
	DebugExceptionMonitor debugExceptionMonitor ;
	DWTInfo dwtInfo ;
	DebugTrap debugTrap; 
	init_DebugTrap(&debugTrap);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
	
	dataToWrite = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_DISABLE,&debugTrap,ENABLE_DWT_ITM);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x1000000));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,control_DWT(&dwtInfo,&debugExceptionMonitor,&debugTrap,DEBUGMONITOR_DISABLE,DWT_ENABLE));
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtInfo.enableDWT);
}

void test_control_DWT_should_write_and_read_DEMCR_REG_given_0x0_should_return_ERR_DWT_NOTENABLED()
{
	uint32_t dataToWrite = 0 ;
	DebugExceptionMonitor debugExceptionMonitor ;
	DWTInfo dwtInfo ;
	DebugTrap debugTrap; 
	init_DebugTrap(&debugTrap);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
	
	dataToWrite = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_DISABLE,&debugTrap,ENABLE_DWT_ITM);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0000000));
	
	TEST_ASSERT_EQUAL(ERR_DWT_NOTENABLED,control_DWT(&dwtInfo,&debugExceptionMonitor,&debugTrap,DEBUGMONITOR_DISABLE,DWT_ENABLE));
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.enableDWT);
}

//Disable mode
void test_control_DWT_should_write_and_read_DEMCR_REG_given_0x0_should_return_ERR_NOERROR()
{
	uint32_t dataToWrite = 0 ;
	DebugExceptionMonitor debugExceptionMonitor ;
	DWTInfo dwtInfo ;
	DebugTrap debugTrap; 
	init_DebugTrap(&debugTrap);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
	
	dataToWrite = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_DISABLE,&debugTrap,DISABLE_DWT_ITM);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0000000));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,control_DWT(&dwtInfo,&debugExceptionMonitor,&debugTrap,DEBUGMONITOR_DISABLE,DWT_DISABLE));
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.enableDWT);
}

void test_control_DWT_should_write_and_read_DEMCR_REG_given_0x1000000_should_return_ERR_DWT_NOTDISABLED()
{
	uint32_t dataToWrite = 0 ;
	DebugExceptionMonitor debugExceptionMonitor ;
	DWTInfo dwtInfo ;
	DebugTrap debugTrap; 
	init_DebugTrap(&debugTrap);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
	
	dataToWrite = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_DISABLE,&debugTrap,DISABLE_DWT_ITM);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x1000000));
	
	TEST_ASSERT_EQUAL(ERR_DWT_NOTDISABLED,control_DWT(&dwtInfo,&debugExceptionMonitor,&debugTrap,DEBUGMONITOR_DISABLE,DWT_DISABLE));
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtInfo.enableDWT);
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
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->enableCycleCount);
	
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->dwtEventStatus->CycleCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->dwtEventStatus->FoldedInstructionCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->dwtEventStatus->LSUCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->dwtEventStatus->SleepCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->dwtEventStatus->ExceptionOverheadCountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->dwtEventStatus->CPICountEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->dwtEventStatus->InterruptTracingEvent);
	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.dwtControl->dwtEventStatus->PCSamplingEvent);
	
}

/*------------------read_DWTComaprator--------------*/
void test_read_DWTComparator_given_DWTCOMP1_should_read_DWTCOMP1_and_update_address_in_DWTCOMP1Info()
{
	DWTInfo dwtInfo ;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x4000EEAE));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTComparator(&dwtInfo,DWT_COMP1));
	
	TEST_ASSERT_EQUAL(0x4000EEAE,dwtInfo.dwtCompInfo[1]->data);
}

void test_read_DWTComparator_given_DWTCOMP3_should_read_DWTCOMP3_and_update_address_in_DWTCOMP3Info()
{
	DWTInfo dwtInfo ;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0xABCDEF));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTComparator(&dwtInfo,DWT_COMP3));
	
	TEST_ASSERT_EQUAL(0xABCDEF,dwtInfo.dwtCompInfo[3]->data);
}

/*------------------read_DWTMask--------------*/
void test_read_DWTMask_given_DWTMASK0_should_read_DWTMASK0_and_update_Watchpoint_AddressMask_in_DWTCOMP0Info()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0xF));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTMask(&dwtInfo,DWT_MASK0));
	
	TEST_ASSERT_EQUAL(0xF,dwtInfo.dwtCompInfo[0]->addressMask);
}

void test_read_DWTMask_given_DWTMASK2_should_read_DWTMASK2_and_update_Watchpoint_AddressMask_in_DWTCOMP2Info()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x2));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTMask(&dwtInfo,DWT_MASK2));
	
	TEST_ASSERT_EQUAL(0x2,dwtInfo.dwtCompInfo[2]->addressMask);
}

/*------------------read_DWTFunction--------------*/
void test_read_DWTFunction_given_DWTFUN0_should_read_DWTFUNC0_and_update_DWTFunction_in_DWTCOMP0Info()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0xFEFAB8F0));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTFunction(&dwtInfo,DWT_FUNC0));
	
	TEST_ASSERT_EQUAL(DWT_NOTMATCH,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->comparatorMatched);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->secondLinkCompNo);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->firstLinkCompNo);
	TEST_ASSERT_EQUAL(WATCHPOINT_WORD,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataMatchingEnable);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->cycleMatchingEnable);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->enableTracePackets);
	TEST_ASSERT_EQUAL(DWTComparator_Disabled,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dwtFunction);
}

/*------------------configure_DWTComparator--------------*/
void test_configure_DWTComparator_given_DWTCOMP0_address_0x12345678_should_write_and_read_DWTCOMP0register_and_update_DWTCOMP0Info()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP0);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x12345678);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x12345678),MSB_LSB_Conversion(0x12345678));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_DWTComparator(&dwtInfo,DWT_COMP0,0x12345678));
	
	TEST_ASSERT_EQUAL(0x12345678,dwtInfo.dwtCompInfo[0]->data);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->data);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->data);
}

/*------------------configure_DWTMask--------------*/
void test_configure_DWTMask_given_DWTMASK0_WATCHPOINTMASK_BIT1_BIT0_should_write_and_read_DWTMASK0register_and_update_DWTCOMP0Info()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Write(DRW_REG,AP,4,WATCHPOINT_MASK_BIT1_BIT0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(WATCHPOINT_MASK_BIT1_BIT0),MSB_LSB_Conversion(WATCHPOINT_MASK_BIT1_BIT0));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_DWTMask(&dwtInfo,DWT_MASK0,WATCHPOINT_MASK_BIT1_BIT0));
	
	TEST_ASSERT_EQUAL(WATCHPOINT_MASK_BIT1_BIT0,dwtInfo.dwtCompInfo[0]->addressMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->addressMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->addressMask);
}
/*------------------configure_DWTFunction--------------*/
void test_configure_DWTFunction_given_DWTFUNC3_should_write_and_read_DWTFUNC3_and_update_DWTCOMP3Info()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x000BA8A1);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x000BA8A1),MSB_LSB_Conversion(0x000BA8A1));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_DWTFunction(&dwtInfo,DWT_FUNC3,0XA,0XB,CYCLECOUNT_COMPARISON,WATCHPOINT_WORD,ITM_Emit_AddressOffset));
	
	TEST_ASSERT_EQUAL(DWT_NOTMATCH,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->comparatorMatched);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->secondLinkCompNo);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->firstLinkCompNo);
	TEST_ASSERT_EQUAL(WATCHPOINT_WORD,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataMatchingEnable);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->cycleMatchingEnable);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->enableTracePackets);
	TEST_ASSERT_EQUAL(ITM_Emit_AddressOffset,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dwtFunction);
}

/*------------------disable_DWTComparator--------------*/
void test_disable_DWTComparator_given_DWTCOMP2_should_write_to_DWTFUNC2_set_all_to_0_to_disable_and_update_DWTCOMP2Info()
{	
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->secondLinkCompNo = 0xD;
	dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->firstLinkCompNo = 0x3;
	dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize = WATCHPOINT_HALFWORD;
	dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize = WATCHPOINT_HALFWORD;
	dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataMatchingEnable = DATA_COMPARISON;
	dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->cycleMatchingEnable = 0;
	dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction = Sample_PC_Data_Read;
	
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,disable_DWTComparator(&dwtInfo,DWT_COMP2));
	
	TEST_ASSERT_EQUAL(DWT_NOTMATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->comparatorMatched);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->secondLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->firstLinkCompNo);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->secondLinkCompEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->cycleMatchingEnable);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->enableTracePackets);
	TEST_ASSERT_EQUAL(DWTComparator_Disabled,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
}

/*------------------setup_DWTOperations--------------*/
//DEBUGMONITOR_DISABLE
void test_setup_DWTOperations_will_set_Core_to_CORE_DEBUG_MODE_and_enable_DWT_in_DEMCR()
{
	uint32_t dataToWrite = 0 ;
	
	DWTInfo dwtInfo; 
	CoreStatus coreStatus;
	DebugExceptionMonitor debugExceptionMonitor;
	
	init_DWTInfo(&dwtInfo);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
 	init_CoreStatus(&coreStatus);
	
	dataToWrite = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_DISABLE,debugExceptionMonitor.debugTrap,ENABLE_DWT_ITM);
	
	//Enable DWT
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x1000000));
	
	//Set Core to CORE_DEBUG_MODE
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x1));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setup_DWTOperations(&dwtInfo,&coreStatus,&debugExceptionMonitor,DEBUGMONITOR_DISABLE));
	
	
	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtInfo.enableDWT);
	
	TEST_ASSERT_EQUAL(1,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	
	TEST_ASSERT_EQUAL(1,debugExceptionMonitor.enableDWT_ITM);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_REQ);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_STEP);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_PEND);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_EN);
}

//DEBUGMONITOR_ENABLE
void test_setup_DWTOperations_will_enable_DWT_and_debugMonitor_in_DEMCR()
{
	uint32_t dataToWrite = 0 ;
	
	DWTInfo dwtInfo; 
	CoreStatus coreStatus;
	DebugExceptionMonitor debugExceptionMonitor;
	
	init_DWTInfo(&dwtInfo);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
 	init_CoreStatus(&coreStatus);
	
	dataToWrite = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_ENABLE,debugExceptionMonitor.debugTrap,ENABLE_DWT_ITM);
	
	//Enable DWT
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x1010000));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setup_DWTOperations(&dwtInfo,&coreStatus,&debugExceptionMonitor,DEBUGMONITOR_ENABLE));

	TEST_ASSERT_EQUAL(DWT_ENABLE,dwtInfo.enableDWT);
	
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	
	TEST_ASSERT_EQUAL(1,debugExceptionMonitor.enableDWT_ITM);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_REQ);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_STEP);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_PEND);
	TEST_ASSERT_EQUAL(1,debugExceptionMonitor.debugMonitor->MON_EN);
}

/*------------------cleanup_DWTOperations--------------*/
void test_cleanup_DWTOperations_should_disable_DEBUGMONITOR_disable_DWT_and_set_CORE_NORMAL_MODE()
{
	uint32_t dataToWrite = 0 ;
	
	DWTInfo dwtInfo; 
	CoreStatus coreStatus;
	DebugExceptionMonitor debugExceptionMonitor;
	
	init_DWTInfo(&dwtInfo);
	init_DebugExceptionMonitor(&debugExceptionMonitor);
 	init_CoreStatus(&coreStatus);
	
	dataToWrite = get_DebugExceptionMonitorControl_WriteValue(DEBUGMONITOR_DISABLE,debugExceptionMonitor.debugTrap,DISABLE_DWT_ITM);
	
	//Disable DWT and Debug Monitoro
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DEMCR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	
	//Set Core to CORE_NORMAL_MODE
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_NORMAL);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,cleanup_DWTOperations(&dwtInfo,&coreStatus,&debugExceptionMonitor));

	TEST_ASSERT_EQUAL(DWT_DISABLE,dwtInfo.enableDWT);
	
	TEST_ASSERT_EQUAL(0,coreStatus.C_DEBUGEN);
	TEST_ASSERT_EQUAL(0,coreStatus.C_HALT);
	TEST_ASSERT_EQUAL(0,coreStatus.S_HALT);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.enableDWT_ITM);
	
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_REQ);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_STEP);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_PEND);
	TEST_ASSERT_EQUAL(0,debugExceptionMonitor.debugMonitor->MON_EN);
}