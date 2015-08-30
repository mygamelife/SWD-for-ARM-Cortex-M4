#include "ProgramLoader.h"
#include "CoreDebug_Utilities.h"
#include <stdlib.h>

int main(void) {
  // uint32_t buffer32[] = { 0xBEEFCAFE, 0xAABBCCDD,
                          // 0xDEADBEEF, 0xAABBCCDD,
                          // 0xBEEFCAFE, 0xAABBCCDD };
  Tlv *tlv, *receivedPacket;
  
  int time = 10, lastIndex = 0;
  uint32_t address = CORE_REG_R0;
  uint32_t data = 0xDEADBEEF;
  
  Tlv_Session *session = tlvCreateLoaderSession();
  printf("Opening port\n");
  
  printf("Write register\n");
  tlvWriteTargetRegister(session, &address, &data);
  receivedPacket = tlvReceive(session);
  
  if(receivedPacket->type == TLV_OK)  {
    printf("Probe reply OK\n");
    printf("chksum valu %x\n", receivedPacket->value[receivedPacket->length - 1]);
  }
  
  printf("Read register\n");
  tlvReadTargetRegister(session, &address);

  receivedPacket = tlvReceive(session);
  if(receivedPacket->type == TLV_READ_REGISTER)  {
    printf("value %x\n", get4Byte(&receivedPacket->value[0]));
    lastIndex = receivedPacket->length - 1;
    printf("chksum valu %x\n", receivedPacket->value[lastIndex]);
  }
  

  
  printf("Closing port\n");
  closeSerialPort(session->handler);
  
  return 0;
}