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
	static DWT_32bitCounter cycle,pcSample ;
	static DWT_ComparatorInfo dwtComp[4] ;
	static DWT_FunctionInfo dwtFunction[4];
	
	init_DWTControl(&dwtCtrl);
	init_DWT32bitCounter(&cycle);
	init_DWT8bitCounter(&cpi);
	init_DWT8bitCounter(&exceptionOverhead);
	init_DWT8bitCounter(&sleep);
	init_DWT8bitCounter(&lsu);
	init_DWT8bitCounter(&folderInstruction);
	init_DWT32bitCounter(&pcSample);
	
	init_DWTComparatorInfo(&(dwtComp[0]),&(dwtFunction[0]));
	init_DWTComparatorInfo(&(dwtComp[1]),&(dwtFunction[1]));
	init_DWTComparatorInfo(&(dwtComp[2]),&(dwtFunction[2]));
	init_DWTComparatorInfo(&(dwtComp[3]),&(dwtFunction[3]));
	
	dwtInfo->enableDWT 				= DWT_DISABLE;
	dwtInfo->dwtControl 			= &dwtCtrl;
	
	dwtInfo->CycleCount 			= &cycle;
	dwtInfo->CPICount 				= &cpi;
	dwtInfo->ExceptionOverheadCount = &exceptionOverhead ;
	dwtInfo->SleepCount 			= &sleep;
	dwtInfo->LSUCount 				= &lsu;
	dwtInfo->FoldedInstructionCount = &folderInstruction;
	dwtInfo->PCSample 				= &pcSample;
	
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
	static DWT_EventStatus event ;
	
	init_DWTEventStatus(&event);
	
	dwtControl->numberComparator 			= 4 ;
	dwtControl->support_Tracing 			= 0 ;
	dwtControl->support_ExternalTrigger 	= 0 ;
	dwtControl->support_CycleCount 			= 0 ;
	dwtControl->support_ProfilingCount		= 0 ;
	dwtControl->dwtEventStatus 				= &event;
	dwtControl->syncTap				 		= Tap_Disabled;
	dwtControl->cycTap 						= 0 ;
	dwtControl->postCnt 					= 0;
	dwtControl->postReset 					= 0;
	dwtControl->enableCycleCount		 	= DWT_DISABLE;
}

/**
 * Initialise DWT_EventStatus structure
 *
 * Input : dwtEventStatus is a pointer to DWT_EventStatus which store the information about the status of event enabled in DWT Control Register
 */
void init_DWTEventStatus(DWT_EventStatus *dwtEventStatus)
{
	dwtEventStatus->CycleCountEvent  				= DWT_DISABLE ;
	dwtEventStatus->FoldedInstructionCountEvent  	= DWT_DISABLE ;
	dwtEventStatus->LSUCountEvent  				= DWT_DISABLE ;
	dwtEventStatus->SleepCountEvent  				= DWT_DISABLE ;
	dwtEventStatus->ExceptionOverheadCountEvent  	= DWT_DISABLE ;
	dwtEventStatus->CPICountEvent  				= DWT_DISABLE ;
	dwtEventStatus->InterruptTracingEvent  		= DWT_DISABLE ;
	dwtEventStatus->PCSamplingEvent  				= DWT_DISABLE ;
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

	dwtCompInfo->data				= 0 ;
	dwtCompInfo->addressMask 	 	= WATCHPOINT_MASK_NOTHING ;
	dwtCompInfo->dwtFunctionInfo 	= dwtFunction;
}

/**
 * Initialise DWT Function Info 
 *
 * Input : dwtFunctionInfo is a pointer to DWT Function Info which store the information about DWT Function Register
 */
void init_DWTFunctionInfo(DWT_FunctionInfo *dwtFunctionInfo)
{
	dwtFunctionInfo->comparatorMatched 		= 0 ;
	dwtFunctionInfo->secondLinkCompNo		= 0 ;
	dwtFunctionInfo->firstLinkCompNo		= 0 ;
	dwtFunctionInfo->dataSize				= WATCHPOINT_BYTE ;
	dwtFunctionInfo->secondLinkCompEnable 	= 0 ;
	dwtFunctionInfo->dataMatchingEnable		= 0	;
	dwtFunctionInfo->cycleMatchingEnable 	= 0 ;
	dwtFunctionInfo->enableTracePackets		= 0 ;
	dwtFunctionInfo->dwtFunction			= DWTComparator_Disabled ;
}

