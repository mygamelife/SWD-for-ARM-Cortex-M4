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
}

/** tlvWriteDataChunk is a function used to send data in chunk
  * by using tlv protocol
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *           memorySelect can be one of the following value :
  *             TLV_WRITE_RAM
  *             TLV_WRITE_FLASH
  *
  * return  : NONE
  */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size, Tlv_Command memorySelect) {
  Tlv *tlv; uint8_t chksum = 0;
  
  /* create tlv packet with register address */
  tlv = tlvCreatePacket(memorySelect, size + 4, NULL);
  
  chksum = tlvPackIntoBuffer(tlv->value, (uint8_t *)&destAddress, 4);
  chksum += tlvPackIntoBuffer(&tlv->value[4], dataAddress, size);

  /* Update checksum with destAddress */
  tlv->value[tlv->length - 1] = chksum;
  
  tlvSend(session, tlv);
}

/** tlvWriteTargetMemory is a function used to write data into target memory
  * by using tlv protocol
  * 
  * Input   : session contain a element/handler used by tlv protocol
  *           dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *           memorySelect can be one of the following value :
  *            - TLV_WRITE_RAM
  *            - TLV_WRITE_FLASH
  *
  * return  : NONE
  */
void tlvWriteTargetMemory(Tlv_Session *session, uint8_t **dataAddress, uint32_t *destAddress, int *size, Tlv_Command memorySelect)  {

  if(*size > TLV_DATA_SIZE) 
    tlvWriteDataChunk(session, *dataAddress, *destAddress, TLV_DATA_SIZE, memorySelect);
  else 
    tlvWriteDataChunk(session, *dataAddress, *destAddress, *size, memorySelect);
  
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
  tlv = tlvCreatePacket(TLV_READ_MEMORY, 8, (uint8_t *)buffer);

  tlvSend(session, tlv);
}

/** tlvReadTargetMemory is a function to read data from target memory
  * base on tlv->type
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           destAddress is the address of the data need to be read
  *           size is the size of the data can be any value
  *
  * Return  : NONE
  */
void tlvReadTargetMemory(Tlv_Session *session, uint32_t *destAddress, int *size) {
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

/** tlvLoadProgram is a function to load program into target memory
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target RAM
  *           memorySelect can be one of the following value :
  *            - TLV_WRITE_RAM
  *            - TLV_WRITE_FLASH
  *
  * Return  : NONE
  */
void tlvLoadProgram(Tlv_Session *session, char *file, Tlv_Command memorySelect) {
  static ElfData *elfData;
  static ElfSection *isr, *rodata, *data, *text;
  session->ongoingProcessFlag = true;
  
  switch(session->loadProgramState) {
    case TLV_OPEN_FILE :
      elfData = openElfFile(file);
      isr     = getElfSectionInfo(elfData, ".isr_vector");
      rodata  = getElfSectionInfo(elfData, ".rodata");
      data    = getElfSectionInfo(elfData, ".data");
      text    = getElfSectionInfo(elfData, ".text");

      tlvWriteTargetRegister(session, PROGRAM_COUNTER, &get4Byte(&isr->dataAddress[4]));
      session->loadProgramState = TLV_LOAD_ISR_VECTOR;
      break;
    
    case TLV_LOAD_ISR_VECTOR :
      tlvWriteTargetMemory(session, &isr->dataAddress, &isr->destAddress, &isr->size, memorySelect);
      
      if(isr->size <= 0) {
        printf("finish load TLV_LOAD_ISR_VECTOR\n");
        free(isr);
        session->loadProgramState = TLV_LOAD_RO_DATA;
      }
      break;
      
    case TLV_LOAD_RO_DATA :
      tlvWriteTargetMemory(session, &rodata->dataAddress, &rodata->destAddress, &rodata->size, memorySelect);
        
      if(rodata->size <= 0) {
        printf("finish load TLV_LOAD_RO_DATA\n");
        free(rodata);
        session->loadProgramState = TLV_LOAD_DATA;
      }
      break;
      
    case TLV_LOAD_DATA :
      tlvWriteTargetMemory(session, &data->dataAddress, &data->destAddress, &data->size, memorySelect);
        
      if(data->size <= 0) {
        printf("finish load TLV_LOAD_DATA\n");
        free(data);
        session->loadProgramState = TLV_LOAD_TEXT;
      }
      break;
      
    case TLV_LOAD_TEXT :
      tlvWriteTargetMemory(session, &text->dataAddress, &text->destAddress, &text->size, memorySelect);
        
      if(text->size <= 0) {
        printf("finish load TLV_LOAD_TEXT\n");
        free(text);   closeElfFile(elfData);
        session->ongoingProcessFlag = false;
        session->loadProgramState = TLV_OPEN_FILE;
      }
      break;
  }
}

/** tlvLoadProgramToFlash
  *
  * Input   : NONE
  *
  * Return  : NONE
  */
void tlvLoadToFlash(Tlv_Session *session, char *file)  {
  tlvLoadToRam(session, file); //Load FlashProgrammer into target RAM
  // tlvLoadProgram(session, file, TLV_WRITE_RAM);
  // tlvLoadProgram(session, file, TLV_WRITE_RAM);
  //1. Load Flash Prgrammger tlvLoadProgramToRam(Tlv_Session *session, char *fileName);
  //2. Load Actual Program
}

void tlvHaltTarget(Tlv_Session *session)  {
  
  Tlv *tlv = tlvCreatePacket(TLV_HALT_TARGET, 0, 0);

  tlvSend(session, tlv);
}

void tlvRunTarget(Tlv_Session *session) {
  
  Tlv *tlv = tlvCreatePacket(TLV_RUN_TARGET, 0, 0);

  tlvSend(session, tlv);
}

void tlvMultipleStepTarget(Tlv_Session *session, int nInstructions) {

  Tlv *tlv = tlvCreatePacket(TLV_STEP, 4, (uint8_t *)&nInstructions);

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
    case TLV_WRITE_RAM        : tlvLoadToRam(session, userSession->fileName); break;
    case TLV_WRITE_FLASH      : tlvLoadToFlash(session, userSession->fileName); break;
    case TLV_READ_MEMORY      : tlvReadTargetMemory(session, &userSession->address, &userSession->size); break;
    case TLV_WRITE_REGISTER   : tlvWriteTargetRegister(session, userSession->address, userSession->data); break;
    case TLV_READ_REGISTER    : tlvReadTargetRegister(session, userSession->address); break;
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
  
  switch(session->hostState)  {
    case HOST_WAIT_USER_COMMAND :
      userSession = waitUserCommand();
      if(userSession->tlvCommand == TLV_EXIT) 
        session->hostState = HOST_EXIT;
      else  session->hostState = HOST_INTERPRET_COMMAND;
      break;
      
    case HOST_INTERPRET_COMMAND :
      selectCommand(session, userSession);
      session->hostState = HOST_WAITING_RESPONSE;
      break;
      
    case HOST_WAITING_RESPONSE :
      response = tlvReceive(session);

      if(verifyTlvPacket(response)) {
        #if !defined (TEST)
          displayTlvData(response); 
        #endif
        
        if(session->ongoingProcessFlag == false)
          session->hostState = HOST_WAIT_USER_COMMAND;
        else  session->hostState = HOST_INTERPRET_COMMAND;
      }
      break;
  }
}