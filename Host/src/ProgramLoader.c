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
    case TLV_SEND_PACKET :
      printf("Send Write Register Packet\n");
      /* create tlv packet with register address */
      tlv = tlvCreatePacket(TLV_WRITE_REGISTER, 4, (uint8_t *)registerAddress);
      /* Insert data into tlv packet value */
      tlvPackIntoBuffer(&tlv->value[4], (uint8_t *)data, 4);
      tlv->length += 4;
      
      tlvSend(session, tlv);
      session->writeRegisterState = TLV_WAIT_RESPONSE;
      break;
      
    case TLV_WAIT_RESPONSE :
      // printf("Write Register Wait Response\n");
      response = tlvReceive(session);
      if(verifyTlvPacket(response)) {
        // printf("OK Reply\n");
        session->writeRegisterState = TLV_SEND_PACKET;
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
    case TLV_SEND_PACKET :
      // printf("Send Read Register Packet\n");
      /* create tlv packet with register address */
      tlv = tlvCreatePacket(TLV_READ_REGISTER, 4, (uint8_t *)registerAddress);
      tlvSend(session, tlv);
      session->readRegisterState = TLV_WAIT_RESPONSE;
      break;
      
    case TLV_WAIT_RESPONSE :
      // printf("Write Register Wait Response\n");
      response = tlvReceive(session);
      if(verifyTlvPacket(response)) {
        // printf("OK Reply\n");
        session->readRegisterState = TLV_SEND_PACKET;
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
  session->ONGOING_PROCESS_FLAG = true;
  
  switch(session->writeRAMState) {
    case TLV_SEND_PACKET :
      if(*size > 0) {
        if(*size > TLV_DATA_SIZE)
          tlvWriteDataChunk(session, dataAddress, *destAddress, TLV_DATA_SIZE);
        else
          tlvWriteDataChunk(session, dataAddress, *destAddress, *size);
        
        dataAddress += TLV_DATA_SIZE;
        *destAddress += TLV_DATA_SIZE;
        *size -= TLV_DATA_SIZE;
      }
      session->writeRAMState = TLV_WAIT_RESPONSE;
      break;
      
    case TLV_WAIT_RESPONSE :
      // printf("Write Register Wait Response\n");
      response = tlvReceive(session);
      if(verifyTlvPacket(response)) {
        // printf("OK Reply\n");
        session->writeRAMState = TLV_SEND_PACKET;
      }
      break;
  }
}

/** selectCommand is a function to select instruction 
  * base on tlv->type
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
void selectCommand(Tlv_Session *session, Tlv *tlv)  {
  static uint8_t dataAddress;
  switch(tlv->type) {
    case TLV_WRITE_RAM      : break;
    case TLV_READ_RAM       : break;
    case TLV_WRITE_REGISTER : tlvWriteTargetRegister(session, &get4Byte(&tlv->value[0]), &get4Byte(&tlv->value[4])); break;
    case TLV_READ_REGISTER  : tlvReadTargetRegister(session, &get4Byte(&tlv->value[0])); break;
    case TLV_HALT_TARGET    : break;
    case TLV_RUN_TARGET     : break;
    case TLV_STEP           : break;
  }
}

/** commandInterpreter
  */
// void commandInterpreter(Tlv_Session *session)  {
  // static Tlv *userCommand = NULL;
  // static Tlv *userCommand = NULL;
  // CEXCEPTION_T err;
  
  // switch(session->InterpreterState)  {
    // case HOST_RECEIVE_COMMAND :
       // session->userCommand = waitUserCommand();
       // if(userCommand != NULL)
         // session->InterpreterState = HOST_INTERPRET_COMMAND;
      // break;
  // }
// }

/** hostProgrammer
  */
void hostProgrammer(Tlv_Session *session)  {
  static Tlv *response;
  CEXCEPTION_T err;
  
  switch(session->programmerState)  {
    case HOST_INTERPRET_COMMAND :
      if(session->userCommand != NULL) {
        selectCommand(session, session->userCommand);
        // session->programmerState = HOST_WAITING_RESPONSE;
      }
      break;
      
    // case HOST_WAITING_RESPONSE :
      // printf("Write Register Wait Response\n");
      // response = tlvReceive(session);
      // if(verifyTlvPacket(response)) {
        // printf("OK Reply\n");
        // session->programmerState = HOST_INTERPRET_COMMAND;
      // }
      // break;
  }
}