/**
 * Configure the event going to be enabled or disabled inside DWT Control Register
 *
 * Input :  dwtEventStatus is a pointer to DWT_EventStatus which store the event that going to be enabled in the DWT Control Register
 *			cycleCountEnable			Enable/Disable generation of Cycle count event
 *				Possible Value :
 *					DWT_ENABLE
 *					DWT_DISABLE
 *			folderInstructionEnable		Enable/Disable generation of Folded Instruction count event
 *				Possible Value :
 *					DWT_ENABLE
 *					DWT_DISABLE
 *			LSUEnable					Enable/Disable generation of LSU count event
 *				Possible Value :
 *					DWT_ENABLE
 *					DWT_DISABLE
 *			sleepEnable					Enable/Disable generation of Sleep count event
 *				Possible Value :
 *					DWT_ENABLE
 *					DWT_DISABLE
 *			exceptionOverheadEnable		Enable/Disable generation of Interrupt Overhead event
 *				Possible Value :
 *					DWT_ENABLE
 *					DWT_DISABLE
 *			CPIEnable					Enable/Disable generation of CPI count event
 *				Possible Value :
 *					DWT_ENABLE
 *					DWT_DISABLE
 *			interruptTracingEnable		Enable/Disable generation of Interrupt event tracing
 *				Possible Value :
 *					DWT_ENABLE
 *					DWT_DISABLE
 *			PCSamplingEnable			Enable/Disable generation of PC Sampling event
 *				Possible Value :
 *					DWT_ENABLE
 *					DWT_DISABLE
 *			
 */
void configure_DWTEventGeneration(DWT_EventStatus *dwtEventStatus,int cycleCountEnable,int folderInstructionEnable,int LSUEnable,int sleepEnable,int exceptionOverheadEnable,int CPIEnable,int interruptTracingEnable,int PCSamplingEnable)
{
  dwtEventStatus->CycleCountEvent 				= cycleCountEnable;
  dwtEventStatus->FoldedInstructionCountEvent	= folderInstructionEnable;
  dwtEventStatus->LSUCountEvent 				= LSUEnable;
  dwtEventStatus->SleepCountEvent 				= sleepEnable;
  dwtEventStatus->ExceptionOverheadCountEvent	= exceptionOverheadEnable;
  dwtEventStatus->CPICountEvent					= CPIEnable;
  dwtEventStatus->InterruptTracingEvent 		= interruptTracingEnable;
  dwtEventStatus->PCSamplingEvent 				= PCSamplingEnable;
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
	
	process_DWTEventStatusData(dwtControl->dwtEventStatus,dataRead);
	
	dwtControl->syncTap = (dataRead & SYNCTAP_MASK) >> 10;
	dwtControl->cycTap = (dataRead & CYCTAP_MASK) >> 9;
	dwtControl->postCnt = (dataRead & POSTINIT_MASK) >> 5; 
	dwtControl->postReset = (dataRead & POSTRESET_MASK) >> 1;
	
	if ( check_BitSetWithMask(dataRead,CYCCNTENA_MASK) == ERR_NOERROR )
		dwtControl->enableCycleCount = DWT_ENABLE ; 
	else
		dwtControl->enableCycleCount = DWT_DISABLE ; 

}

/**
 * Process data read from DWT Control register and update DWT_EventStatus
 *
 * Input : dwtControl is a pointer to DWTControl which store the information about the status of DWT Control register
 * 		   dataRead contains the data read from DWT Control register 
 */
