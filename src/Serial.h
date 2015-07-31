#ifndef __Serial_H__
#define __Serial_H__

#include <windows.h>
#include <stdint.h>

void closeSerialPort(HANDLE hSerial);
DWORD writeToSerialPort(HANDLE hSerial, uint8_t * data, int length);
DWORD readFromSerialPort(HANDLE hSerial, uint8_t * buffer, int buffersize);
HANDLE initSerialComm(LPCSTR portname, DWORD baudrate);

#endif  // __Serial_H__