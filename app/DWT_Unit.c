#include "DWT_Unit.h"


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
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNCno,0,0,0,0,DWTComparator_Disabled); //disable the comparator first
	
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
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNCno,0,0,0,0,DWTComparator_Disabled); //disable the comparator first
	
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
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNC1,0,0,0,0,DWTComparator_Disabled); //disable the comparator first
	
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
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNC1,0,0,0,0,DWTComparator_Disabled); //disable the comparator first
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTComparator(dwtInfo,DWT_COMP1,value);
	
	if(status != ERR_NOERROR)
		return status ;
	
	status = configure_DWTFunction(dwtInfo,DWT_FUNC1,numberCOMP1,numberCOMP2,DataValue_Comparison,size,mode);
	
	return status ;
}

