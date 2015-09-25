#include "ProgramLoader.h"

int fileStatus = FILE_CLOSED;
static uint32_t entryAddress = 0;

static ElfData *elfData;
static ElfSection *isr, *rodata, *data, *text;
  
char *FLASH_PROGRAMMER_FILE_PATH = "C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/FlashProgrammer/FlashProgrammer/Debug/bin/FlashProgrammer.elf";
        
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
  session->ongoingProcessFlag = FLAG_SET;

  if(*size > TLV_DATA_SIZE) {
    tlvReadDataChunk(session, *destAddress, TLV_DATA_SIZE);
  }
  else  {
    tlvReadDataChunk(session, *destAddress, *size);
    session->ongoingProcessFlag = FLAG_CLEAR;
  }
  
  *destAddress += TLV_DATA_SIZE;
  *size -= TLV_DATA_SIZE;
}

/** !< Internal Function used by Load Program To Memory >!
  * tlvLoadProgram is a function to load program into target memory
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *           memorySelect can be one of the following value :
  *            - TLV_WRITE_RAM
  *            - TLV_WRITE_FLASH
  *
  * Return  : NONE
  */
void tlvLoadProgram(Tlv_Session *session, char *file, Tlv_Command memorySelect) {
  
  switch(session->loadProgramState) {
    
    case TLV_LOAD_ISR_VECTOR :
      if(fileStatus == FILE_CLOSED) {
        fileStatus = FILE_OPENED;
        
        elfData = openElfFile(file);
        isr     = getElfSectionInfo(elfData, ".isr_vector");
        rodata  = getElfSectionInfo(elfData, ".rodata");
        data    = getElfSectionInfo(elfData, ".data");
        text    = getElfSectionInfo(elfData, ".text");
        
        entryAddress = get4Byte(&isr->dataAddress[4]);
        session->ongoingProcessFlag = FLAG_SET;
      }
      
      tlvWriteTargetMemory(session, &isr->dataAddress, &isr->destAddress, &isr->size, memorySelect);
      
      if(isr->size <= 0) {
        printf("finish load TLV_LOAD_ISR_VECTOR\n");
        free(isr); session->loadProgramState = TLV_LOAD_RO_DATA;
      }
    break;
      
    case TLV_LOAD_RO_DATA :
      tlvWriteTargetMemory(session, &rodata->dataAddress, &rodata->destAddress, &rodata->size, memorySelect);
        
      if(rodata->size <= 0) {
        printf("finish load TLV_LOAD_RO_DATA\n");
        free(rodata); session->loadProgramState = TLV_LOAD_DATA;
      }
    break;
      
    case TLV_LOAD_DATA :
      tlvWriteTargetMemory(session, &data->dataAddress, &data->destAddress, &data->size, memorySelect);
        
      if(data->size <= 0) {
        printf("finish load TLV_LOAD_DATA\n");
        free(data); session->loadProgramState = TLV_LOAD_TEXT;
      }
    break;
      
    case TLV_LOAD_TEXT :
      printf("text->size %d\n", text->size);
      tlvWriteTargetMemory(session, &text->dataAddress, &text->destAddress, &text->size, memorySelect);
      
      if(text->size <= 0) {
        printf("finish load TLV_LOAD_TEXT\n");
        /* Close elf file */
        free(text); closeElfFile(elfData); fileStatus = FILE_CLOSED;
        session->ongoingProcessFlag = FLAG_CLEAR;
        session->loadProgramState = TLV_LOAD_ISR_VECTOR;
      }
    break;
  }
}

/** !< Internal Function used by Load Program To Flash >!
  * tlvLoadAndRunFlashProgrammer is a function to load and run flash programmer on target sram
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : NONE
  */
