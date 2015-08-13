#include "main.h"

int main(void)
{
	TLVProbe_TypeDef tlvProbe;

    /* Configure Hardware */
    HAL_Init();
    SystemClock_Config();

    /* UART port and setting */
    configureLED();
    configureUartPorts();
    tlvProbe.state = TLV_INITIATE;
    tlvProbe.uartHandle = initUart();

	//UART_HandleTypeDef *uartHandle = initUart();
	//stm32UartSendByte(uartHandle, PROBE_OK);

	while(tlvProbe.state != TLV_END)	{
		tlvWriteToTargetRam(&tlvProbe);
	}

	while(1)
	{

	}
}
