#include "MemoryReadWrite.h"

/* Global Tlv Session */
Tlv_Session *_session = NULL;

/* Global Error code */
CEXCEPTION_T err = 0;

int cswDataSize = 0;
/**
  ==============================================================================
            ################ Used To Testing TDD Purpose ################
  ==============================================================================  
  */
void initMemoryReadWrite(void) {
  if(_session == NULL)
    _session = tlvCreateSession();
  do {
      tlvService(_session);
      tlvLoadToRam(_session, FLASH_PROGRAMMER_FILE_PATH);
    } while(GET_FLAG_STATUS(_session, TLV_ONGOING_PROCESS_FLAG) == FLAG_SET); 
       
}

int memoryRead(uint32_t address, uint32_t *dataRead, int size) {
  int i, dataSize = size; uint8_t *data = NULL;
  
  Try {
    /* Waiting reply from probe */
    while((data = tlvReadTargetMemory(_session, &address, &dataSize)) == NULL) {
      tlvService(_session);
    };
  } Catch(err) { Throw(err); }
  
  if(size == WORD_SIZE)           {*dataRead = get4Byte(&data[4]);}
  else if(size == HALFWORD_SIZE)  {*dataRead = get2Byte(&data[4]);}
  else if(size == BYTE_SIZE)      {*dataRead = data[4];}
  
  return 1;
}

int memoryWrite(uint32_t address, uint32_t dataWrite, int size) {
  uint8_t *data = (uint8_t *)&dataWrite;

  Try {
    /* Waiting reply from probe */
    while(tlvWriteToRam(_session, &data, &address, &size) != PROCESS_DONE) {
      tlvService(_session);
    }; 
  } Catch(err) { Throw(err); }
  
  return 1;
}

/* ########################## Mock ############################ */
int memoryReadByte(uint32_t address, uint32_t *dataRead) {
  return memoryRead(address, dataRead, BYTE_SIZE);
}

int memoryReadHalfword(uint32_t address, uint32_t *dataRead) {
  return memoryRead(address, dataRead, HALFWORD_SIZE);
}

int memoryReadWord(uint32_t address, uint32_t *dataRead) {
  return memoryRead(address, dataRead, WORD_SIZE);
}

int memoryWriteByte(uint32_t address, uint32_t dataWrite) {
  return memoryWrite(address, dataWrite, BYTE_SIZE);
}

int memoryWriteHalfword(uint32_t address, uint32_t dataWrite) {
  return memoryWrite(address, dataWrite, HALFWORD_SIZE);
}

int memoryWriteWord(uint32_t address, uint32_t dataWrite) {
  return memoryWrite(address, dataWrite, WORD_SIZE);
}
/* ########################## Mock ############################ */

int _flashWrite(uint32_t address, uint32_t writeData, int size) {
  uint8_t *pData = (uint8_t *)&writeData;
  int dataSize = size;
  
  Try {
    /* Waiting reply from probe */
    while(tlvWriteToFlash(_session, &pData, &address, &dataSize) != PROCESS_DONE) {
      tlvService(_session);
    };
  } Catch(err) { Throw(err); }
  
  return 1;
}

int _flashErase(uint32_t address, int size) {
  int status = 0;
  
  Try {
    while((status = tlvRequestFlashErase(_session, address, size)) == 0) {
      tlvService(_session);
    };
  } Catch(err) { Throw(err); }
  
  return 1;
}