#include "FPB_Utilities.h"

/* Initialise FPBInfo and ComparatorInfo
 * 		Number of literal comparator  = 2 
 * 		Number of instruction comparator = 6 
 * 		Enable status =  Disable 
 *		
 *		InstructionCOMP0 = (Number = 0),(ComparatorType = Instruction),(Address = 0),(Enable status = Disable) ,(MatchingMode = REMAP)
 *		InstructionCOMP1 = (Number = 1),(ComparatorType = Instruction),(Address = 0),(Enable status = Disable) ,(MatchingMode = REMAP)
 *												-----
 *		LiteralCOMP0 = (Number = 0),(ComparatorType = Literal),(Address = 0),(Enable status = Disable) ,(MatchingMode = REMAP)
 *		LiteralCOMP1 = (Number = 1),(ComparatorType = Literal),(Address = 0),(Enable status = Disable) ,(MatchingMode = REMAP)
 *
 * Input : fpbInfo is a pointer to FPBInfo which store the information about the status of FlashPatch Breakpoint Unit
 */
void init_FPBInfo(FPBInfo *fpbInfo)
{
	static ComparatorInfo compInfo[8];
	
	init_ComparatorInfo(&(compInfo[0]),0,"Instruction",0,Disable,Match_REMAP);
	init_ComparatorInfo(&(compInfo[1]),1,"Instruction",0,Disable,Match_REMAP);
	init_ComparatorInfo(&(compInfo[2]),2,"Instruction",0,Disable,Match_REMAP);
	init_ComparatorInfo(&(compInfo[3]),3,"Instruction",0,Disable,Match_REMAP);
	init_ComparatorInfo(&(compInfo[4]),4,"Instruction",0,Disable,Match_REMAP);
	init_ComparatorInfo(&(compInfo[5]),5,"Instruction",0,Disable,Match_REMAP);
	init_ComparatorInfo(&(compInfo[6]),0,"Literal",0,Disable,Match_REMAP);
	init_ComparatorInfo(&(compInfo[7]),1,"Literal",0,Disable,Match_REMAP);
		
	fpbInfo->numberLiteralCOMP = 2 ;
	fpbInfo->numberInstructionCOMP = 6 ;
	fpbInfo->EnableDisable = Disable ;
	
	fpbInfo->compInfo[0] = &(compInfo[0]) ;
	fpbInfo->compInfo[1] = &(compInfo[1]) ;
	fpbInfo->compInfo[2] = &(compInfo[2]) ;
	fpbInfo->compInfo[3] = &(compInfo[3]) ;
	fpbInfo->compInfo[4] = &(compInfo[4]) ;
	fpbInfo->compInfo[5] = &(compInfo[5]) ;
	fpbInfo->compInfo[6] = &(compInfo[6]) ;
	fpbInfo->compInfo[7] = &(compInfo[7]) ;
}

/* Initialise all the member of ComparatorInfo with the passed in parameter
 *
 * Input : compInfo is a pointer to ComparatorInfo which store the information about the specific comparator
 *	       compNo is the comparator number
 *		   compType is the comparator type either Instruction or Literal
 *		   address is the address set in the comparator
 *		   EnableDisable is the enable bit for the comparator
 *		   matchingMode is the behaviour of the comparator when the address set is matched
 *				Possible value :
 *					Match_REMAP			remap to address specified in FlashPatch Remap Register, FP_REMAP
 *					Match_Lower16bits	Set breakpoint on lower halfword
 *					Match_Upper16bits	Set breakpoint on upper halfword
 *					Match_32bits		Set breakpoint on both upper and lower halfword
 */
void init_ComparatorInfo(ComparatorInfo *compInfo,int compNo,char *compType,uint32_t address,int EnableDisable,int matchingMode)
{
	compInfo->comparatorNumber = compNo ;
	strcpy(compInfo->comparatorType,compType);
	compInfo->address = address ;
	compInfo->EnableDisable = EnableDisable ;
	compInfo->matchingMode = matchingMode ;
}

/* Process data read from FP_CTRL register to extract information about number of literal & instruction comparator and the enable bit for PFB
 *
 * Input : fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 * 		   dataRead contains the data read from FP_CTRL register 
 */
void process_FPControlData(FPBInfo *fpbInfo,uint32_t dataRead)
{
	
	if ( check_BitSetWithMask(dataRead,FPB_ENABLED_MASK) == ERR_NOERROR )
		fpbInfo->EnableDisable = Enable ;
	else
		fpbInfo->EnableDisable = Disable ;
	
	fpbInfo->numberLiteralCOMP = (dataRead & FPB_NUMLIT_MASK) >> 8;
	fpbInfo->numberInstructionCOMP = (dataRead & FPB_NUMINSTRUCTION_MASK) >> 4 ;
	
}

/* Process data read from FP_Comparator register and update ComparatorInfo in FPBInfo
 *
 * Input : compInfo is a pointer to ComparatorInfo which store the information about the specific comparator
 * 		   dataRead contains the data read from FP_COMPn register 
 */
void process_FPComparatorData(ComparatorInfo *compInfo,uint32_t dataRead)
{	
	uint32_t data = 0, address = 0 , matchingMode = 0 , EnableDisable = Disable ;
	
	matchingMode = (dataRead & FP_COMP_MATCH_MASK);
	
	if (matchingMode == Match_Upper16bits)
		data = 2;
	
	address = (dataRead & FP_COMP_ADDRESS_MASK) + data ;
	EnableDisable = (dataRead & FP_COMP_ENABLE_MASK);	

  compInfo->address = address ;
  compInfo->EnableDisable = EnableDisable ;
  compInfo->matchingMode = matchingMode ;
}

