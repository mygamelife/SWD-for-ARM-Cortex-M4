#include "swdProtocol.h"

int main(void)
{
	int ack = 0, parity = 0;
	uint32_t ctrlStatusRegData = 0;

	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

  	/*************WRITE ABORT REGISTER*****************/
	writeAbortReg();

	SWDIO_InputMode();
	turnAround();

	read3bit(&ack);

	SWDIO_OutputMode();
	turnAround();

	send32bit(0x8); //clear WDERRCLR
	sendBit(1); //Parity bit

	extraIdleClock(8);
	/*************WRITE ABORT REGISTER*****************/

	/**********READ CTRL/STAT*****************/
	ctrlStatusReg(READ);
	//setCtrlStatusReg();
	SWDIO_InputMode();
	turnAround();


	read3bit(&ack);
	read32bit(&ctrlStatusRegData);
	parity = readBit();

	SWDIO_OutputMode();
	turnAround();

	extraIdleClock(3);
	/**********READ CTRL/STAT*****************/




	while(1)
	{
	}

}
