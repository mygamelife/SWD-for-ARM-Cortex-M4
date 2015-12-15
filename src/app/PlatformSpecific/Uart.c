#include "uart.h"

UART_HandleTypeDef uartHandle;

static __IO ITStatus uartTxReady = 1;
static __IO ITStatus uartRxReady = 1;

int isRxBusy(void) {
  return uartRxReady;
}

int isTxBusy(void) {
  return uartRxReady;
}

/**  initUart is a function to configure the UART peripheral
  *
  */
void uartInit(void **huart) {
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
  
  *huart = &uartHandle;
}

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length) {
  turnOnLED3();
  
  if(HAL_UART_Transmit_IT((UART_HandleTypeDef *)handler, txBuffer, length) == HAL_OK) {
    uartTxReady = 0;
    return UART_OK;
  }
  return UART_BUSY;
}

/* Uart Receive Function */
uint8_t getByte(void *handler, uint8_t *rxBuffer) {
  turnOnLED3();
  
  if(HAL_UART_Receive((UART_HandleTypeDef *)handler, rxBuffer, 1, 20) == HAL_OK)
    return UART_OK;
  
  return UART_BUSY;
}

uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length) {
  turnOnLED3();

  if(HAL_UART_Receive_IT((UART_HandleTypeDef *)handler, rxBuffer, length) == HAL_OK) {
    uartRxReady = 0;
    return UART_OK;
  }
  return UART_BUSY;
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
  uartTxReady = 1;
  turnOffLED3();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set receive flag: receive complete*/
  uartRxReady = 1;
  turnOffLED3();
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
  turnOnLED4();
  while(1) {}
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

void cancelRx(void) {
  /* Disable the UART Receive Interrupt */
  __HAL_UART_DISABLE_IT(&uartHandle, UART_IT_RXNE);

  /* Check if a transmit process is ongoing or not */
  if(uartHandle.State == HAL_UART_STATE_BUSY_TX_RX)
  {
    uartHandle.State = HAL_UART_STATE_BUSY_TX;
  }
  else
  {
    /* Flush Data Register */
    __HAL_UART_FLUSH_DRREGISTER(&uartHandle);
    /* Disable the UART Parity Error Interrupt */
    __HAL_UART_DISABLE_IT(&uartHandle, UART_IT_PE);
    /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_DISABLE_IT(&uartHandle, UART_IT_ERR);

    uartHandle.State = HAL_UART_STATE_READY;
  }
  HAL_UART_RxCpltCallback(&uartHandle);
}
