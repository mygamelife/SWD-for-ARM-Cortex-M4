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
  int received = 0;

  while(HAL_UART_Receive(uartHandle, buffer, ONE_BYTE, 5000) == HAL_OK) {
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
  uint32_t value = (*(uint32_t *)(&tlv->value[4]));
  uint32_t value2 = (*(uint32_t *)(&tlv->value[8]));
  uint32_t addr = (*(uint32_t *)(&tlv->value[0]));
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
  else  return WRITE_FAIL;
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

/** tlvWriteToTargetRam
  *
  * input     : 
  *
  * return    : NONE
  */
void probeProgrammer(Probe_TypeDef *probe)  {
	int writeRamStatus = 0, i;
  switch(probe->state)
  {
    case PROBE_WAIT :
      waitIncomingData(probe->uartHandle, probe->rxBuffer);
      probe->state = PROBE_INTERPRET_INSTRUCTION;
      break;
      
    case PROBE_INTERPRET_INSTRUCTION :
      if(probe->rxBuffer[0] == TLV_START_TRANSMISSION) {
        stm32UartSendByte(probe->uartHandle, PROBE_OK);
      }

      else if(probe->rxBuffer[0] == TLV_WRITE) {
        writeRamStatus = tlvDecodeAndWriteToRam(probe->rxBuffer);
        //stm32UartSendByte(probe->uartHandle, PROBE_OK);
        if(WRITE_SUCCESS) {
          probe->txBuffer[0] = PROBE_OK;
        }
        else  {
          probe->txBuffer[0] = TLV_DATA_CORRUPTED;
        }
        HAL_UART_Transmit(probe->uartHandle, probe->txBuffer, 1, 5000);
      }

      else if(probe->rxBuffer[0] == TLV_END_TRANSMISSION) {
    	  int i;

    	  for(i = 0; i < 255; i ++)
    		  probe->rxBuffer[i] = 0;

        probe->state = PROBE_END;
        return;
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
