#include "swdProtocol.h"

int main(void)
{
	//int ack = 0, parity = 0;
	//uint32_t ctrlStatusRegData = 0;

	configure_IOPorts();
	//resetTarget();


	while(1)
	{
		SWD_Initialisation();
		/*
		requestToAccessCtrlStatusReg();
		//setCtrlStatusReg();
		turnAround();
		SWDIO_InputMode();

		read3bit(&ack);
		read32bit(&ctrlStatusRegData);
		parity = readBit();

		turnAround();
		SWDIO_OutputMode();
		extraIdleClock(3);*/
	}

}
