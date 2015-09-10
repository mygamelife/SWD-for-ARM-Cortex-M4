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
  session->ongoingProcessFlag = false;
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
  session->ongoingProcessFlag = false;
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
  Tlv *tlv; uint8_t chksum = 0;
  
  /* create tlv packet with register address */
  tlv = tlvCreatePacket(TLV_WRITE_RAM, size + 4, NULL);
  
  chksum = tlvPackIntoBuffer(tlv->value, (uint8_t *)&destAddress, 4);
  chksum += tlvPackIntoBuffer(&tlv->value[4], dataAddress, size);

  /* Update checksum with destAddress */
  tlv->value[tlv->length - 1] = chksum;
  
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
void tlvWriteTargetRam(Tlv_Session *session, uint8_t **dataAddress, uint32_t *destAddress, int *size)  {
  session->ongoingProcessFlag = true;

  if(*size > TLV_DATA_SIZE) {
    printf("here 1\n");
    tlvWriteDataChunk(session, *dataAddress, *destAddress, TLV_DATA_SIZE);
  }
  else  {
    printf("here 2\n");
    tlvWriteDataChunk(session, *dataAddress, *destAddress, *size);
    session->ongoingProcessFlag = false;
  }
  
  *dataAddress += TLV_DATA_SIZE;
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
  session->ongoingProcessFlag = true;

  if(*size > TLV_DATA_SIZE) {
    tlvReadDataChunk(session, *destAddress, TLV_DATA_SIZE);
  }
  else  {
    tlvReadDataChunk(session, *destAddress, *size);
    session->ongoingProcessFlag = false;
  }
  
  *destAddress += TLV_DATA_SIZE;
  *size -= TLV_DATA_SIZE;
}

/** extractElfFile is a function to extract the file info
  * by using elf reader function
  *
  * Input   : NONE
  *
  * Return  : NONE
  */
FileInfo *extractElfFile(ElfData *elfData, char *section)  {
  FileInfo *file = malloc(sizeof(FileInfo));
  
  file->index = getIndexOfSectionByName(elfData, section);
  file->dataAddress = (uint8_t *)getSectionAddress(elfData, file->index);
  file->destAddress = getSectionHeaderAddrUsingIndex(elfData, file->index);
  file->size = (int)getSectionSize(elfData, file->index);

  return file;
}

/** tlvLoadProgramToRam
  *
  * Input   : NONE
  *
  * Return  : NONE
  */
void tlvLoadProgramToRam(Tlv_Session *session, char *fileName) {
  static FileInfo *file;
  static int fileStatus = 0;
  static ElfData *elfData;
  
  switch(session->loadProgramState) {
    case TLV_LOAD_ISR_VECTOR :
      if(fileStatus != 1) {
        fileStatus = 1;
        elfData = openElfFile(fileName);
        file = extractElfFile(elfData, ".isr_vector");
      }
    
      printf("load TLV_LOAD_ISR_VECTOR\n");
      tlvWriteTargetRam(session, &file->dataAddress, &file->destAddress, &file->size);
      session->ongoingProcessFlag = true;
      
      if(file->size <= 0) {
        printf("finish load TLV_LOAD_ISR_VECTOR\n");
        // free(file);
        fileStatus = 0;
        session->loadProgramState = TLV_LOAD_RO_DATA;
      }
      break;
      
    case TLV_LOAD_RO_DATA :
      if(fileStatus != 1) {
        fileStatus = 1;
        file = extractElfFile(elfData, ".rodata");
      }
      printf("TLV_LOAD_RO_DATA\n");
      tlvWriteTargetRam(session, &file->dataAddress, &file->destAddress, &file->size);
      session->ongoingProcessFlag = true;
        
      if(file->size <= 0) {
        printf("finish load TLV_LOAD_RO_DATA\n");
        // free(file);
        fileStatus = 0;
        session->loadProgramState = TLV_LOAD_DATA;
      }
      break;
      
    case TLV_LOAD_DATA :
      if(fileStatus != 1) {
        fileStatus = 1;
        file = extractElfFile(elfData, ".data");
      }
      
      tlvWriteTargetRam(session, &file->dataAddress, &file->destAddress, &file->size);
      session->ongoingProcessFlag = true;
        
      if(file->size <= 0) {
        printf("finish load TLV_LOAD_DATA\n");
        // free(file);
        fileStatus = 0;
        session->loadProgramState = TLV_LOAD_TEXT;
      }
      break;
      
    case TLV_LOAD_TEXT :
      if(fileStatus != 1) {
        fileStatus = 1;
        file = extractElfFile(elfData, ".text");
      }
      
      tlvWriteTargetRam(session, &file->dataAddress, &file->destAddress, &file->size);
      session->ongoingProcessFlag = true;
        
      if(file->size <= 0) {
        printf("finish load TLV_LOAD_TEXT\n");
        // free(file);
        fileStatus = 0;
        session->ongoingProcessFlag = false;
        session->loadProgramState = TLV_LOAD_ISR_VECTOR;
      }
      break;
  }
}

void tlvHaltTarget(Tlv_Session *session)  {
  /* create tlv packet with register address */
  Tlv *tlv = tlvCreatePacket(TLV_HALT_TARGET, 0, 0);

  tlvSend(session, tlv);
}

void tlvRunTarget(Tlv_Session *session) {
  /* create tlv packet with register address */
  Tlv *tlv = tlvCreatePacket(TLV_RUN_TARGET, 0, 0);

  tlvSend(session, tlv);
}

void tlvMultipleStepTarget(Tlv_Session *session, int nInstructions) {
  printf("nInstructions %d\n", nInstructions);
  /* create tlv packet with register address */
  Tlv *tlv = tlvCreatePacket(TLV_RUN_TARGET, 4, (uint8_t *)&nInstructions);

  tlvSend(session, tlv);
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
    case TLV_WRITE_RAM        : {
      uint8_t *pData = (uint8_t *)userSession->data;
      tlvWriteTargetRam(session, &pData, &userSession->address, &userSession->size); break;
    }
    case TLV_READ_RAM         : tlvReadTargetRam(session, &userSession->address, &userSession->size); break;
    case TLV_WRITE_REGISTER   : tlvWriteTargetRegister(session, userSession->address, userSession->data); break;
    case TLV_READ_REGISTER    : tlvReadTargetRegister(session, userSession->address); break;
    case TLV_LOAD_PROGRAM_RAM : tlvLoadProgramToRam(session, userSession->fileName); break;
    case TLV_HALT_TARGET      : tlvHaltTarget(session); break;
    case TLV_RUN_TARGET       : tlvRunTarget(session); break;
    case TLV_STEP             : tlvMultipleStepTarget(session, (int)(*userSession->data)); break;
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
      // printf("HOST_INTERPRET_COMMAND\n");
      if(userSession->tlvCommand == TLV_EXIT)
        session->hostState = HOST_EXIT;
      else  
        session->hostState = HOST_INTERPRET_COMMAND;
      break;
      
    case HOST_INTERPRET_COMMAND :
      selectCommand(session, userSession);
      // printf("HOST_WAITING_RESPONSE\n");
      session->hostState = HOST_WAITING_RESPONSE;
      break;
      
    case HOST_WAITING_RESPONSE :
      response = tlvReceive(session);

      if(verifyTlvPacket(response)) {
        #if !defined (TEST)
        displayTlvData(response);
        #endif
        if(session->ongoingProcessFlag == false)  {
          session->hostState = HOST_WAIT_USER_COMMAND;
        }
        else  session->hostState = HOST_INTERPRET_COMMAND;
      }
      break;
  }
}