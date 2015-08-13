#ifndef __Serial_H__
#define __Serial_H__

#include <windows.h>
#include <stdint.h>

#define UART_PORT               "COM7"
#define UART_BAUD_RATE          115200

void closeSerialPort(HANDLE hSerial);
DWORD uartSendBytes(HANDLE hSerial, uint8_t * data, int length);
DWORD uartGetBytes(HANDLE hSerial, uint8_t * buffer, int buffersize);
uint8_t uartGetByte(HANDLE hSerial);
HANDLE initSerialComm(LPCSTR portname, DWORD baudrate);

#endif  // __Serial_H__