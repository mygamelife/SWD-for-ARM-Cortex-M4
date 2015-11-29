#include "ProgramLoader.h"

static int programSize = 0;
static Tlv *response;
static User_Session *userSession;
static uint32_t FLASH_BEGIN_ADDRESS = 0x08000000;

/** writeRegister is a function to write data into target register
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *           data is the data need to write into the target register
  *
  * return  : 1 successfully write into target register
  *           0 waiting reply from target
  */
Process_Status writeRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t data) {
  uint32_t dataBuffer[] = {registerAddress, data};
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  
  /* Start task */
  startTask(tb);
  
  /* Send tlv request */
  tlvSendRequest(session, TLV_WRITE_REGISTER, 8, (uint8_t *)dataBuffer);
  
  /* Waiting reply from probe */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(TWO_SECOND), tb);
    yield(tb);
  };
  
  resetSystemTime();
  
  /* End tlv request task */
  endTask(tb);
  
  returnThis(PROCESS_DONE, tb);
}

/** readRegister is a function to read target register
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *
  * return  : register value if successfully read from probe
  *           0 if waiting reply from probe
  */
uint32_t readRegister(Tlv_Session *session, uint32_t registerAddress) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  
  /* Start task */
  startTask(tb);
  
  /* Send tlv request */
  tlvSendRequest(session, TLV_READ_REGISTER, 4, (uint8_t *)&registerAddress);
  
  /* Waiting reply from probe */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(TWO_SECOND), tb);
    yield(tb);
  };
  
  resetSystemTime();
  
  /* End task */
  endTask(tb);
  
  #ifdef HOST
  printf("value %x\n", get4Byte(&response->value[0]));
  #endif
  
  returnThis(get4Byte(&response->value[0]), tb);
}

/** halt is a function halt target processor
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *
  * return  : 1 if successfully halted
  *           0 if waiting reply from probe
  */
Process_Status halt(Tlv_Session *session) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  
  /* Start tlv request task */
  startTask(tb);

  /* Send tlv request */
  tlvSendRequest(session, TLV_HALT_TARGET, 0, NULL);
  
  /* Waiting reply from probe */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(TWO_SECOND), tb);
    yield(tb);
  };
  
  resetSystemTime();
  
  /* End tlv request task */
  endTask(tb);
  
  returnThis(PROCESS_DONE, tb);
}

/** run is a function run target processor
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           registerAddress is the address of the target register
  *
  * return  : 1 if successfully run
  *           0 if waiting reply from probe
  */
Process_Status run(Tlv_Session *session) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  
  /* Start tlv request task */
  startTask(tb);
  
  /* Send tlv request */
  tlvSendRequest(session, TLV_RUN_TARGET, 0, NULL);
  
  /* Waiting reply from probe */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(TWO_SECOND), tb);
    yield(tb);
  };
  
  resetSystemTime();
  
  /* End tlv request task */
  endTask(tb);
  
  returnThis(PROCESS_DONE, tb);
}

/** tlvMultipleStepTarget is a function send tlv request to
  * execute number of instructions
  * 
  * input   : session contain a element/handler used by tlv protocol
  *           nInstructions is the number of instruction need to execute
  *
  * return  : Current program counter address if successfully step
  *           0 if waiting reply from probe
  */
uint32_t tlvMultipleStepTarget(Tlv_Session *session, int nInstructions) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);

  /* Start tlv request task */
  startTask(tb);
  
  /* Send tlv request */
  tlvSendRequest(session, TLV_STEP, 4, (uint8_t *)&nInstructions);
  
  /* Waiting reply from probe */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(TWO_SECOND), tb);
    yield(tb);
  };
  
  resetSystemTime();
  
  /* End tlv request task */
  endTask(tb);
  
  #ifdef HOST
  printf("value %x\n", get4Byte(&response->value[0]));
  #endif
  
  returnThis(get4Byte(&response->value[0]), tb);
}

/** tlvSoftReset is a function software reset target
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : 1 if successfully software reset
  *           0 if waiting reply from probe
  */
Process_Status tlvSoftReset(Tlv_Session *session) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  
  /* Start tlv request task */
  startTask(tb);
  
  /* Send tlv request */
  tlvSendRequest(session, TLV_SOFT_RESET, 0, NULL);
  
  /* Waiting reply from probe */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(TWO_SECOND), tb);
    yield(tb);
  };
  
  resetSystemTime();
  
  /* End tlv request task */
  endTask(tb);
  
  returnThis(PROCESS_DONE, tb);
}

