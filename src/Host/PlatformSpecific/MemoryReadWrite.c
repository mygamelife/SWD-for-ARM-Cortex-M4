#include "MemoryReadWrite.h"

/* Global Tlv Session */
Tlv_Session *_session = NULL;

/* Global Error code */
CEXCEPTION_T err = 0;

int cswDataSize = 0;
/**
  ==============================================================================
            ################ Used For TDD Purpose ################
  ==============================================================================  
  */
void initMemoryReadWrite(void) {
  if(_session == NULL)
    _session = tlvCreateSession();
    
  do {
    tlvService(_session);
    loadRam(_session, FLASH_PROGRAMMER_FILE_PATH);
  } while(GET_FLAG_STATUS(_session, TLV_ONGOING_PROCESS_FLAG) == FLAG_SET);    
}

int memoryRead(uint32_t address, uint32_t *dataRead, int size) {
  Storage *s = NULL;
  
  Try {
    /* Waiting reply from probe */
    while((s = readMemory(_session, address, size)) == NULL) {
      tlvService(_session);
    };
  } Catch(err) { Throw(err); }
  
  if(size == WORD_SIZE)           {*dataRead = get4Byte(&s->data[0]);}
  else if(size == HALFWORD_SIZE)  {*dataRead = get2Byte(&s->data[0]);}
  else if(size == BYTE_SIZE)      {*dataRead = s->data[0];}
  
  return 1;
}

int memoryWrite(uint32_t address, uint32_t dataWrite, int size) {
  uint8_t *data = (uint8_t *)&dataWrite;

  Try {
    /* Waiting reply from probe */
    while(writeRam(_session, &data, &address, &size) != PROCESS_DONE) {
      tlvService(_session);
    }; 
  } Catch(err) { Throw(err); }
  
  return 1;
}

int _flashWrite(uint32_t address, uint32_t writeData, int size) {
  uint8_t *pData = (uint8_t *)&writeData;
  
  Try {
    /* Waiting reply from probe */
    while(writeFlash(_session, &pData, &address, &size) != PROCESS_DONE) {
      tlvService(_session);
    };
  } Catch(err) { Throw(err); }
  
  return 1;
}

int _flashErase(uint32_t address, int size) {
  int status = 0;
  uint32_t buffer[] = {address, size};
  
  Try {
    while(eraseSection(_session, address, size) != PROCESS_DONE) {
      tlvService(_session);
    };
  } Catch(err) { Throw(err); }
  
  return 1;
}