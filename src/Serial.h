#ifndef __Serial_H__
#define __Serial_H__

#include <windows.h>
#include <stdint.h>

void closeSerialPort(HANDLE hSerial);
DWORD serialWriteByte(HANDLE hSerial, uint8_t * data, int length);
DWORD serialGetBytes(HANDLE hSerial, uint8_t * buffer, int buffersize);
uint8_t serialGetByte(HANDLE hSerial);
HANDLE initSerialComm(LPCSTR portname, DWORD baudrate);

#endif  // __Serial_H__