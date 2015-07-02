#include "swdProtocol.h"
#include "Register_ReadWrite.h"
#include "CoreDebug.h"
#include "FPB_Unit.h"

int main(void)
{
	int ack = 0, parity = 0;

	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	swdWriteSelect(&ack, BANK_0);
	SWDRegister_Write(CSW_REG,AP,&ack, 0x23000042);

	setCore_Halt();


	while(1)
	{
	}

}
