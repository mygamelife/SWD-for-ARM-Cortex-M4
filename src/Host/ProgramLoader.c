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
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };

  resetSystemTime();
  /* End tlv request task */
  endTask(tb);
  returnThis(PROCESS_DONE);
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
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };

  resetSystemTime();
  /* End task */
  endTask(tb);
  #ifdef HOST
  printf("value %x\n", get4Byte(&response->value[0]));
  #endif
  returnThis(get4Byte(&response->value[0]));
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
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };

  resetSystemTime();
  /* End tlv request task */
  endTask(tb);
  returnThis(PROCESS_DONE);
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
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };

  resetSystemTime();
  /* End tlv request task */
  endTask(tb);
  returnThis(PROCESS_DONE);
}

/** multipleStep is a function send tlv request to
  * execute number of instructions
  *
  * input   : session contain a element/handler used by tlv protocol
  *           nInstructions is the number of instruction need to execute
  *
  * return  : Current program counter address if successfully step
  *           0 if waiting reply from probe
  */
uint32_t multipleStep(Tlv_Session *session, int nInstructions) {
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
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };

  resetSystemTime();
  /* End tlv request task */
  endTask(tb);
  #ifdef HOST
  printf("value %x\n", get4Byte(&response->value[0]));
  #endif
  returnThis(get4Byte(&response->value[0]));
}

/** softReset is a function software reset target
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : 1 if successfully software reset
  *           0 if waiting reply from probe
  */
Process_Status softReset(Tlv_Session *session) {
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
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };

  resetSystemTime();
  /* End tlv request task */
  endTask(tb);
  returnThis(PROCESS_DONE);
}

/** hardReset is a function hardware reset target
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : 1 if successfully software reset
  *           0 if waiting reply from probe
  */
Process_Status hardReset(Tlv_Session *session) {
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
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };

  resetSystemTime();
  /* End tlv request task */
  endTask(tb);
  returnThis(PROCESS_DONE);
}

/** vectorReset is a function vector reset target
  *
  * Input   : session contain a element/handler used by tlv protocol
  *
  * Return  : 1 if successfully software reset
  *           0 if waiting reply from probe
  */
Process_Status vectorReset(Tlv_Session *session) {
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
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };

  resetSystemTime();
  /* End tlv request task */
  endTask(tb);
  returnThis(PROCESS_DONE);
}

/** readMemory is a function to read data from target memory
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           address is the address of the data need to be read
  *           size is the size of the data can be any value
  *
  * Return  : s is a Storage structure pointer contain read data
  */
uint8_t *readMemory(Tlv_Session *session, uint32_t address, int size) {
  static TaskBlock taskBlock = {.state = 0};
  static uint8_t *db;
  static uint32_t tAddress = 0;
  static int index = 0, tSize = 0;
  TaskBlock *tb = &taskBlock;
  int i;

  if(session == NULL) Throw(TLV_NULL_SESSION);
  /* Start tlv request task */
  startTask(tb);
  
  /* Create temporary storage to store neccessary data
     to prevent lose track */
  db = createDataBlock(size);
  tSize = size;
  tAddress = address;

  while(tSize > 0) {
    /* Send tlv request */
    tlvReadDataChunk(session, &tAddress, &tSize);
    while((response = tlvReceive(session)) == NULL) {
      /* Check is maximum timeout is reached */
      isProbeAlive(isTimeOut(FIVE_SECOND), tb);
      yield(tb);
    };
    /* Waiting reply from probe */
    for(i = 0; i < response->length - 1; i++) {
      db[index] = response->value[i];
      index++;
    }
  }

  resetSystemTime();
  /* End task */
  endTask(tb);

  #ifdef HOST
  displayMemoryMap(db, address, size);
  #endif
  returnThis(db);
}

/** writeMemory is a function used to write data into target memory
  * by using tlv protocol
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *           memory can be one of the following value :
  *            - TLV_WRITE_RAM
  *            - TLV_WRITE_FLASH
  *
  * return  : NONE
  */
