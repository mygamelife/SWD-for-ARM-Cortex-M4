#include "main.h"

int main(void)
{
	TLV_Session tlvSession;

    /* Configure Hardware */
    HAL_Init();
    SystemClock_Config();

    /* UART port and setting */
    configureLED();
    configureUartPorts();
    tlvSession.state = TLV_INITIATE;
    tlvSession.UartHandle = initUart();

	while(1)
	{
		tlvWriteToTargetRam(&tlvSession);
	}
}
