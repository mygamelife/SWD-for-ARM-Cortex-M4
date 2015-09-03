#include "Tlv.h"
#include <stdlib.h>

#define HOST

int main(void) {
  Tlv_Session *session = tlvCreateSession();
  printf("Opening port\n");

  uint32_t regAddress = 0;
  uint32_t data = 0xBEEFCAFE;
  
  while(1) {
    tlvService(session);
    tlvWriteTargetRegister(session, &regAddress, &data);
    // if(session->writeRegisterState == TLV_SEND_PACKET)  {
      // break;
    // }
    tlvReadTargetRegister(session, &regAddress);
  }
  
  printf("Closing port\n");
  HANDLE hSerial = (HANDLE)session->handler;
  closeSerialPort(hSerial);
  
  return 0;
}