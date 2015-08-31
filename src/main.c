#include "ProgramLoader.h"
// #include "CoreDebug_Utilities.h"
#include <stdlib.h>

int main(void) {
  // uint32_t buffer32[] = { 0xBEEFCAFE, 0xAABBCCDD,
                          // 0xDEADBEEF, 0xAABBCCDD,
                          // 0xBEEFCAFE, 0xAABBCCDD };
  Tlv *tlv, *receivedPacket;
  
  int time = 10, lastIndex = 0;
  uint32_t address = 0;
  uint32_t data = 0xDEADBEEF;
  
  Tlv_Session *session = tlvCreateLoaderSession();
  printf("Opening port\n");
  
  // printf("Write register\n");
  // tlvWriteTargetRegister(session, &address, &data);
  // receivedPacket = tlvReceive(session);
  
  // if(receivedPacket->type == TLV_OK)  {
    // printf("Probe reply OK\n");
    // printf("chksum valu %x\n", receivedPacket->value[receivedPacket->length - 1]);
  // }
  
  // printf("Read register\n");
  // tlvReadTargetRegister(session, &address);

  // receivedPacket = tlvReceive(session);
  // if(receivedPacket->type == TLV_READ_REGISTER)  {
    // printf("value %x\n", get4Byte(&receivedPacket->value[0]));
    // lastIndex = receivedPacket->length - 1;
    // printf("chksum valu %x\n", receivedPacket->value[lastIndex]);
  // }
  uint8_t errorCode = ERR_INVALID_COMMAND;
  uint8_t nack = TLV_NOT_OK;
  
  tlv = tlvCreatePacket(0x90, 0, 0);
  tlvSend(session, tlv);
  receivedPacket = tlvReceive(session);
  printf("type %x\n", receivedPacket->type);
  printf("length %x\n", receivedPacket->length);
  printf("data %x\n", receivedPacket->value[0]);
  printf("chksum %x\n", receivedPacket->value[1]);
  
  if(receivedPacket->type == nack)  {
    printf("length %d\n", receivedPacket->length);
    if((Tlv_ErrorCode)receivedPacket->value[0] == ERR_INVALID_COMMAND) {
      printf("INVALID COMMAND!\n");
    }
  }
  
  printf("Closing port\n");
  closeSerialPort(session->handler);
  
  return 0;
}