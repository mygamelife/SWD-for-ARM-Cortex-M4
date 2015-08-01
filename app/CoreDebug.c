#include "CoreDebug.h"

/**
 *	Set the Core to the User Defined Mode
 *	
 *	Input : coreControl is the mode to be applied to the core
 *				Possible value :
 *					CORE_DEBUG_MODE 		Enable debug mode
 *					CORE_DEBUG_HALT			Enable halting debug mode
 *					CORE_SINGLE_STEP_NOMASKINT_NOMASK		Enable processor single stepping
 *					CORE_MASK_INTERRUPT		Enable masking of PendSV,SysTick and external configurable interrupts
 *					CORE_SNAPSTALL			Force to enter imprecise debug mode (Used when processor is stalled )
 *
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *			 return ERR_CORE_CONTROL_FAILED if the core does not switch to the specified mode
 */
int setCore(CoreControl coreControl,CoreStatus *coreStatus)
{
	int status =  0 ;
	uint32_t data = 0;
	
	init_CoreStatus(coreStatus);

	data = get_Core_WriteValue(coreControl);
	
	status = setCore_Exception(coreControl,coreStatus);
	
	if (status != ERR_NOERROR) //Force quit if error occurs
		return status; 
			
	memoryAccessWrite(DHCSR_REG,data) ;

	status = check_CoreStatus(coreStatus);
	
	if (status != ERR_NOERROR)
		return status;

	status = isCore(coreControl,coreStatus);
	
	return status ;
}

/**
 *	Manages exception when setting CORE_SINGLE_STEP_NOMASKINT_NOMASK and CORE_MASK_INTERRUPT mode as the core must be already in CORE_DEBUG_HALT mode
 *	
 *	Input : coreControl is the mode to be applied to the core
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *			 return ERR_CORE_CONTROL_FAILED if the core does not switch to the specified mode
 */
int setCore_Exception(CoreControl coreControl,CoreStatus *coreStatus)
{	
	if (coreControl == CORE_SINGLE_STEP_NOMASKINT || coreControl == CORE_SINGLE_STEP_MASKINT || coreControl == CORE_MASK_INTERRUPT)
	{
		return(setCore(CORE_DEBUG_HALT,coreStatus));
	}
	else 
		return ERR_NOERROR ;
}

/**
 *	Check for the core status by reading DHCSR_REG and update the CoreStatus structure
 *	
 *	Input : coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 */
int check_CoreStatus(CoreStatus *coreStatus)
{
	int status =  0 ;
	
	uint32_t dataRead = 0 ;
	
	init_CoreStatus(coreStatus);
	
	status = memoryAccessRead(DHCSR_REG,&dataRead);
	update_CoreStatus(coreStatus,dataRead);
	
	return status ;
}

/**
 *	Check for the debug event occurred by reading DFSR_REG and update the DebugEvent structure
 *	
 *	Input :  debugEvent is a pointer to DebugEvent which store the event of the core occurred for example debug event generation of breakpoint BKPT
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 */
int check_DebugEvent(DebugEvent *debugEvent)
{
	int status =  0 ;
	
	uint32_t dataRead = 0 ;

	init_DebugEvent(debugEvent);
	
	status = memoryAccessRead(DFSR_REG,&dataRead);
	update_DebugEvent(debugEvent,dataRead);
	
	return status ;
}

/**
 *	Clear the debug event set in the debugEvent structure
 *	
 *	Input :  debugEvent is a pointer to DebugEvent which store the event going to be cleared (1 = going to be clear)
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 */
int clear_DebugEvent(DebugEvent *debugEvent)
{
	int status =  0 ;
	uint32_t data = 0 ;
	
	data = get_ClearDebugEvent_WriteValue(debugEvent);
	
	memoryAccessWrite(DFSR_REG,data);
	status = check_DebugEvent(debugEvent);
	
	return status ;
}

/**
 *	Check for enabled/activated vector catch by reading DEMCR_REG and update the debugTrap structure
 *	
 *	Input : debugTrap is a pointer to DebugTrap which store whether the vector catch is enabled/disabled for example debug trap on HARDFAULT VC_HARDERR
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 */
int check_DebugTrapStatus(DebugTrap *debugTrap)
{
	int status =  0 ;
	
	uint32_t dataRead = 0 ;
	
	init_DebugTrap(debugTrap);
	
	status = memoryAccessRead(DEMCR_REG,&dataRead);
	update_DebugTrapStatus(debugTrap,dataRead);
	
	return status ;
}

