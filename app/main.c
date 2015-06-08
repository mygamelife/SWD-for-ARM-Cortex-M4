#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "configurePort.h"
#include "Bit_ReadSend.h"
#include "swdProtocol.h"


int main(void)
{
	configure_IOPorts();

	initialisation();
	while(1)
	{
	}

}