void tlvLoadAndRunFlashProgrammer(Tlv_Session *session, char *file) {
  
  switch(session->fPState) {
    
    case TLV_LOAD_FLASH_PROGRAMMER :
      /* Load FlashProgrammer into target RAM */
      tlvLoadToRam(session, file);
      
      if(session->ongoingProcessFlag == FLAG_CLEAR) {
        session->ongoingProcessFlag = FLAG_SET;
        session->fPState = TLV_RUN_FLASH_PROGRAMMER;
      }
      break;
    
    case TLV_RUN_FLASH_PROGRAMMER :
      /* Run loaded FlashProgrammer in the target */
      tlvRunTarget(session);
      session->fPFlag = RUNNING;
      session->fPState = TLV_LOAD_FLASH_PROGRAMMER;
    break;
  }
}

/** tlvFlashErase
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *
  * Return  : NONE
  */
void tlvFlashErase(Tlv_Session *session, uint32_t address, int size) {
  Tlv *tlv; 
  static uint32_t tempAddress = 0; static int tempSize = 0;
  
  if(bankSelection == 0) bankSelection = banks;
  
  if(session->fPFlag == NOT_RUNNING)
    tlvLoadAndRunFlashProgrammer(session, FLASH_PROGRAMMER_FILE_PATH);
    
  else {
    session->ongoingProcessFlag = FLAG_CLEAR;
    /* create tlv packet address and size */
    tlv = tlvCreatePacket(TLV_FLASH_ERASE, 8, (uint8_t *)buffer);
    tlvSend(session, tlv);
  }
}

/** tlvFlashMassErase
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *
  * Return  : NONE
  */
void tlvFlashMassErase(Tlv_Session *session, uint32_t banks) {
  Tlv *tlv; static uint32_t bankSelection = 0;
  
  if(bankSelection == 0) bankSelection = banks;
  
  if(session->fPFlag == NOT_RUNNING)
    tlvLoadAndRunFlashProgrammer(session, FLASH_PROGRAMMER_FILE_PATH);
    
  else {
    session->ongoingProcessFlag = FLAG_CLEAR;
    /* create tlv packet address and size */
    tlv = tlvCreatePacket(TLV_FLASH_MASS_ERASE, 4, (uint8_t *)&bankSelection);
    bankSelection = 0;
    tlvSend(session, tlv);
  }
}

/** tlvLoadToRam is a function to load elf file and update PC 
  * to the program entry addresss
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *
  * Return  : NONE
  */
void tlvLoadToRam(Tlv_Session *session, char *file) {
  
  switch(session->ramState) {
    
    case TLV_LOAD_PROGRAM :
      /* Load elf file isr_vector, ro_data, data
         and text section into target RAM */
      tlvLoadProgram(session, file, TLV_WRITE_RAM);
      
      if(session->ongoingProcessFlag == FLAG_CLEAR) {
        session->ongoingProcessFlag = FLAG_SET;
        session->ramState = TLV_UPDATE_PC;
      }
    break;
    
    case TLV_UPDATE_PC :        
      /* Update program counter to the entry address 
         of the loaded program */
      printf("entryAddress %x\n", entryAddress);
      tlvWriteTargetRegister(session, PC, &entryAddress);
      
      session->ongoingProcessFlag = FLAG_CLEAR;
      session->fPFlag = NOT_RUNNING;
      session->ramState = TLV_LOAD_PROGRAM;
    break;
  }
}

/** tlvLoadToFlash is a function to load elf file and update PC 
  * to the program entry addresss
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *
  * Return  : NONE
  */
void tlvLoadToFlash(Tlv_Session *session, char *file)  {
      
  switch(session->flashState) {
    
    case TLV_LOAD_PROGRAM :
    
      if(session->fPFlag == NOT_RUNNING)
        tlvLoadAndRunFlashProgrammer(session, FLASH_PROGRAMMER_FILE_PATH);

      else {
        /* Load elf file isr_vector, ro_data, data
         and text section into target RAM */
        tlvLoadProgram(session, file, TLV_WRITE_FLASH);
        
        /* Change state when the program is finish transmit */
        if(session->ongoingProcessFlag == FLAG_CLEAR) {
          session->ongoingProcessFlag = FLAG_SET;
          session->flashState = TLV_UPDATE_PC;
        }
      }
      break;
    
    case TLV_UPDATE_PC :
      /* Update program counter to the entry address 
         of the loaded program */
      printf("entryAddress %x\n", entryAddress);
      tlvWriteTargetRegister(session, PC, &entryAddress);
      
      session->ongoingProcessFlag = FLAG_CLEAR;
      session->fPFlag = NOT_RUNNING;
      session->flashState = TLV_LOAD_PROGRAM;
      break;
  }
}

