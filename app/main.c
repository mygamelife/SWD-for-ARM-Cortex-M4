#include "main.h"
int main(void)
{
 	uint32_t errorFlag  = 0 ;
 	uint32_t dataRead = 0 ;
 	
	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	errorFlag = swdCheckErrorFlag() ;
	swdClearErrorFlagInAbort(errorFlag); //Clear error flag

	readAhbIDR(&dataRead);

	while(1)
	{
	}

}

