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
									if ((dataRead & CORE_DEBUG_MASK) ==	CORE_DEBUG_bit_SET)
										response = TRUE ;
									break ;
		
		case CORE_DEBUG_HALT :
									if ((dataRead & CORE_DEBUG_HALT_MASK) == CORE_DEBUG_HALT_bit_SET)
										response = TRUE ;
									break ;					
		case CORE_SINGLE_STEP	:
									if ((dataRead & CORE_STEP_MASK) == CORE_STEP_bit_SET)
										response = TRUE ;
									break ;
		case CORE_MASK_INTERRUPT:
									if ((dataRead & CORE_MASKINT_MASK) == CORE_MASKINT_bit_SET)
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
