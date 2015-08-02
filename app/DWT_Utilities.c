#include "DWT_Utilities.h"

/**
 * Initialise whole DWTInfo structure which contains of DWT Control, DWT Comparator, DWT Mask, DWT Function, 
 * DWT Cycle Count , DWT CPI Count , DWT ExceptionOverhead Count, DWT Sleep Count, DWT LSU Count,
 * DWT FoldedInstruction Count,DWT Program Counter sample
 *
 * Input : dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 */
void init_DWTInfo(DWTInfo *dwtInfo)
{
	static DWT_Control dwtCtrl ;
	static DWT_8bitCounter cpi,exceptionOverhead,sleep,lsu,folderInstruction ;
	static DWT_32bitCounter cycle,pcSampling ;
	static DWT_ComparatorInfo dwtComp[4] ;
	static DWT_FunctionInfo dwtFunction[4];
	
	init_DWTControl(&dwtCtrl);
	init_DWT32bitCounter(&cycle);
	init_DWT8bitCounter(&cpi);
	init_DWT8bitCounter(&exceptionOverhead);
	init_DWT8bitCounter(&sleep);
	init_DWT8bitCounter(&lsu);
	init_DWT8bitCounter(&folderInstruction);
	init_DWT32bitCounter(&pcSampling);
	
	init_DWTComparatorInfo(&(dwtComp[0]),&(dwtFunction[0]));
	init_DWTComparatorInfo(&(dwtComp[1]),&(dwtFunction[1]));
	init_DWTComparatorInfo(&(dwtComp[2]),&(dwtFunction[2]));
	init_DWTComparatorInfo(&(dwtComp[3]),&(dwtFunction[3]));
	
	
	dwtInfo->dwtControl 			= &dwtCtrl;
	
	dwtInfo->CycleCount 			= &cycle;
	dwtInfo->CPICount 				= &cpi;
	dwtInfo->ExceptionOverheadCount = &exceptionOverhead ;
	dwtInfo->SleepCount 			= &sleep;
	dwtInfo->LSUCount 				= &lsu;
	dwtInfo->FoldedInstructionCount = &folderInstruction;
	dwtInfo->PCSample 				= &pcSampling;
	
	dwtInfo->dwtCompInfo[0] 		= &(dwtComp[0]) ;	
	dwtInfo->dwtCompInfo[1]			= &(dwtComp[1]) ;	
	dwtInfo->dwtCompInfo[2] 		= &(dwtComp[2]) ;	
	dwtInfo->dwtCompInfo[3] 		= &(dwtComp[3]) ;	
}

/**
 * Initialise DWT Control structure
 *
 * Input : dwtControl is a pointer to DWTControl which store the information about the status of DWT Control register
 */
void init_DWTControl(DWT_Control *dwtControl)
{
	static EventStatus event ;
	
	init_EventStatus(&event);
	
	dwtControl->numberComparator 				= 4 ;
	dwtControl->support_Tracing 				= 0 ;
	dwtControl->support_ExternalTrigger 		= 0 ;
	dwtControl->support_CycleCount 				= 0 ;
	dwtControl->support_ProfilingCount			= 0 ;
	dwtControl->eventStatus 					= &event;
	dwtControl->syncTap				 			= Tap_Disabled;
	dwtControl->cycTap 							= 0 ;
	dwtControl->postCnt 						= 0;
	dwtControl->postReset 						= 0;
	dwtControl->EnableDisable_CycleCountCounter = Disable;
}

/**
 * Initialise EventStatus structure
 *
 * Input : eventStatus is a pointer to EventStatus which store the information about the status of event enabled in DWT Control Register
 */
void init_EventStatus(EventStatus *eventStatus)
{
	eventStatus->CycleCountEvent  			= Disable ;
	eventStatus->FoldedInstructionCountEvent  	= Disable ;
	eventStatus->LSUCountEvent  					= Disable ;
	eventStatus->SleepCountEvent  				= Disable ;
	eventStatus->ExceptionOverheadCountEvent  	= Disable ;
	eventStatus->CPICountEvent  					= Disable ;
	eventStatus->InterruptTracingEvent  			= Disable ;
	eventStatus->PCSamplingEvent  				= Disable ;
}


/**
 * Initialise 8 bit counter (for CPICount,ExceptionOverheadCount,SleepCount,LSUCount,FoldedInstructionCount)
 *
 * Input : counter is a pointer to DWT 8 bit counter which store the information about the specific counter
 */
