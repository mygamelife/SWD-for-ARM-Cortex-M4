#include "DWT_Unit.h"

/**
 * Read DWT Control register and store the data read into dwtInfo
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
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
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
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
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
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
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
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
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 *
 */
int configure_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t value)
{
	int status = 0 ;

	memoryAccessWrite(DWT_COMPno,value);
	status = read_DWTComparator(dwtInfo,DWT_COMPno);
	
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
 *				ignoreMask is the IgnoreMask going to be applied when during address comparison
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
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 *
 */
int configure_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno,IgnoreMask ignoreMask)
{
	int status = 0 ;
	memoryAccessWrite(DWT_MASKno,ignoreMask);
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
 *				firstLinkComp is the number of comparator to be used for linked comparison
 *				secondLinkComp is the second number of comparator to be used for linked comparison (if supported)
 *
 *				mode is the comparison mode for the comparator
 *					Possible value :
 *						Address_Comparison 			Perform address comparison
 *						DataValue_Comparaison 		Perform data value comparison
 *						CycleCount_Comparaison		Perform cycle count comparison
 *
 *				size is the data size for data value matching
 *					Possible value :
 *						Byte 						8 bit comparison
 *						Halfword					16 bit comparison
 *						Word 						32 bit comparison
 *				
 *				function is use to select action taken on comparator match
 *					Possible value :
 *						DWTFunction_Disabled 						Disabled
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
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 *
 */
int configure_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno,int firstLinkComp,int secondLinkComp,ComparisonMode mode,DATAVSIZE size,DWTFunction function)
{
	uint32_t dataToWrite = 0 ;
	int status = 0;
	
	dataToWrite = get_DWTFunction_WriteValue(firstLinkComp,secondLinkComp,mode,size,function);

	memoryAccessWrite(DWT_FUNCno,dataToWrite);
	status= read_DWTFunction(dwtInfo,DWT_FUNCno);
	
	return status ;
}

/**
 *	Disable the selected comparator and retain the settings set 
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_COMPno is the comparator number chosen to be disabled
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP1
 *						DWT_COMP2
 *						DWT_COMP3
 *
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 */
int disable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno)
{
	int status = 0 ,number = 0 ;
	int firstLinkComp = 0, secondLinkComp = 0 ;
	ComparisonMode mode = 0 ;
	DATAVSIZE size  = 0;
	
	number = get_DWTComparatorInfoNumber(DWT_COMPno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	secondLinkComp =dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->DATAVADDR1 ;
	firstLinkComp = dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->DATAVADDR0 ;
	size = dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->dataSize ;
	
	if (dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->CYCMATCH == 1)
		mode = CycleCount_Comparison ;
	else
		mode = dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->DATAVMATCH ;
	
	status = configure_DWTFunction(dwtInfo,DWT_COMPno + 0x8,firstLinkComp,secondLinkComp,mode,size,DWTFunction_Disabled);


	return status ;
}

/**
 *	Reenable the selected comparator with the user pass in function parameter and retain the others settings set 
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_COMPno is the comparator number chosen to be enabled
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP1
 *						DWT_COMP2
 *						DWT_COMP3
 *				function is use to select action taken on comparator match
 *					Possible value :
 *						DWTFunction_Disabled 						Disabled
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
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 */
int reenable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno,DWTFunction function)
{
	int status = 0 ,number = 0 ;
	int firstLinkComp = 0, secondLinkComp = 0 ,emitrange = 0 ; 
	ComparisonMode mode = 0 ;
	DATAVSIZE size  = 0;
	
	number = get_DWTComparatorInfoNumber(DWT_COMPno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	secondLinkComp =dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->DATAVADDR1 ;
	firstLinkComp = dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->DATAVADDR0 ;
	size = dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->dataSize ;
	emitrange = dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->EMITRANGE ;
	
	if(emitrange)
		function  = function /100 ;
	
	if (dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->CYCMATCH == 1)
		mode = CycleCount_Comparison ;
	else
		mode = dwtInfo->dwtCompInfo[number]->dwtFunctionInfo->DATAVMATCH ;
	
	status = configure_DWTFunction(dwtInfo,DWT_COMPno + 0x8,firstLinkComp,secondLinkComp,mode,size,function);

	return status ;
}

/**
 *	Set watchpoint for instruction address matching
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_COMPno is the comparator number chosen to be disabled
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP1
 *						DWT_COMP2
 *						DWT_COMP3
 *				address is the instruction address going to be compared
 *				ignoreMask is the IgnoreMask going to be applied when during address comparison
 *					Possible Value :
 *							Ignore_Non = 0 			Compare all 32 bits of address set in DWT_COMPn (the address must be halfword aligned if this mask is selected)
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
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 */
int setWatchpoint_PC(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t address,IgnoreMask ignoreMask)
{
	int status = 0 ;
	uint32_t DWT_FUNCno = DWT_COMPno+8 ;
	uint32_t DWT_MASKno = DWT_COMPno+4 ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNCno,0,0,0,0,DWTFunction_Disabled); //disable the comparator first
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTComparator(dwtInfo,DWT_COMPno,address);
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTMask(dwtInfo,DWT_MASKno,ignoreMask);
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNCno,0,0,Address_Comparison,0,Watchpoint_PCMatch);
	
	return status ;
}