Process_Status writeMemory(Tlv_Session *session, uint8_t *data, uint32_t address, int size, Tlv_Command memory) {
  static TaskBlock taskBlock = {.state = 0};
  static uint8_t *pData = NULL;
  static uint32_t tAddress = 0;
  static int tSize = 0;
  TaskBlock *tb = &taskBlock;
  
  if(session == NULL) Throw(TLV_NULL_SESSION);
  /* Start tlv request task */
  startTask(tb);
  
  /* pData is a static pointer to keep track the original pointer 
     this pointer address will be shift after write data chunk */
  pData = data;
  tSize = size;
  tAddress = address;
  
  while(tSize > 0) {
    /* Send tlv request */
    tlvWriteDataChunk(session, &pData, &tAddress, &tSize, memory);
    /* Waiting reply from probe */
    while((response = tlvReceive(session)) == NULL) {
      /* Check is maximum timeout is reached */
      isProbeAlive(isTimeOut(FIVE_SECOND), tb);
      yield(tb);
    };
  }
  resetSystemTime();
  /* End task */
  endTask(tb);
  returnThis(PROCESS_DONE);
}

/** loadProgram is a function to load program into target memory
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *           memorySelect can be one of the following value :
  *            - TLV_WRITE_RAM
  *            - TLV_WRITE_FLASH
  *
  * Return  : NONE
  */
int loadProgram(Tlv_Session *session, char *file, Tlv_Command memorySelect) {
  CEXCEPTION_T err;
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;

  if(session == NULL) Throw(TLV_NULL_SESSION);

  Try {
    /* Start tlv request task */
    startTask(tb);

    if(fileStatus == FILE_CLOSED) getElfSection(file);

    await(writeMemory(session, isr->dataAddress, isr->destAddress, isr->size, memorySelect), tb);
    printf("ISR VECTOR...OK\n");
    await(writeMemory(session, text->dataAddress, text->destAddress, text->size, memorySelect), tb);
    printf("TEXT.........OK\n");
    await(writeMemory(session, roData->dataAddress, roData->destAddress, roData->size, memorySelect), tb);
    printf("RODATA.......OK\n");
    await(writeMemory(session, initArray->dataAddress, initArray->destAddress, initArray->size, memorySelect), tb);
    printf("INITARRAY....OK\n");
    await(writeMemory(session, finiArray->dataAddress, finiArray->destAddress, finiArray->size, memorySelect), tb);
    printf("FINIARRAY....OK\n");
    await(writeMemory(session, data->dataAddress, data->destAddress, data->size, TLV_WRITE_RAM), tb);
    printf("DATA.........OK\n");

    resetSystemTime();
    /* End task */
    endTask(tb);

    closeElfFile();
    returnThis(PROCESS_DONE);
  }
  Catch(err) {
    resetTask(tb);
    closeElfFile();
    Throw(err);
  }
}

int isProgramExist(Tlv_Session *session, char *file) {
  static TaskBlock taskBlock = {.state = 0};
  static uint8_t *db;
  TaskBlock *tb = &taskBlock;

  int i;

  /* Start task */
  startTask(tb);
  if(fileStatus == FILE_CLOSED) getElfSection(file);
  await(db = readMemory(session, isr->destAddress, isr->size), tb);

  /* Compare and verify */
  printf("Verifying........ISR SECTION\n");
  for(i = 0; i < isr->size; i++) {
    if(db[i] != isr->dataAddress[i]) {
      printf("%d, memoryData %x, actualData %x\n", i, get4Byte(&db[i]), get4Byte(&isr->dataAddress[i]));
      resetTask(tb);
      delDataBlock(db);
      closeElfFile();
      returnThis(PROCESS_DONE);
    }
  }

  await(db = readMemory(session, text->destAddress, text->size), tb);

  printf("Verifying........TEXT SECTION\n");
  /* Compare and verify */
  for(i = 0; i < text->size; i++) {
    if(db[i] != text->dataAddress[i]) {
      printf("%d, memoryData %x, actualData %x\n", i, db[i], isr->dataAddress[i]);
      resetTask(tb);
      delDataBlock(db);
      closeElfFile();
      returnThis(PROCESS_DONE);
    }
  }

  // await(writeMemory(session, text->dataAddress, &text->destAddress, &text->size, memorySelect), tb);
  // printf("TEXT.........OK\n");
  // await(writeMemory(session, roData->dataAddress, &roData->destAddress, &roData->size, memorySelect), tb);
  // printf("RODATA.......OK\n");
  // await(writeMemory(session, initArray->dataAddress, &initArray->destAddress, &initArray->size, memorySelect), tb);
  // printf("INITARRAY....OK\n");
  // await(writeMemory(session, finiArray->dataAddress, &finiArray->destAddress, &finiArray->size, memorySelect), tb);
  // printf("FINIARRAY....OK\n");
  // await(writeMemory(session, data->dataAddress, &data->destAddress, &data->size, TLV_WRITE_RAM), tb);
  // printf("DATA.........OK\n");

  // printf("isr address %x, size %d\n", isr->destAddress, isr->size);
  // printf("done reading\n");
  endTask(tb);
  delDataBlock(db);
  closeElfFile();
  returnThis(PROCESS_DONE);
}

