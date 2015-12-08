#include "Uart.h"

#define MAXIMUM_PORT_SIZE     20
#define RXBUFFER_SIZE         255

const char PASSKEY[] = "Hello From The Other Side";

/* COM PORT 0 - 20 */
static char *comPort[] = {
  "COM0", "COM1", "COM2", "COM3", "COM4",
  "COM5", "COM6", "COM7", "COM8", "COM9",
  /* ########### COM PORT > 9 ########### */
  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
  "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15",
  "\\\\.\\COM16", "\\\\.\\COM17", "\\\\.\\COM18",
  "\\\\.\\COM19", "\\\\.\\COM20" };

/** openComPort is a function to create a handler for
  * serial communication and initialization
  *
  * input     : portname is the name of the com port in this machine, Example : "COM4"
  *             baudrate is the desired baudrate that want to be set it
  *             serial communication
  *
  * return    : the serial comm handler
  *             returns 0 on error (if handler is failed to create)
  */
HANDLE openComPort(LPCSTR portname, DWORD baudrate) {
  COMMTIMEOUTS timeouts={0};
  DCB dcbSerialParams = {0};
  DWORD  accessdirection = GENERIC_READ | GENERIC_WRITE;

  /* Create file for with given port name and baudrate
     if it failed the handler will contain error value */
  HANDLE handler = CreateFile(portname,
                              accessdirection,
                              0,
                              0,
                              OPEN_EXISTING,
                              0,
                              0);
  /* Close port and return if open port is failed */
  if (handler == INVALID_HANDLE_VALUE) {
    CloseHandle(handler);
    return NULL;
  }

  dcbSerialParams.DCBlength=sizeof(dcbSerialParams);

  if (!GetCommState(handler, &dcbSerialParams))
  { Throw(ERR_GET_COMM_STATE); }

  dcbSerialParams.BaudRate = baudrate;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;

  if(!SetCommState(handler, &dcbSerialParams))
  { Throw(ERR_SET_COMM_STATE); }

  /* Set internal timeout */
  timeouts.ReadIntervalTimeout = 20;
  timeouts.ReadTotalTimeoutConstant = 20;
  timeouts.ReadTotalTimeoutMultiplier = 20;
  timeouts.WriteTotalTimeoutConstant = 20;
  timeouts.WriteTotalTimeoutMultiplier = 20;

  if(!SetCommTimeouts(handler, &timeouts))
  { Throw(ERR_SET_COMM_TIMEOUTS); }

  return handler;
}

/** isComPortAlive is a function to send message to probe and wait
  * for it to reply to verify this is the actual com port used to
  * communicate with probe.
  *
  * input     : handler is serial handler contain all the neccessary data structure
  *             used for serial communication such as receive and send
  *
  * return    : 1 if comPort is reply from probe
  *             returns 0 on error (if com port didnt receive anything)
  */
int isComPortAlive(HANDLE handler) {
  uint8_t txBuffer[] = {TLV_VERIFY_COM_PORT, 1, 0};
  uint8_t rxBuffer[255];

  if(sendBytes(handler, txBuffer, 3) != UART_OK)
  {
    printf("COMPORT is failed to send the message.\n");
    printf("There are several reasons can cause this issues, for example :\n");
    printf("- Cable is disconnected\n");
  }

  /* Should receive response if Com Port is alive */
  if(getBytes(handler, rxBuffer, sizeof(PASSKEY) + 3) == UART_OK)
  {
    char *reply = &rxBuffer[2];
    
    if(strcmp(reply, PASSKEY) == 0) {
      printf("passkey reply from probe %s\n", reply);
      return 1;
    }
    
    else {
      printf("passkey reply from probe %s\n", reply);
      Throw(ERR_INVALID_PASSKEY);
    }
  }
  else {
    printf("COMPORT is failed to receive from probe.\n");
    printf("There are several reasons can cause this issues, for example :\n");
    printf("- Cable is disconnected\n");
    printf("- Probe did not receive any message\n");
  }    

  return 0;
}

/** findProbe is used to find the actual comPort is connecting with probe
  *
  * input   : NONE
  *
  * return  : the serial comm handler
  *           returns 0 on error (if none of the com port is found)
  */
HANDLE findProbe(void) {
  int i; HANDLE handler;

  for(i = 0; i < MAXIMUM_PORT_SIZE; i++) {
    /* Searching for available com port */
    handler = openComPort((LPCSTR)comPort[i], UART_BAUD_RATE);
    /* Send a message to probe and wait for reply */
    if(handler) {
      printf("ComPort in use %s\n", comPort[i]);
      if(isComPortAlive(handler))
        return handler;
    }
    CloseHandle(handler);
  }

  return NULL;
}

/** Initialize and configure serial comm.
  *
  * input   : handler is a void pointer-to-pointer used to get an initialized serial
  *           handler from other function
  *
  * return  : The serial comm. handle
  */
void uartInit(void **handler) {

  HANDLE hSerial = findProbe();

  if(hSerial) 
  { *handler = hSerial; }

  else Throw(ERR_NO_COM_PORT);
}

/* Uart Transmit Function */
uint8_t sendBytes(void *handler, uint8_t *txBuffer, int length) {
  DWORD dwBytesWrite = 0;

  if(!WriteFile((HANDLE)handler, txBuffer, length, &dwBytesWrite, NULL)){
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

  if(!ReadFile((HANDLE)handler, rxBuffer, 1, &dwBytesRead, NULL)){
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
  
  if(!ReadFile((HANDLE)handler, rxBuffer, length, &dwBytesRead, NULL)){
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