/**
 *	Set watchpoint for data address matching
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				DWT_COMPno is the comparator number chosen to be disabled
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP1
 *						DWT_COMP2
 *						DWT_COMP3
 *				address is the data address going to be compared
 *				ignoreMask is the IgnoreMask going to be applied when during address comparison
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
 *				mode is use to define the watchpoint mode for selected access 
 *					Possible Value :
 *							Comparator_Disable 		Disable comparator
 *							Read					Watchpoint on read access
 *							Write					Watchpoint on write access
 *							ReadWrite 				Watchpoint on read & write access
 *
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 */
int setWatchpoint_DataAddr(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t address,IgnoreMask ignoreMask,RWmode mode)
{
	int status = 0 ;
	uint32_t DWT_FUNCno = DWT_COMPno+8 ;
	uint32_t DWT_MASKno = DWT_COMPno+4 ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNCno,0,0,0,0,DWTFunction_Disabled); //disable the comparator first
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTComparator(dwtInfo,DWT_COMPno,address);
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTMask(dwtInfo,DWT_MASKno,ignoreMask);
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNCno,0,0,Address_Comparison,0,mode);
	
	return status ;
}

/**
 *	Set watchpoint for data value matching
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				value is the data value going to be compared
 *				size is the data size for data value matching
 *					Possible value :
 *						Byte 						8 bit comparison
 *						Halfword					16 bit comparison
 *						Word 						32 bit comparison 
 * 
 *				mode is use to define the watchpoint mode for selected access 
 *					Possible Value :
 *							Comparator_Disable		Disable comparator
 *							Read					Watchpoint on read access
 *							Write					Watchpoint on write access
 *							ReadWrite 				Watchpoint on read & write access
 *
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 */
int setWatchpoint_DataValue(DWTInfo *dwtInfo,uint32_t value,DATAVSIZE size,RWmode mode)
{
	int status = 0 ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNC1,0,0,0,0,DWTFunction_Disabled); //disable the comparator first
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTComparator(dwtInfo,DWT_COMP1,value);
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNC1,0,0,DataValue_Comparison,size,mode);
	
	return status ;
}

/**
 *	Set watchpoint for data value matching with aid of linked comparator to perform address matching as well
 *
 * Input 	: 	dwtInfo is a pointer to DWTInfo which store the information about the status of the whole DataWatchpoint and Trace Unit
 *				value is the data value going to be compared
 *				size is the data size for data value matching
 *					Possible value :
 *						Byte 						8 bit comparison
 *						Halfword					16 bit comparison
 *						Word 						32 bit comparison 
 * 
 *				mode is use to define the watchpoint mode for selected access 
 *					Possible Value :
 *							Comparator_Disable		Disable comparator
 *							Read					Watchpoint on read access
 *							Write					Watchpoint on write access
 *							ReadWrite 				Watchpoint on read & write access
 *
 *				first_COMPno is the first linked comparator 
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP2
 *						DWT_COMP3
 *
 *				firstAddress is the address to be set into first linked comparator
 *				firstMask is the ignore mask to be applied for firstAddress
 *
 *				second_COMPno is the second linked comparator 
 *					Possible Value :
 *						DWT_COMP0
 *						DWT_COMP2
 *						DWT_COMP3
 *						Disable						Second linked comparator is not used
 *
 *				secondAddress is the address to be set into second linked comparator (Set to 0 if not used)
 *				secondMask is the ignore mask to be applied for secondAddress (Set to 0 if not used)
 *
 * Output 	:	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled		
 *				return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *				return ERR_INVALID_DWTREGISTER if the chosen comparator is not exist/valid
 */
int setWatchpoint_LinkedComparison(DWTInfo *dwtInfo,uint32_t value,DATAVSIZE size,uint32_t first_COMPno,uint32_t firstAddress,IgnoreMask firstMask,uint32_t second_COMPno,uint32_t secondAddress,IgnoreMask secondMask,RWmode mode)
{
	int status = 0 ;
	int numberCOMP1,numberCOMP2 ;
	
	numberCOMP1 = get_DWTComparatorInfoNumber(first_COMPno);
	numberCOMP2 = get_DWTComparatorInfoNumber(second_COMPno);
	
	if(numberCOMP1 == ERR_INVALID_DWTREGISTER)
		return numberCOMP1 ;
	
	if(numberCOMP2 == ERR_INVALID_DWTREGISTER) //case when second comparator is not selected for linked comparison
		equaliseLinkedComparator(first_COMPno,firstAddress,firstMask,numberCOMP1,&second_COMPno,&secondAddress,&secondMask,&numberCOMP2);
	
	status = setWatchpoint_DataAddr(dwtInfo,first_COMPno,firstAddress,firstMask,Comparator_Disable); 
	if(status != ERR_NOERROR)
		return status ;
	
	if(numberCOMP2 != numberCOMP1) // use to fix of reprogramming the same comparator when second comparator is not selected for linked comparison
		status = setWatchpoint_DataAddr(dwtInfo,second_COMPno,secondAddress,secondMask,Comparator_Disable);
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNC1,0,0,0,0,DWTFunction_Disabled); //disable the comparator first
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTComparator(dwtInfo,DWT_COMP1,value);
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNC1,numberCOMP1,numberCOMP2,DataValue_Comparison,size,mode);
	
	return status ;
}