void process_DWTEventStatusData(DWT_EventStatus *dwtEventStatus,uint32_t dataRead)
{
	
	if ( check_BitSetWithMask(dataRead,CYCEVTENA_MASK) == ERR_NOERROR)
		dwtEventStatus->CycleCountEvent = DWT_ENABLE ;
	else
		dwtEventStatus->CycleCountEvent = DWT_DISABLE ;
	
	if ( check_BitSetWithMask(dataRead,FOLDEVTENA_MASK) == ERR_NOERROR)
		dwtEventStatus->FoldedInstructionCountEvent = DWT_ENABLE ;
	else
		dwtEventStatus->FoldedInstructionCountEvent = DWT_DISABLE;
	
	if ( check_BitSetWithMask(dataRead,LSUEVTENA_MASK) == ERR_NOERROR)
		dwtEventStatus->LSUCountEvent = DWT_ENABLE ;
	else
		dwtEventStatus->LSUCountEvent = DWT_DISABLE ;
	
	if ( check_BitSetWithMask(dataRead,SLEEPEVTENA_MASK) == ERR_NOERROR)
		dwtEventStatus->SleepCountEvent = DWT_ENABLE ;
	else
		dwtEventStatus->SleepCountEvent = DWT_DISABLE ;
	
	if ( check_BitSetWithMask(dataRead,EXCEVTENA_MASK) == ERR_NOERROR)
		dwtEventStatus->ExceptionOverheadCountEvent = DWT_ENABLE ;
	else
		dwtEventStatus->ExceptionOverheadCountEvent = DWT_DISABLE ;
	
	if ( check_BitSetWithMask(dataRead,CPIEVTENA_MASK) == ERR_NOERROR)
		dwtEventStatus->CPICountEvent = DWT_ENABLE ;
	else
		dwtEventStatus->CPICountEvent = DWT_DISABLE ;
	
	if ( check_BitSetWithMask(dataRead,EXCTRCENA_MASK) == ERR_NOERROR)
		dwtEventStatus->InterruptTracingEvent = DWT_ENABLE ;
	else
		dwtEventStatus->InterruptTracingEvent = DWT_DISABLE ;

	if ( check_BitSetWithMask(dataRead,PCSAMPLENA_MASK) == ERR_NOERROR)
		dwtEventStatus->PCSamplingEvent= DWT_ENABLE ;
	else
		dwtEventStatus->PCSamplingEvent = DWT_DISABLE ;
	
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
	dwtCompInfo->addressMask = dataRead & DWTMASK_MASK;
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
		dwtFunctionInfo->comparatorMatched = DWT_MATCH ;
	else
		dwtFunctionInfo->comparatorMatched = DWT_NOTMATCH ;
	
	dwtFunctionInfo->secondLinkCompNo = (dataRead & DATAVADDR1_MASK) >> 16;
	dwtFunctionInfo->firstLinkCompNo = (dataRead & DATAVADDR0_MASK) >> 12;
	dwtFunctionInfo->dataSize = (dataRead & DATAVSIZE_MASK) >> 10;
	dwtFunctionInfo->secondLinkCompEnable = (dataRead & LNK1ENA_MASK) >> 9;
	dwtFunctionInfo->dataMatchingEnable = (dataRead &  DATAVMATCH_MASK) >> 8;
	dwtFunctionInfo->cycleMatchingEnable = (dataRead & CYCMATCH_MASK) >> 7;
	dwtFunctionInfo->enableTracePackets = (dataRead & EMITRANGE_MASK) >> 5;
	dwtFunctionInfo->dwtFunction = dataRead & FUNCTION_MASK;
	
	if(dwtFunctionInfo->enableTracePackets == 1 )
		dwtFunctionInfo->dwtFunction = dwtFunctionInfo->dwtFunction * 100;
	
}

