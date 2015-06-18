#include "swdProtocol.h"
int main(void)
{
	configure_IOPorts();
	//resetTarget();

	while(1)
	{

		SWD_Initialisation();
	}

}
