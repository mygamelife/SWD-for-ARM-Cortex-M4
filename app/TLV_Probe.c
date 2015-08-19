#include "TLV_Probe.h"

/**
  * load_SectorErase_Instruction is a function to load the sector erase
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

/** <!For internal use only!>
  * waitIncomingData is a function to waiting data coming from host
  *
  * input     : 
  *
  * return    : NONE
  */
void waitIncomingData(UART_HandleTypeDef *uartHandle, uint8_t *buffer) {

  while(HAL_UART_Receive(uartHandle, buffer, ONE_BYTE, 5000) == HAL_OK)	{
	  break;
  }
}

/** tlvDecodeAndWriteToRam
  *
  * input   :   buffer is a pointer poiniting to an array that contain tlvPacket
  *
  * return  :   1 data is successful write into target ram
  *             0 data fail to write into target ram
  */
int tlvDecodeAndWriteToRam(uint8_t *buffer) {
  int i = 0, verifyStatus = 0;
  
  /* Decode packet from buffer */
  TLV *tlv = (TLV *)buffer;
  uint8_t length = tlv->length - CHECKSUM_LENGTH - ADDRESS_LENGTH;
  uint32_t address = get4Byte(&tlv->value[0]);
  
  verifyStatus = verifyValue(&tlv->value[4], length);

  if(verifyStatus)  {
    /* Write to ram using swd */
    for(i = 0; i < length; i += 4)  {
      /* Data start at position 4 */
      memoryWriteWord(address, get4Byte(&tlv->value[i + 4]));
      address = address + 4;
    }
      return WRITE_SUCCESS;
  }
  else {
    return WRITE_FAIL;
  }
}

/** verifyValue is a function to verify tlv data by adding
  * with the checksum value
  *
  * input     : data is a pointer pointing to TLV structure
  *
  * return    : 1   data is invalid
  *             0   data is valid
  */
int verifyValue(uint8_t *data, uint8_t length) {
  int i = 0;  uint8_t sum = 0;
  
  for(i; i < length + 1; i++) {
    sum += data[i];
  }
  
  if(sum == 0)  return 1;
  
  else  return 0;
}

/** readFromTargetRam
  *
  * input     : 
  *
  * return    : NONE
  */
void readFromTargetRam(Probe_TypeDef *probe)  {
  TLV *tlv = (TLV *)probe->rxBuffer;
  int i;
  //uint8_t txBuffer[255] = {0};
  uint8_t length = tlv->length;
  uint32_t address = get4Byte(&tlv->value[0]);
  uint32_t data = 0;
  
  probe->txBuffer[0] = TLV_SEND;
  /* start reading from ram */
  for(i = 1; i < length; i+= 4) {
    memoryReadWord(address, &data);
    probe->txBuffer[i]     = (data & 0xff000000) >> 24 ;
    probe->txBuffer[i + 1] = (data & 0x00ff0000) >> 16;
    probe->txBuffer[i + 2] = (data & 0x0000ff00) >> 8;
    probe->txBuffer[i + 3] = (data & 0x000000ff) >> 0;
    address = address + 0x4;
  }
  
  stm32UartSendBytes(probe->uartHandle, probe->txBuffer);
}

uint8_t *convertWordToByte(uint32_t wordData) {
  static uint8_t buffer[4];
  
  buffer[0] = (wordData & 0xff000000) >> 24 ;
  buffer[1] = (wordData & 0x00ff0000) >> 16;
  buffer[2] = (wordData & 0x0000ff00) >> 8;
  buffer[3] = (wordData & 0x000000ff) >> 0;
  
  return buffer;
}

/** 
  *
  * input     : 
  *
  * return    : NONE
  */
void writeTargetRegister(TLV_Session *session, uint32_t registerAddress, uint32_t data) {
  TLV *tlv;
  
  writeCoreRegister(registerAddress, data);
  //setCoreMode(SET_CORE_NORMAL);
  
  tlv = createTlvPacket(PROBE_OK, 0, 0);
  tlvSend(session, tlv);
}

/** 
  *
  * input     : 
  *
  * return    : NONE
  */
void readTargetRegister(TLV_Session *session, uint32_t registerAddress) {
  uint8_t *byteData;  uint32_t data = 0;
  TLV *tlv;
  
  readCoreRegister(registerAddress, &data);
  //setCoreMode(SET_CORE_NORMAL);
  
  byteData = convertWordToByte(data);
  tlv = createTlvPacket(PROBE_OK, 4, byteData);
  tlvSend(session, tlv);
}

