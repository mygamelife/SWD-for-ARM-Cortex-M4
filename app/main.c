#include "UART.h"
#include "TLV_Probe.h"
#include "swdProtocol.h"
#include "configurePort.h"
#include "SystemConfigure.h"
#include "Register_ReadWrite.h"

int main(void)
{
	uint8_t rxBuffer[1024] = {0}, received = 1;
	uint8_t txBuffer[] = {PROBE_OK};

	HAL_Init();
	configureUartPorts();
	configureLED();

	SystemClock_Config();

	static UART_HandleTypeDef UartHandle;

	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
	    - Word Length = 8 Bits
	    - Stop Bit = One Stop bit
	    - Parity = None
	    - BaudRate = 9600 baud
	    - Hardware flow control disabled (RTS and CTS signals) */
	UartHandle.Instance          = USARTx;

	UartHandle.Init.BaudRate     = 115200;
	UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits     = UART_STOPBITS_1;
	UartHandle.Init.Parity       = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode         = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		  errorHandler();
	}

	while(rxBuffer[0] != TLV_TRANSFER_COMPLETE)	{
		if(HAL_UART_Receive(&UartHandle, rxBuffer, sizeof(rxBuffer), 5000) == HAL_OK)	{
			if(HAL_UART_Transmit(&UartHandle, txBuffer, sizeof(txBuffer), 5000) != HAL_OK)	{
				errorHandler();
			}
		}
	}


	while(1)
	{
	}
}
