#include "FPB_Unit.h"

int enable_FPB()
{
	uint32_t dataRead = 0;
	int ACK = 0 , status = 0 ;
		
	memoryAccessWrite(FP_CTRL,ENABLE_FPB);
	memoryAccessRead(FP_CTRL,&dataRead);

	status = isFPB_Enabled(dataRead);
	
	return status;
}

int configure_FP_COMP(uint32_t COMP_no,uint32_t address,int Matching_mode)
{
	uint32_t dataToWrite = 0, dataRead = 0 ;
	int status = 0;
	
	dataToWrite = get_FP_COMP_WriteValue(address,Matching_mode);
	
	memoryAccessWrite(COMP_no,dataToWrite);
	memoryAccessRead(COMP_no,&dataRead);

	status = isFP_COMP_Updated(dataToWrite,dataRead);
	
	return status ;
}

int configure_FP_REMAP(uint32_t SRAM_REMAP_address)
{
	uint32_t dataToWrite = 0, dataRead = 0 ;
	int status = 0 ;
	
	dataToWrite = get_FP_REMAP_WriteValue(SRAM_REMAP_address);
	
	memoryAccessWrite(FP_REMAP,dataToWrite);
	memoryAccessRead(FP_REMAP,&dataRead);
	
	return status ;
}

int set_InstructionBKPT(uint32_t InstructionCOMP_no,uint32_t address,int Matching_mode)
{
	int status = 0 ;
	status = configure_FP_COMP(InstructionCOMP_no,address,Matching_mode);
	return status ;
}

int set_InstructionREMAP(uint32_t InstructionCOMP_no,uint32_t address,uint32_t REMAP_address)
{
	int status = 0 ;
	status = configure_FP_COMP(InstructionCOMP_no,address,Match_REMAP);
	return status ;
}

int set_LiteralREMAP(uint32_t LiteralCOMP_no,uint32_t address,uint32_t REMAP_address)
{
	int status = 0 ;
	status = configure_FP_COMP(LiteralCOMP_no,address,Match_REMAP);
	return status ;
}

int isFPB_Enabled(uint32_t data)
{
	if ((data&FPB_MASK) == 1)
		return TRUE ;
	else
		return FALSE ;
}

int isFP_COMP_Updated(uint32_t dataWrote,uint32_t dataRead)
{
	if (dataRead == dataWrote)
		return TRUE ;
	else
		return FALSE ;
}

uint32_t get_FP_COMP_WriteValue(uint32_t address,int Matching_mode)
{
	uint32_t data = 0 , Address;
	Address = address & FP_COMP_Address_MASK  ; //Bits[31:29] & Bits[1:0] are now 0
	
	data = Matching_mode + Address + FP_COMP_Enable;
	
	return data ;
}

uint32_t get_FP_REMAP_WriteValue(uint32_t address)
{
	uint32_t data = 0 , Address;
	Address = address & FP_REMAP_Address_MASK ; //Bits[31:29] & Bit[5:0] are now  0
	
	data = Address ;
	return data ;
}