/**
 * Use to get the value to be written into DWT Function Register
 *
 * Input : 	firstLinkCompNo is the first comparator number for linked comparison
 *			secondLinkCompNo is the second comparator number for linked comparison
 *			mode is the comparison mode for the comparator
 *				Possible value :
 *					ADDRESS_COMPARISON 			Perform address comparison
 *					DATA_COMPARISON 			Perform data value comparison
 *					CYCLECOUNT_COMPARISON		Perform cycle count comparison
 *			
 *			size is the data size for data value matching
 *				Possible value :
 *					WATCHPOINT_BYTE 			8 bit comparison
 *					WATCHPOINT_HALFWORD			16 bit comparison
 *					WATCHPOINT_WORD 			32 bit comparison
 *			
 *			function is use to select action taken on comparator match
 *				Possible value :
 *					DWTComparator_Disabled 						Disabled
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
uint32_t get_DWTFunction_WriteValue(int firstLinkCompNo,int secondLinkCompNo,Watchpoint_ComparisonMode mode,Watchpoint_DataSize size,DWTFunction function)
{
	uint32_t data = 0 ;
	int dataMode = 0 ;
	int emitrange = 0 ;
	
	if(function > 16) // auto enable EMITRANGE bit for certain DWT function
	{
		emitrange = 1 ;
		function = function /100 ;
	}
	
	if(mode == CYCLECOUNT_COMPARISON)
		dataMode = 1 << 7;
	else
		dataMode = mode << 8;
	
	data = (firstLinkCompNo << 12) + (secondLinkCompNo << 16 ) + dataMode + (size << 10) + (emitrange << 5) + function;
	
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
 *			enableCycleCount is use to enable/disable cycle count counter
 * 				Possible value :
 *					DWT_ENABLE 		enable cycle count counter
 *					DWT_DISABLE 	disable cycle count counter
 *
 * Output :	return 32 bits of  to be written into DWT Control Register
 *
 */
uint32_t get_DWTControl_WriteValue(DWT_EventStatus *eventStatus,SyncTap syncTap,int cycTap,int posCnt,int postReset,int enableCycleCount)
{
	uint32_t data = 0 ;
	
	data = (eventStatus->CycleCountEvent << 22) + (eventStatus->FoldedInstructionCountEvent << 21) + (eventStatus->LSUCountEvent << 20)
		 + (eventStatus->SleepCountEvent << 19) + (eventStatus->ExceptionOverheadCountEvent << 18) + (eventStatus->CPICountEvent << 17)
		 + (eventStatus->InterruptTracingEvent << 16) + (eventStatus->PCSamplingEvent << 12) + (syncTap << 10) + (cycTap << 9) 
		 + (posCnt << 5) + (postReset << 1) + enableCycleCount ;
	
	return data ;
}

/**
 * Check if the Data Watchpoint & Trace Unit is enabled/disabled
 *
 * Input : 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *			EnableDisable is used to determine whether the Data Watchpoint & Trace Unit is in the selected mode
 *				Possible value :
 *					DWT_ENABLE		check if Data Watchpoint & Trace Unit is enabled
 *					DWT_DISABLE		check if Data Watchpoint & Trace Unit is disable
 *
 * Output :	return ERR_NOERROR if Data Watchpoint & Trace Unit is enabled
 *			return ERR_DWT_NOTENABLED if Data Watchpoint & Trace Unit is not enabled
 *			return ERR_DWT_NOTDISABLED if Data Watchpoint & Trace Unit is not disabled
 *
 */
