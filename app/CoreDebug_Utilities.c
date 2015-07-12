#include "CoreDebug_Utilities.h"

/**
 *	Use to check the current mode of the core
 *	
 *	Input : coreControl is used to define which mode is going to be compared with the core
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *	
 *	Output : return TRUE if the core is in the current mode as defined in coreControl
 *			 return FALSE if it is not match
 */
int isCore(CoreControl coreControl,CoreStatus *coreStatus)
{
	int response = FALSE ;
	
	switch(coreControl)
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

/**
 *	Use to initialise all members in CoreStatus structure to 0
 *	
 *	Input : coreStatus is a pointer to CoreStatus which all the members are going to be initialised to 0
 *	
 */
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


/**
 *	Use to initialise all members in DebugEvent structure to 0
 *	
 *	Input : debugEvent is a pointer to DebugEvent which all the members are going to be initialised to 0
 *	
 */
void init_DebugEvent(DebugEvent *debugEvent)
{
	debugEvent->EXTERNAL	= 0 ; 
	debugEvent->VCATCH 		= 0 ;
	debugEvent->DWTTRAP 	= 0 ; 
	debugEvent->BKPT 		= 0 ;
	debugEvent->HALTED		= 0 ;	
}

/**
 *	Use to initialise all members in VectorCatch structure to 0
 *	
 *	Input : vectorCatch is a pointer to vectorCatch which all the members are going to be initialised to 0
 *	
 */
void init_VectorCatch(VectorCatch *vectorCatch)
{
	vectorCatch->VC_HARDERR		= 0;
	vectorCatch->VC_INTERR		= 0;
	vectorCatch->VC_BUSERR		= 0;
	vectorCatch->VC_STATERR		= 0;
	vectorCatch->VC_CHKERR		= 0;
	vectorCatch->VC_NOCPERR		= 0;
	vectorCatch->VC_MMERR		= 0;
	vectorCatch->VC_CORERESET	= 0;
}

/**
 *	Use to update the CoreStatus structure with the data received
 *	
 *	Input : coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *			dataRead contain the data read from Debug Halting Control and Status Register
 *	
 */
void update_CoreStatus(CoreStatus *coreStatus, uint32_t dataRead)
{	
	if ( checkIs_BitSet(dataRead,DHCSR_C_DEBUGEN_MASK) )
		coreStatus-> C_DEBUGEN = 1 ;
	else
		coreStatus-> C_DEBUGEN = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_C_HALT_MASK) )
		coreStatus-> C_HALT = 1 ;
	else
		coreStatus-> C_HALT = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_C_STEP_MASK) )
		coreStatus-> C_STEP = 1 ;
	else
		coreStatus-> C_STEP = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_C_MASKINTS_MASK) )
		coreStatus-> C_MASKINTS  = 1 ;
	else
		coreStatus-> C_MASKINTS = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_C_SNAPSTALL_MASK) )
		coreStatus-> C_SNAPSTALL = 1 ;
	else
		coreStatus-> C_SNAPSTALL = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_REGRDY_MASK) )
		coreStatus-> S_REGRDY = 1 ;
	else
		coreStatus-> S_REGRDY = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_HALT_MASK) )
		coreStatus-> S_HALT = 1 ;
	else
		coreStatus-> S_HALT = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_SLEEP_MASK) )
		coreStatus-> S_SLEEP = 1 ;
	else
		coreStatus-> S_SLEEP = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_LOCKUP_MASK) )
		coreStatus-> S_LOCKUP = 1 ;
	else
		coreStatus-> S_LOCKUP = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_RETIRE_MASK))
		coreStatus-> S_RETIRE = 1 ;
	else
		coreStatus-> S_RETIRE = 0 ;
	
	if ( checkIs_BitSet(dataRead,DHCSR_S_RESET_MASK))
		coreStatus-> S_RESET = 1 ;
	else
		coreStatus-> S_RESET = 0 ;
	
}

