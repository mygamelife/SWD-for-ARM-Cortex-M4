#include "ProgramLoader.h"

/** tlvWriteTargetRegister is a function to write data into target register
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *           data is the data need to write into the target register
  *
  * return  : NONE
  */
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t *data)  {
  uint32_t buffer[] = {registerAddress, *data};
  
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
  char chksum = 0;
  
  /* create tlv packet with register address */
  tlv = tlvCreatePacket(TLV_WRITE_RAM, 4, (uint8_t *)&destAddress);
  chksum = tlv->value[4];
  tlvPackIntoBuffer(&tlv->value[4], dataAddress, size);
  tlv->length += size;

  tlv->value[tlv->length - 1] = tlv->value[tlv->length - 1] + chksum;

  tlvSend(session, tlv);
}

/** tlvWriteTargetRam is a function used to write data into target RAM
  * by using tlv protocol
  * 
  * Input   : session contain a element/handler used by tlv protocol
  *           dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *
  * return  : NONE
  */
void tlvWriteTargetRam(Tlv_Session *session, uint32_t *dataAddress, uint32_t *destAddress, int *size)  {
  session->ONGOING_PROCESS_FLAG = true;

  if(*size > TLV_DATA_SIZE) {
    tlvWriteDataChunk(session, (uint8_t *)dataAddress, *destAddress, TLV_DATA_SIZE);
  }
  else  {
    tlvWriteDataChunk(session, (uint8_t *)dataAddress, *destAddress, *size);
    session->ONGOING_PROCESS_FLAG = false;
  }
  
  dataAddress += TLV_DATA_SIZE;
  *destAddress += TLV_DATA_SIZE;
  *size -= TLV_DATA_SIZE;
}

/** tlvReadDataChunk is a function used to read data in chunk
  * by using tlv protocol
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *
  * return  : NONE
  */
void tlvReadDataChunk(Tlv_Session *session, uint32_t destAddress, int size) {
  Tlv *tlv;
  uint32_t buffer[] = {destAddress, size};
  
  /* create tlv packet with register address */
  tlv = tlvCreatePacket(TLV_READ_RAM, 8, (uint8_t *)buffer);

  tlvSend(session, tlv);
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
void tlvReadTargetRam(Tlv_Session *session, uint32_t *destAddress, int *size) {
  Tlv *tlv; 
  session->ONGOING_PROCESS_FLAG = true;

  if(*size > TLV_DATA_SIZE) {
    tlvReadDataChunk(session, *destAddress, *size);
  }
  else  {
    tlvReadDataChunk(session, *destAddress, *size);
    session->ONGOING_PROCESS_FLAG = false;
  }
  
  *destAddress += TLV_DATA_SIZE;
  *size -= TLV_DATA_SIZE;
}

/** selectCommand is a function to select instruction 
  * base on tlv->type
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
void selectCommand(Tlv_Session *session, User_Session *userSession) {

  switch(userSession->tlvCommand) {
    case TLV_WRITE_RAM      : tlvWriteTargetRam(session, userSession->data, &userSession->address, &userSession->size); break;
    case TLV_READ_RAM       : tlvReadTargetRam(session, &userSession->address, &userSession->size); break;
    case TLV_WRITE_REGISTER : tlvWriteTargetRegister(session, userSession->address, userSession->data); break;
    case TLV_READ_REGISTER  : tlvReadTargetRegister(session, userSession->address); break;
    case TLV_HALT_TARGET    : break;
    case TLV_RUN_TARGET     : break;
    case TLV_STEP           : break;
  }
}

/** hostInterpreter
  */
void hostInterpreter(Tlv_Session *session) {
  static Tlv *response;
  static User_Session *userSession;
  CEXCEPTION_T err;
  
  switch(session->hostState)  {
    case HOST_WAIT_USER_COMMAND :
      userSession = waitUserCommand();
      printf("HOST_INTERPRET_COMMAND\n");
      if(userSession->tlvCommand == TLV_EXIT)
        session->hostState = HOST_EXIT;
      else  
        session->hostState = HOST_INTERPRET_COMMAND;
      break;
      
    case HOST_INTERPRET_COMMAND :
      selectCommand(session, userSession);
      printf("HOST_WAITING_RESPONSE\n");
      session->hostState = HOST_WAITING_RESPONSE;
      break;
      
    case HOST_WAITING_RESPONSE :
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