int isDWT_EnabledDisabled(DWTInfo *dwtInfo,int enable)
{
	if (enable)
	{
		if ( check_BitSetWithMask(dwtInfo->enableDWT,DWT_ENABLED_MASK) == ERR_NOERROR)
			return ERR_NOERROR ;
		else
			return ERR_DWT_NOTENABLED ;
	}
	else
	{
		if ( check_BitSetWithMask(dwtInfo->enableDWT,DWT_ENABLED_MASK) != ERR_NOERROR)
			return ERR_NOERROR ;
		else
			return ERR_DWT_NOTDISABLED ;
	}
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
 *			firstMask is the Watchpoint_AddressMask going to be applied for the first comparator
 *					Possible Value :
 *							WATCHPOINT_MASK_NOTHING  		Compare all 32 bits of address set in DWT_COMPn 
 *							WATCHPOINTMASK_BIT0 			Ignore Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT1_BIT0		Ignore Bit1 and Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT2_BIT0		Ignore Bit2 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT3_BIT0		Ignore Bit3 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT4_BIT0		Ignore Bit4 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT5_BIT0						""
 *							WATCHPOINT_MASK_BIT6_BIT0						""
 *							WATCHPOINT_MASK_BIT7_BIT0						""
 *							WATCHPOINT_MASK_BIT8_BIT0						""
 *							WATCHPOINT_MASK_BIT9_BIT0						""
 *							WATCHPOINT_MASK_BIT10_BIT0						""
 *							WATCHPOINT_MASK_BIT11_BIT0						""
 *							WATCHPOINT_MASK_BIT12_BIT0						""
 *							WATCHPOINT_MASK_BIT13_BIT0						""
 *							WATCHPOINT_MASK_BIT14_BIT0		Ignore Bit14 to Bit 0 of address set in DWT_COMPn during comparison
 *
 *		numberCOMP1 is the value obtained from get_DWTComparatorInfoNumber by pasing in first_COMPno
 *		second_COMPno is the pointer to second linked comparator
 *		second_Address is the pointer to address for second comparator
 *		secondMask	is the pointer to ignore mask for second comparator
 *		numberCOMP2 is the pointer to the number location of the second comparator
 */
void equaliseLinkedComparator(uint32_t first_COMPno,uint32_t firstAddress,Watchpoint_AddressMask firstMask,int numberCOMP1,uint32_t *second_COMPno,uint32_t *secondAddress,Watchpoint_AddressMask *secondMask,int *numberCOMP2)
{
	*second_COMPno = first_COMPno  ;
	*secondAddress = firstAddress ;
	*secondMask = firstMask ;
	*numberCOMP2 = numberCOMP1 ;
}


/**
 * Enable/Disable Data Watchpoint and Trace unit
 *
 * Input :	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *			debugExceptionMonitor is a pointer to DebugExceptionAndMonitor which store information about Debug Exception and Monitor Control Register, DEMCR
 *			debugTrap is a pointer to DebugTrap which store whether the vector catch is enabled/disabled for example debug trap on HARDFAULT VC_HARDERR
 *			debugMonitorControl is used to control the behaviour of Debug Monitor in ARM
 *				Possible input value :
 *					DEBUGMONITOR_DISABLED  	Disable debug monitor
 *					DEBUGMONITOR_ENABLED	Enable debug monitor	
 *					DEBUGMONITOR_STEP		Enable stepping in debug monitor
 *			EnableDisable EnableDisable is use to enable/disable the Data Watchpoint and Trace unit 
 *				Possible values :
 *					DWT_ENABLE			enable DWT unit
 *					DWT_DISABLE			disable DWT unit
 *
 * Output :	return ERR_NOERROR if Data Watchpoint and Trace unit is enabled
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *			return ERR_DWT_NOTENABLED if Data Watchpoint and Trace unit is not enabled
 *			return ERR_DWT_NOTDISABLE if Data Watchpoint and Trace unit is not disabled 
 */
int control_DWT(DWTInfo *dwtInfo,DebugExceptionMonitor *debugExceptionMonitor,DebugTrap *debugTrap,DebugMonitorControl debugMonitorControl,int enable)
{
	int status = 0 ;
	
	status = configure_DebugExceptionMonitorControl(debugExceptionMonitor,debugMonitorControl,debugTrap,enable);
	if (status != ERR_NOERROR)
		return status;
	
	dwtInfo->enableDWT = debugExceptionMonitor->enableDWT_ITM ;

	status = isDWT_EnabledDisabled(dwtInfo,enable);
	
	return status;
}

/**
 * Read DWT Control register and store the data read into dwtInfo
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 * Output 	:	return ERR_NOERROR if the read of DWT Control Register has been done successfully
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *
 */
int read_DWTControl(DWTInfo *dwtInfo)
{
	uint32_t dataRead = 0;
	int status = 0 ;
	
	status = memoryAccessRead(DWT_CTRL,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_DWTControlData(dwtInfo->dwtControl,dataRead);
	
	return ERR_NOERROR;
}

/**
 * Read selected DWT Comparator register and store the data read into respective dwtcompInfo in dwtInfo
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_COMPno is the comparator number chosen to be read 
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP1
 *						DWT_COMP2
 *						DWT_COMP3
 *
 * Output 	:	return ERR_NOERROR if the read of DWT_COMPn register has been done successfully
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 *
 */
int read_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno)
{
	uint32_t dataRead = 0;
	int status = 0,number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_COMPno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	status = memoryAccessRead(DWT_COMPno,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_DWTComparatorData(dwtInfo->dwtCompInfo[number],dataRead);
	
	return ERR_NOERROR;
}

/**
 * Read selected DWT Mask register and store the data read into respective dwtcompInfo in dwtInfo
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_Maskno is the number of mask register chosen to be read 
 *					Possible Value :
 *						DWT_MASK0
 *						DWT_MASK1
 *						DWT_MASK2
 *						DWT_MASK3 
 *
 * Output 	:	return ERR_NOERROR if the read of DWT_MASKn register has been done successfully		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen mask register is not exist/valid
 *
 */
int read_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno)
{
	uint32_t dataRead = 0;
	int status = 0,number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_MASKno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	status = memoryAccessRead(DWT_MASKno,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_DWTMaskData(dwtInfo->dwtCompInfo[number],dataRead);
	
	return ERR_NOERROR;
}

/**
 *  Read selected DWT Function register and store the data read into respective dwtFunction in dwtcompInfo of dwtInfo
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_FUNCno is the number of function register chosen to be read 
 *					Possible Value :
 *						DWT_FUNC0
 *						DWT_FUNC1
 *						DWT_FUNC2
 *						DWT_FUNC3 
 *
 * Output 	:	return ERR_NOERROR if the read of DWT_FUNCn register has been done successfully		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen function register is not exist/valid
 *
 */
int read_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno)
{
	uint32_t dataRead = 0;
	int status = 0,number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_FUNCno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	status = memoryAccessRead(DWT_FUNCno,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_DWTFunctionData(dwtInfo->dwtCompInfo[number]->dwtFunctionInfo,dataRead);
	
	return ERR_NOERROR;
}



/**
 * Configure the selected DWT Comparator register and store the data read into respective dwtcompInfo in dwtInfo
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_COMPno is the comparator number chosen to be read 
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP1
 *						DWT_COMP2
 *						DWT_COMP3
 *				value is the address/data value going to be written into the comparator depending on the comparison mode
 *
 * Output 	:	return ERR_NOERROR if the configuration of DWT_COMPn has been completed successfully	
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_DATARW_NOT_MATCH if the data written into DWT_COMPn is different from data read from DWT_COMPn
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 *
 */
int configure_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t value)
{
	int status = 0 ;
	int number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_COMPno); 
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	memoryAccessWrite(DWT_COMPno,value);
	status = read_DWTComparator(dwtInfo,DWT_COMPno);
	
	if (status != ERR_NOERROR)
		return status ;
	
	status = check_dataWrote_dataRead(value,dwtInfo->dwtCompInfo[number]->data);
	
	return status;
}

