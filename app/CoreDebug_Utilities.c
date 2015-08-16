#include "CoreDebug_Utilities.h"

/**
 *  Use to check whether the selected CoreMode requires the processor to be in halted and debug state 
 *
 *  Input : mode is the CoreMode to be applied to the core
 *				  Possible value :
 *					  CORE_NORMAL_MODE				    Normal operation mode without masking of PendSV,SysTick and external configurable interrupts
 *					  CORE_NORMAL_MASKINT				  Normal operation mode with masking of PendSV,SysTick and external configurable interrupts
 *					  CORE_DEBUG_MODE 				    Enable debug mode
 *					  CORE_DEBUG_HALT					    Enable halting debug mode
 *					  CORE_SINGLE_STEP				    Enable processor single stepping without masking of PendSV,SysTick and external configurable interrupts
 *					  CORE_SINGLE_STEP_MASKINT		Enable processor single stepping with masking of PendSV,SysTick and external configurable interrupts
 *				  	CORE_SNAPSTALL					    Force to enter imprecise debug mode (Used when processor is stalled )
 *  
 *  Output :  return 1 for true
 *            return 0 for false
 *
 */
int isCoreModeRequiresHaltedAndDebug(CoreMode mode)
{
  if(mode == CORE_NORMAL_MASKINT || mode == CORE_SINGLE_STEP || mode == CORE_SINGLE_STEP_MASKINT)
    return 1;
  
  return 0 ;
}


/**
 *  Determine the current Core Mode by passing the data read from Debug Halting Control and Status Register, DHCSR
 *
 *  Input :   dataRead contains the data read from Debug Halting Control and Status Register, DHCSR
 *  Output :  return the current CoreMode based on the dataRead
 *            Possible return value :
 *              CORE_NORMAL_MODE
 *              Core_NORMAL_MASKINT
 *              CORE_DEBUG_MODE
 *              CODE_DEBUG_HALT
 *              CORE_SINGLE_STEP
 *              CORE_SINGLE_STEP_MASKINT
 *              CORE_SNAPSTALL
 *              -1                          unknown/undefined CoreMode
 */
CoreMode determineCoreModeFromDataRead(uint32_t dataRead)
{
  int debugEnableBit = 0 , haltBit = 0 , stepBit = 0 , maskIntBit = 0 , haltedStatusBit = 0 , snapStallBit = 0 ;

  debugEnableBit    =  dataRead & DHCSR_C_DEBUGEN_MASK ;
  haltBit           = (dataRead & DHCSR_C_HALT_MASK)      >> 1 ;
  stepBit           = (dataRead & DHCSR_C_STEP_MASK)      >> 2 ;
  maskIntBit        = (dataRead & DHCSR_C_MASKINTS_MASK)  >> 3 ;
  snapStallBit      = (dataRead & DHCSR_C_SNAPSTALL_MASK) >> 4 ;
  haltedStatusBit   = (dataRead & DHCSR_S_HALT_MASK)      >> 16 ;

  if ( !(debugEnableBit) && !(haltBit) && !(stepBit) && !(maskIntBit) && !(snapStallBit) && !(haltedStatusBit) )
    return CORE_NORMAL_MODE ;

  if ( !(debugEnableBit) && !(haltBit) && !(stepBit) &&  (maskIntBit) && !(snapStallBit) && !(haltedStatusBit) )
    return CORE_NORMAL_MASKINT ;

  if (  (debugEnableBit) && !(haltBit) && !(stepBit) && !(maskIntBit) && !(snapStallBit) && !(haltedStatusBit) )
    return CORE_DEBUG_MODE ;

  if (  (debugEnableBit) &&  (haltBit) && !(stepBit) && !(maskIntBit) && !(snapStallBit) &&  (haltedStatusBit) )
    return CORE_DEBUG_HALT ;

  if (  (debugEnableBit) &&  (haltBit) &&  (stepBit) && !(maskIntBit) && !(snapStallBit) &&  (haltedStatusBit) )
    return CORE_SINGLE_STEP ;

  if (  (debugEnableBit) &&  (haltBit) &&  (stepBit) &&  (maskIntBit) && !(snapStallBit) &&  (haltedStatusBit) )
    return CORE_SINGLE_STEP_MASKINT ;

  if (  (debugEnableBit) &&  (haltBit) && !(stepBit) && !(maskIntBit) &&  (snapStallBit) &&  (haltedStatusBit) )
    return CORE_SNAPSTALL ;

  return -1 ;
}

