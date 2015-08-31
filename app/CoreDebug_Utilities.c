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
int doesCoreModeRequiresHaltedAndDebug(CoreMode mode)
{
  printf("\nnormal %x\n",CORE_NORMAL_MODE);
  
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
  int debugEnableBit = 0 , stepBit = 0 , maskIntBit = 0 , haltedStatusBit = 0 , snapStallBit = 0 ;

  debugEnableBit    =  dataRead & CoreDebug_DHCSR_C_DEBUGEN_Msk ;
  stepBit           = (dataRead & CoreDebug_DHCSR_C_STEP_Msk)      >> CoreDebug_DHCSR_C_STEP_Pos ;
  maskIntBit        = (dataRead & CoreDebug_DHCSR_C_MASKINTS_Msk)  >> CoreDebug_DHCSR_C_MASKINTS_Pos ;
  snapStallBit      = (dataRead & CoreDebug_DHCSR_C_SNAPSTALL_Msk) >> CoreDebug_DHCSR_C_SNAPSTALL_Pos ;
  haltedStatusBit   = (dataRead & CoreDebug_DHCSR_S_HALT_Msk)      >> CoreDebug_DHCSR_S_HALT_Pos ;

  printf("NORMLAL %x\n",CORE_NORMAL_MODE);
  
  if (!debugEnableBit)
  {
    if(maskIntBit)
      return CORE_NORMAL_MASKINT ;
    
    return CORE_NORMAL_MODE ;
  }
  else
  {
    if(!haltedStatusBit)
      return CORE_DEBUG_MODE ;
    
    if(!stepBit)
      return CORE_DEBUG_HALT ;
    else
    {
      if(maskIntBit)
        return CORE_SINGLE_STEP_MASKINT ;
      
      return CORE_SINGLE_STEP ;
    }
    if(snapStallBit)
      return CORE_SNAPSTALL ;
  }
  
  return -1 ;
}
