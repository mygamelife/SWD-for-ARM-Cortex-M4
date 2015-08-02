#include "TLV_Host.h"

/**
  * tlvHost
  */
void tlvHost(TLVSession *tlvSession)  {
  int size = 0; TLV_TypeDef *tlv;
  uint8_t txBuffer[1024], *rxBuffer;
  
  switch(*tlvSession->state)  {
    case TLV_START :
      /*
      TLV_Byte tlvGetByte(uint8_t *data, int index);
      tlv = tlvCreatePacket(TLV_WRITE, sizeof(data), data);
      size = tlvPackPacketIntoBuffer(txBuffer, tlv);
      *state = TLV_TRANSMIT_DATA;*/
      break;
      
    case TLV_TRANSMIT_DATA :
      break;
      
    case TLV_WAIT_REPLY :
      break;
  }
}