void haltTarget(TLV_Session *session, uint32_t setCoreCommand) {
  setCoreMode(setCoreCommand);
  
  TLV *tlv = createTlvPacket(PROBE_OK, 0, 0);
  tlvSend(session, tlv);
}

void runTarget(TLV_Session *session, uint32_t setCoreCommand) {
  setCoreMode(setCoreCommand);
  
  TLV *tlv = createTlvPacket(PROBE_OK, 0, 0);
  tlvSend(session, tlv);
}

void step(TLV_Session *session, int nInstructions)  {
  stepOnly(nInstructions);
  
  TLV *tlv = createTlvPacket(PROBE_OK, 0, 0);
  tlvSend(session, tlv);
}

/** 
  *
  * input     : 
  *
  * return    : NONE
  */
TLV_Session *createTlvSession(void) {
  static TLV_Session session;
  
  session.uartHandle = initUart();
  
  return &session;
}

/** 
  *
  * input     : 
  *
  * return    : NONE
  */
TLV *createTlvPacket(uint8_t command, uint8_t size, uint8_t *data) {
  int i, chksum = 0; static TLV tlv;
  
  tlv.type = command;
  tlv.length = size + 1; //extrac length for chksum
  
  if(size > 0)  
  {
    for(i = 0; i < tlv.length; i++) 
    {
      tlv.value[i] = data[i];
      chksum += tlv.value[i];
    }
    tlv.value[tlv.length - 1] = ~chksum + 1;  
  }
  
  return &tlv;
}

/** 
  *
  * input     : 
  *
  * return    : NONE
  */
void tlvSend(TLV_Session *session, TLV *tlv) {
  
  /* TLV contain type, length and value **/
  stm32UartSendBytes(session->uartHandle, (uint8_t *)tlv);
}

/** 
  *
  * input     : 
  *
  * return    : NONE
  */
TLV *tlvReceive(TLV_Session *session) {
  static TLV tlv; 

  while(HAL_UART_Receive(session->uartHandle, session->receive, 2, 5000) != HAL_OK);
  tlv.type = session->receive[0];
  tlv.length = session->receive[1];
  
  if(tlv.length > 0)  {
    HAL_UART_Receive(session->uartHandle, session->receive, tlv.length, 5000);
    tlv.values = session->receive;
  }
  
  return &tlv;
}

void verifyTlvData()  {
  
}

void verifyTlvInstruction(TLV *tlv) {
  switch(tlv->type) {
    case TLV_READ_RAM :
      break;
    case TLV_WRITE_RAM :
      break;
    case TLV_READ_REGISTER :
      break;
    case TLV_WRITE_REGISTER :
      break;
    case TLV_HALT_TARGET :
      break;
    case TLV_RUN_TARGET :
      break;
    case TLV_STEP :
      break;
      
    default :
      break;  
  }
}

/** tlvWriteToTargetRam
  *
  * input     : 
  *
  * return    : NONE
  */
void probeProgrammer(Probe_TypeDef *probe, TLV_Session *session)  {
	int writeRamStatus = 0, i;
  switch(probe->state)
  {
    case PROBE_WAIT :
      waitIncomingData(session->uartHandle, probe->rxBuffer);
      verifyTlvInstruction((TLV *)probe->rxBuffer);
      probe->state = PROBE_INTERPRET_INSTRUCTION;
      break;
      
    case PROBE_INTERPRET_INSTRUCTION :
      if(probe->rxBuffer[0] == TLV_START_TRANSMISSION) {
        stm32UartSendByte(session->uartHandle, PROBE_OK);
      }

      else if(probe->rxBuffer[0] == TLV_WRITE_RAM) {
        writeRamStatus = tlvDecodeAndWriteToRam(probe->rxBuffer);
        //stm32UartSendByte(probe->uartHandle, PROBE_OK);
        if(writeRamStatus == WRITE_FAIL) {
          stm32UartSendByte(session->uartHandle, TLV_DATA_CORRUPTED);
        }
        else if(writeRamStatus == WRITE_SUCCESS) {
          stm32UartSendByte(session->uartHandle, PROBE_OK);
        }
      }
      
      else if(probe->rxBuffer[0] == TLV_READ_REGISTER) {
        readTargetRegister(session, get4Byte(&probe->rxBuffer[2]));
        probe->rxBuffer[0] = 0;
      }

      else if(probe->rxBuffer[0] == TLV_WRITE_REGISTER) {
    	writeTargetRegister(session, get4Byte(&probe->rxBuffer[2]), get4Byte(&probe->rxBuffer[5]));
        probe->rxBuffer[0] = 0;
      }

      else if(probe->rxBuffer[0] == TLV_HALT_TARGET) {
        writeTargetRegister(session, get4Byte(&probe->rxBuffer[2]), get4Byte(&probe->rxBuffer[5]));
          probe->rxBuffer[0] = 0;
      }

      else if(probe->rxBuffer[0] == TLV_RUN_TARGET) {
        writeTargetRegister(session, get4Byte(&probe->rxBuffer[2]), get4Byte(&probe->rxBuffer[5]));
          probe->rxBuffer[0] = 0;
      }

      else if(probe->rxBuffer[0] == TLV_STEP) {
        writeTargetRegister(session, get4Byte(&probe->rxBuffer[2]), get4Byte(&probe->rxBuffer[5]));
          probe->rxBuffer[0] = 0;
      }

      else if(probe->rxBuffer[0] == TLV_WRITE_REGISTER) {
          	writeTargetRegister(session, get4Byte(&probe->rxBuffer[2]), get4Byte(&probe->rxBuffer[5]));
              probe->rxBuffer[0] = 0;
            }
      else if(probe->rxBuffer[0] == TLV_READ_RAM) {
        readFromTargetRam(probe);
      }

      else if(probe->rxBuffer[0] == TLV_END_TRANSMISSION) {
        probe->rxBuffer[0] = 0;
        stm32UartSendByte(session->uartHandle, TLV_SEND);
        //return;
      }
      
      probe->state = PROBE_WAIT;
      break;
  }
}

