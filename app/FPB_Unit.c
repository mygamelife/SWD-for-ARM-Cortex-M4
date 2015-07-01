#include "FPB_Unit.h"

int FPB_Enable()
{
	uint32_t dataRead = 0;
	int ACK = 0 , status = 0 ;
		
	MemoryAccess_Write(FPB_REG,ENABLE_FPB);
	MemoryAccess_Read(FPB_REG,&dataRead);

	status = isFPB_Enabled(dataRead);
	
	return status;
}

int FPB_COMP_Access(uint32_t COMP_no,uint32_t address,int Matching_mode)
{
	
}

int InstructionCOMP_Access(uint32_t InstructionCOMP_no,uint32_t address,int Matching_mode)
{
	FPB_COMP_Access(InstructionCOMP_no,address,Matching_mode);
}

int LiteralCOMP_Access(uint32_t LiteralCOMP_no,uint32_t address)
{
	FPB_COMP_Access(LiteralCOMP_no,address,Match_REMAP);
}

int isFPB_Enabled(uint32_t data)
{
	if ((data&FPB_MASK) == 1)
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