/** loadRam is a function to load elf file and update PC
  * to the program entry addresss
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *
  * Return  : NONE
  */
int loadRam(Tlv_Session *session, char *file) {
  CEXCEPTION_T err;
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;

  if(session == NULL) Throw(TLV_NULL_SESSION);

  Try {
    /* Start task */
    startTask(tb);

    /* Load specified program */
    await(loadProgram(session, file, TLV_WRITE_RAM), tb);
    printf("Done loading.....\n");
    /* Update program counter */
    await(writeRegister(session, PC, entryAddress), tb);
    printf("Update PC........\n");
    /* Run the program */
    await(run(session), tb);
    printf("Run Program......\n");

    /* End task */
    endTask(tb);
    resetSystemTime();
    returnThis(PROCESS_DONE);
  }
  Catch(err) {
    resetTask(tb);
    Throw(err);
  }
}

/** loadFlash is a function to load elf file and update PC
  * to the program entry addresss
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *
  * Return  : NONE
  */
int loadFlash(Tlv_Session *session, char *file) {
  CEXCEPTION_T err;
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;

  if(session == NULL) Throw(TLV_NULL_SESSION);

  Try {
    /* Start task */
    startTask(tb);

    if(programSize == 0)
      programSize = getProgramSize(file);

    /* Erase section */
    await(eraseSection(session, FLASH_BEGIN_ADDRESS, programSize), tb);
    programSize = 0;
    printf("Done Erase.....\n");
    /* Load actual program to flash */
    await(loadProgram(session, file, TLV_WRITE_FLASH), tb);
    printf("Loaded Actual Program.....\n");
    /* Update program counter */
    await(writeRegister(session, PC, entryAddress), tb);
    printf("Update PC........\n");
    /* Run the program */
    await(run(session), tb);
    printf("Run Program......\n");

    /* Waiting reply */
    while((response = tlvReceive(session)) == NULL) {
      /* Check is maximum timeout is reached */
      isProbeAlive(isTimeOut(FIVE_SECOND), tb);
      yield(tb);
    };

    endTask(tb);
    resetSystemTime();
    returnThis(PROCESS_DONE);
  }
  Catch(err) {
    resetTask(tb);
    Throw(err);
  }
}

/** eraseSection is a function to erase flash section by section
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *
  * Return  : NONE
  */
Process_Status eraseSection(Tlv_Session *session, uint32_t address, int size) {
  CEXCEPTION_T err;
  uint32_t data[] = {address, size};
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;

  if(session == NULL) Throw(TLV_NULL_SESSION);

  Try {
    /* Start task */
    startTask(tb);
    /* Load Flash Programmer */
    await(loadRam(session, FLASH_PROGRAMMER_FILE_PATH), tb);
    printf("Loaded FlashProgrammer.....\n");

    tlvSendRequest(session, TLV_FLASH_ERASE, 8, (uint8_t *)data);
    printf("size %d\n", get4Byte(&session->txBuffer[6]));
    printf("Sent Request.....\n");
    /* Waiting reply */
    while((response = tlvReceive(session)) == NULL) {
      /* Check is maximum timeout is reached */
      isProbeAlive(isTimeOut(FIVE_SECOND), tb);
      yield(tb);
    };

    endTask(tb);
    resetSystemTime();
    returnThis(PROCESS_DONE);
  }
  Catch(err) {
    resetTask(tb);
    Throw(err);
  }
}

/** eraseAll is a function used to erase flash by selecting bank
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           file is the file need to be load into target memory
  *
  * Return  : NONE
  */
