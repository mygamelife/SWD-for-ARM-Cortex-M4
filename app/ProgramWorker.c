#include "ProgramWorker.h"

/* temp SRAM address 0x20005000 */
uint32_t tempAddress = 0x20005000;

/** IsStubBusy is a function to check if stub
  * is busy with the last operation
  *
  * input   : NONE
  *
  * return  : 1 stub is free for next operation
  *           0 stub is busy with the last operation
  */
int IsStubBusy(void)  {
  unsigned int stubStatus = memoryReadAndReturnWord((uint32_t)&STUB->status);
  
  if(stubStatus == STUB_OK)
    return 1;
  
  else return 0;
}

/** requestStubErase is a function to load the sector erase
  * instruction into SRAM to tell the swdStub
  *
  * input   : startAddress is the address to begin erase
  *           endAddress is the address to end erase
  *
  * output  : NONE
  */
void requestStubErase(uint32_t address, int size) {
  
  /* load flash start and end address to sram */
  memoryWriteWord((uint32_t)&STUB->flashAddress, (uint32_t)address);
  memoryWriteWord((uint32_t)&STUB->dataSize, (uint32_t)size);
  
  /* load instruction to sram */
  memoryWriteWord((uint32_t)&STUB->instruction, STUB_ERASE);
}

/** requestMassErase is a function to load the mass erase
  * instruction into SRAM to tell the swdStub
  *
  * input   : bankSelect can be one of the following value
  *            + FLASH_BANK_1: Bank1 to be erased
  *            + FLASH_BANK_2: Bank2 to be erased
  *            + FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  * output  : NONE
  */
void requestStubMassErase(uint32_t bankSelect)  {
  
  /* load bank select to sram */
  memoryWriteWord((uint32_t)&STUB->banks, (uint32_t)bankSelect);
  
  /* load instruction to sram */
  memoryWriteWord((uint32_t)&STUB->instruction, STUB_MASSERASE);
}

/** requestStubCopy is a function copy data from src (SRAM) to dest (Flash)
  *
  * input   : src is the beginning SRAM address contain all the information
  *           dest is the flash address all the information need to copy over there
  *           length is to determine how many words need to copy over
  *
  * output  : NONE
  */
void requestStubCopy(uint32_t dataAddress, uint32_t destAddress, int size) {

  /* load SRAM start address into sram */
  memoryWriteWord((uint32_t)&STUB->sramAddress, (uint32_t)dataAddress);
  
  /* load Flash start address into sram */
  memoryWriteWord((uint32_t)&STUB->flashAddress, (uint32_t)destAddress);
  
  /* load length into sram */
  memoryWriteWord((uint32_t)&STUB->dataSize, (uint32_t)size);

	/* load copy instructoin into sram */
  memoryWriteWord((uint32_t)&STUB->instruction, STUB_COPY);
}

/** writeTargetRegister is a function to write value into target register using swd
  *
  * input     : session contain a element/handler used by tlv protocol
  *             registerAddress is the address of the target register
  *             data is the data need to write into the target register
  *
  * return    : NONE
  */
void writeTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t data) {
  
  writeCoreRegister(registerAddress, data);
  
  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  tlvSend(session, tlv);
}

/** readTargetRegister is a function to read value from target register using swd
  *
  * input     : session contain a element/handler used by tlv protocol
  *             registerAddress is the address of the target register
  *
  * return    : NONE
  */
void readTargetRegister(Tlv_Session *session, uint32_t registerAddress) {
  uint32_t data = 0;
  
  data = readCoreRegister(registerAddress);
  
  Tlv *tlv = tlvCreatePacket(TLV_OK, 4, (uint8_t *)&data);
  
  tlvSend(session, tlv);
}

