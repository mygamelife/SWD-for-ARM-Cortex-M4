#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "configurePort.h"
#include "Bit_ReadSend.h"
#include "swdProtocol.h"

int main(void)
{
	long ACK =0;
	long IDCODE=0 ;
	long SWD_RequestData = 0 ;

	configureClock();
	SWDIO_OutputMode();
	SWD_RequestData = SWD_Request(DP,READ,0x00);

	resetTarget();
	lineReset();
	sendBits(0xE79E,16);
	lineReset();
	sendBits(SWD_RequestData,8);
	clockGenerator_1cycle();
	SWDIO_InputMode();
	clockGenerator_1cycle();
	readBits(&ACK,3);
	readBits(&IDCODE,32);
	resetTarget();
	while(1)
	{
	}

}
