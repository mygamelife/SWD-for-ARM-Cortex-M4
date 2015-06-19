#include "swdProtocol.h"

int main(void)
{
	int ack = 0, parity = 0;
	uint32_t ctrlStatusRegData = 0;

	configure_IOPorts();
	//resetTarget();

	SWD_Initialisation();

	/**********READ CTRL/STAT*****************/
	ctrlStatusReg(READ);

	turnAround_ToRead();
	SWDIO_InputMode();

	read3bit(&ack);
	read32bit(&ctrlStatusRegData);
	parity = readBit();

	turnAround_ToWrite();
	SWDIO_OutputMode();

	extraIdleClock(8);
	/**********READ CTRL/STAT*****************/

	writeDataToAbortReg(WDERRCLR);

	/**********READ CTRL/STAT*****************/
	ctrlStatusReg(READ);

	turnAround_ToRead();
	SWDIO_InputMode();

	read3bit(&ack);
	read32bit(&ctrlStatusRegData);
	parity = readBit();

	turnAround_ToWrite();
	SWDIO_OutputMode();

	extraIdleClock(8);
	/**********READ CTRL/STAT*****************/

	while(1)
	{
	}

}
