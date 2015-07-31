#ifndef __Serial_H__
#define __Serial_H__

#if defined TEST
#include <windows.h>
#endif

#include <stdint.h>


#if defined TEST
void closeSerialPort(HANDLE hSerial);
DWORD writeToSerialPort(HANDLE hSerial, uint8_t * data, int length);
DWORD readFromSerialPort(HANDLE hSerial, uint8_t * buffer, int buffersize);
HANDLE initSerialComm(LPCSTR portname, DWORD baudrate);
#endif
#endif  // __Serial_H__
