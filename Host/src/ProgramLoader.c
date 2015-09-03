#include "ProgramLoader.h"

/** tlvWriteTargetRegister is a function to write data into target register
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *           data is the data need to write into the target register
  *
  * return  : NONE
  */
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t *registerAddress, uint32_t *data)  {
  Tlv *tlv, *response;
  
  switch(session->writeRegisterState) {
    case SEND_PACKET :
      /* create tlv packet with register address */
      tlv = tlvCreatePacket(TLV_WRITE_REGISTER, 4, (uint8_t *)registerAddress);
      /* Insert data into tlv packet value */
      tlvPackIntoBuffer(&tlv->value[4], (uint8_t *)data, 4);
      tlv->length += 4;
      
      tlvSend(session, tlv);
      session->writeRegisterState = WAIT_RESPONSE;
      break;
      
    case WAIT_RESPONSE :
      response = tlvReceive(session);
      if(verifyTlvResponse(response)) {
        if(verifyTlvData(response)) {
          session->writeRegisterState = SEND_PACKET;
        }
        else
          Throw(TLV_CHECKSUM_ERROR);
      }
      break;
  }
}

/** tlvReadTargetRegister is a function to read target register
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *
  * return  : NONE
  */
void tlvReadTargetRegister(Tlv_Session *session, uint32_t *registerAddress)  {
  Tlv *tlv, *response;
  
  switch(session->readRegisterState) {
    case SEND_PACKET :
      /* create tlv packet with register address */
      tlv = tlvCreatePacket(TLV_READ_REGISTER, 4, (uint8_t *)registerAddress);
      tlvSend(session, tlv);
      session->readRegisterState = WAIT_RESPONSE;
      break;
      
    case WAIT_RESPONSE :
      response = tlvReceive(session);
      if(verifyTlvResponse(response)) {
        if(verifyTlvData(response)) {
          session->readRegisterState = SEND_PACKET;
        }
        else
          Throw(TLV_CHECKSUM_ERROR);
      }
      break;
  }
}

/** tlvWriteDataChunk is a function used to send data in chunk
  * by using tlv protocol
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *
  * return  : NONE
  */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size) {
  Tlv *tlv;
  
  /* create tlv packet with register address */
  tlv = tlvCreatePacket(TLV_WRITE_RAM, 4, (uint8_t *)&destAddress);
  tlvPackIntoBuffer(&tlv->value[4], dataAddress, size);
  tlv->length += size;
  
  tlvSend(session, tlv);
}

/** tlvWriteTargetRam is a function used to write data into target RAM
  * by using tlv protocol
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *
  * return  : NONE
  */
void tlvWriteTargetRam(Tlv_Session *session, uint8_t *dataAddress, uint32_t *destAddress, int *size)  {
  Tlv *response;
  
  switch(session->writeRAMState) {
    case SEND_PACKET :
      if(*size > 0) {
        if(*size > TLV_DATA_SIZE)
          tlvWriteDataChunk(session, dataAddress, *destAddress, TLV_DATA_SIZE);
        else
          tlvWriteDataChunk(session, dataAddress, *destAddress, *size);
        
        dataAddress += TLV_DATA_SIZE;
        *destAddress += TLV_DATA_SIZE;
        *size -= TLV_DATA_SIZE;
      }
      session->writeRAMState = WAIT_RESPONSE;
      break;
      
    case WAIT_RESPONSE :
      response = tlvReceive(session);
      if(verifyTlvResponse(response)) {
        if(verifyTlvData(response)) {
          session->writeRAMState = SEND_PACKET;
        }
        else
          Throw(TLV_CHECKSUM_ERROR);
      }
      break;
  }
}

/** InterpretUserCommand is a function to select instruction 
  * base on tlv->type
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
// void InterpretUserCommand(Tlv_Session *session, Tlv *tlv)  {
  // Tlv *error; uint8_t errorCode = 0;
  
  // switch(tlv->type) {
    // case TLV_WRITE_RAM      : break;
    // case TLV_READ_RAM       : break;
    // case TLV_WRITE_REGISTER : writeTargetRegister(session, &get4Byte(&tlv->value[0]), &get4Byte(&tlv->value[4])); break;
    // case TLV_READ_REGISTER  : readTargetRegister(session, &get4Byte(&tlv->value[0])); break;
    // case TLV_HALT_TARGET    : break;
    // case TLV_RUN_TARGET     : break;
    // case TLV_STEP           : break;
    
    // default :
      // errorCode = TLV_INVALID_COMMAND;
      // error = tlvCreatePacket(TLV_NOT_OK, 1, &errorCode);
      // tlvSend(session, error);
      // break;
  // }
// }

/** programWorker
  */
// void programWorker(Tlv_Session *session)  {
  // Tlv *packet, *error;
  // uint8_t errorCode = 0;
  
  // switch(session->state)  {
    // case WAITING_PACKET :
      // packet = tlvReceive(session);
      // if(packet != NULL)
        // session->state = INTERPRET_PACKET;
      // break;
      
    // case INTERPRET_PACKET :
      // if(tlvVerifyData(packet) == DATA_VALID)  {
        // selectInstruction(session, packet);
      // }
      // else  {
        // errorCode = TLV_CORRUPTED_DATA;
        // error = tlvCreatePacket(TLV_NOT_OK, 1, &errorCode);
        // tlvSend(session, error);
      // }
      // session->state = WAITING_PACKET;
      // break;
  // }
// }