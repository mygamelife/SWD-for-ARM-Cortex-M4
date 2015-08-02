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
void test_read_DWTMask_given_DWTMASK0_should_read_DWTMASK0_and_update_ignoreMask_in_DWTCOMP0Info()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0xF));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,read_DWTMask(&dwtInfo,DWT_MASK0));
	
	TEST_ASSERT_EQUAL(0xF,dwtInfo.dwtCompInfo[0]->ignoreMask);
}

void test_read_DWTMask_given_DWTMASK2_should_read_DWTMASK2_and_update_ignoreMask_in_DWTCOMP2Info()
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
void test_read_DWTFunction_given_DWTFUN0_should_read_DWTFUNC0_and_update_DWTFunction_in_DWTCOMP0Info()
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
void test_configure_DWTMask_given_DWTMASK0_Ignore_Bit1_Bit0_should_write_and_read_DWTMASK0register_and_update_DWTCOMP0Info()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Write(DRW_REG,AP,4,Ignore_Bit1_Bit0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(Ignore_Bit1_Bit0),MSB_LSB_Conversion(Ignore_Bit1_Bit0));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_DWTMask(&dwtInfo,DWT_MASK0,Ignore_Bit1_Bit0));
	
	TEST_ASSERT_EQUAL(Ignore_Bit1_Bit0,dwtInfo.dwtCompInfo[0]->ignoreMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->ignoreMask);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->ignoreMask);
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
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,configure_DWTFunction(&dwtInfo,DWT_FUNC3,0XA,0XB,CycleCount_Comparison,Word,ITM_Emit_AddressOffset));
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(ITM_Emit_AddressOffset,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dwtFunction);
}

/*------------------disable_DWTComparator--------------*/
void test_disable_DWTComparator_given_DWTCOMP2_should_write_to_DWTFUNC2_to_disable_and_update_DWTCOMP2Info()
{	
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x0));
	
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

void test_disable_DWTComparator_should_retain_the_previous_settings_of_DWTFunction_except_EMITRANGE_and_disable_the_comparator()
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
	TEST_ASSERT_EQUAL(ITM_Emit_AddressOffset,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
	
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x000AB880);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x000AB880),MSB_LSB_Conversion(0x000AB880));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,disable_DWTComparator(&dwtInfo,DWT_COMP2));
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
}

/*------------------reenable_DWTComparator--------------*/
void test_reenable_DWTComparator_givenDWTCOMP2_Sample_PC_Data_Write_should_change_DWTCOMP2_from_DWTFunction_Disabled_to_Sample_PC_Data_Write()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	process_DWTFunctionData(dwtInfo.dwtCompInfo[2]->dwtFunctionInfo,0x000AB880);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x000AB88F);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0x12345677));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x000AB88F),MSB_LSB_Conversion(0x000AB88F));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,reenable_DWTComparator(&dwtInfo,DWT_COMP2,Sample_PC_Data_Write));
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0xA,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0xB,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(Sample_PC_Data_Write,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
}

/*------------------setWatchpoint_PC--------------*/
void test_setWatchpoint_PC_should_disable_comparator_first_program_then_setPCwatchpoint_and_update_COMPinfo()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	uint32_t dataToWrite = 0 ;
	
	dataToWrite = get_DWTFunction_WriteValue(0,0,Address_Comparison,0,Watchpoint_PCMatch);
	
	// Disable comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	
	// Program comparator 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP0);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x12345677);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x12345677),MSB_LSB_Conversion(0x12345677));
	
	// Program mask 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Write(DRW_REG,AP,4,Ignore_Bit0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(Ignore_Bit0),MSB_LSB_Conversion(Ignore_Bit0));
	
	// Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(dataToWrite),MSB_LSB_Conversion(dataToWrite));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_PC(&dwtInfo,DWT_COMP0,0x12345677,Ignore_Bit0));
	
	TEST_ASSERT_EQUAL(0x12345677,dwtInfo.dwtCompInfo[0]->data);
	TEST_ASSERT_EQUAL(Ignore_Bit0,dwtInfo.dwtCompInfo[0]->ignoreMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(Watchpoint_PCMatch,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dwtFunction);
}

/*------------------setWatchpoint_DataAddr--------------*/
//Read
void test_setWatchpoint_DataAddr_Read_should_disable_comparator_first_program_then_setWatchpoint_Read_and_update_COMPinfo()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	//Disable comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	
	//Program comparator 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP0);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x12345677);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x12345677),MSB_LSB_Conversion(0x12345677));
	
	//Program mask 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Write(DRW_REG,AP,4,Ignore_Non);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,Ignore_Non,Ignore_Non);
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Write(DRW_REG,AP,4,5);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(5),MSB_LSB_Conversion(5));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_DataAddr(&dwtInfo,DWT_COMP0,0x12345677,Ignore_Non,Read));
	
	TEST_ASSERT_EQUAL(0x12345677,dwtInfo.dwtCompInfo[0]->data);
	TEST_ASSERT_EQUAL(Ignore_Non,dwtInfo.dwtCompInfo[0]->ignoreMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(Watchpoint_Read,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dwtFunction);
}

//ReadWrite
void test_setWatchpoint_DataAddr_ReadWrite_should_disable_comparator_first_program_then_setWatchpoint_ReadWrite_and_update_COMPinfo()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	
	//Disable comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Program comparator 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP3);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x100);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x100),MSB_LSB_Conversion(0x100));
	
	//Program mask 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK3);
	emulateSWDRegister_Write(DRW_REG,AP,4,Ignore_Non);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(Ignore_Non),MSB_LSB_Conversion(Ignore_Non));
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Write(DRW_REG,AP,4,5);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(5),MSB_LSB_Conversion(5));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_DataAddr(&dwtInfo,DWT_COMP3,0x100,Ignore_Non,ReadWrite));
	
	TEST_ASSERT_EQUAL(0x100,dwtInfo.dwtCompInfo[3]->data);
	TEST_ASSERT_EQUAL(Ignore_Non,dwtInfo.dwtCompInfo[3]->ignoreMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(Watchpoint_ReadWrite,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dwtFunction);
}