/** readAllTargetRegister is a function to read value from all target register using swd
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void readAllTargetRegister(Tlv_Session *session)
{
  int i = 0 , j = 0;
  uint32_t data[52] = {} ;

  for (i = 0 ; i < 20 ; i ++)
    data[i] = readCoreRegister(i);
  
  data[i] = readCoreRegister(CORE_REG_FPSCR);
  
  for(j = 64 ; j < 96 ; j++)
  {
    i++ ;
    data[i] = readCoreRegister(j);
  }
  
  Tlv *tlv = tlvCreatePacket(TLV_OK, (4*52), (uint8_t *)&data);
  
  tlvSend(session,tlv);
}

/** Perform a soft reset on the target device
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void performSoftResetOnTarget(Tlv_Session *session)
{
  softResetTarget();
  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  tlvSend(session, tlv);
}

/** Perform a hard reset on the target device
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void performHardResetOnTarget(Tlv_Session *session)
{
  hardResetTarget();
  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  tlvSend(session, tlv);
}

/** Perform a hard reset on the target device
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void performVectorResetOnTarget(Tlv_Session *session)
{
  vectorResetTarget();
  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  tlvSend(session, tlv);
}

/** Halt the processor of the target device 
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void haltTarget(Tlv_Session *session)
{
  Tlv *tlv ;
  setCoreMode(CORE_DEBUG_HALT);
  
  if(getCoreMode() == CORE_DEBUG_HALT)
    tlv = tlvCreatePacket(TLV_OK, 0, 0);
  else
    Throw(TLV_NOT_HALTED);
  
  tlvSend(session, tlv);
}

/** Run the target device 
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void runTarget(Tlv_Session *session)
{
  Tlv *tlv ;
  
  if(GET_FLAG_STATUS(session, TLV_SET_BREAKPOINT_FLAG) != FLAG_SET) {
    stepIntoOnce();
    enableFPBUnit();
    setCoreMode(CORE_DEBUG_MODE);
    if(getCoreMode() == CORE_DEBUG_MODE) {
      tlv = tlvCreatePacket(TLV_OK, 0, 0);
    }
    else Throw(TLV_NOT_RUNNING);
    tlvSend(session, tlv);
  }
  else {
    SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
    breakpointEventHandler(session);
  }
}

/** Step the processor of target device once and send the current PC to host
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void performSingleStepInto(Tlv_Session *session)
{
  Tlv *tlv ;
  uint32_t pc = 0 , initialPC = 0 ;
  
  initialPC = readCoreRegister(CORE_REG_PC);
  
  pc = stepIntoOnce();
  
  if(pc == initialPC)
    Throw(TLV_NOT_STEPPED);
  else
    tlv = tlvCreatePacket(TLV_STEP,4, (uint8_t *)&pc);

  tlvSend(session, tlv);
}

/** Step the processor of target device multiple times and send the current PC to host
  *
  * Input     : session contain a element/handler used by tlv protocol
  *           : nInstructions is the number of instructions to step
  *
  */
void performMultipleStepInto(Tlv_Session *session, int nInstructions)
{
  int i = 0 ;
  
  for(i = 0 ; i < nInstructions ; i ++)
    performSingleStepInto(session);
}

/** Step over single instruction 
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void performStepOver(Tlv_Session *session)
{
  Tlv *tlv ;
  uint32_t pc = 0 , initialPC = 0  ;
  
  initialPC = readCoreRegister(CORE_REG_PC);
  pc = stepOver();
  
  if(pc == 0 || pc == initialPC)
    Throw(TLV_NOT_STEPOVER);
  else
    tlv = tlvCreatePacket(TLV_STEPOVER,4, (uint8_t *)&pc);
    
  tlvSend(session, tlv);
}

/** Step out from subroutine
  *
  * Input     : session contain a element/handler used by tlv protocol
  *
  */
void performStepOut(Tlv_Session *session)
{
  Tlv *tlv ;
  uint32_t pc = 0 ;
  
  pc = stepOut();
  
  if(pc == 0)
    Throw(TLV_NOT_STEPOUT);
  else
    tlv = tlvCreatePacket(TLV_STEPOUT,4, (uint8_t *)&pc);
    
  tlvSend(session, tlv);
}