/** tlvHardReset is a function hardware reset target
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : 1 if successfully software reset
  *           0 if waiting reply from probe
  */
Process_Status tlvHardReset(Tlv_Session *session) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  
  /* Start tlv request task */
  startTask(tb);
  
  /* Send tlv request */
  tlvSendRequest(session, TLV_HARD_RESET, 0, NULL);
  
  /* Waiting reply from probe */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(TWO_SECOND), tb);
    yield(tb);
  };
  
  resetSystemTime();
  
  /* End tlv request task */
  endTask(tb);
  
  returnThis(PROCESS_DONE, tb);
}

/** tlvVectReset is a function vector reset target
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : 1 if successfully software reset
  *           0 if waiting reply from probe
  */
Process_Status tlvVectReset(Tlv_Session *session) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  
  /* Start tlv request task */
  startTask(tb);
  
  /* Send tlv request */
  tlvSendRequest(session, TLV_VECT_RESET, 0, NULL);
  
  /* Waiting reply from probe */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(TWO_SECOND), tb);
    yield(tb);
  };
  
  resetSystemTime();
  
  /* End tlv request task */
  endTask(tb);
  
  returnThis(PROCESS_DONE, tb);
}

// /** tlvReadDataChunk is a function used to read data in chunk
  // * by using tlv protocol
  // * 
  // * input   : session contain a element/handler used by tlv protocol
  // *           destAddress is the address of the data need to be store
  // *           size is the size of the data can be any value
  // *
  // * return  : NONE
  // */
// void tlvReadDataChunk(Tlv_Session *session, uint32_t destAddress, int size) {
  // Tlv *tlv;
  // uint32_t buffer[] = {destAddress, size};
  
  // if(session == NULL) Throw(TLV_NULL_SESSION);
  // /* create tlv packet with register address */
  // tlv = tlvCreatePacket(TLV_READ_MEMORY, 8, (uint8_t *)buffer);

  // tlvSend(session, tlv);
// }

/** tlvReadTargetMemory is a function to read data from target memory
  * base on tlv->type
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           destAddress is the address of the data need to be read
  *           size is the size of the data can be any value
  *
  * Return  : NONE
  */
uint8_t *tlvReadTargetMemory(Tlv_Session *session, uint32_t *destAddress, int *size) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  
  /* Start tlv request task */
  startTask(tb);
  
  /* Send tlv request */
  if(*size > TLV_DATA_SIZE) 
    tlvReadDataChunk(session, *destAddress, TLV_DATA_SIZE);
  else tlvReadDataChunk(session, *destAddress, *size);
  
  SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  *destAddress += TLV_DATA_SIZE;
  *size -= TLV_DATA_SIZE;
  
  // /* Waiting reply from probe */
  // while((response = tlvReceive(session)) == NULL) {
    // /* Check is maximum timeout is reached */
    // if(isTimeOut(FIVE_SECOND)) {
      // resetTask(session->rmemState);
      // Throw(PROBE_NOT_RESPONDING);
    // }
    // yield(session->rmemState);
  // };
  
  // resetSystemTime();
  // if(*size < TLV_DATA_SIZE) {
    // CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  // }
  // /* Verify response reply from probe */
  // verifyTlvPacket(response);
  
  /* End task */
  endTask(tb);

  // #ifdef HOST
  // displayMemoryMap(response->value, response->length - 1);
  // #endif
  
  // return response->value;
}

// /** tlvWriteDataChunk is a function used to send data in chunk
  // * by using tlv protocol
  // * 
  // * input   : session contain a element/handler used by tlv protocol
  // *           dataAddress is the address of the data need to send
  // *           destAddress is the address of the data need to be store
  // *           size is the size of the data can be any value
  // *           memorySelect can be one of the following value :
  // *             TLV_WRITE_RAM
  // *             TLV_WRITE_FLASH
  // *
  // * return  : NONE
  // */
