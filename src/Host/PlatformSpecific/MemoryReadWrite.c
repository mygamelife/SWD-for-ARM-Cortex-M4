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
  if(_session == NULL) {
    _session = tlvCreateSession();
  }
}

int memoryRead(uint32_t address, uint32_t *dataRead, int size) {
  uint8_t *data = NULL;

  Try {
    /* Waiting reply from probe */
    while((data = readMemory(_session, address, size)) == NULL) {
      tlvService(_session);
    };
  } Catch(err) {
    displayErrorMessage(err);
    Throw(err);
  }

  if(size == WORD_SIZE)           {*dataRead = get4Byte(data);}
  else if(size == HALFWORD_SIZE)  {*dataRead = get2Byte(data);}
  else if(size == BYTE_SIZE)      {*dataRead = *data;}

  return 1;
}

int memoryWrite(uint32_t address, uint32_t writeData, int size) {

  Try {
    /* Waiting reply from probe */
    while(writeRam(_session, (uint8_t *)&writeData, address, size) != PROCESS_DONE) {
      tlvService(_session);
    };
  } Catch(err) {
    displayErrorMessage(err);
    Throw(err);
  }

  return 1;
}

int _flashWrite(uint32_t address, uint8_t *data, int size) {
  int status = 0;

  Try {
    /* Erase flash */
    _flashErase(address, size);
    /* Write flash */
    while(writeFlash(_session, data, address, size) != PROCESS_DONE) {
      tlvService(_session);
    };
  } Catch(err) {
    displayErrorMessage(err);
    Throw(err);
  }

  return 1;
}

int _flashErase(uint32_t address, int size) {
  int status = 0;

  Try {
    while(eraseSection(_session, address, size) != PROCESS_DONE) {
      tlvService(_session);
    };
  } Catch(err) {
    displayErrorMessage(err);
    Throw(err);
  }

  return 1;
}