void init_DWT8bitCounter(DWT_8bitCounter *counter)
{
	counter->previousValue = 0 ;
	counter->currentValue  = 0 ;
}

/**
 * Initialise 32 bit counter (for CycleCount,PCSample)
 *
 * Input : counter is a pointer to DWT 8 bit counter which store the information about the specific counter
 */
void init_DWT32bitCounter(DWT_32bitCounter *counter)
{
	counter->previousValue = 0 ;
	counter->currentValue  = 0 ;
}

/**
 * Initialise DWT Comparator Info and DWT Function Info
 *
 * Input : dwtCompInfo is a pointer to DWT Comparator Info which store the information about DWT Comparator,Mask and Function Registers
 *		   dwtFunctionInfo is a pointer to DWT Function Info which store the information about DWT Function Register
 */
void init_DWTComparatorInfo(DWT_ComparatorInfo *dwtCompInfo,DWT_FunctionInfo *dwtFunction)
{	
	init_DWTFunctionInfo(dwtFunction);

	dwtCompInfo->data 		 = 0 ;
	dwtCompInfo->ignoreMask 	 = Ignore_Non ;
	dwtCompInfo->dwtFunctionInfo = dwtFunction;
}

/**
 * Initialise DWT Function Info 
 *
 * Input : dwtFunctionInfo is a pointer to DWT Function Info which store the information about DWT Function Register
 */
void init_DWTFunctionInfo(DWT_FunctionInfo *dwtFunctionInfo)
{
	dwtFunctionInfo->MATCHED 	= 0 ;
	dwtFunctionInfo->DATAVADDR1	= 0 ;
	dwtFunctionInfo->DATAVADDR0	= 0 ;
	dwtFunctionInfo->dataSize	= Byte ;
	dwtFunctionInfo->LNK1ENA 	= 0 ;
	dwtFunctionInfo->DATAVMATCH	= 0	;
	dwtFunctionInfo->CYCMATCH 	= 0 ;
	dwtFunctionInfo->EMITRANGE	= 0 ;
	dwtFunctionInfo->dwtFunction= DWTFunction_Disabled ;
}

/**
 * Configure the event going to be enabled inside DWT Control Register
 *
 * Input :  eventStatus is a pointer to EventStatus which store the event that going to be enabled in the DWT Control Register
 *			CYCEVTENA		Enable/Disable Cycle count event
 *			FOLDEVTENA		Enable/Disable Folded Instruction count event
 *			LSUEVTENA		Enable/Disable LSU count event
 *			SLEEPEVTENA		Enable/Disable Sleep count event
 *			EXCEVTENA		Enable/Disable Interrupt Overhead event
 *			CPIEVTENA		Enable/Disable CPI count event
 *			EXCEVTENA		Enable/Disable Interrupt event tracing
 *			PCSAMPLEENA		Enable/Disable PC Sampling Event
 *			
 */
void configure_DWTEventGeneration(EventStatus *eventStatus,int CYCEVTENA,int FOLDEVTENA,int LSUEVTENA,int SLEEPEVTENA,int EXCEVTENA,int CPIEVTENA,int EXCTRCENA,int PCSAMPLEENA)
{
  eventStatus->CycleCountEvent = CYCEVTENA;
  eventStatus->FoldedInstructionCountEvent = FOLDEVTENA;
  eventStatus->LSUCountEvent = LSUEVTENA;
  eventStatus->SleepCountEvent = SLEEPEVTENA;
  eventStatus->ExceptionOverheadCountEvent = EXCEVTENA;
  eventStatus->CPICountEvent = CPIEVTENA;
  eventStatus->InterruptTracingEvent = EXCTRCENA;
  eventStatus->PCSamplingEvent = PCSAMPLEENA;
}


/**
 * Process data read from DWT Control register and update DWTControl
 *
 * Input : dwtControl is a pointer to DWTControl which store the information about the status of DWT Control register
 * 		   dataRead contains the data read from DWT Control register 
 */
