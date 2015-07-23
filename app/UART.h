#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "Delay.h"
#include "stm32f4xx_it.h"
#include "configurePort.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f429i_discovery.h"

/* Size of Transmission buffer */
#define TXBUFFERSIZE                     (COUNTOF(aTxBuffer))//(COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                     TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

void test_Uart(void);

#endif // UART_H
