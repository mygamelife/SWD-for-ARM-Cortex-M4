#include "Uart.h"

#define MAXIMUM_PORT_SIZE     21
#define RXBUFFER_SIZE         255

/* COM PORT 0 - 20 */
static char *comPort[] = {
  "COM0", "COM1", "COM2", "COM3", "COM4",
  "COM5", "COM6", "COM7", "COM8", "COM9",
  /* ########### COM PORT > 9 ########### */
  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
  "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15",
  "\\\\.\\COM16", "\\\\.\\COM17", "\\\\.\\COM18",
  "\\\\.\\COM19", "\\\\.\\COM20" };
  
const char UART_VERIFY_STRING[] = "Uart is connected";
static int i = 0;

/** getAvailableComPort available Serial Comm Port
  * in this machine
  */
int getAvailableComPort(void **handler) {
  DWORD accessdirection = GENERIC_READ | GENERIC_WRITE;

  for(i; i < MAXIMUM_PORT_SIZE; i++) {
    *handler = CreateFile((LPCSTR)comPort[i], 
                              accessdirection, 
                              0,
                              0, 
                              OPEN_EXISTING,
                              0,
                              0);
    
    if(*handler != INVALID_HANDLE_VALUE) {
      uartConfig((HANDLE)*handler);
      printf("ComPort In Use %s\n", comPort[i]);
      return 1;
    }
  }
  
  return -1;
}

/** uartConfig is a function to configure allocmem
  * the detail about uart hardware
  */
void uartConfig(HANDLE handler) {
  COMMTIMEOUTS timeouts={0};
  DCB dcbSerialParams = {0};

  dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
  
  if (!GetCommState(handler, &dcbSerialParams)) {
    //could not get the state of the comport
    Throw(ERR_GET_COMM_STATE);
  }

  dcbSerialParams.BaudRate = (DWORD)UART_BAUD_RATE;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  
  if(!SetCommState(handler, &dcbSerialParams)) {
    //analyse error
    Throw(ERR_SET_COMM_STATE);
  }
  
  // Configure the interval timeout
  timeouts.ReadIntervalTimeout = 20;
  timeouts.ReadTotalTimeoutConstant = 20;
  timeouts.ReadTotalTimeoutMultiplier = 20;
  timeouts.WriteTotalTimeoutConstant = 20;
  timeouts.WriteTotalTimeoutMultiplier = 20;
  
  if(!SetCommTimeouts(handler, &timeouts)){
    //handle error
    Throw(ERR_SET_COMM_TIMEOUTS);
  }
}

/** isComPortCorrect
  */
int isComPortAlive(void *handler) {
  uint8_t txBuffer[] = {TLV_VERIFY_COM_PORT, 1, 0};
  uint8_t rxBuffer[RXBUFFER_SIZE];
  
  if(sendBytes(handler, txBuffer, sizeof(txBuffer)) != UART_OK) 
  {printf("send failed\n");}

  if(getBytes(handler, rxBuffer, RXBUFFER_SIZE) != UART_OK) 
  {printf("receive failed\n");}

  /* Should receive response if Com Port is alive */
  if(rxBuffer[0] == TLV_OK) return 1;
  
  else return 0;
}

/**
 * Initialize and configure serial comm.
 * 
 * Input:
 *   portname   is the name of the comm. port, i.e. "COM4"
 * Return:
 *   The serial comm. handle
 */
void uartInit(void **handler) {
  int pStatus = 0; //port status
  CEXCEPTION_T err;
  
  while(pStatus == 0) {
    /* Looking for availabe serial port in this machine */
    if(getAvailableComPort(&(*handler)) == -1) Throw(ERR_NO_COM_PORT);
    /* Verify if selected port is connecting to probe */
    pStatus = isComPortAlive(*handler);          
  }
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