void process_DWTControlData(DWT_Control *dwtControl,uint32_t dataRead)
{
	
	dwtControl->numberComparator = (dataRead & NUMCOMP_MASK ) >>28;
	
	if ( check_BitSetWithMask(dataRead,NOTRCPKT_MASK) == ERR_NOERROR )
		dwtControl->support_Tracing = NOT_SUPPORTED ;
	else
		dwtControl->support_Tracing = SUPPORTED ;
	
	if ( check_BitSetWithMask(dataRead,NOEXTTRIG_MASK) == ERR_NOERROR )
		dwtControl->support_ExternalTrigger = NOT_SUPPORTED ;
	else
		dwtControl->support_ExternalTrigger = SUPPORTED ;
	
	if ( check_BitSetWithMask(dataRead,NOCYCCNT_MASK) == ERR_NOERROR )
		dwtControl->support_CycleCount = NOT_SUPPORTED ;
	else
		dwtControl->support_CycleCount = SUPPORTED ;
	
	if ( check_BitSetWithMask(dataRead,NOPRFCNT_MASK) == ERR_NOERROR )
		dwtControl->support_ProfilingCount = NOT_SUPPORTED ;
	else
		dwtControl->support_ProfilingCount = SUPPORTED ;
	
	process_EventStatusData(dwtControl->eventStatus,dataRead);
	
	dwtControl->syncTap = (dataRead & SYNCTAP_MASK) >> 10;
	dwtControl->cycTap = (dataRead & CYCTAP_MASK) >> 9;
	dwtControl->postCnt = (dataRead & POSTINIT_MASK) >> 5; 
	dwtControl->postReset = (dataRead & POSTRESET_MASK) >> 1;
	
	
	if ( check_BitSetWithMask(dataRead,CYCCNTENA_MASK) == ERR_NOERROR )
		dwtControl->EnableDisable_CycleCountCounter = Enable ; 
	else
		dwtControl->EnableDisable_CycleCountCounter = Disable ; 

}

/**
 * Process data read from DWT Control register and update EventStatus
 *
 * Input : dwtControl is a pointer to DWTControl which store the information about the status of DWT Control register
 * 		   dataRead contains the data read from DWT Control register 
 */
void process_EventStatusData(EventStatus *eventStatus,uint32_t dataRead)
{
	
	if ( check_BitSetWithMask(dataRead,CYCEVTENA_MASK) == ERR_NOERROR)
		eventStatus->CycleCountEvent = Enable ;
	else
		eventStatus->CycleCountEvent = Disable ;
	
	if ( check_BitSetWithMask(dataRead,FOLDEVTENA_MASK) == ERR_NOERROR)
		eventStatus->FoldedInstructionCountEvent = Enable ;
	else
		eventStatus->FoldedInstructionCountEvent = Disable;
	
	if ( check_BitSetWithMask(dataRead,LSUEVTENA_MASK) == ERR_NOERROR)
		eventStatus->LSUCountEvent = Enable ;
	else
		eventStatus->LSUCountEvent = Disable ;
	
	if ( check_BitSetWithMask(dataRead,SLEEPEVTENA_MASK) == ERR_NOERROR)
		eventStatus->SleepCountEvent = Enable ;
	else
		eventStatus->SleepCountEvent = Disable ;
	
	if ( check_BitSetWithMask(dataRead,EXCEVTENA_MASK) == ERR_NOERROR)
		eventStatus->ExceptionOverheadCountEvent = Enable ;
	else
		eventStatus->ExceptionOverheadCountEvent = Disable ;
	
	if ( check_BitSetWithMask(dataRead,CPIEVTENA_MASK) == ERR_NOERROR)
		eventStatus->CPICountEvent = Enable ;
	else
		eventStatus->CPICountEvent = Disable ;
	
	if ( check_BitSetWithMask(dataRead,EXCTRCENA_MASK) == ERR_NOERROR)
		eventStatus->InterruptTracingEvent = Enable ;
	else
		eventStatus->InterruptTracingEvent = Disable ;

	if ( check_BitSetWithMask(dataRead,PCSAMPLENA_MASK) == ERR_NOERROR)
		eventStatus->PCSamplingEvent= Enable ;
	else
		eventStatus->PCSamplingEvent = Disable ;
	
}

/**
 * Process data read from DWT CycleCount and ProgramCounter Sample and update the counter's info
 *
 * Input : counter is a pointer to DWT 8 bit counter which store the information about the specific counter
 * 		   dataRead contains the data read from the counter
 */
