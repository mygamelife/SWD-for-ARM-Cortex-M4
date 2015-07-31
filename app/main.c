#include "swdProtocol.h"
#include "Register_ReadWrite.h"
int main(void)
{
	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	while(1)
	{
		
	}
}