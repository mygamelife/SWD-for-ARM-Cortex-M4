#include "swdProtocol.h"
#include "configurePort.h"
#include "Register_ReadWrite.h"
#include "CoreDebug.h"
#include "FPB_Unit.h"
#include "DWT_Unit.h"
int main(void)
{
 	uint32_t errorFlag  = 0 ;
 	
	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	errorFlag = swdCheckErrorFlag() ;
	swdClearErrorFlagInAbort(errorFlag); //Clear error flag

	powerUpSystemAndDebug();



	while(1)
	{
	}

}

