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

int isFPB_Enabled(uint32_t data)
{
	if ((data&FPB_MASK) == 1)
		return TRUE ;
	else
		return FALSE ;
}