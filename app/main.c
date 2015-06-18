#include "swdProtocol.h"

int main(void)
{
	int ack = 0, parity = 0;
	uint32_t ctrlStatusRegData = 0;

	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	while(1)
	{
		writeDataToSelectReg(WDERRCLR);
		/**********READ CTRL/STAT*****************/
		ctrlStatusReg(READ);
		SWDIO_InputMode();
		turnAround();

		read3bit(&ack);
		read32bit(&ctrlStatusRegData);
		parity = readBit();

		SWDIO_OutputMode();
		turnAround();
		extraIdleClock(8);
		/**********READ CTRL/STAT*****************/
	}

}
