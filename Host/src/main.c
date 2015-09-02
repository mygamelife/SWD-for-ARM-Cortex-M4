#include "Tlv.h"
#include <stdlib.h>

#define HOST

int main(void) {
  Tlv_Session *session = tlvCreateSession();
  printf("Opening port\n");
  
  uint8_t buffer[] = {0xAD, 0xDE, 0xFE, 0xCA};
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_REGISTER, sizeof(buffer), buffer);
  Tlv *received;
  tlvSend(session, tlv);

  while(1) {
    tlvService(session);
    if(session->DATA_ARRIVE_FLAG == true) {
      received = tlvReceive(session);
      printf("type %x\n", received->type);
      printf("length %x\n", received->length);
      printf("value %x\n", get4Byte(&received->value[0]));
      printf("chksum %x\n", received->value[4]);
      break;
    }
  }
  
  printf("Closing port\n");
  HANDLE hSerial = (HANDLE)session->handler;
  closeSerialPort(hSerial);
  
  return 0;
}