// void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size, Tlv_Command memorySelect) {
  // Tlv *tlv; uint8_t chksum = 0;
  
  // if(session == NULL) Throw(TLV_NULL_SESSION);
  
  // /* create tlv packet with register address */
  // tlv = tlvCreatePacket(memorySelect, size + 4, NULL);
  
  // chksum = tlvPackIntoBuffer(tlv->value, (uint8_t *)&destAddress, 4);
  // chksum += tlvPackIntoBuffer(&tlv->value[4], dataAddress, size);

  // /* Update checksum with destAddress */
  // tlv->value[tlv->length - 1] = chksum;
  
  // tlvSend(session, tlv);
// }

// /** tlvWriteTargetMemory is a function used to write data into target memory
  // * by using tlv protocol
  // * 
  // * Input   : session contain a element/handler used by tlv protocol
  // *           dataAddress is the address of the data need to send
  // *           destAddress is the address of the data need to be store
  // *           size is the size of the data can be any value
  // *           memorySelect can be one of the following value :
  // *            - TLV_WRITE_RAM
  // *            - TLV_WRITE_FLASH
  // *
  // * return  : NONE
  // */
Process_Status tlvWriteTargetMemory(Tlv_Session *session, uint8_t **dataAddress, uint32_t *destAddress, int *size, Tlv_Command memorySelect) {
  // if(session == NULL) Throw(TLV_NULL_SESSION);
  
  // /* Start tlv request task */
  // startTask(session->wramState);
  
  // /* Send tlv request */
  // if(*size > TLV_DATA_SIZE) 
    // tlvWriteDataChunk(session, *dataAddress, *destAddress, TLV_DATA_SIZE, memorySelect);
  // else tlvWriteDataChunk(session, *dataAddress, *destAddress, *size, memorySelect);
  
  // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  // *dataAddress += TLV_DATA_SIZE;
  // *destAddress += TLV_DATA_SIZE;
  // *size -= TLV_DATA_SIZE;
  
  // /* Waiting reply from probe */
  // while((response = tlvReceive(session)) == NULL) {
    // /* Check is maximum timeout is reached */
    // if(isTimeOut(FIVE_SECOND)) {
      // resetTask(session->wramState);
      // Throw(PROBE_NOT_RESPONDING);
    // }
    // yield(session->wramState);
  // };
  
  // resetSystemTime();
  // CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  // /* Verify response reply from probe */
  // verifyTlvPacket(response);
  
  // /* End tlv request task */
  // endTask(session->wramState);
  
  // return PROCESS_DONE;
}

// /** !< Internal Function used by Load Program To Memory >!
  // * tlvLoadProgram is a function to load program into target memory
  // *
  // * Input   : session contain a element/handler used by tlv protocol
  // *           file is the file need to be load into target memory
  // *           memorySelect can be one of the following value :
  // *            - TLV_WRITE_RAM
  // *            - TLV_WRITE_FLASH
  // *
  // * Return  : NONE
  // */
// void tlvLoadProgram(Tlv_Session *session, char *file, Tlv_Command memorySelect) {

  // switch(session->loadProgramState) {    
    // case TLV_LOAD_ISR_VECTOR :
      // if(fileStatus == FILE_CLOSED) getElfSection(file);
      
      // tlvWriteTargetMemory(session, &isr->dataAddress, &isr->destAddress, &isr->size, memorySelect);
      // if(isr->size <= 0) {
        // printf("Load ISR_VECTOR...OK\n");
        // session->loadProgramState = TLV_LOAD_TEXT;
      // } else SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
    // break;
    
    // case TLV_LOAD_TEXT :
      // tlvWriteTargetMemory(session, &text->dataAddress, &text->destAddress, &text->size, memorySelect);
      // if(text->size <= 0) {
        // printf("Load Text...OK\n");
        // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        // session->loadProgramState = TLV_LOAD_RO_DATA;
      // } else SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
    // break;
    
    // case TLV_LOAD_RO_DATA :
      // tlvWriteTargetMemory(session, &roData->dataAddress, &roData->destAddress, &roData->size, memorySelect);
      // if(roData->size <= 0) {
        // printf("Load RO Data...OK\n");
        // session->loadProgramState = TLV_LOAD_INIT_ARRAY;
      // } else SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
    // break;
  
    // case TLV_LOAD_INIT_ARRAY :
      // tlvWriteTargetMemory(session, &initArray->dataAddress, &initArray->destAddress, &initArray->size, memorySelect);
      // if(initArray->size <= 0) {
        // printf("Load Init Array...OK\n");
        // session->loadProgramState = TLV_LOAD_FINI_ARRAY;
      // } else SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
    // break;
    
    // case TLV_LOAD_FINI_ARRAY :
      // tlvWriteTargetMemory(session, &finiArray->dataAddress, &finiArray->destAddress, &finiArray->size, memorySelect);
      // if(finiArray->size <= 0) {
        // printf("Load Fini Array...OK\n");
        // session->loadProgramState = TLV_LOAD_DATA;
      // } else SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
    // break;
    
    // case TLV_LOAD_DATA :
      // tlvWriteTargetMemory(session, &data->dataAddress, &data->destAddress, &data->size, TLV_WRITE_RAM);
      // if(data->size <= 0 && GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR) {
        // /* Close elf file */
        // printf("Load Data...OK\n");
        // closeElfFile(); session->loadProgramState = TLV_LOAD_ISR_VECTOR;
      // } else SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
    // break;
  // }
