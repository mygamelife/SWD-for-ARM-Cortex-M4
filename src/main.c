#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Serial.h"
// #include "TLV_Host.h"
// #include "GetHeaders.h"

int main(void) {
  int size = 0;
  uint8_t txBuffer[] = "Hello Word";
  uint8_t rxBuffer[255];
  uint8_t byteData;
  char data;
  
  printf("Open Serial Port\n");
  HANDLE hSerial = initSerialComm("COM7", UART_BAUD_RATE);

  uartSendBytes(hSerial, txBuffer, sizeof(txBuffer));
  printf("Byte data %c\n", uartGetByte(hSerial));
  
  uartSendBytes(hSerial, txBuffer, sizeof(txBuffer));
  
  size = uartGetBytes(hSerial, rxBuffer, sizeof(rxBuffer));
  printf("Bytes data %.*s\n", size, rxBuffer);
  

  
  //uartSendBytes(hSerial, txBuffer, sizeof(txBuffer));
  

  printf("Closing Serial Port\n");  

  closeSerialPort(hSerial);

  return 0;
}