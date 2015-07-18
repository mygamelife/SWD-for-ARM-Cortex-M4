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
	int status = 0;

	switch(coreControl)
	{
		case CORE_DEBUG_MODE 	:
									status = coreStatus->C_DEBUGEN ;
									break ;
		case CORE_DEBUG_HALT :
									status = (coreStatus->S_HALT & coreStatus->C_DEBUGEN & coreStatus->C_HALT) ;
									break ;					
		case CORE_SINGLE_STEP	:
									status = (coreStatus->S_HALT & coreStatus->C_DEBUGEN & coreStatus->C_HALT & coreStatus->C_STEP);
									break ;
		case CORE_MASK_INTERRUPT:
									status = (coreStatus->S_HALT & coreStatus->C_DEBUGEN & coreStatus->C_HALT & coreStatus->C_MASKINTS) ;
									break ;
		case CORE_SNAPSTALL	:
									status = (coreStatus->C_DEBUGEN & coreStatus->C_HALT & coreStatus->C_SNAPSTALL);
									break ;
		
		default : break ;
	}
	
	if(status)
		return ERR_NOERROR ;
	else
		return ERR_CORECONTROL_FAILED ;
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
	coreStatus->S_HALT		= 0 ; 
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
 *	Use to initialise all members in DebugTrap structure to 0
 *	
 *	Input : debugTrap is a pointer to DebugTrap which all the members are going to be initialised to 0
 *	
 */
void init_DebugTrap(DebugTrap *debugTrap)
{
	debugTrap->VC_HARDERR		= 0;
	debugTrap->VC_INTERR		= 0;
	debugTrap->VC_BUSERR		= 0;
	debugTrap->VC_STATERR		= 0;
	debugTrap->VC_CHKERR		= 0;
	debugTrap->VC_NOCPERR		= 0;
	debugTrap->VC_MMERR			= 0;
	debugTrap->VC_CORERESET		= 0;
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
	if ( check_BitSetWithMask(dataRead,DHCSR_C_DEBUGEN_MASK) == ERR_NOERROR )
		coreStatus-> C_DEBUGEN = 1 ;
	else
		coreStatus-> C_DEBUGEN = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_C_HALT_MASK) == ERR_NOERROR  )
		coreStatus-> C_HALT = 1 ;
	else
		coreStatus-> C_HALT = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_C_STEP_MASK) == ERR_NOERROR )
		coreStatus-> C_STEP = 1 ;
	else
		coreStatus-> C_STEP = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_C_MASKINTS_MASK) == ERR_NOERROR )
		coreStatus-> C_MASKINTS  = 1 ;
	else
		coreStatus-> C_MASKINTS = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_C_SNAPSTALL_MASK) == ERR_NOERROR )
		coreStatus-> C_SNAPSTALL = 1 ;
	else
		coreStatus-> C_SNAPSTALL = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_S_REGRDY_MASK) == ERR_NOERROR )
		coreStatus-> S_REGRDY = 1 ;
	else
		coreStatus-> S_REGRDY = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_S_HALT_MASK) == ERR_NOERROR )
		coreStatus-> S_HALT = 1 ;
	else
		coreStatus-> S_HALT = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_S_SLEEP_MASK) == ERR_NOERROR )
		coreStatus-> S_SLEEP = 1 ;
	else
		coreStatus-> S_SLEEP = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_S_LOCKUP_MASK) == ERR_NOERROR )
		coreStatus-> S_LOCKUP = 1 ;
	else
		coreStatus-> S_LOCKUP = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_S_RETIRE_MASK) == ERR_NOERROR )
		coreStatus-> S_RETIRE = 1 ;
	else
		coreStatus-> S_RETIRE = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DHCSR_S_RESET_MASK) == ERR_NOERROR )
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
	if ( check_BitSetWithMask(dataRead,DFSR_EXTERNAL_MASK) == ERR_NOERROR)
		debugEvent->EXTERNAL = 1; 
	else
		debugEvent->EXTERNAL = 0; 
	
	if ( check_BitSetWithMask(dataRead,DFSR_VCATCH_MASK) == ERR_NOERROR)
		debugEvent->VCATCH = 1; 
	else
		debugEvent->VCATCH = 0; 
	
	if ( check_BitSetWithMask(dataRead,DFSR_DWTTRAP_MASK) == ERR_NOERROR)
		debugEvent->DWTTRAP = 1; 
	else
		debugEvent->DWTTRAP = 0;
	
	if ( check_BitSetWithMask(dataRead,DFSR_BKPT_MASK) == ERR_NOERROR)
		debugEvent->BKPT = 1;
	else
		debugEvent->BKPT = 0;
	
	if ( check_BitSetWithMask(dataRead,DFSR_HALTED_MASK) == ERR_NOERROR)
		debugEvent->HALTED = 1;	
	else
		debugEvent->HALTED = 0;	
	

}

