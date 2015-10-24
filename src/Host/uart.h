#ifndef Uart_H
#define Uart_H

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "ErrorCode.h"
#include "CException.h"

typedef enum 
{
  UART_OK = 0,
  UART_ERROR
} Uart_Status;

#define UART_PORT                     "COM7"//"COM3"
#define UART_BAUD_RATE                128000//9600
#define closePort(__SESSION__)        CloseHandle(((HANDLE *)((__SESSION__)->handler)))

void uartInit(void **handler);
DWORD uartSendBytes(HANDLE hSerial, uint8_t * data, int length);
DWORD uartGetBytes(HANDLE hSerial, uint8_t * buffer, int buffersize);

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length);

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length);
uint8_t getByte(void *handler, uint8_t *rxBuffer);

int isByteAvailable();

#endif // Uart_H