/** tlvGetValue is a function to get tlv value inside the buffer
  *
  * input     : buffer contain length in bytes
  *             index is the start position of the length
  *
  * return    : length value
  */
// void tlvGetValue(uint8_t *buffer, uint8_t *tlvBuffer, int index, int length) {
  // int i = 0;
  
  // for(i; i < length; i++) {
    // tlvBuffer[i] = buffer[i + index];
  // }
// }

/** tlvVerifyType is a function to verify tlv type
  *
  * input     : type is a variable contain tlv type
  *
  * return    : 0   data is invalid
  *             1   data is valid
  */
// int tlvVerifyType(uint8_t type) {
  // if(type == TLV_WRITE)
    // return 1;
  
  // else if(type == TLV_TRANSFER_COMPLETE)
    // return 1;
  
  // else return 0;
// }

/** tlvVerifyLength is a function to verify tlv length
  *
  * input     : length is a variable contain tlv length
  *
  * return    : 0   data is invalid
  *             1   data is valid
  */
// int tlvVerifyLength(int length) {
  // if(length > 0)
    // return 1;
  
  // else
    // return 0;
// }



/** tlvConvertDataFromByteToWord is a function convert the data from bytes to word
  *
  * input   :   buffer contain all the information in bytes
  *             index is the start position to convert to word
  *
  * return  :   NONE
  */
// uint32_t tlvConvertFromByteToWord(uint8_t *buffer, int index) {
  // int i = 0, shiftValue = 24;
  // uint32_t data32 = 0;
  
  // for(i; i < 4; i++) {
    // data32 |= buffer[index++] << shiftValue;
    // shiftValue = shiftValue - 8;
  // }
  
  // printf("data %x\n", data32);
  
  // return data32;
// }

/** tlvCheckOperation is a function check operation and perform task accordingly
  *
  * input   :   tlv is a pointer pointing to TLV structure
  *
  * return  :   NONE
  */
// void tlvCheckOperation(TLV *tlv) {
  // if(tlv->type == TLV_WRITE)  {
    // /* Write data into RAM */
    
    // memoryAccessWrite(tlv->sectionAddress, tlvConvertFromByteToWord(tlv->value, 0));
  // }
  // else  {
    // /* implement other function here */
  // }
  
// }

    //        Pseudocode
    // state diagram {
       // switch(state) {
          // receive {
            // waiting data to arrive
          // }
          
          // decode  {
            // getType;
            // verifyType;
            // errorChecking;
            
            // getLength;
            // verifyLength;
            // errorChecking;
            
            // getData;
            // verifyData;
            // errorChecking;
            
            // no error?
            // }
          // }
          
          // check operation {
            // if(Type == WRITE) {
              // write_into_sram
              // convertDataToWord {
              // {0xAB,0xCD,0xEF,0x00} = {0xABCDEF00};
            // }
          // }
          
          // reply to host {
            // transmit reply here
          // }
          
        // }
      // }
  //