Process_Status eraseAll(Tlv_Session *session, uint32_t banks) {
  CEXCEPTION_T err;
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;

  if(session == NULL) Throw(TLV_NULL_SESSION);

  Try {
    /* Start task */
    startTask(tb);
    /* Load Flash Programmer */
    await(loadRam(session, FLASH_PROGRAMMER_FILE_PATH), tb);
    printf("Loaded FlashProgrammer.....\n");

    tlvSendRequest(session, TLV_FLASH_MASS_ERASE, 1, (uint8_t *)&banks);
    printf("banks %d\n", session->txBuffer[2]);
    printf("Sent Request.....\n");
    /* Waiting reply */
    while((response = tlvReceive(session)) == NULL) {
      /* Check is maximum timeout is reached */
      isProbeAlive(isTimeOut(FORTY_SECOND), tb);
      yield(tb);
    };

    endTask(tb);
    resetSystemTime();
    returnThis(PROCESS_DONE);
  }
  Catch(err) {
    resetTask(tb);
    Throw(err);
  }
}

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

/** tlvWaitDebugEvents is a function to send expected event
  * to probe and wait for event to happen
  *
  * packet :    type     length   event   chksum
  *          +-----------------------------------+
  *          | 1 byte | 1 byte | 1 byte | 1 byte |
  *          +-----------------------------------+
  *
  * Input   : event is the expected event defined in Tlv_Command enum
  *
  * Return  : NONE
  */
EventType tlvWaitDebugEvents(Tlv_Session *session, EventType event) {
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;

  if(session == NULL) Throw(TLV_NULL_SESSION);

  /* Start tlv request task */
  startTask(tb);
  /* Send tlv request */
  tlvSendRequest(session, TLV_DEBUG_EVENTS, 1, (uint8_t *)&event);
  /* Waiting reply */
  while((response = tlvReceive(session)) == NULL) {
    /* Check is maximum timeout is reached */
    isProbeAlive(isTimeOut(FIVE_SECOND), tb);
    yield(tb);
  };
  /* End task */
  endTask(tb);

  resetSystemTime();
  returnThis(response->value[0]);
}

/** selectCommand is a function to select instruction
  * base on userSession
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
int selectCommand(Tlv_Session *session, User_Session *us) {
  
  switch(userSession->tlvCommand) {
    case TLV_WRITE_RAM          : writeRam(session, (uint8_t *)us->data, us->address, us->size);    break;
    case TLV_LOAD_RAM           : loadRam(session, us->fileName);                                   break;
    case TLV_LOAD_FLASH         : loadFlash(session, us->fileName);                                 break;
    case TLV_READ_MEMORY        : readMemory(session, us->address, us->size);                       break;
    case TLV_WRITE_REGISTER     : writeRegister(session, us->address, us->data[0]);                 break;
    case TLV_READ_REGISTER      : readRegister(session, us->address);                               break;
    case TLV_HALT_TARGET        : halt(session);                                                    break;
    case TLV_RUN_TARGET         : run(session);                                                     break;
    case TLV_STEP               : multipleStep(session, us->data[0]);                               break;
    case TLV_FLASH_ERASE        : eraseSection(session, us->address, us->size);                     break;
    case TLV_FLASH_MASS_ERASE   : eraseAll(session, us->address);                                   break;
    // case TLV_BREAKPOINT         : tlvSetBreakpoint(session, userSession->address);               break;
    case TLV_SOFT_RESET         : softReset(session);                                               break;
    case TLV_HARD_RESET         : hardReset(session);                                               break;
    case TLV_EXIT               : systemExit(session);                                              break;
  }
}

/** hostInterpreter
  */
int hostInterpreter(Tlv_Session *session) {
  CEXCEPTION_T err;
  static TaskBlock taskBlock = {.state = 0};
  TaskBlock *tb = &taskBlock;

  Try {
    startTask(tb);
    while((userSession = waitUserCommand()) == NULL) { yield(tb); }
    await(selectCommand(session, userSession), tb);
    endTask(tb);

    deleteUserSession(userSession);
    returnThis(PROCESS_DONE);
  }
  Catch(err) {
    resetTask(tb);
    deleteUserSession(userSession);
    Throw(err);
  }
}

uint8_t *createDataBlock(int size) {
  uint8_t *dataBlock;
  dataBlock = malloc(size * sizeof(uint8_t));

  if(dataBlock == NULL) printf("Failed to create storage\n");

  return dataBlock;
}

void delDataBlock(uint8_t *dataBlock) {
  if(dataBlock != NULL) {
    free(dataBlock);
  }
}