/**
 * Configure the selected DWT Mask register and store the data read into respective dwtcompInfo in dwtInfo
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_Maskno is the number of mask register chosen to be read 
 *					Possible Value :
 *						DWT_MASK0
 *						DWT_MASK1
 *						DWT_MASK2
 *						DWT_MASK3 
 *				addressMask is the mask going to be applied to the address when during address comparison
 *					Possible Value :
 *							WATCHPOINT_MASK_NOTHING  		Compare all 32 bits of address set in DWT_COMPn 
 *							WATCHPOINTMASK_BIT0 			Ignore Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT1_BIT0		Ignore Bit1 and Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT2_BIT0		Ignore Bit2 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT3_BIT0		Ignore Bit3 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT4_BIT0		Ignore Bit4 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT5_BIT0						""
 *							WATCHPOINT_MASK_BIT6_BIT0						""
 *							WATCHPOINT_MASK_BIT7_BIT0						""
 *							WATCHPOINT_MASK_BIT8_BIT0						""
 *							WATCHPOINT_MASK_BIT9_BIT0						""
 *							WATCHPOINT_MASK_BIT10_BIT0						""
 *							WATCHPOINT_MASK_BIT11_BIT0						""
 *							WATCHPOINT_MASK_BIT12_BIT0						""
 *							WATCHPOINT_MASK_BIT13_BIT0						""
 *							WATCHPOINT_MASK_BIT14_BIT0		Ignore Bit14 to Bit 0 of address set in DWT_COMPn during comparison
 *
 * Output 	:	return ERR_NOERROR if the configuration of DWT_MASKn has been completed successfully		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 *
 */
