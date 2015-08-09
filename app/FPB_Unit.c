#include "FPB_Unit.h"

/**
 * Enable/Disable Flash Patch Breakpoint unit for the usage of flash patching/remapping and instruction breakpoint
 *
 * Input : fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *		   EnableDisable is use to enable/disable the Flash Patch Breakpoint unit
 *				Possible values :
 *					Enable			enable FPB unit
 *					Disable			disable FPB unit
 *
 * Output :	return ERR_NOERROR if FlashPatch Breakpoint Unit is enabled
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *			return ERR_FPB_NOTENABLED if FlashPatch Breakpoint Unit is not enabled
 */
int control_FPB(FPBInfo *fpbInfo,int EnableDisable)
{
	uint32_t dataToWrite ;
	int status = 0 ;
		
	dataToWrite = get_FP_CTRL_WriteValue(EnableDisable);
		
	memoryWriteWord(FP_CTRL,dataToWrite);
	status = read_FPBControl(fpbInfo);

	if (status != ERR_NOERROR)
		return status;
	
	status = isFPB_EnabledDisabled(fpbInfo,EnableDisable);
	
	return status;
}

/**
 * Read FP_CTRL register and store the information into FPBInfo
 *
 * Input : fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *
 * Output :	return ERR_NOERROR if the read of FP_CTRL is successful
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 */
int read_FPBControl(FPBInfo *fpbInfo)
{
	uint32_t dataRead = 0;
	uint32_t status = 0 ;
	
	status = memoryReadWord(FP_CTRL,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_FPControlData(fpbInfo,dataRead);
	
	return ERR_NOERROR;
}


/**
 * Configure the selected FP Comparator as defined by the pass in parameter
 * 
 * Input : 	fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *			COMP_no is the comparator number being configured
 *				Possible values : 
 *					InstructionCOMP_0				LiteralCOMP_0
 *					InstructionCOMP_1				LiteralCOMP_1
 *					InstructionCOMP_2
 *					InstructionCOMP_4
 *					InstructionCOMP_5
 *
 *   	   	address is the instruction/literal address in CODE region going to be compared (0x00000000 - 0x1FFFFFFF)
 *
 *		   	Matching mode is the operation mode of the comparator 
 *				Possible values : 
 *					Match_REMAP			remap to address specified in FlashPatch Remap Register, FP_REMAP 	(please use set_InstructionREMAP() for this mode)
 *					Match_Lower16bits	Set breakpoint on lower halfword (Bits[1:0] are 0b00)				(Setting this mode for LiteralCOMPn will be ignored by hardware)
 *					Match_Upper16bits	Set breakpoint on upper halfword (Bits[1:0] are 0b10)				(Setting this mode for LiteralCOMPn will be ignored by hardware)
 *					Match_32bits		Set breakpoint on both upper and lower halfword						(Setting this mode for LiteralCOMPn will be ignored by hardware)
 *
 *			EnableDisable is use to enable / disable the comparator
 *				Possible value :
 *					Enable 				enable the comparator
 *					Disable				disable the comparator
 *
 * Output : return ERR_NOERROR if the FP Comparator has been updated successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 * 			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 *			return ERR_INVALID_COMPARATOR if the selected comparator is not valid/found in the device
 */
int configure_FP_COMP(FPBInfo *fpbInfo,uint32_t COMP_no,uint32_t address,int matchingMode,int EnableDisable)
{
	uint32_t dataToWrite = 0, dataRead = 0 ;
	int status = 0, number = 0 ;
	
	number = get_ComparatorInfoNumber(COMP_no);
	
	if(number == ERR_INVALID_COMPARATOR)
		return number ;
	
	dataToWrite = get_FP_COMP_WriteValue(address,matchingMode,EnableDisable);
	
	memoryWriteWord(COMP_no,dataToWrite);
	status = memoryReadWord(COMP_no,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	status = isFP_COMP_Updated(dataToWrite,dataRead);
	if (status == ERR_NOERROR)
		process_FPComparatorData(fpbInfo->compInfo[number],dataRead);
	
	return status ;
}

/**
 * Configure FP REMAP register to set the base address for remapping
 *
 * Input :	fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *			SRMAP_REMAP_address is the base address for remapping which is located in the SRAM region (0x20000000 - 0x3FFFFFFF)
 *
 * Output : return ERR_NOERROR if the FP REMAP has been updated successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 * 			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 */
int configure_FP_REMAP(FPBInfo *fpbInfo,uint32_t SRAM_REMAP_address)
{
	uint32_t dataToWrite = 0, dataRead = 0 ;
	int status = 0 ;
	
	dataToWrite = get_FP_REMAP_WriteValue(SRAM_REMAP_address);
	
	memoryWriteWord(FP_REMAP,dataToWrite);
	status = memoryReadWord(FP_REMAP,&dataRead);
	
	if (status != ERR_NOERROR)
		return status;
	
	status = isFP_REMAP_Updated(dataToWrite,dataRead);
	
	if (status == ERR_NOERROR)
		process_FPRemapData(fpbInfo,dataRead);
	
	return status ;
}

/** 
 * Set instruction breakpoint using the user defined comparator at at the user defined address 
 *
 * Input :	fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *			InstructionCOMP_no is the number of the selected instruction comparator to perform comparison
 *				Possible values : 
 *					InstructionCOMP_0				
 *					InstructionCOMP_1				
 *					InstructionCOMP_2
 *					InstructionCOMP_4
 *					InstructionCOMP_5
 *
 *	   		address is the instruction/literal address in CODE region going to be compared (0x00000000 - 0x1FFFFFFF)
 *
 *		   	Matching mode is the operation mode of the comparator 
 *				Possible values : 
 *					Match_REMAP			remap to address specified in FlashPatch Remap Register, FP_REMAP 	(please use set_InstructionREMAP() for this mode)
 *					Match_Lower16bits	Set breakpoint on lower halfword (Bits[1:0] are 0b00)				(Setting this mode for LiteralCOMPn will be ignored by hardware)
 *					Match_Upper16bits	Set breakpoint on upper halfword (Bits[1:0] are 0b10)				(Setting this mode for LiteralCOMPn will be ignored by hardware)
 *					Match_32bits		Set breakpoint on both upper and lower halfword						(Setting this mode for LiteralCOMPn will be ignored by hardware)
 *
 * Output : return ERR_NOERROR if the FP Comparator has been updated successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 * 			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 */
int set_InstructionBKPT(FPBInfo *fpbInfo,uint32_t InstructionCOMP_no,uint32_t address,int matchingMode)
{	
	int status = 0 ;

	status = configure_FP_COMP(fpbInfo,InstructionCOMP_no,address,matchingMode,Enable);
	return status ;
}

/** 
 * Set instruction remapping using the user defined comparator at at the user defined address 
 * 
 * Input :	fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *			InstructionCOMP_no is the number of the selected instruction comparator to perform comparison
 *				Possible values : 
 *					InstructionCOMP_0				
 *					InstructionCOMP_1				
 *					InstructionCOMP_2
 *					InstructionCOMP_4
 *					InstructionCOMP_5
 *
 *	   		address is the instruction/literal address in CODE region going to be compared (0x00000000 - 0x1FFFFFFF)
 *
 * Output : return ERR_NOERROR if the FP Comparator has been updated successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 * 			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 */
int set_InstructionREMAP(FPBInfo *fpbInfo,uint32_t InstructionCOMP_no,uint32_t address)
{
	int status = 0 ;
	
	status = configure_FP_COMP(fpbInfo,InstructionCOMP_no,address,Match_REMAP,Enable);
	return status ;
}

/** 
 * Set literal remapping using the user defined comparator at at the user defined address 
 *
 * Input :	fpbInfo is a pointer to FPBInfo which store information about the status of FlashPatch Breakpoint Unit
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *			LiteralCOMP_no is the number of the selected litreal comparator to perform comparison
 *				Possible values : 
 *					LiteralCOMP_0	
 *					LiteralCOMP_1
 *
 *	   		address is the instruction/literal address in CODE region going to be compared (0x00000000 - 0x1FFFFFFF)
 *
 * Output : return ERR_NOERROR if the FP Comparator has been updated successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 * 			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 */
int set_LiteralREMAP(FPBInfo *fpbInfo,uint32_t LiteralCOMP_no,uint32_t address)
{
	int status = 0 ;
	
	status = configure_FP_COMP(fpbInfo,LiteralCOMP_no,address,Match_REMAP,Enable);
	return status ;
}



/**
 * Disable the selected FP Comparator and preserve the address and matching mode set inside
 * 
 * Input : 	fpbInfo is a pointer to FPBInfo which contain information about the selected Comparator
 *			COMP_no is the comparator number being configured
 *				Possible values : 
 *					InstructionCOMP_0				LiteralCOMP_0
 *					InstructionCOMP_1				LiteralCOMP_1
 *					InstructionCOMP_2
 *					InstructionCOMP_4
 *					InstructionCOMP_5
 *
 * Output : return ERR_NOERROR if the FP Comparator has been updated successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 * 			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 *			return ERR_INVALID_COMPARATOR if the selected comparator is not valid/found in the device
 */
int disable_FPComp(FPBInfo *fpbInfo,uint32_t COMP_no)
{
	int status = 0 , number = 0  ;
	uint32_t address = 0 , matchingMode = 0 ;
	
	number = get_ComparatorInfoNumber(COMP_no);
	address = fpbInfo->compInfo[number]->address ;
	matchingMode = fpbInfo->compInfo[number]->matchingMode ;
	
	status = configure_FP_COMP(fpbInfo,COMP_no,address,matchingMode,Disable);
	
	return status ;
}

/**
 * Re-enable the selected previously disabled comparator
 * 
 * Input : 	fpbInfo is a pointer to FPBInfo which contain information about the selected Comparator
 *			COMP_no is the comparator number being configured
 *				Possible values : 
 *					InstructionCOMP_0				LiteralCOMP_0
 *					InstructionCOMP_1				LiteralCOMP_1
 *					InstructionCOMP_2
 *					InstructionCOMP_4
 *					InstructionCOMP_5
 *
 * Output : return ERR_NOERROR if the FP Comparator has been updated successfully
 *			return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 * 			return ERR_DATARW_NOT_MATCH if data read is the different as the data written
 *			return ERR_INVALID_COMPARATOR if the selected comparator is not valid/found in the device
 */
int reenable_FPComp(FPBInfo *fpbInfo,uint32_t COMP_no)
{
	int status = 0 , number = 0  ;
	uint32_t address = 0 , matchingMode = 0 ;
	
	number = get_ComparatorInfoNumber(COMP_no);
	address = fpbInfo->compInfo[number]->address ;
	matchingMode = fpbInfo->compInfo[number]->matchingMode ;
	
	status = configure_FP_COMP(fpbInfo,COMP_no,address,matchingMode,Enable);
	
	return status ;
}
