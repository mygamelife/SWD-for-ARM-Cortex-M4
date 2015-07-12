#include "swdProtocol.h"
#include "Register_ReadWrite.h"
#include "CoreDebug.h"
#include "FPB_Unit.h"

int main(void)
{
 	int ack = 0, parity = 0;
 	uint32_t errorFlag = 0, dataRead = 0 ;
 	CoreStatus coreStatus;
 	DebugEvent debugEvent;


	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	errorFlag = swdCheckErrorFlag() ;
	swdWriteAbort(&ack, errorFlag); //Clear error flag
	swdWriteSelect(&ack, BANK_0);
	
	powerUpSystemAndDebug();
	SWDRegister_Write(CSW_REG,AP,&ack, 0x23000042);

	setCore(CORE_DEBUG_MODE,&coreStatus);

//	enable_FPB();

//	set_InstructionBKPT(InstructionCOMP_0,0x80002ee,Match_32bits);

	write_CoreRegister(ARM_R0,&coreStatus,0xA55A);
	read_CoreRegister(ARM_R0,&coreStatus,dataRead);
	while(1)
	{

	}

}
