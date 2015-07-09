#include "CoreDebug.h"

int setCore(CoreControl corecontrol,CoreStatus *coreStatus)
{
	int status =  0 ;
	uint32_t data = 0 , dataRead = 0;
	
	init_CoreStatus(coreStatus);
	data = get_Core_WriteValue(corecontrol);
	
	memoryAccessWrite(DHCSR_REG,data);
	memoryAccessRead(DHCSR_REG,&dataRead);
	
	update_CoreStatus(coreStatus,dataRead);
	
	status = isCore(corecontrol,coreStatus);
	
	return status ;
}

int isCore(CoreControl corecontrol,CoreStatus *coreStatus)
{
	int response = FALSE ;
	
	switch(corecontrol)
	{
		case CORE_DEBUG_MODE 	:
									response = coreStatus->C_DEBUGEN ;
									break ;
		case CORE_DEBUG_HALT :
									response = (coreStatus->S_HALT & coreStatus->C_DEBUGEN & coreStatus->C_HALT) ;
									break ;					
		case CORE_SINGLE_STEP	:
									response = (coreStatus->S_HALT & coreStatus->C_DEBUGEN & coreStatus->C_HALT & coreStatus->C_STEP);
									break ;
		case CORE_MASK_INTERRUPT:
									response = (coreStatus->S_HALT & coreStatus->C_DEBUGEN & coreStatus->C_HALT & coreStatus->C_MASKINTS) ;
									break ;
		case CORE_SNAPSTALL	:
									response = (coreStatus->C_DEBUGEN & coreStatus->C_HALT & coreStatus->C_SNAPSTALL);
									break ;
		
		default : break ;
	}
	
	return response ;
}

void check_DebugEvent(DebugEvent *debugEvent)
{
	uint32_t dataRead = 0 ;

	init_DebugEvent(debugEvent);
	
	memoryAccessRead(DFSR_REG,&dataRead);
	update_DebugEvent(debugEvent,dataRead);
}

void update_DebugEvent(DebugEvent *debugEvent, uint32_t dataRead)
{
	if ( checkIs_BitSet(dataRead,DFSR_EXTERNAL_MASK) )
		debugEvent->EXTERNAL = 1; 
	else
		debugEvent->EXTERNAL = 0; 
	
	if ( checkIs_BitSet(dataRead,DFSR_VCATCH_MASK) )
		debugEvent->VCATCH = 1; 
	else
		debugEvent->VCATCH = 0; 
	
	if ( checkIs_BitSet(dataRead,DFSR_DWTTRAP_MASK) )
		debugEvent->DWTTRAP = 1; 
	else
		debugEvent->DWTTRAP = 0;
	
	if ( checkIs_BitSet(dataRead,DFSR_BKPT_MASK) )
		debugEvent->BKPT = 1;
	else
		debugEvent->BKPT = 0;
	
	if ( checkIs_BitSet(dataRead,DFSR_HALTED_MASK) )
		debugEvent->HALTED = 1;	
	else
		debugEvent->HALTED = 0;	
	

}

void update_CoreStatus(CoreStatus *coreStatus, uint32_t dataRead)
{	
	if ( checkIs_BitSet(dataRead,DHCSR_C_DEBUGEN_MASK) )
		coreStatus-> C_DEBUGEN = 1 ;
	else
		coreStatus-> C_DEBUGEN = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_C_HALT_MASK) )
		coreStatus-> C_HALT = 1 ;
	else
		coreStatus-> C_HALT = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_C_STEP_MASK) )
		coreStatus-> C_STEP = 1 ;
	else
		coreStatus-> C_STEP = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_C_MASKINTS_MASK) )
		coreStatus-> C_MASKINTS  = 1 ;
	else
		coreStatus-> C_MASKINTS = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_C_SNAPSTALL_MASK) )
		coreStatus-> C_SNAPSTALL = 1 ;
	else
		coreStatus-> C_SNAPSTALL = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_REGRDY_MASK) )
		coreStatus-> S_REGRDY = 1 ;
	else
		coreStatus-> S_REGRDY = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_HALT_MASK) )
		coreStatus-> S_HALT = 1 ;
	else
		coreStatus-> S_HALT = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_SLEEP_MASK) )
		coreStatus-> S_SLEEP = 1 ;
	else
		coreStatus-> S_SLEEP = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_LOCKUP_MASK) )
		coreStatus-> S_LOCKUP = 1 ;
	else
		coreStatus-> S_LOCKUP = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_RETIRE_MASK))
		coreStatus-> S_RETIRE = 1 ;
	else
		coreStatus-> S_RETIRE = 1 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_RESET_MASK))
		coreStatus-> S_RESET = 1 ;
	else
		coreStatus-> S_RESET = 1 ;
	
}

int checkIs_BitSet(uint32_t data,uint32_t dataMask)
{
	if ((data & dataMask) == dataMask)
		return 1 ;
	else
		return 0 ;
}

void init_DebugEvent(DebugEvent *debugEvent)
{
	debugEvent->EXTERNAL	= 0 ; 
	debugEvent->VCATCH 		= 0 ;
	debugEvent->DWTTRAP 	= 0 ; 
	debugEvent->BKPT 		= 0 ;
	debugEvent->HALTED		= 0 ;	
}

void init_CoreStatus(CoreStatus *coreStatus)
{
	coreStatus->S_RESET		= 0 ; 
	coreStatus->S_RETIRE	= 0 ; 
	coreStatus->S_LOCKUP	= 0 ; 
	coreStatus->S_SLEEP 	= 0 ;
	coreStatus->S_HALT	= 0 ; 
	coreStatus->S_REGRDY	= 0 ; 
	coreStatus->C_SNAPSTALL	= 0 ; 
	coreStatus->C_MASKINTS	= 0 ; 
	coreStatus->C_STEP 		= 0 ;	
	coreStatus->C_HALT 		= 0 ;
	coreStatus->C_DEBUGEN	= 0 ;
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
		case CORE_SNAPSTALL	:
									data = SET_CORE_SNAPSTALL ;
									break ;
		
		default : break ;
	}
	
	return data ;
}