/**
 *	Check for enabled/activated vector catch by reading DEMCR_REG and update the debugTrap structure
 *	
 *	Input : debugTrap is a pointer to DebugTrap which store which vector catch is going to be disabled(1 = going to be disabled)
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 */
int clear_DebugTrap(DebugTrap *debugTrap)
{
	int status =  0 ;
	uint32_t data = 0 ;
	
	data = get_ClearDebugTrap_WriteValue(debugTrap);
	
	memoryAccessWrite(DEMCR_REG,data);
	status = check_DebugTrapStatus(debugTrap);

	return status ;
}

/**
 *	Perform write on the selected ARM Core Register
 *  Automatic set core to CORE_DEBUG_HALT before writing
 *	
 *	Input : coreRegister is the selected ARM Core register to be written
 *			coreStatus is a pointer to CoreStatus which store S_REGRDY to check for the transaction status
 *			data is the data that going to be written into the selected ARM Core Register
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 */
int write_CoreRegister(Core_RegisterSelect coreRegister,CoreStatus *coreStatus,uint32_t data)
{
	int status =  0 ; 
	uint32_t coreSelectData = 0,dataRead;
	
	coreSelectData =  get_CoreRegisterAccess_WriteValue(coreRegister,CoreRegister_Write);
	
	if(setCore(CORE_DEBUG_HALT,coreStatus)) 
	{
		memoryAccessWrite(DCRDR_REG,data);
		memoryAccessWrite(DCRSR_REG,coreSelectData);
	
		status = wait_CoreRegisterTransaction(coreStatus,10);
	}
	return status ;
}

/**
 *	Perform read on the selected ARM Core Register
 *  Automatic set core to CORE_DEBUG_HALT before reading
 *	
 *	Input : coreRegister is the selected ARM Core register to be read
 *			coreStatus is a pointer to CoreStatus which store S_REGRDY to check for the transaction status
 *			dataRead is a pointer to where the data read is stored
 *
 *	Output : return ERR_NOERROR if the operation completed successfully
 *           return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *			 return ERR_COREREGISTERRW_FAILED if transaction is not completed
 */
int read_CoreRegister(Core_RegisterSelect coreRegister,CoreStatus *coreStatus,uint32_t *dataRead)
{
	int status =  0 ; 
	uint32_t coreSelectData = 0;
	
	coreSelectData =  get_CoreRegisterAccess_WriteValue(coreRegister,CoreRegister_Read);
	
	if(setCore(CORE_DEBUG_HALT,coreStatus)) 
	{
		memoryAccessWrite(DCRSR_REG,coreSelectData);
	
		status = wait_CoreRegisterTransaction(coreStatus,10);
	
		memoryAccessRead(DCRDR_REG,dataRead);
	}
	return status ;
}

/**
 *	Loop for a certain amount of cycles and check for the ARM Core Register Read/Write transaction status
 *	
 *	Input : coreStatus is a pointer to CoreStatus which store S_REGRDY to check for the transaction status
 *			numberOfTries is the maximum number of tries to wait for the S_REGRDY to set 
 *
 *	Output : return ERR_NOERROR if transaction is complete
 *			 return ERR_INVALID_PARITY_RECEIVED if SWD received wrong data/parity
 *			 return ERR_COREREGRW_FAILED if transaction is not completed
 */
int wait_CoreRegisterTransaction(CoreStatus *coreStatus, int numberOfTries)
{
	int i = 0 ;
	
	if (numberOfTries <= 0 )
		numberOfTries = 10 ;
	
	do
	{		
		if(check_CoreStatus(coreStatus) == ERR_INVALID_PARITY_RECEIVED)
			return ERR_INVALID_PARITY_RECEIVED;
	
	}while(coreStatus->S_REGRDY != 1 && i != numberOfTries);
	
	if(coreStatus->S_REGRDY)
		return ERR_NOERROR;
	else
		return ERR_COREREGRW_FAILED ;
}

int configure_DebugExceptionMonitorControl(DebugMonitorControl debugMonitorControl,DebugTrap *debugTrap,int enable_DWT_ITM)
{
	int status = 0 ;
	uint32_t data = 0 ,dataRead = 0 ;
	
	data = get_DebugExceptionMonitorControl_WriteValue(debugMonitorControl,debugTrap,enable_DWT_ITM);
	
	memoryAccessWrite(DEMCR_REG,data);
	status = memoryAccessRead(DEMCR_REG,&dataRead);
	
	return status = 0 ;
}

int configure_DebugTrap(DebugTrap *debugTrap)
{
	return configure_DebugExceptionMonitorControl(DebugMonitor_DISABLE,debugTrap,DISABLE_DWT_ITM);
}