// }

// /** tlvLoadToRam is a function to load elf file and update PC 
  // * to the program entry addresss
  // *
  // * Input   : session contain a element/handler used by tlv protocol
  // *           file is the file need to be load into target memory
  // *
  // * Return  : NONE
  // */
void tlvLoadToRam(Tlv_Session *session, char *file) {
  
  // switch(session->lramState) {
    // case TLV_LOAD_PROGRAM :
      // /* Load elf file isr_vector, ro_data, data
         // and text section into target RAM */
      // tlvLoadProgram(session, file, TLV_WRITE_RAM);
      // if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR) {
        // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        // session->lramState = TLV_UPDATE_PC;
      // }
    // break;
    
    // case TLV_UPDATE_PC :
      // /* Update program counter to the entry address 
         // of the loaded program */ 
      // writeRegister(session, PC, entryAddress);
      // if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR) {
        // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        // session->lramState = TLV_RUN_PROGRAM;
      // }
    // break;
    
    // case TLV_RUN_PROGRAM :
      // /* Run program on target */
      // run(session);
      // if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR)
        // session->lramState = TLV_LOAD_PROGRAM;
    // break;
  // }
}

Process_Status tlvRequestFlashErase(Tlv_Session *session, uint32_t address, int size) {
  // Tlv *tlv;
  // uint32_t buffer[] = {address, size};
  
  // if(session == NULL) Throw(TLV_NULL_SESSION);
  // /* Start tlv request task */
  // startTask(session->rEraseState);
  
  // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  
  // /* Send tlv request */
  // tlv = tlvCreatePacket(TLV_FLASH_ERASE, 8, (uint8_t *)buffer);
  // tlvSend(session, tlv);

  // /* Waiting reply from probe */
  // while((response = tlvReceive(session)) == NULL) {
    // /* Check is maximum timeout is reached */
    // if(isTimeOut(FIVE_SECOND)) {
      // resetTask(session->rEraseState);
      // Throw(PROBE_NOT_RESPONDING);
    // }
    // yield(session->rEraseState);
  // };
  
  // resetSystemTime();
  // CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  // /* Verify response reply from probe */
  // verifyTlvPacket(response);
  
  // /* End tlv request task */
  // endTask(session->rEraseState);
  
  // return PROCESS_DONE;
}

// /** tlvFlashErase
  // *
  // * Input   : session contain a element/handler used by tlv protocol
  // *           file is the file need to be load into target memory
  // *
  // * Return  : NONE
  // */
// Process_Status tlvEraseTargetFlash(Tlv_Session *session, uint32_t address, int size) {
  // switch(session->eraseState) {
    // case TLV_LOAD_FLASH_PROGRAMMER :
      // tlvLoadToRam(session, FLASH_PROGRAMMER_FILE_PATH);
      // if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR) {
        // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        // session->eraseState = TLV_REQUEST_ERASE;
      // }
    // break;
    
    // case TLV_REQUEST_ERASE :
      // if(tlvRequestFlashErase(session, address, size) == PROCESS_DONE) {
        // session->eraseState = TLV_LOAD_FLASH_PROGRAMMER;
        // return PROCESS_DONE;
      // }
    // break;
  // }
// }

// /** tlvRequestFlashMassErase
  // *
  // * Input   : session contain a element/handler used by tlv protocol
  // *           banks is the memory banks need to erase can be one of the 
  // *           following values :
  // *             - BANK_1
  // *             - BANK_2
  // *             - BOTH_BANK
  // *
  // * Return  : 0 if process stil doing
  // *           1 if process done
  // */