void process_8bitCounterData(DWT_8bitCounter *counter,uint32_t dataRead,Enum8bitCounter counterEnum)
{
	static int numberAccess[5] = {0};
	int temp = 0;
	
	temp = counter->currentValue ; //act as a storage of the previous read of currentValue
	
	counter->currentValue = dataRead ;
	
	if (numberAccess[counterEnum] > 0)
		counter->previousValue = temp ;
	
	numberAccess[counterEnum] ++ ;
}
	
/** 
 * Process data read from DWT CPICount,ExceptionOverheadCount,SleepCount,LSUCount,FoldedInstructionCount and update the counter's info
 *
 * Input : counter is a pointer to DWT 32 bit counter which store the information about the specific counter
 * 		   dataRead contains the data read from the counter
 */
void process_32bitCounterData(DWT_32bitCounter *counter,uint32_t dataRead,Enum32bitCounter counterEnum)
{
	static int numberAccess[2] = {0};
	uint32_t temp = 0;
	
	temp = counter->currentValue;
	
	counter->currentValue = dataRead ;
	
	if (numberAccess[counterEnum] > 0)
		counter->previousValue = temp ;

	numberAccess[counterEnum] ++ ;
}

/**
 * Process data read from DWT Comparator and update DWTComparator Info
 *
 * Input : dwtCompInfo is a pointer to DWT Comparator Info which store the information about DWT Comparator,Mask and Function Registers
 *		   dataRead contains the data read from DWT Comparator
 */
void process_DWTComparatorData(DWT_ComparatorInfo *dwtCompInfo,uint32_t dataRead)
{
	dwtCompInfo->data = dataRead ;
}

/**
 * Process data read from DWT Mask and update DWTComparator Info
 *
 * Input : dwtCompInfo is a pointer to DWT Comparator Info which store the information about DWT Comparator,Mask and Function Registers
 *		   dataRead contains the data read from DWT Mask
 */
void process_DWTMaskData(DWT_ComparatorInfo *dwtCompInfo,uint32_t dataRead)
{
	dataRead = dataRead & DWTMASK_MASK;
	dwtCompInfo->ignoreMask = dataRead ;
}

/**
 * Process data read from DWT Function Registers and update DWTFunctionInfo
 *
 * Input : dwtFunctionInfo is a pointer to DWT Function Info which store the information about DWT Function Register
 *		   dataRead contains the data read from DWT Function
 */
void process_DWTFunctionData(DWT_FunctionInfo *dwtFunctionInfo,uint32_t dataRead)
{
	if(check_BitSetWithMask(dataRead,MATCHED_MASK) == ERR_NOERROR)
		dwtFunctionInfo->MATCHED = MATCH ;
	else
		dwtFunctionInfo->MATCHED = NOT_MATCH ;
	
	dwtFunctionInfo->DATAVADDR1 = (dataRead & DATAVADDR1_MASK) >> 16;
	dwtFunctionInfo->DATAVADDR0 = (dataRead & DATAVADDR0_MASK) >> 12;
	dwtFunctionInfo->dataSize = (dataRead & DATAVSIZE_MASK) >> 10;
	dwtFunctionInfo->LNK1ENA = (dataRead & LNK1ENA_MASK) >> 9;
	dwtFunctionInfo->DATAVMATCH = (dataRead &  DATAVMATCH_MASK) >> 8;
	dwtFunctionInfo->CYCMATCH = (dataRead & CYCMATCH_MASK) >> 7;
	dwtFunctionInfo->EMITRANGE = (dataRead & EMITRANGE_MASK) >> 5;
	dwtFunctionInfo->dwtFunction = dataRead & FUNCTION_MASK;
	if(dwtFunctionInfo->EMITRANGE == 1 )
	
	dwtFunctionInfo->dwtFunction = dwtFunctionInfo->dwtFunction * 100;
	
}

