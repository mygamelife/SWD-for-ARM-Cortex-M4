#ifndef uart_H
#define uart_H

#if !defined(TEST)
  #include "stm32f4xx_it.h"
  #include "stm32f4xx_hal.h"
#endif

#include <stdint.h>
#include "configurePort.h"
#include "stm32f4xx_hal_uart.h"

#define FIVE_SEC    5000
#define TEN_SEC     10000

UART_HandleTypeDef *uartInit(void);
void uartErrorHandler(void);

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length);

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length);

#endif // uart_H