// Process_Status tlvRequestFlashMassErase(Tlv_Session *session, uint32_t banks) {
  // if(session == NULL) Throw(TLV_NULL_SESSION);
  
  // /* Start tlv request task */
  // startTask(session->rMassEraseState);
  
  // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  
  // /* Send tlv request */
  // Tlv *tlv = tlvCreatePacket(TLV_FLASH_MASS_ERASE, 4, (uint8_t *)&banks);
  // tlvSend(session, tlv);

  // /* Waiting reply from probe */
  // while((response = tlvReceive(session)) == NULL) {
    // /* Check is maximum timeout is reached */
    // if(isTimeOut(FORTY_SECOND)) {
      // resetTask(session->rMassEraseState);
      // Throw(PROBE_NOT_RESPONDING);
    // }
    // yield(session->rMassEraseState);
  // };
  
  // resetSystemTime();
  // CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  // /* Verify response reply from probe */
  // verifyTlvPacket(response);
  
  // /* End tlv request task */
  // endTask(session->rMassEraseState);
  
  // return PROCESS_DONE;
// }

// /** tlvFlashMassErase
  // *
  // * Input   : session contain a element/handler used by tlv protocol
  // *           file is the file need to be load into target memory
  // *
  // * Return  : NONE
  // */
// void tlvMassEraseTargetFlash(Tlv_Session *session, uint32_t banks) {
  // switch(session->mEraseState) {
    // case TLV_LOAD_FLASH_PROGRAMMER :
      // tlvLoadToRam(session, FLASH_PROGRAMMER_FILE_PATH);
      // if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR) {
        // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        // session->mEraseState = TLV_REQUEST_ERASE;
      // }
    // break;
    
    // case TLV_REQUEST_ERASE :
      // if(tlvRequestFlashMassErase(session, banks) == PROCESS_DONE) {
        // printf("Done Mass Erase\n");
        // session->mEraseState = TLV_LOAD_FLASH_PROGRAMMER;
      // }
    // break;
  // }
// }

// /** tlvLoadToFlash is a function to load elf file and update PC 
  // * to the program entry addresss
  // *
  // * Input   : session contain a element/handler used by tlv protocol
  // *           file is the file need to be load into target memory
  // *
  // * Return  : NONE
  // */
// void tlvLoadToFlash(Tlv_Session *session, char *file) {
  // switch(session->flashState) {
    // case TLV_REQUEST_ERASE :
      // if(programSize == 0) programSize = getProgramSize(file);
      // /* Request Flash Programmer to erase flash according to program size */
      // tlvEraseTargetFlash(session, FLASH_BEGIN_ADDRESS, programSize);
      // if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR) {
        // programSize = 0;
        // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        // session->flashState = TLV_LOAD_ACTUAL_PROGRAM;
      // }
    // break;
      
    // case TLV_LOAD_ACTUAL_PROGRAM :
      // /* Load elf file isr_vector, ro_data, data
      // and text section into target Flash */
      // tlvLoadProgram(session, file, TLV_WRITE_FLASH);
      // if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR) {
        // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        // session->flashState = TLV_UPDATE_PC;
      // }
    // break;
    
    // case TLV_UPDATE_PC :
      // /* Update program counter to the entry address 
         // of the loaded program */
      // if(writeRegister(session, PC, entryAddress) == PROCESS_DONE) {
        // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        // session->flashState = TLV_RUN_PROGRAM;
      // }
      // break;
      
    // case TLV_RUN_PROGRAM :
      // /* Run program on target */
      // if(run(session) == PROCESS_DONE)
        // session->flashState = TLV_REQUEST_ERASE;
    // break;
  // }
// }

// /** tlvSetBreakpoint is a function to set breakpoint
  // * at specified address
  // *
  // * Input   : session contain a element/handler used by tlv protocol
  // *           address is the address want to be stop at
  // *
  // * Return  : NONE
  // */
// void tlvSetBreakpoint(Tlv_Session *session, uint32_t address) {
  // if(session == NULL) Throw(TLV_NULL_SESSION);
  
  // Tlv *tlv = tlvCreatePacket(TLV_BREAKPOINT, 4, (uint8_t *)&address);

  // tlvSend(session, tlv);
// }