/**
 * Use to get the value to be written into DWT Function Register
 *
 * Input : 	firstLinkComp is the first comparator number for linked comparison
 *			secondLinkComp is the second comparator number for linked comparison(if supported)
 *			mode is the comparison mode for the comparator
 *				Possible value :
 *					Address_Comparison 			Perform address comparison
 *					DataValue_Comparaison 		Perform data value comparison
 *					CycleCount_Comparaison		Perform cycle count comparison
 *			
 *			size is the data size for data value matching
 *				Possible value :
 *					Byte 						8 bit comparison
 *					Halfword					16 bit comparison
 *					Word 						32 bit comparison
 *			
 *			function is use to select action taken on comparator match
 *				Possible value :
 *					DWTFunction_Disabled 						Disabled
 *					ITM_SampleEmit_PC 							sample and emit PC through ITM
 *					ITM_Emit_AddressOffset						Emit address offset through ITM 
 *					ITM_Emit_DataRW								Emit data through ITM on read and write
 *					ITM_Emit_Data_AddressOffset_RW 				Emit data and address offset through ITM on read or write
 *					ITM_Sample_PC_DataValue_RW 					Sample PC and data value through ITM on read or write
 *					ITM_Emit_AddressOffset_DataValue_RW 		Emit address offset and data value through ITM on read or write 
 *					Watchpoint_PCMatch  						Watchpoint on PC match
 *					Watchpoint_Read 							Watchpoint on read
 *					Watchpoint_Write 							Watchpoint on write
 *					Watchpoint_ReadWrite 						Watchpoint on read or write
 *					ETM_Trigger_PCMatch 						ETM trigger on PC match
 *					ETM_Trigger_Read 							ETM trigger on read
 *					ETM_Trigger_Write 							ETM trigger on write
 *					ETM_Trigger_ReadWrite 						ETM trigger on read or write
 *					Sample_DataRead 							Sample data for read transfers
 *					Sample_DataAddressOffset_Read				Sample Daddr[15:0] (address offset) for read transfers
 *					Sample_DataWrite 							Sample data for write transfers
 *					Sample_DataAddressOffset_Write				Sample Daddr[15:0] (address offset) for write transfers 
 *					Sample_PC_Data_Read 						Sample PC + data for read transfers
 *					Sample_DataAddressOffset_Data_Read			Sample Daddr[15:0] (address offset) + data value for read transfers 
 *					Sample_PC_Data_Write 						Sample PC + data for write transfers
 *					Sample_DataAddressOffset_Data_Write			Sample Daddr[15:0] (address offset) + data value for write transfers 
 *
 *
 * Output :	return 32 bits of to be written into DWT Function Register
 *
 */
uint32_t get_DWTFunction_WriteValue(int firstLinkComp,int secondLinkComp,ComparisonMode mode,DATAVSIZE size,DWTFunction function)
{
	uint32_t data = 0 ;
	int dataMode = 0 ;
	int emitrange = 0 ;
	
	if(function > 16) // auto enable EMITRANGE bit for certain DWT function
	{
		emitrange = 1 ;
		function = function /100 ;
	}
	
	if(mode == CycleCount_Comparison)
		dataMode = 1 << 7;
	else
		dataMode = mode << 8;
	
	data = (firstLinkComp << 12) + (secondLinkComp << 16 ) + dataMode + (size << 10) + (emitrange << 5) + function;
	
	return data ;
}

/**
 * Use to get the value to be written into DWT Control Register
 *
 * Input : 	eventStatus is a pointer to EventStatus which store the event that going to be enabled in the DWT Control Register
 *			syncTap  is use to select the position of the synchronization packet counter tap on the CYCCNT counter
 *				Possible value :
 *					Tap_Disabled 		Disable
 *					Tap_CYCCNTbit24 	Synchronization counter tap at CYCCNT[24]
 *					Tap_CYCCNTbit26 	Synchronization counter tap at CYCCNT[26]
 *					Tap_CYCCNTbit28 	Synchronization counter tap at CYCCNT[28]
 *
 *			cycTap is use to select the position of the POSTCNT tap on the CYCCNT counter
 *				Possible value :
 *					0 					POSTCNT tap at CYCCNT[6]
 *					1 					POSTCNT tap at CYCCNT[10]
 *			
 *			postCnt is the initial value for the POSTCNT counter
 *			postReset is the reload value for the POSTCNT counter
 *			EnableDisable_CycleCountCounter is use to enable/disable cycle count counter
 * 				Possible value :
 *					Enable 		enable cycle count counter
 *					Disable 	disable cycle count counter
 *
 * Output :	return 32 bits of  to be written into DWT Control Register
 *
 */
uint32_t get_DWTControl_WriteValue(EventStatus *eventStatus,SyncTap syncTap,int cycTap,int posCnt,int postReset,int EnableDisable_CycleCountCounter)
{
	uint32_t data = 0 ;
	
	data = (eventStatus->CycleCountEvent << 22) + (eventStatus->FoldedInstructionCountEvent << 21) + (eventStatus->LSUCountEvent << 20)
		 + (eventStatus->SleepCountEvent << 19) + (eventStatus->ExceptionOverheadCountEvent << 18) + (eventStatus->CPICountEvent << 17)
		 + (eventStatus->InterruptTracingEvent << 16) + (eventStatus->PCSamplingEvent << 12) + (syncTap << 10) + (cycTap << 9) 
		 + (posCnt << 5) + (postReset << 1) + EnableDisable_CycleCountCounter ;
	
	return data ;
}