/* Process data read from FP_Comparator register and update SRAM_REMAP_Address in FPBInfo
 *
 * Input : compInfo is a pointer to ComparatorInfo which store the information about the specific comparator
 * 		   dataRead contains the data read from FP_COMPn register 
 */
void process_FPRemapData(FPBInfo *fpbInfo,uint32_t dataRead)
{
	fpbInfo->SRAM_REMAP_address = (dataRead & FP_REMAP_ReadAddress_MASK);
}

/* Check if the FlashPatch Breakpoint Unit is enabled/disabled
 *
 * Input : 	fpbInfo is a pointer to FPBInfo which store the enable status of FlashPatch Breakpoint Unit
 *
 * Output :	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled
 *			return ERR_FPB_NOTENABLED if FlashPatch Breakpoint Unit is not enabled
 *
 */
int isFPB_EnabledDisabled(FPBInfo *fpbInfo,int EnableDisable)
{
	if (EnableDisable)
	{
		if ( check_BitSetWithMask(fpbInfo->EnableDisable,FPB_ENABLED_MASK) == ERR_NOERROR)
			return ERR_NOERROR ;
		else
			return ERR_FPB_NOTENABLED ;
	}
	else
	{
		if ( check_BitSetWithMask(fpbInfo->EnableDisable,FPB_ENABLED_MASK) != ERR_NOERROR)
			return ERR_NOERROR ;
		else
			return ERR_FPB_NOTDISABLED ;
	}
}

/* Check if the previous write of the comparator has been written and updated the comparator by checking the data wrote and data read
 *
 * Input : 	fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *
 * Output :	return ERR_NOERROR if the data read is the same as the data written
 *			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 *
 */
int isFP_COMP_Updated(uint32_t dataWrote,uint32_t dataRead)
{
	return (check_dataWrote_dataRead(dataWrote,dataRead));
}

/* Check if the previous write of the remap register has been written and updated the remap register by checking the data wrote and data read
 *
 * Input : 	fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *
 * Output :	return ERR_NOERROR if the data read is the same as the data written
 *			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 *
 */
int isFP_REMAP_Updated(uint32_t dataWrote,uint32_t dataRead)
{
	return (check_dataWrote_dataRead(dataWrote,dataRead));
}

/* Use to get the value to be written into FlashPatch Control Register,FP_CTRL to enable/disable FlashPatch Breakpoint Unit
 *
 * Input : EnableDisable determine to enable/disable FlashPatch Breakpoint Unit
 *				Possible value :
 *					Enable 		enable FPB unit
 *					Disable 	disable FPB unit
 *
 * Output :	return 32 bits of  to be written into FlashPatch Control Register,FP_CTRL
 *
 */
uint32_t get_FP_CTRL_WriteValue(int EnableDisable)
{
	if(EnableDisable)
		return ENABLE_FPB ;
	else
		return DISABLE_FPB ;
}
 
 
/* Use to get the value to be written into FlashPatch Comparator Register, FP_COMP
 *
 * Input : 	address is the address going to be compared in the comparator
 *		    matchingMode is the behaviour of the comparator when the address set is matched
 *				Possible value :
 *					Match_REMAP			remap to address specified in FlashPatch Remap Register, FP_REMAP
 *					Match_Lower16bits	Set breakpoint on lower halfword
 *					Match_Upper16bits	Set breakpoint on upper halfword
 *					Match_32bits		Set breakpoint on both upper and lower halfword
 *			EnableDisable is use to enable / disable the comparator
 *				Possible value :
 *					Enable 				enable the comparator
 *					Disable				disable the comparator
 *
 * Output :	return 32 bits of  to be written into FlashPatch Comparator Register, FP_COMP
 *
 */
uint32_t get_FP_COMP_WriteValue(uint32_t address,int matchingMode,int EnableDisable)
{
	uint32_t data = 0 , Address;
	Address = address & FP_COMP_ADDRESS_MASK  ; //Bits[31:29] & Bits[1:0] are now 0
	
	data = matchingMode + Address + EnableDisable;
	
	return data ;
}

/* Use to get the value to be written into FlashPatch Remap Register, FP_REMAP
 *
 * Input : 	address is the base address of remapping located in SRAM region
 *
 * Output :	return 32 bits of to be written into FlashPatch Remap Register, FP_REMAP
 */
uint32_t get_FP_REMAP_WriteValue(uint32_t address)
{
	uint32_t data = 0 ;
	data = address & FP_REMAP_Address_MASK ; //Bits[31:29] & Bit[4:0] are now  0
	
	return data ;
}

/* Use to get the number location of ComparatorInfo for the selected comparator
 *
 * Input : 	COMP_no is the comparator number
 *				Possible value :
 *					FP_COMP0	
 *					FP_COMP1	
 *					FP_COMP2	
 *					FP_COMP3	
 *					FP_COMP4	
 *					FP_COMP5
 *					FP_COMP6
 *					FP_COMP7	
 *
 * Output :	return number to the location of ComparatorInfo
 *			return ERR_INVALID_COMPARATOR if comparator out of range
 */
int get_ComparatorInfoNumber(uint32_t COMP_no)
{
	int data = 0;
	
	switch(COMP_no)
	{
		case FP_COMP0 :
						data = 0 ;
						break ;
		case FP_COMP1 :
						data = 1 ;
						break ;
		case FP_COMP2 :
						data = 2 ;
						break ;
		case FP_COMP3 :
						data = 3 ;
						break ;
		case FP_COMP4 :
						data = 4 ;
						break ;
		case FP_COMP5 :
						data = 5 ;
						break ;
		case FP_COMP6 :
						data = 6 ;
						break ;
		case FP_COMP7 :
						data = 7 ;
						break ;
		default :
						data = ERR_INVALID_COMPARATOR ;
						break ;
	}
	
	return data ;
}
