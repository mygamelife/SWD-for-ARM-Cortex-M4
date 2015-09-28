#include "uart.h"

/**
 * Initialize and configure serial comm.
 * 
 * Input:
 *   portname   is the name of the comm. port, i.e. "COM4"
 * Return:
 *   The serial comm. handle
 */
HANDLE uartInit(void) {
  COMMTIMEOUTS timeouts={0};
  DCB dcbSerialParams = {0};
  DWORD  accessdirection = GENERIC_READ | GENERIC_WRITE;
  HANDLE hSerial = CreateFile((LPCSTR)UART_PORT, 
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
  dcbSerialParams.BaudRate = (DWORD)UART_BAUD_RATE;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  if(!SetCommState(hSerial, &dcbSerialParams)){
     //analyse error
     DWORD errId = GetLastError();
     printf("SetCommState Error: %d\n", errId);
  }
  
  // The interval 
  timeouts.ReadIntervalTimeout = 10;
  timeouts.ReadTotalTimeoutConstant = 10;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.WriteTotalTimeoutConstant = 10;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  if(!SetCommTimeouts(hSerial, &timeouts)){
    //handle error
     DWORD errId = GetLastError();
     printf("SetCommTimeouts Error: %d\n", errId);
  }  
  return hSerial;
}

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length) {
  DWORD dwBytesRead = 0;
  HANDLE *hSerial = (HANDLE *)handler;
  // printf("after typecast\n");
  if(!WriteFile(hSerial, txBuffer, length, &dwBytesRead, NULL)){
    DWORD errId = GetLastError();
    printf("WriteFile Error: %d\n", errId);
    // printLastError();
    return UART_ERROR;
	}
  if(dwBytesRead != 0) {
    return UART_OK;
  }
  else return UART_ERROR;
}

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length)  {
  DWORD dwBytesRead = 0;
  HANDLE *hSerial = (HANDLE *)handler;
  
  if(!ReadFile(hSerial, rxBuffer, length, &dwBytesRead, NULL)){
    // handle error
    DWORD errId = GetLastError();
    printf("ReadFile Error: %d\n", errId);
    return UART_ERROR;
  }
  if(dwBytesRead != 0) {
    return UART_OK;  
  }
  else return UART_ERROR;
}

void closeSerialPort(HANDLE hSerial) {
	CloseHandle(hSerial);
}