/**
 * Use to get the number location of DWT ComparatorInfo for the selected register address
 *
 * Input : 	address is the address of DWTCOMPn or DWTMASKn or DWTFUNCn
 *				Possible value :
 *					DWT_COMP0	
 *					DWT_COMP1	
 *					DWT_COMP2	
 *					DWT_COMP3	
 *					
 *				 	DWT_MASK0
 *				 	DWT_MASK1
 *				 	DWT_MASK2
 *				 	DWT_MASK3
 *				 	
 *				 	DWT_FUNC0
 *				 	DWT_FUNC1
 *				 	DWT_FUNC2
 *				 	DWT_FUNC3
 *
 * Output :	return number to the location of ComparatorInfo
 *			return ERR_INVALID_COMPARATOR if comparator out of range
 */
int get_DWTComparatorInfoNumber(uint32_t address)
{	
	if (address == DWT_COMP0 || address == DWT_MASK0 || address == DWT_FUNC0)
		return 0 ;
	
	if (address == DWT_COMP1 || address == DWT_MASK1 || address == DWT_FUNC1)
		return 1 ;
	
	if (address == DWT_COMP2 || address == DWT_MASK2 || address == DWT_FUNC2)
		return 2 ;
	
	if (address == DWT_COMP3 || address == DWT_MASK3 || address == DWT_FUNC3)
		return 3 ;
	
	return ERR_INVALID_DWTREGISTER ;
}


/**
 * Use to set the information of second linked comparator to be same as the first linked comparator
 *
 * Input : 	first_COMPno is the first linked comparator
 *				Possible Value :
 *					DWT_COMP0
 *					DWT_COMP1
 *					DWT_COMP2
 *					DWT_COMP3
 *
 *			firstAddress is the address going to be set into the first comparator
 *			firstMask is the IgnoreMask going to be applied for the first comparator
 *					Possible Value :
 *							Ignore_Non = 0 			Compare all 32 bits of address set in DWT_COMPn 
 *							Ignore_Bit0 			Ignore Bit 0 of address set in DWT_COMPn during comparison
 *							Ignore_Bit1_Bit0		Ignore Bit1 and Bit 0 of address set in DWT_COMPn during comparison
 *							Ignore_Bit2_Bit0		Ignore Bit2 to Bit 0 of address set in DWT_COMPn during comparison
 *							Ignore_Bit3_Bit0		Ignore Bit3 to Bit 0 of address set in DWT_COMPn during comparison
 *							Ignore_Bit4_Bit0		Ignore Bit4 to Bit 0 of address set in DWT_COMPn during comparison
 *							Ignore_Bit5_Bit0						""
 *							Ignore_Bit6_Bit0						""
 *							Ignore_Bit7_Bit0						""
 *							Ignore_Bit8_Bit0						""
 *							Ignore_Bit9_Bit0						""
 *							Ignore_Bit10_Bit0						""
 *							Ignore_Bit11_Bit0						""
 *							Ignore_Bit12_Bit0						""
 *							Ignore_Bit13_Bit0						""
 *							Ignore_Bit14_Bit0		Ignore Bit14 to Bit 0 of address set in DWT_COMPn during comparison
 *
 *		numberCOMP1 is the value obtained from get_DWTComparatorInfoNumber by pasing in first_COMPno
 *		second_COMPno is the pointer to second linked comparator
 *		second_Address is the pointer to address for second comparator
 *		secondMask	is the pointer to ignore mask for second comparator
 *		numberCOMP2 is the pointer to the number location of the second comparator
 */
void equaliseLinkedComparator(uint32_t first_COMPno,uint32_t firstAddress,IgnoreMask firstMask,int numberCOMP1,uint32_t *second_COMPno,uint32_t *secondAddress,IgnoreMask *secondMask,int *numberCOMP2)
{
	*second_COMPno = first_COMPno  ;
	*secondAddress = firstAddress ;
	*secondMask = firstMask ;
	*numberCOMP2 = numberCOMP1 ;
}
