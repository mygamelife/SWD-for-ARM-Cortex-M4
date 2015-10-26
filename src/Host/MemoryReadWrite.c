#include "MemoryReadWrite.h"

/* Global Tlv Session */
Tlv_Session *_session = NULL;

/* Global Error code */
CEXCEPTION_T err = 0;

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

int memoryReadWord(uint32_t address, uint32_t *dataRead) {
  int size = WORD_SIZE; uint8_t *data = NULL;

  Try {
    /* Waiting reply from probe */
    while((data = tlvReadTargetMemory(_session, &address, &size)) == NULL) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  *dataRead = get4Byte(&data[4]);
  
  return 1;
}

int memoryReadHalfword(uint32_t address, uint32_t *dataRead) {
  int size = WORD_SIZE; uint8_t *data = NULL;

  Try {
    /* Waiting reply from probe */
    while((data = tlvReadTargetDataWithType(_session, address, TLV_READ_HALFWORD)) == NULL) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }

  *dataRead = getDataInHalfWord(&data[0]);

  return 1;
}

int memoryWriteWord(uint32_t address, uint32_t writeData) {
  int status = 0;
  
  Try {
    /* Waiting reply from probe */
    while((status = tlvWriteDataInWord(_session, address, writeData)) == 0) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  return 1;
}

int memoryWriteHalfword(uint32_t address, uint16_t writeData) {
  int status = 0;
  
  Try {
    /* Waiting reply from probe */
    while((status = tlvWriteDataInHalfword(_session, address, writeData)) == 0) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  return 1;
}

int memoryWriteByte(uint32_t address, uint8_t writeData) {
  int status = 0;
  
  Try {
    /* Waiting reply from probe */
    while((status = tlvWriteDataInByte(_session, address, writeData)) == 0) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  return 1;
}

int _flashWriteWord(uint32_t address, uint32_t writeData) {
  int status = 0, size = 4;
  uint8_t *pData = (uint8_t *)&writeData;
  
  Try {
    /* Waiting reply from probe */
    while((status = tlvWriteToFlash(_session, &pData, &address, &size)) == 0) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  return 1;
}