#include "CoreDebug.h"

int setCore_DebugMode()
{
	uint32_t dataRead = 0;
	int ACK = 0 , status = 0 ;
		
	MemoryAccess_Write(DHCSR_REG,SET_CORE_DEBUG);
	MemoryAccess_Read(DHCSR_REG,&dataRead);

	status = isCore_DebugMode(dataRead);
	
	return status;
}

int setCore_Halt()
{
	uint32_t dataRead = 0;
	int ACK = 0, status = 0;
	
	MemoryAccess_Write(DHCSR_REG,SET_CORE_HALT);
	MemoryAccess_Read(DHCSR_REG,&dataRead);
	
	status = isCore_Halted(dataRead);
	
	
	return status ;
}

int isCore_DebugMode(uint32_t data)
{
	if ((data & CORE_DEBUG_MASK) == 1)
		return TRUE ;
	else 
		return FALSE ;
}

int isCore_Halted(uint32_t data)
{
	
	if ((data & CORE_HALT_MASK) == CORE_HALT_MASK )
		return TRUE ;
	else 
		return FALSE ;
}