int configure_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno,Watchpoint_AddressMask addressMask)
{
	int status = 0, number  = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_MASKno); 
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	memoryAccessWrite(DWT_MASKno,addressMask);
	status = read_DWTMask(dwtInfo,DWT_MASKno);
	
	return status;
}

/**
 * Configure the selected DWT Function register and store the data read into respective dwtFunction in dwtcompInfo of dwtInfo
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_FUNCno is the number of function register chosen to be read 
 *					Possible Value :
 *						DWT_FUNC0
 *						DWT_FUNC1
 *						DWT_FUNC2
 *						DWT_FUNC3 
 *
 *				firstLinkCompNo is the number of comparator to be used for linked comparison
 *				secondLinkCompNo is the second number of comparator to be used for linked comparison (if supported)
 *
 *				mode is the comparison mode for the comparator
 *					Possible value :
 *						ADDRESS_COMPARISON 			Perform address comparison
 *						DATA_COMPARISON 			Perform data value comparison
 *						CYCLECOUNT_COMPARISON		Perform cycle count comparison
 *
 *				size is the data size for data value matching
 *					Possible value :
 *						WATCHPOINT_BYTE 			8 bit comparison
 *						WATCHPOINT_HALFWORD			16 bit comparison
 *						WATCHPOINT_WORD 			32 bit comparison
 *				
 *				function is use to select action taken on comparator match
 *					Possible value :
 *						DWTComparator_Disabled 						Disabled
 *						ITM_SampleEmit_PC 							sample and emit PC through ITM
 *						ITM_Emit_AddressOffset						Emit address offset through ITM 
 *						ITM_Emit_DataRW								Emit data through ITM on read and write
 *						ITM_Emit_Data_AddressOffset_RW 				Emit data and address offset through ITM on read or write
 *						ITM_Sample_PC_DataValue_RW 					Sample PC and data value through ITM on read or write
 *						ITM_Emit_AddressOffset_DataValue_RW 		Emit address offset and data value through ITM on read or write 
 *						Watchpoint_PCMatch  						Watchpoint on PC match
 *						Watchpoint_Read 							Watchpoint on read
 *						Watchpoint_Write 							Watchpoint on write
 *						Watchpoint_ReadWrite 						Watchpoint on read or write
 *						ETM_Trigger_PCMatch 						ETM trigger on PC match
 *						ETM_Trigger_Read 							ETM trigger on read
 *						ETM_Trigger_Write 							ETM trigger on write
 *						ETM_Trigger_ReadWrite 						ETM trigger on read or write
 *						Sample_DataRead 							Sample data for read transfers
 *						Sample_DataAddressOffset_Read				Sample Daddr[15:0] (address offset) for read transfers
 *						Sample_DataWrite 							Sample data for write transfers
 *						Sample_DataAddressOffset_Write				Sample Daddr[15:0] (address offset) for write transfers 
 *						Sample_PC_Data_Read 						Sample PC + data for read transfers
 *						Sample_DataAddressOffset_Data_Read			Sample Daddr[15:0] (address offset) + data value for read transfers 
 *						Sample_PC_Data_Write 						Sample PC + data for write transfers
 *						Sample_DataAddressOffset_Data_Write			Sample Daddr[15:0] (address offset) + data value for write transfers 
 *
 * Output 	:	return ERR_NOERROR if configuration of DWT_FUNCn has been completed successfully		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 *
 */
