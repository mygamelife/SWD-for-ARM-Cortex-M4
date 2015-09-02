#include "Tlv.h"
#include <stdlib.h>

#define HOST

int main(void) {
  Tlv_Session session;
  session.handler = uartInit(UART_PORT, UART_BAUD_RATE);
  printf("Opening port\n");
  
  // uint8_t errorCode = ERR_INVALID_COMMAND;
  // uint8_t nack = TLV_NOT_OK;
  uint8_t buffer[] = {0xAD, 0xDE, 0xCA, 0xFE, 0xBB, 0xAA};
  uint8_t rxBuffer[255];
  // uint8_t buffer[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
  // Tlv *tlv = tlvCreatePacket(TLV_WRITE_REGISTER, sizeof(buffer), buffer);
  // Tlv *dummy;
  // session.receiveState = START_RECEIVE;
  // tlvSend(&session, tlv);
  tlvSendService(&session);
  // while(1) {
    // if(!getBytes(session.handler, rxBuffer, 10))  {
      // printf("rxBuffer[0] %x\n", rxBuffer[0]);
      // printf("rxBuffer[1] %x\n", rxBuffer[1]);
      // printf("rxBuffer[2] %x\n", rxBuffer[2]);
      // printf("rxBuffer[3] %x\n", rxBuffer[3]);
      // if(!sendBytes(session.handler, rxBuffer, 10)) {
        // printf("data sent!\n");
        // break;
      // }
    // }
    // else  {
      // printf("Receiving\n");
    // }
  // }
  
  printf("Closing port\n");
  HANDLE hSerial = (HANDLE)session.handler;
  closeSerialPort(hSerial);
  
  return 0;
}