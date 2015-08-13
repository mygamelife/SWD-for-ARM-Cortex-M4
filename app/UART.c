#include "UART.h"

/**  initUart is a function to configure the UART peripheral
  *
  */
UART_HandleTypeDef *initUart(void) {
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

/** stm32UartSendByte is a function sending byte data from stm32 uart port
  *
  * input     :   uartHandle is UART_HandleTypeDef pointer pointing to the uart configuration structure
  *               data is a single byte data need to transfer
  *
  * return    :   NONE
  */
void stm32UartSendByte(UART_HandleTypeDef *uartHandle, uint8_t data)  {
  if(HAL_UART_Transmit(uartHandle, &data, 1, 5000)!= HAL_OK)  {
    /* Capture error here to prevent transmission go on */
    uartErrorHandler();
  }
}

/** stm32UartSendBytes is a function sending bytes data from stm32 uart port
  *
  * input     :   uartHandle is UART_HandleTypeDef pointer pointing to the uart configuration structure
  *               data is a multiple bytes data need to transfer, usually is a buffer
  *
  * return    :   NONE
  */
void stm32UartSendBytes(UART_HandleTypeDef *uartHandle, uint8_t *data)  {
  if(HAL_UART_Transmit(uartHandle, data, sizeof(data), 5000)!= HAL_OK)  {
    /* Capture error here to prevent transmission go on */
    uartErrorHandler();
  }
}

/** stm32UartGetByte is a function receive single byte data from stm32 uart port
  *
  * input     :   uartHandle is UART_HandleTypeDef pointer pointing to the uart configuration structure
  *
  * return    :   rxBuffer contain single byte data received from uart port
  */
uint8_t stm32UartGetByte(UART_HandleTypeDef *uartHandle) {
  uint8_t rxBuffer = 0;
  
  if(HAL_UART_Receive(uartHandle, &rxBuffer, 1, 5000) == HAL_OK)  {
    return rxBuffer;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void uartErrorHandler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}