/*------------------setWatchpoint_DataValue--------------*/
//Write
void test_setWatchpoint_DataValue_Read_should_disable_DWTCOMP1_first_program_then_setWatchpoint_Write_and_update_COMPinfo()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	uint32_t dataToWrite = 0 ;
	
	dataToWrite = get_DWTFunction_WriteValue(0,0,DataValue_Comparison,Word,Watchpoint_Write);
	
	//Disable comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Program comparator
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x34567890);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x34567890),MSB_LSB_Conversion(0x34567890));
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(dataToWrite),MSB_LSB_Conversion(dataToWrite));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_DataValue(&dwtInfo,0x34567890,Word,Write));
	
	TEST_ASSERT_EQUAL(0x34567890,dwtInfo.dwtCompInfo[1]->data);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Word,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(Watchpoint_Write,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dwtFunction);
}

/*------------------setWatchpoint_LinkedComparison--------------*/
//Single Linked ReadWrite
void test_setWatchpoint_LinkedComparison_singleLinkedReadWrite_should_first_disable_DWTCOMP1_program_DWTCMOP1_and_linkedComparator_setWatchpointRW_and_update_compInfo()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	uint32_t dataToWrite = 0 ;
	
	dataToWrite = get_DWTFunction_WriteValue(0,0,DataValue_Comparison,Byte,Watchpoint_ReadWrite);
	
	//Disable comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//First linked comparator
	//Disable linked comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Program linked comparator 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP0);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x12345670);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x12345670),MSB_LSB_Conversion(0x12345670));
	
	//Program mask 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Write(DRW_REG,AP,4,Ignore_Bit1_Bit0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(Ignore_Bit1_Bit0),MSB_LSB_Conversion(Ignore_Bit1_Bit0));
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Main comparator
	//Disable comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Program comparator
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0xABCDEF);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0xABCDEF),MSB_LSB_Conversion(0xABCDEF));
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(dataToWrite+ 1<<9),MSB_LSB_Conversion(dataToWrite+1<<9));

	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_LinkedComparison(&dwtInfo,0xABCDEF,Byte,DWT_COMP0,0x12345670,Ignore_Bit1_Bit0,0,0,0,ReadWrite));
	
	TEST_ASSERT_EQUAL(0xABCDEF,dwtInfo.dwtCompInfo[1]->data);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Byte,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(Watchpoint_ReadWrite,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0x12345670,dwtInfo.dwtCompInfo[0]->data);
	TEST_ASSERT_EQUAL(Ignore_Bit1_Bit0,dwtInfo.dwtCompInfo[0]->ignoreMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dwtFunction);
}


//Double Linked Read
void test_setWatchpoint_LinkedComparison_doubleLinkedRead_should_first_disable_DWTCOMP1_program_DWTCMOP1_and_linkedComparator_setWatchpointR_and_update_compInfo()
{
	DWTInfo dwtInfo;
	init_DWTInfo(&dwtInfo);
	uint32_t dataToWrite = 0 ;
	
	dataToWrite = get_DWTFunction_WriteValue(2,3,DataValue_Comparison,Halfword,Watchpoint_Read);
	
	//Disable comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//First linked comparator
	//Disable linked comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Program linked comparator 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP2);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x12345670);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x12345670),MSB_LSB_Conversion(0x12345670));
	
	//Program mask 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK2);
	emulateSWDRegister_Write(DRW_REG,AP,4,Ignore_Bit1_Bit0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(Ignore_Bit1_Bit0),MSB_LSB_Conversion(Ignore_Bit1_Bit0));
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Second linked comparator
	//Disable linked comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Program linked comparator 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP3);
	emulateSWDRegister_Write(DRW_REG,AP,4,0x34567890);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0x34567890),MSB_LSB_Conversion(0x34567890));
	
	//Program mask 
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK3);
	emulateSWDRegister_Write(DRW_REG,AP,4,Ignore_Non);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(Ignore_Non),MSB_LSB_Conversion(Ignore_Non));
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Main comparator
	//Disable comparator first
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0),MSB_LSB_Conversion(0));
	
	//Program comparator
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP1);
	emulateSWDRegister_Write(DRW_REG,AP,4,0xABCDEF);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_COMP1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(0xABCDEF),MSB_LSB_Conversion(0xABCDEF));
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC1);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(dataToWrite+1<<9),MSB_LSB_Conversion(dataToWrite + 1 << 9));

	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_LinkedComparison(&dwtInfo,0xABCDEF,Halfword,DWT_COMP2,0x12345670,Ignore_Bit1_Bit0,DWT_COMP3,0x34567890,Ignore_Non,Read));


	TEST_ASSERT_EQUAL(0xABCDEF,dwtInfo.dwtCompInfo[1]->data);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(3,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(2,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(Halfword,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(1,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(Watchpoint_Read,dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0x12345670,dwtInfo.dwtCompInfo[2]->data);
	TEST_ASSERT_EQUAL(Ignore_Bit1_Bit0,dwtInfo.dwtCompInfo[2]->ignoreMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0x34567890,dwtInfo.dwtCompInfo[3]->data);
	TEST_ASSERT_EQUAL(Ignore_Non,dwtInfo.dwtCompInfo[3]->ignoreMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTFunction_Disabled,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dwtFunction);
	
}