/**
 *	Use to update the DebugEvent structure with the data received
 *	
 *	Input : debugEvent is a pointer to DebugEvent which store the event of the core occurred for example debug event generation of breakpoint BKPT
 *			dataRead contain the data read from Debug Fault Status Register
 *	
 */
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

/**
 *	Use to update the VectorCatch structure with the data received
 *	
 *	Input : vectorCatch is a pointer to VectorCatch which store whether the debug trap is enabled/disabled for example debug trap on HARDFAULT VC_HARDERR
 *			dataRead contain the data read from Debug Exception and Monitor Control Register
 *	
 */
void update_VectorCatch(VectorCatch *vectorCatch, uint32_t dataRead)
{
	if ( checkIs_BitSet(dataRead,DEMCR_VC_HARDERR_MASK) )
		vectorCatch-> VC_HARDERR = 1 ;
	else
		vectorCatch-> VC_HARDERR = 0 ;
	
	if ( checkIs_BitSet(dataRead,DEMCR_VC_INTERR_MASK) )
		vectorCatch-> VC_INTERR = 1 ;
	else
		vectorCatch-> VC_INTERR = 0 ;
	
	if ( checkIs_BitSet(dataRead,DEMCR_VC_BUSERR_MASK) )
		vectorCatch-> VC_BUSERR = 1 ;
	else
		vectorCatch-> VC_BUSERR = 0 ;
	
	if ( checkIs_BitSet(dataRead,DEMCR_VC_STATERR_MASK) )
		vectorCatch-> VC_STATERR  = 1 ;
	else
		vectorCatch-> VC_STATERR = 0 ;
	
	if ( checkIs_BitSet(dataRead,DEMCR_VC_CHKERR_MASK) )
		vectorCatch-> VC_CHKERR = 1 ;
	else
		vectorCatch-> VC_CHKERR = 0 ;
	
	if ( checkIs_BitSet(dataRead,DEMCR_VC_NOCPERR_MASK) )
		vectorCatch-> VC_NOCPERR = 1 ;
	else
		vectorCatch-> VC_NOCPERR = 0 ;
	
	if ( checkIs_BitSet(dataRead,DEMCR_VC_MMERR_MASK) )
		vectorCatch-> VC_MMERR = 1 ;
	else
		vectorCatch-> VC_MMERR = 0 ;
	
	if ( checkIs_BitSet(dataRead,DEMCR_VC_CORERESET_MASK) )
		vectorCatch-> VC_CORERESET = 1 ;
	else
		vectorCatch-> VC_CORERESET = 0 ;
}

/**
 *	Use to get the value of the data going to be written into Debug Halting and Control Status Register based on CoreControl mode
 *	
 *	Input : coreControl is used to set the core to the desired mode
 *	
 *	Output : return the 32bits data to be written into Debug Halting and Control Status Register
 */
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

/**
 *	Use to get the value of the data going to be written into Debug Halting and Control Status Register based on CoreControl mode
 *	
 *	Input : coreRegister is the ARM core register which is going to be accessed
 *			CoreRegister_ReadWrite is used to define the current access is reed or write
 *	
 *	Output : return the 32bits data to be written into Debug Core Register Selector Register
 */
uint32_t get_CoreRegisterAccess_WriteValue(Core_RegisterSelect coreRegister,int CoreRegister_ReadWrite)
{
	uint32_t data = 0 ;
	
	data = (CoreRegister_ReadWrite << 16) + coreRegister ;
	
	return data ;
}

/**
 *	Use to check if multiple bits is set in the data
 *	
 *	Input : data contains the data going to be compared with
 *			dataMask is used to to check for the specific masked bits
 *
 *	Output : return TRUE if all the bits specified is set
 *			 return FALSE if not all/non of the bits specified is set
 *	
 */
int checkIs_BitSet(uint32_t data,uint32_t dataMask)
{
	if ((data & dataMask) == dataMask)
		return TRUE ;
	else
		return FALSE ;
}