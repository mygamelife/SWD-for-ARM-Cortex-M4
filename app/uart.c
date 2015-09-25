#include "uart.h"

UART_HandleTypeDef uartHandle;

__IO ITStatus uartReady = SET;

/**  initUart is a function to configure the UART peripheral
  *
  */
UART_HandleTypeDef *uartInit(void) {
  static UART_HandleTypeDef *pHandler;
  
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  uartHandle.Instance          = UART_PORT;
  
  uartHandle.Init.BaudRate     = UART_BAUD_RATE;
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
  
  pHandler = &uartHandle;
  return pHandler;
}

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length) {
  UART_HandleTypeDef *uartHandle = (UART_HandleTypeDef *)handler;
  
  return HAL_UART_Transmit_IT(uartHandle, txBuffer, length);
}

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length)  {
  UART_HandleTypeDef *uartHandle = (UART_HandleTypeDef *)handler;
  
  return HAL_UART_Receive(uartHandle, rxBuffer, length, 30);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  /* Enable USART1 clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete*/
  uartReady = SET;
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
