#ifndef uart_H
#define uart_H

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "ErrorCode.h"
#include "CException.h"

#define UART_PORT                     "COM3"//"COM7"//"\\\\.\\COM10"
#define UART_BAUD_RATE                128000//9600

typedef enum 
{
  UART_OK = 0,
  UART_ERROR
} Uart_Status;

void uartInit(void *huart);
void closeSerialPort(HANDLE hSerial);
DWORD uartSendBytes(HANDLE hSerial, uint8_t * data, int length);
DWORD uartGetBytes(HANDLE hSerial, uint8_t * buffer, int buffersize);

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length);

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length);
uint8_t getByte(void *handler, uint8_t *rxBuffer);

int isByteAvailable();

#endif // uart_H
