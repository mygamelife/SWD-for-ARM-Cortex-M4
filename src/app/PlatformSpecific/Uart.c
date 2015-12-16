#include "uart.h"

UART_HandleTypeDef uartHandle;
UART_HandleTypeDef swoHandle;

static __IO ITStatus uartTxReady = 1;
static __IO ITStatus uartRxReady = 1;
static __IO ITStatus swoReady = 1;

static void uartHighLevelInit(  UART_HandleTypeDef *huart, 
                                USART_TypeDef *uartPort,
                                uint32_t baudRate,
                                uint32_t stopBit
                              );

/* Do not modify isRxBusy and isTxBusy this two function contain
   the important static variable modify by Uart Interrupt and use
   by outside of this header */
int isRxBusy(void) {
  return uartRxReady;
}

int isTxBusy(void) {
  return uartRxReady;
}

void uartConfigGpio(GPIO_TypeDef *gpiox, uint32_t pin, uint32_t altFunction) {
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*#### Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  UART_CLK_ENABLE();
  /* Enable SWO TX/RX clock (UART5) */
  SWO_CLK_ENABLE();

  /*##### Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = pin;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = altFunction;

  HAL_GPIO_Init(gpiox, &GPIO_InitStruct);
}

static void uartHighLevelInit(  UART_HandleTypeDef *huart, 
                                USART_TypeDef *uartPort,
                                uint32_t baudRate,
                                uint32_t stopBit
                              ) {
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  huart->Instance          = uartPort;

  huart->Init.BaudRate     = baudRate;
  huart->Init.WordLength   = UART_WORDLENGTH_8B;
  huart->Init.StopBits     = stopBit;
  huart->Init.Parity       = UART_PARITY_NONE;
  huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart->Init.Mode         = UART_MODE_TX_RX;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(huart) != HAL_OK)
  { uartErrorHandler(); }
}

/**  initUart is a function to configure the UART peripheral
  *
  */
void uartInit(void **huart) {
  /*##-1- Configure UART TX #################################################*/
  uartConfigTx(UART_TX_GPIO_PORT, UART_TX_PIN, UART_TX_AF);
  /*##-2- Configure UART RX #################################################*/
  uartConfigRx(UART_RX_GPIO_PORT, UART_RX_PIN, UART_RX_AF);
  /*##-3- Configure the NVIC for UART ########################################*/
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /*##-4- Init Uart structure in high level #################################*/
  uartHighLevelInit(&uartHandle, UART_PORT, UART_BAUD_RATE, UART_STOPBITS_1);

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

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete*/
  uartTxReady = 1;
  turnOffLED3();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  turnOffLED3();
  /* Set the respective Port Interrupt Call: receive complete*/
  if(UartHandle->Instance == UART_PORT) {
    uartRxReady = 1;
  }
  else if(UartHandle->Instance == SWO_PORT) {
    swoReady = 1;
  }
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

/**
  ==============================================================================
            ######## Configure UART5 to receive SWO output ########
  ==============================================================================
  */
void swoInit(void **hswo) {
  /*##-1- Configure UART TX #################################################*/
  // uartConfigTx(UART_TX_GPIO_PORT, UART_TX_PIN, UART_TX_AF);
  /*##-2- Configure UART RX #################################################*/
  uartConfigRx(SWO_RX_PORT, SWO_RX_PIN, UART5_RX_AF);
  /*##-3- Configure the NVIC for UART ########################################*/
  HAL_NVIC_SetPriority(SWO_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(SWO_IRQn);
  /*##-4- Init Uart structure in high level #################################*/
  uartHighLevelInit(&swoHandle, SWO_PORT, SWO_BAUD_RATE, UART_STOPBITS_2);

  *hswo = &swoHandle;
}

uint8_t getSwoBytes(void *handler, uint8_t *rxBuffer, int length) {
  turnOnLED3();

  if(HAL_UART_Receive_IT((UART_HandleTypeDef *)handler, rxBuffer, length) == HAL_OK) {
    swoReady = 0;
    return UART_OK;
  }

  return UART_BUSY;
}

int isSwoDataReady(void) {
  return swoReady;
}
