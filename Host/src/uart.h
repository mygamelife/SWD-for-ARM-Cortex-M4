#ifndef uart_H
#define uart_H

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

#define UART_PORT               "COM6"
#define UART_BAUD_RATE          9600//115200

typedef enum 
{
  UART_OK       = 0x00,
  UART_ERROR    = 0x01,
} Uart_Status;

void closeSerialPort(HANDLE hSerial);
DWORD uartSendBytes(HANDLE hSerial, uint8_t * data, int length);
DWORD uartGetBytes(HANDLE hSerial, uint8_t * buffer, int buffersize);

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length);
/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length);

HANDLE uartInit(LPCSTR portname, DWORD baudrate);

#endif // uart_H