void tlvHaltTarget(Tlv_Session *session) {
  
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

/** tlvSetBreakpoint is a function to set breakpoint
  * at specified address
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           address is the address want to be stop at
  *
  * Return  : NONE
  */
void tlvSetBreakpoint(Tlv_Session *session, uint32_t address) {
  Tlv *tlv = tlvCreatePacket(TLV_BREAKPOINT, 4, (uint8_t *)&address);

  tlvSend(session, tlv);
}

/** tlvSoftReset is a function software reset target
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : NONE
  */
void tlvSoftReset(Tlv_Session *session) {
  Tlv *tlv = tlvCreatePacket(TLV_SOFT_RESET, 0, 0);

  tlvSend(session, tlv);
}

/** tlvSoftReset is a function hardware reset target
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : NONE
  */
void tlvHardReset(Tlv_Session *session) {
  Tlv *tlv = tlvCreatePacket(TLV_HARD_RESET, 0, 0);

  tlvSend(session, tlv);
}

/** selectCommand is a function to select instruction 
  * base on userSession
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
void selectCommand(Tlv_Session *session, User_Session *userSession) {

  switch(userSession->tlvCommand) {
    case TLV_WRITE_RAM          : tlvLoadToRam(session, userSession->fileName);                                                 break;
    case TLV_WRITE_FLASH        : tlvLoadToFlash(session, userSession->fileName);                                               break;
    case TLV_READ_MEMORY        : tlvReadTargetMemory(session, &userSession->address, &userSession->size);                      break;
    case TLV_WRITE_REGISTER     : tlvWriteTargetRegister(session, userSession->address, userSession->data);                     break;
    case TLV_READ_REGISTER      : tlvReadTargetRegister(session, userSession->address);                                         break;
    case TLV_HALT_TARGET        : tlvHaltTarget(session);                                                                       break;
    case TLV_RUN_TARGET         : tlvRunTarget(session);                                                                        break;
    case TLV_STEP               : tlvMultipleStepTarget(session, (int)(*userSession->data));                                    break;
    case TLV_BREAKPOINT         : tlvSetBreakpoint(session, userSession->address);                                              break;
    case TLV_FLASH_ERASE        : tlvFlashErase(session, userSession->address, userSession->size);                              break;
    case TLV_FLASH_MASS_ERASE   : tlvFlashMassErase(session, *userSession->data);                                               break;
    case TLV_SOFT_RESET         : tlvSoftReset(session);                                                                        break;
    case TLV_HARD_RESET         : tlvHardReset(session);                                                                        break;
  }
}

/** isLastOperationDone is a function to check last operation
  * state if still ongoing it will continue intepret
  *
  * input   : session contain a element/handler used by tlv protocol
  *
  * return  : NONE
  */
void isLastOperationDone(Tlv_Session *session) {
  if(session->ongoingProcessFlag == FLAG_CLEAR)
    session->hostState = HOST_WAIT_USER_COMMAND;
  else
    session->hostState = HOST_INTERPRET_COMMAND;
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
      if(userSession->tlvCommand == TLV_EXIT) 
        session->hostState = HOST_EXIT;
      else session->hostState = HOST_INTERPRET_COMMAND;
    break;
      
    case HOST_INTERPRET_COMMAND :
      selectCommand(session, userSession);
      session->hostState = HOST_WAITING_RESPONSE;
    break;
      
    case HOST_WAITING_RESPONSE :
      response = tlvReceive(session);
      
      if(response == NULL)
        printf("waiting response\n");

      if(verifyTlvPacket(response)) {
        elapsedTime = 0;
        
        #if !defined (TEST)
        displayTlvData(response); 
        #endif
        isLastOperationDone(session);
      }
    break;
  }
}