/** Set instruction breakpoint
 *
 * Input : session contain a element/handler used by tlv protocol
 *         instructionAddress is the address that will be breakpointed
 *         matchingMode defines the behaviour when the comparator is matched
 *         Possible value :
 *					  MATCH_LOWERHALFWORD	    Set breakpoint on lower halfword 	
 *					  MATCH_UPPERHALFWORD	    Set breakpoint on upper halfword 	
 *					  MATCH_WORD		          Set breakpoint on both upper and lower halfword			
 *
 */
void setBreakpoint(Tlv_Session *session, uint32_t instructionAddress, int matchingMode)
{
  Tlv *tlv ;
  int comparatorUsed = 0;
  comparatorUsed = autoSetInstructionBreakpoint(instructionAddress,matchingMode);
  
  if( comparatorUsed == -1)
    Throw(TLV_BKPT_MAXSET);
  else  {
    SET_FLAG_STATUS(session, TLV_SET_BREAKPOINT_FLAG);
    tlv = tlvCreatePacket(TLV_OK, 0, 0);
    tlvSend(session, tlv);
  }
}

/** Set data watchpoint 
 *
 * Input     : session contain a element/handler used by tlv protocol
 *             address is the address to be compared
 *             addressMask is the mask going to be applied to the address 
 *             Possible value :
 *              WATCHPOINT_MASK_NOTHING  		  Compare all 32 bits of address set in DWT_COMPn 
 *							WATCHPOINT_MASK_BIT0  			  Ignore Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT1_BIT0,		Ignore Bit1 and Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT2_BIT0,		Ignore Bit2 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT3_BIT0,		Ignore Bit3 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT4_BIT0     Ignore Bit4 to Bit 0 of address set in DWT_COMPn during comparison
 *                        ""                                              ""
 *							WATCHPOINT_MASK_BIT12_BIT0		Ignore Bit12 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT13_BIT0		Ignore Bit13 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT14_BIT0	  Ignore Bit14 to Bit 0 of address set in DWT_COMPn during comparison
 *
 *          accessMode is the access mode for the watchpoint
 *          Possible value :
 *              WATCHPOINT_READ         Watchpoint on read access
 *              WATCHPOINT_WRITE        Watchpoint on write access
 *              WATCHPOINT_READWRITE    Watchpoint on read/write access
 */
void setWatchpoint(Tlv_Session *session,uint32_t address,Watchpoint_AddressMask addressMask,
                   uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode)
{
  Tlv *tlv ;
  
  setDataWatchpoint_MatchingOneComparator(COMPARATOR_3,address,addressMask,matchedData,dataSize,accessMode); 

  tlv = tlvCreatePacket(TLV_OK, 0, 0);
  tlvSend(session, tlv);  
}                   


/** Remove single instruction breakpoint
 *
 * Input     : session contain a element/handler used by tlv protocol
 *             instructionAddress is the address set to breakpoint previously going to be removed
 */
void removeInstructionBreakpoint(Tlv_Session *session, uint32_t instructionAddress)
{
  Tlv *tlv ;
  uint8_t errorCode = TLV_ADDRESS_NOT_FOUND ;
  
  if(disableFPComparatorLoadedWithAddress(instructionAddress,INSTRUCTION_TYPE) == -1)
    tlv = tlvCreatePacket(TLV_NOT_OK,1,&errorCode);
  else
    tlv = tlvCreatePacket(TLV_OK, 0, 0);
  tlvSend(session, tlv);
}


/** Remove all instruction breakpoint
 *
 * Input     : session contain a element/handler used by tlv protocol
 */
void removeAllInstructionBreakpoint(Tlv_Session *session)
{
  Tlv *tlv ;
  
  removeAllFPComparatorSetToBreakpoint();
  
  tlv = tlvCreatePacket(TLV_OK, 0, 0);
  tlvSend(session, tlv);
}

/** Stop single flash patch remapping
 *
 * Input     : session contain a element/handler used by tlv protocol
 *             address is the address going to be stop from remapping
 */
