#ifndef UART_H
#define UART_H

#if !defined(TEST)
  #include "stm32f4xx_hal.h"
#endif

#include <stdint.h>
#include "Delay.h"
#include "stm32f4xx_it.h"
#include "configurePort.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f429i_discovery.h"

#define FIVE_SEC    5000
#define TEN_SEC     10000

UART_HandleTypeDef *initUart(void);
void uartErrorHandler(void);

/* Uart Transmit Function */
void stm32UartSendByte(UART_HandleTypeDef *uartHandle, uint8_t data);
void stm32UartSendBytes(UART_HandleTypeDef *uartHandle, uint8_t *data);

/* Uart Receive Function */
uint8_t stm32UartGetByte(UART_HandleTypeDef *uartHandle);
#define stm32UartGetBytes(uartHandle, __BUFFER__)   HAL_UART_Receive(uartHandle, buffer, sizeof(__BUFFER__), 5000)
// #define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__))

#endif // UART_H
