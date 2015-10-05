#ifndef uart_H
#define uart_H

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

#define UART_PORT               "\\\\.\\COM10"//"COM6"
#define UART_BAUD_RATE          128000//9600

typedef enum 
{
  UART_OK       = 0x00,
  UART_ERROR    = 0x01,
} Uart_Status;


HANDLE uartInit(void);
void closeSerialPort(HANDLE hSerial);
DWORD uartSendBytes(HANDLE hSerial, uint8_t * data, int length);
DWORD uartGetBytes(HANDLE hSerial, uint8_t * buffer, int buffersize);

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length);

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length);

int isByteAvailable();

#define getByte(handler, rxBuffer)  getBytes(handler, rxBuffer, 1)

#endif // uart_H
