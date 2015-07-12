#include "CoreDebug.h"

/**
 *	Set the Core to the User Defined Mode
 *	
 *	Input : coreControl is the mode to be applied to the core
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *
 *	Output : return status where 1 = SUCCESS
 *                               0 = FAIL
 */
int setCore(CoreControl coreControl,CoreStatus *coreStatus)
{
	int status =  0 ;
	uint32_t data = 0 , dataRead = 0;
	
	data = get_Core_WriteValue(coreControl);
	
	if(!(setCore_Exception(coreControl,coreStatus))) // force quit if fail to handle exception	
		return status ;

	memoryAccessWrite(DHCSR_REG,data);
	
	check_CoreStatus(coreStatus);

	status = isCore(coreControl,coreStatus);
	
	return status ;
}

/**
 *	Manages exception when setting CORE_SINGLE_STEP and CORE_MASK_INTERRUPT mode as the core must be already in CORE_DEBUG_HALT mode
 *	
 *	Input : coreControl is the mode to be applied to the core
 *			coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *
 *	Output : return status where 1 = SUCCESS
 *                               0 = FAIL
 */
int setCore_Exception(CoreControl coreControl,CoreStatus *coreStatus)
{	
	if (coreControl == CORE_SINGLE_STEP || coreControl == CORE_MASK_INTERRUPT)
	{
		return(setCore(CORE_DEBUG_HALT,coreStatus));
	}
	else 
		return 1 ;
}

/**
 *	Check for the core status by reading DHCSR_REG and update the CoreStatus structure
 *	
 *	Input : coreStatus is a pointer to CoreStatus which store the information of the core for example processor HALT status S_HALT
 *
 */
void check_CoreStatus(CoreStatus *coreStatus)
{
	uint32_t dataRead = 0 ;
	
	init_CoreStatus(coreStatus);
	
	memoryAccessRead(DHCSR_REG,&dataRead);
	update_CoreStatus(coreStatus,dataRead);
}

/**
 *	Check for the debug event occurred by reading DFSR_REG and update the DebugEvent structure
 *	
 *	Input :  debugEvent is a pointer to DebugEvent which store the event of the core occurred for example debug event generation of breakpoint BKPT
 *
 */
void check_DebugEvent(DebugEvent *debugEvent)
{
	uint32_t dataRead = 0 ;

	init_DebugEvent(debugEvent);
	
	memoryAccessRead(DFSR_REG,&dataRead);
	update_DebugEvent(debugEvent,dataRead);
}

/**
 *	Check for the vectorCatch by reading DEMCR_REG and update the vectorCatch structure
 *	
 *	Input : vectorCatch is a pointer to VectorCatch which store whether the debug trap is enabled/disabled for example debug trap on HARDFAULT VC_HARDERR
 *
 */
void check_VectorCatch(VectorCatch *vectorCatch)
{
	uint32_t dataRead = 0 ;
	
	init_VectorCatch(vectorCatch);
	
	memoryAccessRead(DEMCR_REG,&dataRead);
	update_VectorCatch(vectorCatch,dataRead);
}

/**
 *	Perform write on the selected ARM Core Register
 *  Automatic set core to CORE_DEBUG_HALT before writing
 *	
 *	Input : coreRegister is the selected ARM Core register to be written
 *			coreStatus is a pointer to CoreStatus which store S_REGRDY to check for the transaction status
 *			data is the data that going to be written into the selected ARM Core Register
 *
 *	Output : return status where 1 = SUCCESS
 *                               0 = FAIL
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
 *	Output : return status where 1 = SUCCESS
 *                               0 = FAIL
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
 *	Output : return coreStatus->S_REGRDY bit where 1 =  TRANSACTION COMPLETE 
 *												   0 =  TRANSACTION IS NOT COMPLETE
 */
int wait_CoreRegisterTransaction(CoreStatus *coreStatus, int numberOfTries)
{
	int i = 0 ;
	
	if (numberOfTries <= 0 )
		numberOfTries = 10 ;
	
	do
	{		
		check_CoreStatus(coreStatus);
	
	}while(coreStatus->S_REGRDY != 1 && i != numberOfTries);
	
	return coreStatus->S_REGRDY ;
}