void stopFlashPatchRemapping(Tlv_Session *session,uint32_t address)
{
  Tlv *tlv ;
  int found = 0 ;
  uint8_t errorCode = TLV_ADDRESS_NOT_FOUND;
 
  found = disableFPComparatorLoadedWithAddress(address,INSTRUCTION_TYPE);
  found += disableFPComparatorLoadedWithAddress(address,LITERAL_TYPE);
  
  if(found < 0)
    tlv = tlvCreatePacket(TLV_NOT_OK,1,&errorCode);
  else
    tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  tlvSend(session, tlv);
}

/**
 * Stop all flash patch remapping
 *
 * Input     : session contain a element/handler used by tlv protocol
 */
void stopAllFlashPatchRemapping(Tlv_Session *session)
{
  Tlv *tlv ;
  
  stopAllFPRemapping();
  
  tlv = tlvCreatePacket(TLV_OK, 0, 0);
  tlvSend(session, tlv);
}


/**
 * Check for breakpoint event and return the PC if breakpoint occurs
 *
 * Input  : session contain a element/handler used by tlv protocol
 */
void breakpointEventHandler(Tlv_Session *session)
{
  Tlv *tlv ;
  uint32_t pc =0 , machineCode = 0 ;
  
  if(!hasBreakpointDebugEventOccured())
    return ;
  else 
  {
    pc = readCoreRegister(CORE_REG_PC);
    //machineCode = memoryReadAndReturnWord(pc);
    
    // if(machineCode & SOFTWAREBREAKPOINT_MASK ==  BKPT_ENCODING)
    // {
      
    // }
    // else
      disableFPBUnit();
    
    clearBreakpointDebugEvent();
  }
  
  /* Clear ongoingProcess and breakPoint flag to indicate the breakpoint event is over*/
  CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_SET_BREAKPOINT_FLAG);

  tlv = tlvCreatePacket(TLV_BREAKPOINT, 4, (uint8_t *)&pc);
  tlvSend(session, tlv);  
}

/**
 * Check for watchpoint event and return the PC if watchpoint occurs
 *
 * Input  : session contain a element/handler used by tlv protocol
 */
void watchpointEventHandler(Tlv_Session *session)
{
  Tlv *tlv ;
  uint32_t pc =0 ;
  
  if(!(hasDataWatchpointOccurred())) {
	  return;
  }
  else
  {
    pc = readCoreRegister(CORE_REG_PC);
    disableDWTComparator(COMPARATOR_1);
    clearDWTTrapDebugEvent() ;
  }
  
  tlv = tlvCreatePacket(TLV_WATCHPOINT, 4, (uint8_t *)&pc);
  tlvSend(session, tlv);  
}

/** writeDataToRamInChunk is a function to write data
  * to Ram in specified size
  *
  * Input   : dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *
  * return  : NONE
  */
void writeDataToRamInChunk(uint32_t *dataAddress, uint32_t destAddress, int size) {
  int i;
  
  /* Write to RAM using swd */
  for(i = 0; i < size; i += 4)  {
    /* Data start at position 4 */
    memoryWriteWord(destAddress, *dataAddress++);
    destAddress += 4;
  }  
}

/** writeTargetRam is a function to write target RAM using swd
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *
  * return  : NONE
  */
void writeTargetRam(Tlv_Session *session, uint32_t *dataAddress, uint32_t destAddress, int size)  {

  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  /* Size minus 1 because of the checksum value */
  writeDataToRamInChunk(dataAddress, destAddress, size);
  
  tlvSend(session, tlv);
}

void loopBack(Tlv_Session *session, Tlv *packet) {

  Tlv *tlv;
  int i = 0, delay = 1000;

  /* Size minus 1 because of the checksum value */
  for(i = 0; i < packet->length -1; i++) {
	 packet->value[i] += 2;
  }
  tlv = tlvCreatePacket(TLV_OK, packet->length - 1, packet->value);

  tlvSend(session, tlv);

  //while(delay--);
}

/** readTargetMemory is a function to read target RAM using swd
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           destAddress is the address of the data need to be read
  *           size is the size of the data can be any value
  *
  * return  : NONE
  */
