#ifndef uart_H
#define uart_H

#if !defined(TEST)
  #include "stm32f4xx_hal.h"
#endif

#include <stdint.h>
#include "configurePort.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_uart.h"

#if !defined(TEST) || !defined(HOST)
extern __IO int uartTxReady;
extern __IO int uartRxReady;
#endif

#define FIVE_SEC    5000
#define TEN_SEC     10000

/* Definition for USARTx clock resources */
#define UART_PORT                        USART1
/* USARTx Baud Rate */
#define UART_BAUD_RATE                   128000//9600

#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

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

/* ##### Tlv Uart TX/RX Ready Flags Macros ##### */
#define IS_UART_TX_READY()              (((uartTxReady) == 1) ? 1 : 0)
#define IS_UART_RX_READY()              (((uartRxReady) == 1) ? 1 : 0)
#define SET_UART_TX_READY()             ((uartTxReady) = 1)
#define SET_UART_RX_READY()             ((uartRxReady) = 1)
#define CLEAR_UART_TX_READY()           ((uartTxReady) = 0)
#define CLEAR_UART_RX_READY()           ((uartRxReady) = 0)

UART_HandleTypeDef *uartInit(void);
void uartErrorHandler(void);

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length);

/* Uart Receive Function */
uint8_t getByte(void *handler, uint8_t *rxBuffer);
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length);

#endif // uart_H
