#include "swdProtocol.h"
#include "Register_ReadWrite.h"
#include "CoreDebug.h"
#include "FPB_Unit.h"
#include "DWT_Unit.h"
int main(void)
{
 	int ack = 0, i = 0;
 	uint32_t errorFlag, PC = 0 , R0_Reg = 0 ;
 	uint32_t dataRead = 0 ;

 	DebugExceptionMonitor debugExceptionMonitor ;
 	CoreStatus coreStatus;
 	DebugEvent debugEvent;
 	DebugTrap debugTrap ;
 	FPBInfo fpbInfo ;
 	DWTInfo dwtInfo;

 	init_DebugExceptionMonitor(&debugExceptionMonitor);
 	init_CoreStatus(&coreStatus);
 	init_FPBInfo(&fpbInfo);
 	init_DebugEvent(&debugEvent);
 	init_DebugTrap(&debugTrap);
 	init_DWTInfo(&dwtInfo);

	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	errorFlag = swdCheckErrorFlag() ;
	swdClearErrorFlagInAbort(errorFlag); //Clear error flag

	swdWriteSelect(&ack, BANK_0);

	powerUpSystemAndDebug();
	SWDRegister_Write(CSW_REG,AP,&ack, 0x23000042);


	debugEvent.DWTTRAP = 1;
	debugEvent.BKPT = 1 ;
	debugEvent.EXTERNAL = 1 ;
	debugEvent.HALTED = 1;
	debugEvent.VCATCH = 1;
	clear_DebugEvent(&debugEvent);

	disable_DWTComparator(&dwtInfo,DWT_COMP1);

	setCore(CORE_NORMAL_MODE,&coreStatus);
	prepare_DWTOperations(&dwtInfo,&coreStatus,&debugExceptionMonitor,&debugTrap,DebugMonitor_DISABLE);
	setWatchpoint_LinkedComparison(&dwtInfo,0xABCDEF01,Word,DWT_COMP2,0x2000045C,Ignore_Non,0,0,0,Write);

	while(debugEvent.DWTTRAP !=1)
	{
		check_DebugEvent(&debugEvent);
	}


	while(dwtInfo.dwtCompInfo[1]->dwtFunctionInfo->MATCHED != 1)
	{
		read_DWTFunction(&dwtInfo,DWT_FUNC1);
	}

	check_CoreStatus(&coreStatus);
	read_CoreRegister(DebugReturnAddress,&coreStatus,&PC);
	read_CoreRegister(ARM_R0,&coreStatus,&R0_Reg);

	memoryAccessRead(0x2000045c,&dataRead);

	disable_DWTComparator(&dwtInfo,DWT_COMP1);
	disable_DWTComparator(&dwtInfo,DWT_COMP2);
	debugEvent.DWTTRAP = 1;
	clear_DebugEvent(&debugEvent);
	configure_DebugExceptionMonitorControl(&debugExceptionMonitor,DebugMonitor_DISABLE,&debugTrap,DISABLE_DWT_ITM);
	setCore(CORE_NORMAL_MODE,&coreStatus);
	while(1)
	{

	}

}





/*
	write_CoreRegister(DebugReturnAddress,&coreStatus,0x080022D8);
	read_CoreRegister(DebugReturnAddress,&coreStatus,&PC_Read);
	delay(10000,1,1);
	setCore(CORE_NORMAL_MODE,&coreStatus);

	read_CoreRegister(ARM_R0,&coreStatus,&r0);
	read_CoreRegister(ARM_R1,&coreStatus,&r1);
	read_CoreRegister(ARM_R2,&coreStatus,&r2);
	read_CoreRegister(ARM_R3,&coreStatus,&r3);
	read_CoreRegister(ARM_R4,&coreStatus,&r4);


	write_CoreRegister(ARM_R0,&coreStatus,0x99999999);
	write_CoreRegister(ARM_R1,&coreStatus,0x88888888);
	write_CoreRegister(ARM_R2,&coreStatus,0x77777777);
	write_CoreRegister(ARM_R3,&coreStatus,0x66666666);
	write_CoreRegister(ARM_R4,&coreStatus,0xAAAAAAAA);

	read_CoreRegister(DebugReturnAddress,&coreStatus,&PC_Read1);
	write_CoreRegister(DebugReturnAddress,&coreStatus,0x080022DE);
	setCore(CORE_NORMAL_MODE,&coreStatus);

	read_CoreRegister(DebugReturnAddress,&coreStatus,&PC_Read2);
	read_CoreRegister(ARM_R0,&coreStatus,&Reg0);
	read_CoreRegister(ARM_R1,&coreStatus,&Reg1);
	read_CoreRegister(ARM_R2,&coreStatus,&Reg2);
	read_CoreRegister(ARM_R3,&coreStatus,&Reg3);
	read_CoreRegister(ARM_R4,&coreStatus,&Reg4);

	setCore(CORE_NORMAL_MODE,&coreStatus);

	control_FPB(&fpbInfo,Enable);

	setCore(CORE_DEBUG_MODE,&coreStatus);
	set_InstructionBKPT(&fpbInfo,InstructionCOMP_0,0x080022DE,Match_Upper16bits);
	while(debugEvent.BKPT != 1)
	{
		if(i >10)
		{
			write_CoreRegister(DebugReturnAddress,&coreStatus,0x080022D8);
			setCore(CORE_DEBUG_MODE,&coreStatus);
		}
		check_DebugEvent(&debugEvent);
		i++ ;
	}

	read_CoreRegister(DebugReturnAddress,&coreStatus,&PC_Read3);
	//check_DebugEvent(&debugEvent);
	//clear_DebugEvent(&debugEvent);
	disable_FPComp(&fpbInfo,InstructionCOMP_0);
	setCore(CORE_NORMAL_MODE,&coreStatus);
	read_CoreRegister(DebugReturnAddress,&coreStatus,&PC_Read4);
	setCore(CORE_NORMAL_MODE,&coreStatus);
	check_DebugEvent(&debugEvent);


	reenable_FPComp(&fpbInfo,InstructionCOMP_0);
	write_CoreRegister(DebugReturnAddress,&coreStatus,0x080022D8);
	setCore(CORE_DEBUG_MODE,&coreStatus);

	read_CoreRegister(DebugReturnAddress,&coreStatus,&PC_Read5);

	disable_FPComp(&fpbInfo,InstructionCOMP_0);
	*/


/**
 *  Testing for vector catch
 *
	setCore(CORE_DEBUG_MODE,&coreStatus);

	check_DebugEvent(&debugEvent_Initial);
	memoryAccessWrite(DFSR_REG,0x8); //clear vector catch flag
	check_DebugEvent(&debugEvent_Cleared);

	configure_DebugTrap(&debugTrap);
	resetTarget();
	check_DebugEvent(&debugEvent_Final);
	check_CoreStatus(&coreStatus);
*/


/**
 *  Testing for breakpoint
 *
  	setCore(CORE_DEBUG_MODE,&coreStatus);

	enable_FPB();

	set_InstructionBKPT(InstructionCOMP_0,0x80002ee,Match_32bits);

	while(debugEvent.BKPT != 1)
	{
		check_DebugEvent(&debugEvent);
	}

	read_CoreRegister(DebugReturnAddress,&coreStatus,&Core_Reg);
*/

/* *
 * Testing core register RW
 *	write_CoreRegister(ARM_R0,&coreStatus,0x5AA51234);
	write_CoreRegister(ARM_R3,&coreStatus,0xABCDEF12);
	read_CoreRegister(ARM_R0,&coreStatus,&Core_Reg);

*/
