#ifndef Uart_H
#define Uart_H

#include <stdint.h>
#include "configurePort.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_uart.h"

extern UART_HandleTypeDef uartHandle;
extern UART_HandleTypeDef swoHandle;

typedef enum
{
  UART_ERROR  = -1,
  UART_OK     = 1,
  UART_BUSY   = 2,
} Uart_Status;

/* Definition for USARTx clock resources */
#define UART_PORT                         USART1
#define SWO_PORT                          UART5
/* USARTx Baud Rate */
#define UART_BAUD_RATE                    128000//9600
#define SWO_BAUD_RATE                     128000//9600

#define UART_CLK_ENABLE()                 do {                                \
                                              __HAL_RCC_USART1_CLK_ENABLE();  \
                                              __HAL_RCC_GPIOA_CLK_ENABLE();   \
                                          } while(0)

#define SWO_CLK_ENABLE()                  do {                                \
                                              __HAL_RCC_UART5_CLK_ENABLE();   \
                                              __HAL_RCC_GPIOD_CLK_ENABLE();   \
                                          } while(0)

/* Definition for USARTx Pins */
#define UART_TX_GPIO_PORT                 GPIOA
#define UART_TX_PIN                       GPIO_PIN_9
#define UART_TX_AF                        GPIO_AF7_USART1
#define UART_RX_GPIO_PORT                 GPIOA
#define UART_RX_PIN                       GPIO_PIN_10
#define UART_RX_AF                        GPIO_AF7_USART1

/* Definition for USARTx's NVIC */
#define UART_IRQn                         USART1_IRQn
#define UART_IRQHandler                   USART1_IRQHandler

/* Definition for (USARTx) SWO Pins */
#define SWO_RX_PORT                       GPIOD
#define SWO_RX_PIN                        GPIO_PIN_2
#define UART5_RX_AF                       GPIO_AF8_UART5

#define SWO_IRQn                          UART5_IRQn
#define SWO_IRQHandler                    UART5_IRQHandler

#define uartConfigTx(port, pin, alt)      uartConfigGpio(port, pin, alt)
#define uartConfigRx(port, pin, alt)      uartConfigGpio(port, pin, alt)

void uartInit(void **huart);
void uartErrorHandler(void);
void uartConfigGpio(GPIO_TypeDef *gpiox, uint32_t pin, uint32_t altFunction);

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length);

/* Uart Receive Function */
uint8_t getByte(void *handler, uint8_t *rxBuffer);
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length);

int isRxBusy(void);
int isTxBusy(void);
void cancelRx(void);

void swoInit(void **hswo);
uint8_t getSwoBytes(void *handler, uint8_t *rxBuffer, int length);

#endif // Uart_H
