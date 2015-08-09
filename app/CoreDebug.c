#include "CoreDebug.h"

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
  uint32_t configData = 0 ;
  
  configData = getCoreModeConfiguration(mode);
  
  if(isCoreModeRequiresHaltedAndDebug(mode))
    setCoreMode(CORE_DEBUG_HALT);
  
  memoryWriteWord(DHCSR_REG,configData);
    
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
 *			dataRead is the pointer to where the data read from the core register is going to be stored
 */
void readCoreRegister(CoreRegister coreRegister,uint32_t *dataRead)
{
	uint32_t configData = 0;
	
	configData = coreRegister + CORE_REG_READ ;
  
  setCoreMode(CORE_DEBUG_HALT);
		
  memoryWriteWord(DCRSR_REG,configData);
	waitForCoreRegisterTransactionToComplete();
	memoryReadWord(DCRDR_REG,dataRead);

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
		memoryReadWord(DHCSR,&dataRead) ;
    completed = (dataRead & DHCSR_S_REGRDY_MASK) >> 16;
  }while(completed != 1);

}

/**
 *	Use to check whether is the selected debug event occured 
 *	
 *  Input :   debugEvent is the debug event going to be checked
 *				    Possible value :
 *					    EXTERNAL_DEBUGEVENT       Check for external debug request debug event				    
 *					    VCATCH_DEBUGEVENT				  Check for vector catch triggered debug event
 *					    DWTTRAP_DEBUGEVENT 				Check for data watchpoint & trace unit debug event
 *					    BKPT_DEBUGEVENT					  Check for breakpoint debug event
 *					    HALTED_DEBUGEVENT				  Check for halt request debug event
 *
 *  Output :  return 1 if the selected debug event occured 
 *            return 0 if the selected debug event not occured 
 */
int isSelectedDebugEventOccured(DebugEvent debugEvent)
{
  int result = 0 ;
  uint32_t dataRead = 0 ; 
  
  memoryReadWord(DFSR_REG,&dataRead);
  
  switch(debugEvent)
  {
    case EXTERNAL_DEBUGEVENT  : 
                                if ((dataRead & DFSR_EXTERNAL_MASK) > 0)
                                  result = 1 ;                               
                                break ;
    case VCATCH_DEBUGEVENT    :
                                if ((dataRead & DFSR_VCATCH_MASK) > 0)
                                  result = 1 ;                             
                                break ;
    case DWTTRAP_DEBUGEVENT   :
                                if ((dataRead & DFSR_DWTTRAP_MASK) > 0)
                                  result = 1 ;                              
                                break ;
    case BKPT_DEBUGEVENT      :
                                if ((dataRead & DFSR_BKPT_MASK) > 0)
                                  result = 1 ;       
                                break ;
    case HALTED_DEBUGEVENT    :
                                if ((dataRead & DFSR_HALTED_MASK) > 0)
                                  result = 1 ;
                                break ; 
    default :
                                break ;
  }
  return result ;
}

/**
 *  Enable the selected vector catch 
 *  Note : Enabling the selected vector catch will override any other enabled vector catch
 *
 *  Input : vectorCatch is the vector catch going to be enabled
 *          Possible value :
 *            VC_DISABLEALL     Disable all vector catch
 *            VC_CORERESET      Enable reset vector catch
 *            VC_MMERR          Enable memory management exception vector catch
 *            VC_NOCPERR        Enable usage fault caused by access to Coprocessor vector catch
 *            VC_CHKERR         Enable usage fault exception casued by checking error vector catch
 *            VC_STATERR        Enable usage fault exception casued by state information error vector catch
 *            VC_BUSERR         Enable bus fault exception vector catch
 *            VC_INTERR         Enable interrupt/exception entry & return vector catch
 *            VC_HARDERR        Enable hard fault exception vector catch
 */
void enableSelectedVectorCatch(VectorCatch vectorCatch)
{
  memoryWriteHalfword(DEMCR_REG,vectorCatch);
}

/**
 *	Perform halt on reset which the system will be reset and halted on the first instruction
 *	
 */
void performHaltOnReset()
{
	setCoreMode(CORE_DEBUG_HALT);

	enableVectorCatchCoreReset();
  memoryWriteWord(AIRCR_REG,REQUEST_SYSTEM_RESET);
}