/**
 *	Use to update the DebugTrap structure with the data received
 *	
 *	Input : debugTrap is a pointer to DebugTrap which store whether the debug trap is enabled/disabled for example debug trap on HARDFAULT VC_HARDERR
 *			dataRead contain the data read from Debug Exception and Monitor Control Register
 *	
 */
void update_DebugTrapStatus(DebugTrap *debugTrap, uint32_t dataRead)
{
	if ( check_BitSetWithMask(dataRead,DEMCR_VC_HARDERR_MASK) == ERR_NOERROR)
		debugTrap-> VC_HARDERR = 1 ;
	else
		debugTrap-> VC_HARDERR = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DEMCR_VC_INTERR_MASK) == ERR_NOERROR)
		debugTrap-> VC_INTERR = 1 ;
	else
		debugTrap-> VC_INTERR = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DEMCR_VC_BUSERR_MASK) == ERR_NOERROR)
		debugTrap-> VC_BUSERR = 1 ;
	else
		debugTrap-> VC_BUSERR = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DEMCR_VC_STATERR_MASK) == ERR_NOERROR)
		debugTrap-> VC_STATERR  = 1 ;
	else
		debugTrap-> VC_STATERR = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DEMCR_VC_CHKERR_MASK) == ERR_NOERROR)
		debugTrap-> VC_CHKERR = 1 ;
	else
		debugTrap-> VC_CHKERR = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DEMCR_VC_NOCPERR_MASK) == ERR_NOERROR)
		debugTrap-> VC_NOCPERR = 1 ;
	else
		debugTrap-> VC_NOCPERR = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DEMCR_VC_MMERR_MASK) == ERR_NOERROR)
		debugTrap-> VC_MMERR = 1 ;
	else
		debugTrap-> VC_MMERR = 0 ;
	
	if ( check_BitSetWithMask(dataRead,DEMCR_VC_CORERESET_MASK) == ERR_NOERROR)
		debugTrap-> VC_CORERESET = 1 ;
	else
		debugTrap-> VC_CORERESET = 0 ;
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
 *	Use to get the value of the data going to be written into Debug Exception and Monitor Control Register, DEMCR
 *	
 *	Input : debugMonitorControl is used to control the behaviour of Debug Monitor in ARM
 *				Possible input value :
 *					DebugMonitor_DISABLED  	Disable debug monitor
 *					DebugMonitor_ENABLED	Enable debug monitor	
 *					DebugMonitor_STEP		Enable stepping in debug monitor
 *			debugTrap is a point to DebugTrap which store whether the debug trap is enabled/disabled for example debug trap on HARDFAULT VC_HARDERR
 *	
 *	Output : return the 32bits data to be written into Debug Exception and Monitor Control Register, DEMCR
 */
uint32_t get_DebugExceptionMonitorControl_WriteValue(DebugMonitorControl debugMonitorControl,DebugTrap *debugTrap,int enable_DWT_ITM)
{
	uint32_t data = 0 ;
	
	switch(debugMonitorControl)
	{
		case DebugMonitor_DISABLE :
									data = 0 ;
									break ;
										
		case DebugMonitor_ENABLE :
									data = 0x1000;
									break;
		case DebugMonitor_STEP :		
									data = 0x5000;
									break ;		
		
		default : break ;
	}
	
	if (enable_DWT_ITM == ENABLE_DWT_ITM)
		data += 0x1000000 ;
	
	data += debugTrap->VC_HARDERR	<< Bit_10 ;
	data += debugTrap->VC_INTERR	<< Bit_9  ;
	data += debugTrap->VC_BUSERR	<< Bit_8  ;
	data += debugTrap->VC_STATERR	<< Bit_7  ;
	data += debugTrap->VC_CHKERR	<< Bit_6  ;
	data += debugTrap->VC_NOCPERR	<< Bit_5  ;
	data += debugTrap->VC_MMERR		<< Bit_4  ;
	data += debugTrap->VC_CORERESET 		  ;
	
	return data ;
}

/**
 *	Use to get the value of the data going to be written into Debug Fault Status Register, DFSR to clear debug event
 *	Set the member of DebugEvent to clear 
 *	
 *	Input : debugEvent contains the debug event which is going to be cleared
 *	
 *	Output : return the 32bits data to be written into Debug Fault Status Register, DFSR to clear debug event
 */
uint32_t get_ClearDebugEvent_WriteValue(DebugEvent *debugEvent)
{
	uint32_t data = 0 ;
	
	data += debugEvent->EXTERNAL	<< Bit_4 ;
	data += debugEvent->VCATCH 		<< Bit_3 ;
	data += debugEvent->DWTTRAP 	<< Bit_2 ;
	data += debugEvent->BKPT 		<< Bit_1 ;
	data += debugEvent->HALTED 				 ;
	
	return data ;
}