int configure_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno,int firstLinkCompNo,int secondLinkCompNo,Watchpoint_ComparisonMode mode,Watchpoint_DataSize size,DWTFunction function)
{
	uint32_t dataToWrite = 0 ;
	int status = 0, number = 0;
	
	number = get_DWTComparatorInfoNumber(DWT_FUNCno); 
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	dataToWrite = get_DWTFunction_WriteValue(firstLinkCompNo,secondLinkCompNo,mode,size,function);

	memoryAccessWrite(DWT_FUNCno,dataToWrite);
	status= read_DWTFunction(dwtInfo,DWT_FUNCno);
	
	return status ;
}

/**
 *	Disable the selected comparator
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_COMPno is the comparator number chosen to be disabled
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP1
 *						DWT_COMP2
 *						DWT_COMP3
 *
 * Output 	:	return ERR_NOERROR if the comparator has been disabled 	
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 */
int disable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno)
{
	int status = 0 ,number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_COMPno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
		
	status = configure_DWTFunction(dwtInfo,DWT_COMPno + 0x8,0,0,0,0,DWTComparator_Disabled);
	if (status != ERR_NOERROR)
		return status ;
	
	status = check_dataWrote_dataRead(0,dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->dwtFunction);
	
	return status ;
}

/**
 * Setup DWT operations by enabling the Global Enable for DWT Unit and setting the processor to CORE_DEBUG_MODE or using DebugMonitor
 * If debug monitor is not used, the processor will be automatically set to CORE_DEBUG_MODE
 * 
 * Input : 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *			debugExceptionMonitor is a pointer to DebugExceptionAndMonitor which store information about Debug Exception and Monitor Control Register, DEMCR
 *			debugMonitorControl is used to control the behaviour of Debug Monitor in ARM
 *				Possible input value :
 *					DEBUGMONITOR_DISABLE  	Disable debug monitor
 *					DEBUGMONITOR_ENABLE		Enable debug monitor	
 *
 * Output : return ERR_NOERROR if the prepartions has been completed successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *			return ERR_DWT_NOTENABLED if Data Watchpoint and Trace unit is not enabled
 *			return ERR_CORE_CONTROL_FAILED if the core does not switch to CORE_DEBUG_MODE 
 */
int setup_DWTOperations(DWTInfo *dwtInfo,CoreStatus *coreStatus,DebugExceptionMonitor *debugExceptionMonitor,DebugMonitorControl debugMonitorControl)
{
	int status = 0 ;
	
	status = control_DWT(dwtInfo,debugExceptionMonitor,debugExceptionMonitor->debugTrap,debugMonitorControl,DWT_ENABLE);
	if (status != ERR_NOERROR)
		return status ;
	
	if (debugMonitorControl == DEBUGMONITOR_DISABLE)
	{
		if(isCore(CORE_DEBUG_MODE,coreStatus) == ERR_CORECONTROL_FAILED)
			status = setCore(CORE_DEBUG_MODE,coreStatus);
	}
	return status;
}

/**
 * Cleanup DWT operations by disable global enable for DWT and disable Debug Monitor and set core to CORE_NORMAL_MODE
 *
 * Input : 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *			debugExceptionMonitor is a pointer to DebugExceptionAndMonitor which store information about Debug Exception and Monitor Control Register, DEMCR	
 *
 * Output : return ERR_NOERROR if the prepartions has been completed successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *			return ERR_DWT_NOTENABLED if Data Watchpoint and Trace unit is not enabled
 *			return ERR_CORE_CONTROL_FAILED if the core does not switch to CORE_DEBUG_MODE 
 */
int cleanup_DWTOperations(DWTInfo *dwtInfo,CoreStatus *coreStatus,DebugExceptionMonitor *debugExceptionMonitor)
{
	int status = 0 ;
	status = control_DWT(dwtInfo,debugExceptionMonitor,debugExceptionMonitor->debugTrap,DEBUGMONITOR_DISABLE,DWT_DISABLE);
	
	if (status != ERR_NOERROR)
		return status ;
	
	status = setCore(CORE_NORMAL_MODE,coreStatus);
	
	return status ;
}