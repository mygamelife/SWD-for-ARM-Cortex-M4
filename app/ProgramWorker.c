#include "ProgramWorker.h"

/** load_SectorErase_Instruction is a function to load the sector erase
  * instruction into SRAM to tell the swdStub
  *
  * input   : startAddress is the address to begin erase
  *           endAddress is the address to end erase
  *
  * output  : NONE
  */
void loadEraseSectorInstruction(uint32_t *startSector, uint32_t *endSector)  {
  uint32_t targetStatus = 0;
  /* Continues wait for target to release */
  do  {
    targetStatus = memoryReadAndReturnWord(SWD_TARGET_STATUS);
  } while(targetStatus != TARGET_OK);
  
  /* load flash start and end address to sram */
  memoryWriteWord(SWD_FLASH_START_ADDRESS, (uint32_t)startSector);
  memoryWriteWord(SWD_FLASH_END_ADDRESS, (uint32_t)endSector);
  
  /* load instruction to sram */
  memoryWriteWord(SWD_INSTRUCTION, INSTRUCTION_ERASE_SECTOR);
}

/** loadMassEraseInstruction is a function to load the mass erase
  * instruction into SRAM to tell the swdStub
  *
  * input   : bankSelect can be one of the following value
  *            + FLASH_BANK_1: Bank1 to be erased
  *            + FLASH_BANK_2: Bank2 to be erased
  *            + FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  * output  : NONE
  */
void loadMassEraseInstruction(uint32_t bankSelect)  {
  uint32_t targetStatus = 0;
  /* Continues wait for target to release */
  do  {
    targetStatus = memoryReadAndReturnWord(SWD_TARGET_STATUS);
  } while(targetStatus != TARGET_OK);
  
  /* load bank select to sram */
  memoryWriteWord(SWD_BANK_SELECT, bankSelect);
  
  /* load instruction to sram */
  memoryWriteWord(SWD_INSTRUCTION, INSTRUCTION_MASS_ERASE);  
}

/** loadCopyInstruction is a function copy data from src (SRAM) to dest (Flash)
  *
  * input   : src is the beginning SRAM address contain all the information
  *           dest is the flash address all the information need to copy over there
  *           length is to determine how many words need to copy over
  *
  * output  : NONE
  */
void loadCopyFromSRAMToFlashInstruction(uint32_t *dataAddress, uint32_t *destAddress, int size) {
  uint32_t targetStatus = 0;
  /* Continues wait for target to release */
  do  {
    targetStatus = memoryReadAndReturnWord(SWD_TARGET_STATUS);
  } while(targetStatus != TARGET_OK);

  /* load SRAM start address into sram */
  memoryWriteWord(SWD_SRAM_START_ADDRESS, (uint32_t)dataAddress);
  
  /* load Flash start address into sram */
  memoryWriteWord(SWD_FLASH_START_ADDRESS, (uint32_t)destAddress);
  
  /* load length into sram */
  memoryWriteWord(SWD_DATA_SIZE, size);

	/* load copy instructoin into sram */
  memoryWriteWord(SWD_INSTRUCTION, INSTRUCTION_COPY);
}

/** tlvCreateSession is a function to create necessary element needed by TLV protocol */
Tlv_Session *tlvCreateWorkerSession(void) {
  static Tlv_Session session;
  
  session.handler = uartInit();
  /* Set Initial state for send and receive*/
  session.sendState = END_SEND;
  session.receiveState = START_RECEIVE;
  
  session.tState = TRANSMISSION_FREE;
  session.TIMEOUT_FLAG = false;
  session.DATA_ARRIVE_FLAG = false;
  
  return &session;
}

/** writeTargetRegister is a function to write value into target register using swd
  *
  * input     : session contain a element/handler used by tlv protocol
  *             registerAddress is the address of the target register
  *             data is the data need to write into the target register
  *
  * return    : NONE
  */
void writeTargetRegister(Tlv_Session *session, uint32_t *registerAddress, uint32_t *data) {
  uint32_t regAddress = *registerAddress, regData = *data;
  
  writeCoreRegister(regAddress, regData);
  
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
void readTargetRegister(Tlv_Session *session, uint32_t *registerAddress) {
  uint32_t regAddress = *registerAddress, data = 0;
  
  readCoreRegister(regAddress, &data);
  
  Tlv *tlv = tlvCreatePacket(TLV_READ_REGISTER, 4, (uint8_t *)&data);
  tlvSend(session, tlv);
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

/** selectInstruction is a function to select instruction 
  * base on tlv->type
  *
  * Input   : tlv is pointer pointing to tlv packet
  *
  * Return  : NONE
  */
void selectInstruction(Tlv_Session *session, Tlv *tlv)  {
  Tlv *error; uint8_t errorCode = 0;
  
  switch(tlv->type) {
    case TLV_WRITE_RAM      : break;
    case TLV_READ_RAM       : break;
    case TLV_WRITE_REGISTER : writeTargetRegister(session, &get4Byte(&tlv->value[0]), &get4Byte(&tlv->value[4])); break;
    case TLV_READ_REGISTER  : readTargetRegister(session, &get4Byte(&tlv->value[0])); break;
    case TLV_HALT_TARGET    : break;
    case TLV_RUN_TARGET     : break;
    case TLV_STEP           : break;
    
    default :
      errorCode = ERR_INVALID_COMMAND;
      error = tlvCreatePacket(TLV_NOT_OK, 1, &errorCode);
      tlvSend(session, error);
      break;
  }
}

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
        // errorCode = ERR_CORRUPTED_DATA;
        // error = tlvCreatePacket(TLV_NOT_OK, 1, &errorCode);
        // tlvSend(session, error);
      // }
      // session->state = WAITING_PACKET;
      // break;
  // }
// }