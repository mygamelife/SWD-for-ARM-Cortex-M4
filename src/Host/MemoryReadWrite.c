#include "MemoryReadWrite.h"

/* Global Tlv Session */
Tlv_Session *_session = NULL; 
/* Global Error code */
CEXCEPTION_T err = 0;

int cswDataSize = -1;
/**
  ==============================================================================
            ################ Used To Testing TDD Purpose ################
  ==============================================================================  
  */
int memoryReadWord(uint32_t address, uint32_t *dataRead) {
  int size = 4; uint8_t *data = NULL;
  
  if(_session == NULL) _session = tlvCreateSession();

  Try {
    /* Waiting reply from probe */
    while((data = tlvReadTargetMemory(_session, &address, &size)) == NULL) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  *dataRead = getDataInWord(&data[4]);
  
  return 1;
}

int memoryReadHalfword(uint32_t address,uint32_t *dataRead) {
  int size = 4; uint8_t *data = NULL;
  
  if(_session == NULL) _session = tlvCreateSession();

  Try {
    /* Waiting reply from probe */
    while((data = tlvReadTargetMemory(_session, &address, &size)) == NULL) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  *dataRead = getDataInWord(&data[4]);
  
  return 1;
}

int memoryWriteWord(uint32_t address, uint32_t writeData) {
  int status = 0;
  
  if(_session == NULL) _session = tlvCreateSession();

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

int memoryWriteHalfWord(uint32_t address, uint16_t writeData) {
  // int status = 0;
  
  // if(_session == NULL) _session = tlvCreateSession();
  // /* Start tlv request task */
  // startTask(session->memoryRwState);
  // Try {
    // /* Waiting reply from probe */
    // while((status = tlvWriteDataInHalfWord(_session, address, writeData)) == 0) {
      // yield(session->memoryRwState);
    // };
  // } Catch(err) {
    // return err;
  // }
  // /* End tlv request task */
  // endTask(session->memoryRwState);
  
  // return 1;
}

int memoryWriteByte(uint32_t address, uint8_t writeData) {
  // int status = 0;
  
  // if(_session == NULL) _session = tlvCreateSession();
  // /* Start tlv request task */
  // startTask(session->memoryRwState);
  // Try {
    // /* Waiting reply from probe */
    // while((status = tlvWriteDataInByte(_session, address, writeData)) == 0) {
      // yield(session->memoryRwState);
    // };
  // } Catch(err) {
    // return err;
  // }
  // /* End tlv request task */
  // endTask(session->memoryRwState);
  
  // return 1;
}