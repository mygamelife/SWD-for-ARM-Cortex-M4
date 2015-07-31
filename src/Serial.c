//#include <windows.h>
#include <stdio.h>
#include "Serial.h"

//  LPCSTR portname = "COM6";
/**
 * Initialize and configure serial comm.
 * 
 * Input:
 *   portname   is the name of the comm. port, i.e. "COM4"
 * Return:
 *   The serial comm. handle
 */
HANDLE initSerialComm(LPCSTR portname, DWORD baudrate) {
  COMMTIMEOUTS timeouts={0};
  DCB dcbSerialParams = {0};
  DWORD  accessdirection = GENERIC_READ | GENERIC_WRITE;
  HANDLE hSerial = CreateFile(portname, 
                              accessdirection, 
                              0,  
                              0,  
                              OPEN_EXISTING,
                              0,
                              0);
  if (hSerial == INVALID_HANDLE_VALUE) {
      //call GetLastError(); to gain more information
     DWORD errId = GetLastError();
     printf("Invalid handle. Error: %d\n", errId);
  }

  dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
  if (!GetCommState(hSerial, &dcbSerialParams)) {
    //could not get the state of the comport
  }
//  dcbSerialParams.BaudRate = 460800;
  dcbSerialParams.BaudRate = baudrate;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  if(!SetCommState(hSerial, &dcbSerialParams)){
     //analyse error
     DWORD errId = GetLastError();
     printf("SetCommState Error: %d\n", errId);
  }
  
  // The interval 
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 50;
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  if(!SetCommTimeouts(hSerial, &timeouts)){
    //handle error
     DWORD errId = GetLastError();
     printf("SetCommTimeouts Error: %d\n", errId);
  }  
  return hSerial;  
}

DWORD readFromSerialPort(HANDLE hSerial, uint8_t * buffer, int buffersize) {
  DWORD dwBytesRead = 0;
  if(!ReadFile(hSerial, buffer, buffersize, &dwBytesRead, NULL)){
    //handle error
    DWORD errId = GetLastError();
    printf("ReadFile Error: %d\n", errId);
  }
  return dwBytesRead;
}

DWORD writeToSerialPort(HANDLE hSerial, uint8_t * data, int length) {
	DWORD dwBytesRead = 0;
	if(!WriteFile(hSerial, data, length, &dwBytesRead, NULL)){
    DWORD errId = GetLastError();
    printf("WriteFile Error: %d\n", errId);
//		printLastError();
	}
	return dwBytesRead;
}

void closeSerialPort(HANDLE hSerial) {
	CloseHandle(hSerial);
}