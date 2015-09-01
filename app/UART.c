#include "uart.h"

/**  initUart is a function to configure the UART peripheral
  *
  */
UART_HandleTypeDef *uartInit(void) {
  static UART_HandleTypeDef uartHandle;

  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  uartHandle.Instance          = USARTx;
  
  uartHandle.Init.BaudRate     = USART_BAUD_RATE;
  uartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  uartHandle.Init.StopBits     = UART_STOPBITS_1;
  uartHandle.Init.Parity       = UART_PARITY_NONE;
  uartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  uartHandle.Init.Mode         = UART_MODE_TX_RX;
  uartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    
  if(HAL_UART_Init(&uartHandle) != HAL_OK)
  {
	  uartErrorHandler();
  }
  
  return &uartHandle;
}

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length) {
  UART_HandleTypeDef *uartHandle = (UART_HandleTypeDef *)handler;
  
  return HAL_UART_Transmit(uartHandle, txBuffer, length, FIVE_SEC);
}

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length)  {
  UART_HandleTypeDef *uartHandle = (UART_HandleTypeDef *)handler;
  
  return HAL_UART_Receive(uartHandle, rxBuffer, length, TEN_SEC);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void uartErrorHandler(void)
{
  /* Turn LED4 on */
  turnOnLED4();
  while(1)  {}
}
