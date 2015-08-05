#include "TLV_Host.h"

/**
  * tlvHost
  */
void tlvHost(TLVSession *tlvSession)  {
  TLV_TypeDef *tlv;
  uint8_t bufferState;
  uint8_t txBuffer[1024], rxBuffer = 0;
  
  switch(tlvSession->state)  {
    case TLV_START :
      /* Create new TLV packet */
      tlv = tlvCreateNewPacket(TLV_WRITE);
      
      /* Put Data into Buffer */
      bufferState = tlvPutDataIntoBuffer(tlv, tlvSession->pElf);
  
      /* Pack into TXBUFFER */
      if(bufferState != TLV_BUFFER_EMPTY) {
        tlvPackPacketIntoTxBuffer(txBuffer, tlv);
      }
      else  {
        //something here
      }
      tlvSession->state = TLV_TRANSMIT_DATA;
      break;
      
    case TLV_TRANSMIT_DATA :
      //printf("tlv length %d\n", tlv->length);
      
      /* Transmit all data inside txBuffer to probe */
      serialWriteByte(tlvSession->hSerial, txBuffer, tlv->length + 2);
      tlvSession->state = TLV_WAIT_REPLY;
      break;
      
    case TLV_WAIT_REPLY :
      while(rxBuffer != PROBE_OK) {
        rxBuffer = serialGetByte(tlvSession->hSerial);
      }
      
      tlvSession->state = TLV_START;
      break;
  }
}