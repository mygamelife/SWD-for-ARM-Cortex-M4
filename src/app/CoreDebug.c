#include "CoreDebug.h"

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
  if(mode == CORE_NORMAL_MASKINT || mode == CORE_SINGLE_STEP || mode == CORE_SINGLE_STEP_MASKINT)
    return 1;
  
  return 0 ;
}

/**
 *	Set the core to operate on different mode based on the passed in CoreMode
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
 */
void setCoreMode(CoreMode mode)
{
  if(doesCoreModeRequiresHaltedAndDebug(mode))
    setCoreMode(CORE_DEBUG_HALT);
  
  memoryWriteWord(DHCSR_REG,mode);
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

/**
 *	Get the current operating CoreMode of the processor 
 *	
 *  Output :  return the current operating CoreMode of the processor
 *            Possible return value :
 *              CORE_NORMAL_MODE            Currently in normal operation mode without masking of PendSV,SysTick and external configurable interrupts
 *              Core_NORMAL_MASKINT         Currently in normal operation mode with masking of PendSV,SysTick and external configurable interrupts
 *              CORE_DEBUG_MODE             Currently in debug mode
 *              CODE_DEBUG_HALT             Currently in halting debug mode
 *              CORE_SINGLE_STEP            
 *              CORE_SINGLE_STEP_MASKINT    
 *              CORE_SNAPSTALL              
 *              -1                          unknown/undefined CoreMode
 */
CoreMode getCoreMode()
{
  uint32_t dataRead = 0 ;
  
  memoryReadWord(DHCSR_REG,&dataRead); 
  return (determineCoreModeFromDataRead(dataRead));
}

/**
 *  Step n numbers of instruction as defined by the pass in paramenter
 *
 *  Input : nInstructions is the number of instructions going to be stepped
 */
void stepOnly(int nInstructions)
{
  int i = 0 ;
  
  for(i = 0 ; i < nInstructions ; i ++)
    setCoreMode(CORE_SINGLE_STEP);
}

/**
 *	Use to write data into the selected ARM Core Register
 *  Note : Will automatically set the processor to CORE_DEBUG_HALT 
 *	
 *	Input : coreRegister is the selected ARM Core register to be written
 *          Possible value :
 *          	CORE_REG_R0           ARM Core Register R0
 *            CORE_REG_R1           ARM Core Register R1       
 *            CORE_REG_R2           ARM Core Register R2
 *            CORE_REG_R3           ARM Core Register R3
 *                          ""
 *            CORE_REG_R9           ARM Core Register R9
 *            CORE_REG_R10          ARM Core Register R10
 *            CORE_REG_R11          ARM Core Register R11
 *            CORE_REG_R12          ARM Core Register R12
 *            CORE_REG_SP           Stack Pointer
 *            CORE_REG_LR           Link Register
 *            CORE_REG_PC           Program Counter
 *            CORE_REG_xPSR         xPSR
 *            CORE_REG_MSP          Main Stack Pointer
 *            CORE_REG_PSP          Process Stack Pointer
 *            CORE_REG_SR           Special Register
 *            CORE_REG_FPSCR        Floating Point Status and Control Register
 *            CORE_REG_FPREGS0      Floating Point Register 0
 *            CORE_REG_FPREGS1      Floating Point Register 1
 *            CORE_REG_FPREGS2      Floating Point Register 2
 *            CORE_REG_FPREGS3      Floating Point Register 3
 *            	             ""
 *            CORE_REG_FPREGS28     Floating Point Register 28
 *            CORE_REG_FPREGS29     Floating Point Register 29
 *            CORE_REG_FPREGS30     Floating Point Register 30
 *            CORE_REG_FPREGS31     Floating Point Register 31
 *			data is the data that going to be written into the selected ARM Core Register
 *
 */
void writeCoreRegister(CoreRegister coreRegister,uint32_t data)
{
	uint32_t configData = 0;
	configData = coreRegister + CORE_REG_WRITE ;
  
  setCoreMode(CORE_DEBUG_HALT);
  
	memoryWriteWord(DCRDR_REG,data);
	memoryWriteWord(DCRSR_REG,configData);
	
	waitForCoreRegisterTransactionToComplete();
}

/**
 *	Use to Read data from the selected ARM Core Register
 *  Note : Will automatically set the processor to CORE_DEBUG_HALT
 *	
 *	Input : coreRegister is the selected ARM Core register to be written
 *          Possible value :
 *          	CORE_REG_R0           ARM Core Register R0
 *            CORE_REG_R1           ARM Core Register R1       
 *            CORE_REG_R2           ARM Core Register R2
 *            CORE_REG_R3           ARM Core Register R3
 *                          ""
 *            CORE_REG_R9           ARM Core Register R9
 *            CORE_REG_R10          ARM Core Register R10
 *            CORE_REG_R11          ARM Core Register R11
 *            CORE_REG_R12          ARM Core Register R12
 *            CORE_REG_SP           Stack Pointer
 *            CORE_REG_LR           Link Register
 *            CORE_REG_PC           Program Counter
 *            CORE_REG_xPSR         xPSR
 *            CORE_REG_MSP          Main Stack Pointer
 *            CORE_REG_PSP          Process Stack Pointer
 *            CORE_REG_SR           Special Register
 *            CORE_REG_FPSCR        Floating Point Status and Control Register
 *            CORE_REG_FPREGS0      Floating Point Register 0
 *            CORE_REG_FPREGS1      Floating Point Register 1
 *            CORE_REG_FPREGS2      Floating Point Register 2
 *            CORE_REG_FPREGS3      Floating Point Register 3
 *            	             ""
 *            CORE_REG_FPREGS28     Floating Point Register 28
 *            CORE_REG_FPREGS29     Floating Point Register 29
 *            CORE_REG_FPREGS30     Floating Point Register 30
 *            CORE_REG_FPREGS31     Floating Point Register 31
 *
 *  Output  :		returns the data read
 */
uint32_t readCoreRegister(CoreRegister coreRegister)
{
	uint32_t configData = 0;
	uint32_t dataRead = 0 ;
  
	configData = coreRegister + CORE_REG_READ ;
  
  setCoreMode(CORE_DEBUG_HALT);
		
  memoryWriteWord(DCRSR_REG,configData);
	waitForCoreRegisterTransactionToComplete();
	memoryReadWord(DCRDR_REG,&dataRead);
  
  return dataRead ;
}

/**
 *	Wait for the transaction of reading/writing the core register to complete
 *	
 */
void waitForCoreRegisterTransactionToComplete()
{
  uint32_t dataRead = 0 ;
  int completed = 0 ;
	
  do
	{		
		memoryReadWord(DHCSR_REG,&dataRead) ;
    completed = (dataRead & CoreDebug_DHCSR_S_REGRDY_Msk) >> CoreDebug_DHCSR_S_REGRDY_Pos;
  }while(completed != 1);

}


/**
 *	Read DFSR register to be checked for which debug event occured later
 *
 *  Output : return data read from DFSR register
 */
uint32_t readDebugEventRegister()
{
  uint32_t dataRead = 0 ;
  
  memoryReadWord(DFSR_REG,&dataRead);
  
  return dataRead ;
}

/**
 *	Use to clear the selected debug event 
 *	
 *  Input :   debugEvent is the debug event going to be checked
 *				    Possible value :
 *					    EXTERNAL_DEBUGEVENT       Clear external debug request debug event				    
 *					    VCATCH_DEBUGEVENT				  Clear vector catch triggered debug event
 *					    DWTTRAP_DEBUGEVENT 				Clear data watchpoint & trace unit debug event
 *					    BKPT_DEBUGEVENT					  Clear breakpoint debug event
 *					    HALTED_DEBUGEVENT				  Clear halt request debug event
 *
 */
void clearDebugEvent(uint32_t debugEvent)
{
  memoryWriteWord(DFSR_REG,debugEvent);
}

/**
 *	Perform halt on reset which the system will be reset and halted on the first instruction
 *	
 */
void performHaltOnReset()
{
	setCoreMode(CORE_DEBUG_HALT);
	enableVectorCatchCoreReset();
  softResetTarget();
  disableVectorCatchCoreReset();
}