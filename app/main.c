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

	initialisation();

	resetTarget();
	while(1)
	{
	}

}