// /** tlvWaitDebugEvents is a function to send expected event
  // * to probe and wait for event to happen
  // *
  // * packet :    type     length   event   chksum
  // *          +-----------------------------------+
  // *          | 1 byte | 1 byte | 1 byte | 1 byte |           
  // *          +-----------------------------------+
  // *
  // * Input   : event is the expected event defined in Tlv_Command enum
  // *
  // * Return  : NONE
  // */
// EventType tlvWaitDebugEvents(Tlv_Session *session, EventType event) {
  // if(session == NULL) Throw(TLV_NULL_SESSION);
  
  // /* Start tlv request task */
  // startTask(session->state);
  
  // SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  
  // /* Send tlv request */
  // Tlv *tlv = tlvCreatePacket(TLV_DEBUG_EVENTS, 1, (uint8_t *)&event);
  // tlvSend(session, tlv);

  // /* Waiting reply from probe */
  // while((response = tlvReceive(session)) == NULL) {
    // /* Check is maximum timeout is reached */
    // if(isTimeOut(FIVE_SECOND)) {
      // resetTask(session->state);
      // Throw(PROBE_NOT_RESPONDING);
    // }
    // yield(session->state);
  // };
  
  // resetSystemTime();
  // CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  // /* Verify response reply from probe */
  // verifyTlvPacket(response);
  
  // /* End tlv request task */
  // endTask(session->state);
  
  // return response->value[0];
// }


/** selectCommand is a function to select instruction 
  * base on userSession
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
void selectCommand(Tlv_Session *session, User_Session *userSession) {
  uint8_t *pData;
  switch(userSession->tlvCommand) {
    // case TLV_WRITE_RAM          : pData = (uint8_t *)userSession->data;
                                  // tlvWriteToRam(session, &pData, &userSession->address, &userSession->size);                      break;
    // case TLV_LOAD_RAM           : tlvLoadToRam(session, userSession->fileName);                                                   break;
    // case TLV_LOAD_FLASH         : tlvLoadToFlash(session, userSession->fileName);                                                 break;
    // case TLV_READ_MEMORY        : tlvReadTargetMemory(session, &userSession->address, &userSession->size);                        break;
    // case TLV_WRITE_REGISTER     : writeRegister(session, userSession->address, userSession->data[0]);                    break;
    // case TLV_READ_REGISTER      : readRegister(session, userSession->address);                                           break;
    case TLV_HALT_TARGET        : halt(session);                                                                         break;
    // case TLV_RUN_TARGET         : run(session);                                                                          break;
    // case TLV_STEP               : tlvMultipleStepTarget(session, userSession->data[0]);                                           break;
    // case TLV_BREAKPOINT         : tlvSetBreakpoint(session, userSession->address);                                                break;
    // case TLV_FLASH_ERASE        : tlvEraseTargetFlash(session, userSession->address, userSession->size);                          break;
    // case TLV_FLASH_MASS_ERASE   : tlvMassEraseTargetFlash(session, userSession->address);                                         break;
    // case TLV_SOFT_RESET         : tlvSoftReset(session);                                                                          break;
    // case TLV_HARD_RESET         : tlvHardReset(session);                                                                          break;
  }
}

/** hostInterpreter
  */
void hostInterpreter(Tlv_Session *session) {
  
  switch(session->hostState) {    
    case HOST_WAIT_USER_COMMAND :
      userSession = waitUserCommand();
      /* If command is available */
      if(IS_COMMAND_AVAILABLE(userSession)) {
        if(IS_TLV_EXIT(userSession)) HOST_CHANGE_STATE(session, HOST_EXIT);
        else HOST_CHANGE_STATE(session, HOST_INTERPRET_COMMAND);
      }
    break;
      
    case HOST_INTERPRET_COMMAND :
      selectCommand(session, userSession);
      if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR)
        HOST_CHANGE_STATE(session, HOST_WAIT_USER_COMMAND);
    break;
  }
}


Process_Status _tlvWriteToFlash(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size) {
  // if(session == NULL) Throw(TLV_NULL_SESSION);
  
  // startTask(session->state);
  
  // else if(tlvEraseTargetFlash(session, destAddress, size) == PROCESS_DONE) {
      
  // }
  
  /* End tlv request task */
  // endTask(session->state);
  
  // tlvWriteTargetMemory(session, &dataAddress, &destAddress, &size, TLV_WRITE_FLASH);
}