/**
 *	Use to get the configuration data going to be written into Debug Halting and Control Status Register based on CoreMode
 *
 *	Input : mode is the desired core mode to be set
 *				     Possible value :
 *					      CORE_NORMAL_MODE				    Normal operation mode without masking of PendSV,SysTick and external configurable interrupts
 *					      CORE_NORMAL_MASKINT				  Normal operation mode with masking of PendSV,SysTick and external configurable interrupts
 *					      CORE_DEBUG_MODE 				    Enable debug mode
 *					      CORE_DEBUG_HALT					    Enable halting debug mode
 *					      CORE_SINGLE_STEP				    Enable processor single stepping without masking of PendSV,SysTick and external configurable interrupts
 *					      CORE_SINGLE_STEP_MASKINT		Enable processor single stepping with masking of PendSV,SysTick and external configurable interrupts
 *					      CORE_SNAPSTALL					    Force to enter imprecise debug mode (Used when processor is stalled )
 *
 *	Output : return the 32bits of configuration data to be written into Debug Halting and Control Status Register
 */
uint32_t getCoreModeConfiguration(CoreMode mode)
{
	uint32_t data = 0 ;

	switch(mode)
	{
		case CORE_NORMAL_MODE			    :
                                    data = SET_CORE_NORMAL;
                                    break ;
		case CORE_NORMAL_MASKINT		  :
                                    data = SET_CORE_NORMAL_MASKINT ;
                                    break ;
		case CORE_DEBUG_MODE 			    :
                                    data = SET_CORE_DEBUG ;
                                    break ;
		case CORE_DEBUG_HALT			    :
                                    data = SET_CORE_DEBUG_HALT ;
                                    break ;
		case CORE_SINGLE_STEP	        :
                                    data = SET_CORE_STEP ;
                                    break ;
		case CORE_SINGLE_STEP_MASKINT	:
                                    data = SET_CORE_STEP_MASKINT ;
                                    break ;
		case CORE_SNAPSTALL				    :
                                    data = SET_CORE_SNAPSTALL ;
                                    break ;
		default : break ;
	}

	return data ;
}

/**
 *	Use to get the configuration data going to be written into Debug Fault Status Register to clear the selected debug event
 *
 *  Input :   debugEvent is the debug event going to be checked
 *				    Possible value :
 *					    EXTERNAL_DEBUGEVENT       Clear external debug request debug event				    
 *					    VCATCH_DEBUGEVENT				  Clear vector catch triggered debug event
 *					    DWTTRAP_DEBUGEVENT 				Clear data watchpoint & trace unit debug event
 *					    BKPT_DEBUGEVENT					  Clear breakpoint debug event
 *					    HALTED_DEBUGEVENT				  Clear halt request debug event
 *
 *	Output : return the 32bits of configuration data to be written into Debug Fault Status Register 
 */
uint32_t getClearDebugEventConfiguration(DebugEvent debugEvent)
{
  uint32_t data = 0 ;
  
  switch(debugEvent)
  {
    case EXTERNAL_DEBUGEVENT  : 
                                data = CLEAR_EXTERNAL_EVENT ;                            
                                break ;
    case VCATCH_DEBUGEVENT    :
                                data = CLEAR_VCATCH_EVENT ;                              
                                break ;
    case DWTTRAP_DEBUGEVENT   :
                                data = CLEAR_DWTTRAP_EVENT ;                               
                                break ;
    case BKPT_DEBUGEVENT      :
                                data = CLEAR_BKPT_EVENT ;        
                                break ;
    case HALTED_DEBUGEVENT    :
                                data = CLEAR_HALTED_EVENT ; 
                                break ; 
    default :             
                                break ;
  }
  
  return data ;
}