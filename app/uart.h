#ifndef uart_H
#define uart_H

#if !defined(TEST)
  #include "stm32f4xx_hal.h"
#endif

#include <stdint.h>
#include "configurePort.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_uart.h"

#if !defined(TEST)
#define __IO volatile
extern __IO ITStatus uartReady;
#endif

#define FIVE_SEC    5000
#define TEN_SEC     10000

/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* USARTx Baud Rate */
#define USART_BAUD_RATE                  9600//115200

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART1

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

UART_HandleTypeDef *uartInit(void);
void uartErrorHandler(void);

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length);

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length);

#endif // uart_H
