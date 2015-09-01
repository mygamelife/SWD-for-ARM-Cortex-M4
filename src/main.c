#include "Tlv.h"
#include <stdlib.h>

int main(void) {
  Tlv_Session session;
  session.handler = uartInit(UART_PORT, UART_BAUD_RATE);
  printf("Opening port\n");
  
  // uint8_t errorCode = ERR_INVALID_COMMAND;
  // uint8_t nack = TLV_NOT_OK;
  uint8_t buffer[] = {0xAD, 0xDE, 0xCA, 0xFE, 0xBB, 0xAA};
  // uint8_t buffer[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_REGISTER, sizeof(buffer), buffer);
  Tlv *dummy;
  session.receiveState = START_RECEIVE;
  tlvSend(&session, tlv);
  
  while(1) {
    tlvService(&session);
    
	  dummy = tlvReceive(&session);
    if(dummy != NULL) {
      if(session.TIMEOUT_FLAG) {
        printf("TIME OUT!\n");
        break;
      }
      printf("type %x\n", dummy->type);
      printf("length %x\n", dummy->length);
      printf("value %x\n", dummy->value[0]);
      printf("value %x\n", dummy->value[1]);
      printf("value %x\n", dummy->value[2]);
      printf("value %x\n", dummy->value[3]);
      printf("value %x\n", dummy->value[4]);
      printf("value %x\n", dummy->value[5]);
      printf("chksum %x\n", dummy->value[dummy->length - 1]);
      break;
    }
  }
  
  printf("Closing port\n");
  HANDLE hSerial = (HANDLE)session.handler;
  closeSerialPort(hSerial);
  
  return 0;
}