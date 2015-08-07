#include "unity.h"
#include <stdint.h>
#include "DWT_Unit.h"
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
	emulateSWDRegister_Write(DRW_REG,AP,4,WATCHPOINTMASK_BIT0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(WATCHPOINTMASK_BIT0),MSB_LSB_Conversion(WATCHPOINTMASK_BIT0));
	
	// Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Write(DRW_REG,AP,4,dataToWrite);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(dataToWrite),MSB_LSB_Conversion(dataToWrite));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_PC(&dwtInfo,DWT_COMP0,0x12345677,WATCHPOINTMASK_BIT0));
	
	TEST_ASSERT_EQUAL(0x12345677,dwtInfo.dwtCompInfo[0]->data);
	TEST_ASSERT_EQUAL(WATCHPOINTMASK_BIT0,dwtInfo.dwtCompInfo[0]->Watchpoint_AddressMask);
	
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
	emulateSWDRegister_Write(DRW_REG,AP,4,WATCHPOINT_MASK_NOTHING);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,WATCHPOINT_MASK_NOTHING,WATCHPOINT_MASK_NOTHING);
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Write(DRW_REG,AP,4,5);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,0);
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(5),MSB_LSB_Conversion(5));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_DataAddr(&dwtInfo,DWT_COMP0,0x12345677,WATCHPOINT_MASK_NOTHING,Read));
	
	TEST_ASSERT_EQUAL(0x12345677,dwtInfo.dwtCompInfo[0]->data);
	TEST_ASSERT_EQUAL(WATCHPOINT_MASK_NOTHING,dwtInfo.dwtCompInfo[0]->Watchpoint_AddressMask);
	
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
	emulateSWDRegister_Write(DRW_REG,AP,4,WATCHPOINT_MASK_NOTHING);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(WATCHPOINT_MASK_NOTHING),MSB_LSB_Conversion(WATCHPOINT_MASK_NOTHING));
	
	//Program function
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Write(DRW_REG,AP,4,5);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_FUNC3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(5),MSB_LSB_Conversion(5));
	
	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_DataAddr(&dwtInfo,DWT_COMP3,0x100,WATCHPOINT_MASK_NOTHING,ReadWrite));
	
	TEST_ASSERT_EQUAL(0x100,dwtInfo.dwtCompInfo[3]->data);
	TEST_ASSERT_EQUAL(WATCHPOINT_MASK_NOTHING,dwtInfo.dwtCompInfo[3]->Watchpoint_AddressMask);
	
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
	emulateSWDRegister_Write(DRW_REG,AP,4,WATCHPOINTMASK_BIT1_BIT0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK0);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(WATCHPOINTMASK_BIT1_BIT0),MSB_LSB_Conversion(WATCHPOINTMASK_BIT1_BIT0));
	
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

	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_LinkedComparison(&dwtInfo,0xABCDEF,Byte,DWT_COMP0,0x12345670,WATCHPOINTMASK_BIT1_BIT0,0,0,0,ReadWrite));
	
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
	TEST_ASSERT_EQUAL(WATCHPOINTMASK_BIT1_BIT0,dwtInfo.dwtCompInfo[0]->Watchpoint_AddressMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTComparator_Disabled,dwtInfo.dwtCompInfo[0]->dwtFunctionInfo->dwtFunction);
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
	emulateSWDRegister_Write(DRW_REG,AP,4,WATCHPOINTMASK_BIT1_BIT0);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK2);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(WATCHPOINTMASK_BIT1_BIT0),MSB_LSB_Conversion(WATCHPOINTMASK_BIT1_BIT0));
	
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
	emulateSWDRegister_Write(DRW_REG,AP,4,WATCHPOINT_MASK_NOTHING);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DWT_MASK3);
	emulateSWDRegister_Read(DRW_REG,AP,4,0,MSB_LSB_Conversion(0));
	emulateSWDRegister_Read(DRW_REG,AP,4,calculateParity_32bitData(WATCHPOINT_MASK_NOTHING),MSB_LSB_Conversion(WATCHPOINT_MASK_NOTHING));
	
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

	TEST_ASSERT_EQUAL(ERR_NOERROR,setWatchpoint_LinkedComparison(&dwtInfo,0xABCDEF,Halfword,DWT_COMP2,0x12345670,WATCHPOINTMASK_BIT1_BIT0,DWT_COMP3,0x34567890,WATCHPOINT_MASK_NOTHING,Read));


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
	TEST_ASSERT_EQUAL(WATCHPOINTMASK_BIT1_BIT0,dwtInfo.dwtCompInfo[2]->Watchpoint_AddressMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTComparator_Disabled,dwtInfo.dwtCompInfo[2]->dwtFunctionInfo->dwtFunction);
	
	TEST_ASSERT_EQUAL(0x34567890,dwtInfo.dwtCompInfo[3]->data);
	TEST_ASSERT_EQUAL(WATCHPOINT_MASK_NOTHING,dwtInfo.dwtCompInfo[3]->Watchpoint_AddressMask);
	
	TEST_ASSERT_EQUAL(NOT_MATCH,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->MATCHED);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR1);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVADDR0);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dataSize);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->LNK1ENA);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->DATAVMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->CYCMATCH);
	TEST_ASSERT_EQUAL(0,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->EMITRANGE);
	TEST_ASSERT_EQUAL(DWTComparator_Disabled,dwtInfo.dwtCompInfo[3]->dwtFunctionInfo->dwtFunction);
	
}

