#include "Uart.h"

/**
 * Initialize and configure serial comm.
 * 
 * Input:
 *   portname   is the name of the comm. port, i.e. "COM4"
 * Return:
 *   The serial comm. handle
 */
void uartInit(void **handler) {
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
    Throw(ERR_INVALID_HANDLER);
  }

  dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
  
  if (!GetCommState(hSerial, &dcbSerialParams)) {
    //could not get the state of the comport
    Throw(ERR_GET_COMM_STATE);
  }
  //  dcbSerialParams.BaudRate = 460800;
  dcbSerialParams.BaudRate = (DWORD)UART_BAUD_RATE;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  
  if(!SetCommState(hSerial, &dcbSerialParams)){
     //analyse error
    Throw(ERR_SET_COMM_STATE);
  }
  
  // The interval 
  timeouts.ReadIntervalTimeout = 20;
  timeouts.ReadTotalTimeoutConstant = 20;
  timeouts.ReadTotalTimeoutMultiplier = 20;
  timeouts.WriteTotalTimeoutConstant = 20;
  timeouts.WriteTotalTimeoutMultiplier = 20;
  if(!SetCommTimeouts(hSerial, &timeouts)){
    //handle error
    Throw(ERR_SET_COMM_TIMEOUTS);
  }  
  
  *handler = hSerial;
}

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length) {
  DWORD dwBytesWrite = 0;
  HANDLE *hSerial = (HANDLE *)handler;

  if(!WriteFile(hSerial, txBuffer, length, &dwBytesWrite, NULL)){
    DWORD errId = GetLastError();
    printf("WriteFile Error: %d\n", errId);
    // printLastError();
    return UART_ERROR;
	}
  if(dwBytesWrite != 0) {
    //printf("%d Bytes is Sucessfully Sent!\n", dwBytesWrite);
    // printf("address %x!\n", (*(uint32_t *)(&txBuffer[2])));
    return UART_OK;
  }
  else return UART_ERROR;
}

uint8_t getByte(void *handler, uint8_t *rxBuffer) {
  DWORD dwBytesRead = 0;
  HANDLE *hSerial = (HANDLE *)handler;

  if(!ReadFile(hSerial, rxBuffer, 1, &dwBytesRead, NULL)){
    // handle error
    DWORD errId = GetLastError();
    printf("ReadFile Error: %d\n", errId);
    return UART_ERROR;
  }
  if(dwBytesRead != 0) {
    // printf("Byte is Received!\n");
    return UART_OK;  
  }
  else return UART_ERROR;
}

/* Uart Receive Function */
uint8_t getBytes(void *handler, uint8_t *rxBuffer, int length) {
  DWORD dwBytesRead = 0;
  HANDLE *hSerial = (HANDLE *)handler;
  
  if(!ReadFile(hSerial, rxBuffer, length, &dwBytesRead, NULL)){
    // handle error
    DWORD errId = GetLastError();
    printf("ReadFiles Error: %d\n", errId);
    return UART_ERROR;
  }
  if(dwBytesRead != 0) {
    // printf("Byte is Received!\n");
    return UART_OK;  
  }
  else return UART_ERROR;
}
