#include "swdProtocol.h"
#include "Register_ReadWrite.h"

int main(void)
{
	int ack = 0, parity = 0;

	uint32_t dummyRead = 0 ;

	uint32_t ctrlStatusRegData = 0x50000000  ,CTRLSTAT_READDATA =0 ;
	uint32_t CSW_data = 0x22000002 ;
	uint32_t TAR_data = 0xE000EDF0 ;
	uint32_t DRW_data = 0xA05F0003 ;
	uint32_t DP_Select_data = 0x0 ;

	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	resetTarget();
	while(1)
	{

	}

	while(1)
	{

		writeDataToSelectReg(WDERRCLR);
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
	}

}