void readTargetMemory(Tlv_Session *session, uint32_t destAddress, int size) {
  int i; uint8_t chksum = 0;
  uint32_t readData = 0;
  
  Tlv *tlv = tlvCreatePacket(TLV_OK, size + 4, NULL);
  
  /* store destAddress checksum */
  chksum = tlvPackIntoBuffer(tlv->value, (uint8_t *)&destAddress, 4);
  
  /* Read from RAM using swd */
  for(i = 0; i < size; i += 4)  {
    readData = memoryReadAndReturnWord(destAddress);
    /* Data start at position 4 */
    chksum += tlvPackIntoBuffer(&tlv->value[4 + i], (uint8_t *)&readData, 4);
    destAddress += 4;
  }

  tlv->value[tlv->length - 1] = chksum;
  
  tlvSend(session, tlv);
}

/** writeTargetFlash is a function to write target RAM using swd
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           dataAddress is the address of the data need to send
  *           destAddress is the address of the data need to be store
  *           size is the size of the data can be any value
  *
  * return  : NONE
  */
void writeTargetFlash(Tlv_Session *session, uint32_t *dataAddress, uint32_t destAddress, int size) {
  Tlv *tlv;
  
  switch(session->pFlashState) {
    
    case WRITE_TO_RAM :
      writeDataToRamInChunk(dataAddress, tempAddress, size);
      SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
      session->pFlashState = COPY_TO_FLASH;
    break;
    
    case COPY_TO_FLASH :
      if(IsStubBusy()) {
        requestStubCopy(tempAddress, destAddress, size);
        tlv = tlvCreatePacket(TLV_OK, 0, 0);
        tlvSend(session, tlv);
        CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        session->pFlashState = WRITE_TO_RAM;
      }
    break;
  }
}

void eraseTargetFlash(Tlv_Session *session, uint32_t address, int size) {
  Tlv *tlv;
  
  switch(session->pEraseState) {
    
    case REQUEST_ERASE :
      requestStubErase(address, size);
      SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
      session->pEraseState = WAIT_OPERATION_COMPLETE;
    break;
    
    case WAIT_OPERATION_COMPLETE :
      if(IsStubBusy()) {
        tlv = tlvCreatePacket(TLV_OK, 0, 0);
        tlvSend(session, tlv);
        CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        session->pEraseState = REQUEST_ERASE;
      }
    break;
  }  
}

void massEraseTargetFlash(Tlv_Session *session, uint32_t bankSelect) {
  Tlv *tlv;
  
  switch(session->pMEraseState) {
    
    case REQUEST_ERASE :
      requestStubMassErase(bankSelect);
      SET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
      session->pMEraseState = WAIT_OPERATION_COMPLETE;
    break;
    
    case WAIT_OPERATION_COMPLETE :
      if(IsStubBusy()) {
        tlv = tlvCreatePacket(TLV_OK, 0, 0);
        tlvSend(session, tlv);
        CLEAR_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG);
        session->pMEraseState = REQUEST_ERASE;
      }
    break;
  }
}

/** writeTargetInWord is a function write data into target
  * in Word size (32bit)
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           address is the address of the data need to be store
  *           data is the data need to be send
  *
  * Return  : NONE
  */
void writeTargetInWord(Tlv_Session *session, uint32_t address, uint32_t data) {
  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  /* Write data into specified address */
  memoryWriteWord(address, data);
  
  tlvSend(session, tlv);
}

/** writeTargetInHalfWord is a function write data into target
  * in halfWord size (16bit)
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           address is the address of the data need to be store
  *           data is the data need to be send
  *
  * Return  : NONE
  */
void writeTargetInHalfWord(Tlv_Session *session, uint32_t address, uint16_t data) {
  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  /* Write data into specified address */
  memoryWriteHalfword(address, data);
  
  tlvSend(session, tlv);
}

/** writeTargetInByte is a function write data into target
  * in halfWord size (8bit)
  *
  * Input   : session contain a element/handler used by tlv protocol
  *           address is the address of the data need to be store
  *           data is the data need to be send
  *
  * Return  : NONE
  */
