#include "ProgramLoader.h"

/** tlvWriteTargetRegister is a function to write data into target register
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *           data is the data need to write into the target register
  *
  * return  : NONE
  */
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t data)  {
  uint32_t buffer[] = {registerAddress, data};
  
  /* create tlv packet with register address */
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_REGISTER, 8, (uint8_t *)buffer);
  tlvSend(session, tlv);
  
  /* clear userCommand after the task is done*/
  session->ONGOING_PROCESS_FLAG = false;
}

/** tlvReadTargetRegister is a function to read target register
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *
  * return  : NONE
  */
void tlvReadTargetRegister(Tlv_Session *session, uint32_t registerAddress)  {
  
  /* create tlv packet with register address */
  Tlv *tlv = tlvCreatePacket(TLV_READ_REGISTER, 4, (uint8_t *)&registerAddress);
  tlvSend(session, tlv);
  
  /* clear userCommand after the task is done*/
  session->ONGOING_PROCESS_FLAG = false;
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
void tlvWriteTargetRam(Tlv_Session *session, uint32_t *dataAddress, uint32_t *destAddress, int *size)  {
  session->ONGOING_PROCESS_FLAG = true;

  if(*size > TLV_DATA_SIZE)
    tlvWriteDataChunk(session, (uint8_t *)dataAddress, *destAddress, TLV_DATA_SIZE);
  else  {
    tlvWriteDataChunk(session, (uint8_t *)dataAddress, *destAddress, *size);
    session->ONGOING_PROCESS_FLAG = false;
  }
  
  dataAddress += TLV_DATA_SIZE;
  *destAddress += TLV_DATA_SIZE;
  *size -= TLV_DATA_SIZE;
}

/** tlvReadTargetRam is a function to read data from target RAM
  * base on tlv->type
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           destAddress is the address of the data need to be read
  *           size is the size of the data can be any value
  *
  * Return  : NONE
  */
void tlvReadTargetRam(Tlv_Session *session, uint32_t destAddress, int size) {
  uint32_t buffer[] = {destAddress, size};
  
  /* create tlv packet with register address */
  Tlv *tlv = tlvCreatePacket(TLV_READ_RAM, 8, (uint8_t *)buffer);
  tlvSend(session, tlv);
  
  /* clear userCommand after the task is done*/
  session->ONGOING_PROCESS_FLAG = false;
}

/** selectCommand is a function to select instruction 
  * base on tlv->type
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
void selectCommand(Tlv_Session *session, Tlv *tlv) {

  switch(tlv->type) {
    case TLV_WRITE_RAM      : tlvWriteTargetRam(session, &get4Byte(&tlv->value[8]), &get4Byte(&tlv->value[0]), &get4Byte(&tlv->value[4])); break;
    case TLV_READ_RAM       : tlvReadTargetRam(session, get4Byte(&tlv->value[0]), (int)get4Byte(&tlv->value[4])); break;
    case TLV_WRITE_REGISTER : tlvWriteTargetRegister(session, get4Byte(&tlv->value[0]), get4Byte(&tlv->value[4])); break;
    case TLV_READ_REGISTER  : tlvReadTargetRegister(session, get4Byte(&tlv->value[0])); break;
    case TLV_HALT_TARGET    : break;
    case TLV_RUN_TARGET     : break;
    case TLV_STEP           : break;
  }
}

/** hostInterpreter
  */
void hostInterpreter(Tlv_Session *session) {
  static Tlv *response, *userCommand;
  CEXCEPTION_T err;
  
  switch(session->hostState)  {
    case HOST_WAIT_USER_COMMAND :
      session->userCommand = waitUserCommand();
      if(session->userCommand->type == TLV_EXIT)
        session->hostState = HOST_EXIT;
      else  
        session->hostState = HOST_INTERPRET_COMMAND;
      break;
      
    case HOST_INTERPRET_COMMAND :
      // printf("HOST_INTERPRET_COMMAND\n");
      selectCommand(session, session->userCommand);
      session->hostState = HOST_WAITING_RESPONSE;
      break;
      
    case HOST_WAITING_RESPONSE :
      // printf("HOST_WAITING_RESPONSE\n");
      response = tlvReceive(session);
      
      if(verifyTlvPacket(response)) {
        #if !defined (TEST)
        displayTlvData(response);
        #endif
        if(session->ONGOING_PROCESS_FLAG == false)  {
          session->hostState = HOST_WAIT_USER_COMMAND;
        }
        else  session->hostState = HOST_INTERPRET_COMMAND;
      }
      break;
  }
}