#include "CoreDebug.h"

int setCore(CoreControl corecontrol)
{
	int status =  0 ;
	uint32_t data = 0 , dataRead = 0;
	
	data = get_Core_WriteValue(corecontrol);
	
	memoryAccessWrite(DHCSR_REG,data);
	memoryAccessRead(DHCSR_REG,&dataRead);
	
	status = isCore(corecontrol,dataRead);
	
	return status ;
}

int isCore(CoreControl corecontrol,uint32_t dataRead)
{
	int response = FALSE ;
		
	switch(corecontrol)
	{
		case CORE_DEBUG_MODE 	:
									if ((dataRead & DHCSR_DEBUG_MASK) == DHCSR_DEBUG_bit_SET)
										response = TRUE ;
									break ;
		
		case CORE_DEBUG_HALT :
									if ((dataRead & DHCSR_DEBUG_HALT_MASK) == DHCSR_DEBUG_HALT_bit_SET)
										response = TRUE ;
									break ;					
		case CORE_SINGLE_STEP	:
									if ((dataRead & DHCSR_STEP_MASK) == DHCSR_STEP_bit_SET)
										response = TRUE ;
									break ;
		case CORE_MASK_INTERRUPT:
									if ((dataRead & DHCSR_MASKINT_MASK) == DHCSR_MASKINT_bit_SET)
										response = TRUE ;
									break ;
		/*case CORE_SNAP_STALL	:
									if ((dataRead & CORE_SNAPSTALL_MASK) == CORE_SNAPSTALL_bit_SET)
										response = TRUE ;
									break ;
		*/
		default : break ;
	}
	
	return response ;
}

void check_DebugEvent(DebugEvent *debugEvent)
{
	uint32_t dataRead = 0 ;

	memoryAccessRead(DFSR_REG,&dataRead);
	update_DebugEvent(debugEvent,dataRead);
}

void update_DebugEvent(DebugEvent *debugEvent, uint32_t dataRead)
{
	if ( (dataRead & DFSR_EXTERNAL_MASK)==	DFSR_EXTERNAL_bit_SET)
		debugEvent->EXTERNAL = 1; 
	else
		debugEvent->EXTERNAL = 0; 
	
	if ( (dataRead & DFSR_VCATCH_MASK)	== 	DFSR_VCATCH_bit_SET)
		debugEvent->VCATCH = 1; 
	else
		debugEvent->VCATCH = 0; 
	
	if ( (dataRead & DFSR_DWTTRAP_MASK)	==	DFSR_DWTTRAP_bit_SET)
		debugEvent->DWTTRAP = 1; 
	else
		debugEvent->DWTTRAP = 0;
	
	if ( (dataRead & DFSR_BKPT_MASK)	==	DFSR_BKPT_bit_SET)
		debugEvent->BKPT = 1;
	else
		debugEvent->BKPT = 0;
	
	if ( (dataRead & DFSR_HALTED_MASK)	==	DFSR_HALTED_bit_SET)
		debugEvent->HALTED = 1;	
	else
		debugEvent->HALTED = 0;	
}

void init_DebugEvent(DebugEvent *debugEvent)
{
	debugEvent->EXTERNAL = 0; 
	debugEvent->VCATCH = 0;
	debugEvent->DWTTRAP = 0; 
	debugEvent->BKPT = 0;
	debugEvent->HALTED = 0;	
}

uint32_t get_Core_WriteValue(CoreControl corecontrol)
{
	uint32_t data = 0 ;
	
	switch(corecontrol)
	{
		case CORE_DEBUG_MODE 	:
									data = SET_CORE_DEBUG ;
									break ;
		
		case CORE_DEBUG_HALT	:
									data = SET_CORE_DEBUG_HALT ;
									break ;
								
		case CORE_SINGLE_STEP	:
									data = SET_CORE_STEP ;
									break ;
		case CORE_MASK_INTERRUPT:
									data = SET_CORE_MASKINT ;
									break ;
		/*case CORE_SNAP_STALL	:
									data = SET_CORE_SNAPSTALL ;
									break ;
		*/
		default : break ;
	}
	
	return data ;
}