void writeTargetInByte(Tlv_Session *session, uint32_t address, uint8_t data) {
  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  /* Write data into specified address */
  memoryWriteByte(address, data);
  
  tlvSend(session, tlv);
}

/** selectTask is a function to select instruction 
  * base on tlv->type
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
void selectTask(Tlv_Session *session, Tlv *tlv)  {
  
  switch(tlv->type) {
    case TLV_WRITE_RAM              : writeTargetRam(session, &get4Byte(&tlv->value[4]), get4Byte(&tlv->value[0]), tlv->length - 5);          break;
    case TLV_WRITE_FLASH            : writeTargetFlash(session, &get4Byte(&tlv->value[4]), get4Byte(&tlv->value[0]), tlv->length - 5);        break;
    case TLV_READ_MEMORY            : readTargetMemory(session, get4Byte(&tlv->value[0]), get4Byte(&tlv->value[4]));                          break;
    case TLV_WRITE_REGISTER         : writeTargetRegister(session, get4Byte(&tlv->value[0]), get4Byte(&tlv->value[4]));                       break;
    case TLV_READ_REGISTER          : readTargetRegister(session, get4Byte(&tlv->value[0]));                                                  break;
    case TLV_HALT_TARGET            : haltTarget(session);                                                                                    break;
    case TLV_RUN_TARGET             : runTarget(session);                                                                                     break;
    case TLV_STEP                   : performMultipleStepInto(session, get4Byte(&tlv->value[0]));                                                  break;
    case TLV_BREAKPOINT             : setBreakpoint(session, get4Byte(&tlv->value[0]), MATCH_WORD);                                           break;
    case TLV_REMOVE_BREAKPOINT      : break;
    case TLV_REMOVE_ALL_BREAKPOINT  : removeAllInstructionBreakpoint(session);                                                                break;
    case TLV_STOP_REMAP             : break;
    case TLV_STOP_ALL_REMAP         : stopAllFlashPatchRemapping(session);                                                                    break;
    case TLV_FLASH_ERASE            : eraseTargetFlash(session, get4Byte(&tlv->value[0]), get4Byte(&tlv->value[4]));                          break;
    case TLV_FLASH_MASS_ERASE       : massEraseTargetFlash(session, get4Byte(&tlv->value[0]));                                                break;
    case TLV_SOFT_RESET             : performSoftResetOnTarget(session);                                                                      break;
    case TLV_HARD_RESET             : performHardResetOnTarget(session);                                                                      break;
    case TLV_LOOP_BACK              : loopBack(session, tlv);                                                                                 break;
    case TLV_WRITE_WORD             : writeTargetInWord(session, get4Byte(&tlv->value[0]), getDataInWord(&tlv->value[4]));                    break;
    case TLV_WRITE_HALFWORD         : writeTargetInHalfWord(session, get4Byte(&tlv->value[0]), getDataInHalfWord(&tlv->value[4]));            break;
    case TLV_WRITE_BYTE             : writeTargetInByte(session, get4Byte(&tlv->value[0]), getDataInByte(&tlv->value[4]));                    break;
  }
}

/** probeTaskManager
  */
void probeTaskManager(Tlv_Session *session)  {
  static Tlv *packet;
  CEXCEPTION_T err;
  
  switch(session->probeState)  {
    case PROBE_RECEIVE_PACKET :
      Try {
        packet = tlvReceive(session);
        if(verifyTlvPacket(packet)) {
          session->probeState = PROBE_INTERPRET_PACKET;
        }
      } Catch(err) {
        tlvErrorReporter(session, err);
      }
    break;
      
    case PROBE_INTERPRET_PACKET :
      Try {
        selectTask(session, packet);
        if(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_CLEAR)
          session->probeState = PROBE_RECEIVE_PACKET;
      } Catch(err) {
        tlvErrorReporter(session, err);
        session->probeState = PROBE_RECEIVE_PACKET;
      }
    break;
  }
}
