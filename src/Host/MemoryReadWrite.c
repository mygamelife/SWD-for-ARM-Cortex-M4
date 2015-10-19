#include "MemoryReadWrite.h"

/* Global Error code */
CEXCEPTION_T err = 0;

int memoryReadWord(Tlv_Session *session, uint32_t address, uint32_t *dataRead) {
  int size = 4; uint8_t *data = NULL;
  
  /* Start tlv request task */
  startTask(session->memoryRwState);
  Try {
    /* Waiting reply from probe */
    while((data = tlvReadTargetMemory(session, &address, &size)) == NULL) {
      // tlvService(session);
      yield(session->memoryRwState);
    };
  } Catch(err) {
    return err;
  }
  /* End tlv request task */
  endTask(session->memoryRwState);
  
  *dataRead = getDataInWord(&data[4]);
  
  return 1;
}

int memoryWriteWord(Tlv_Session *session, uint32_t address, uint32_t writeData) {
  int status = 0;
  
  /* Start tlv request task */
  startTask(session->memoryRwState);
  Try {
    /* Waiting reply from probe */
    while((status = tlvWriteDataInWord(session, address, writeData)) == 0) {
      // tlvService(session);
      yield(session->memoryRwState);
    };
  } Catch(err) {
    return err;
  }
  /* End tlv request task */
  endTask(session->memoryRwState);
  
  return 1;
}

int memoryWriteHalfWord(Tlv_Session *session, uint32_t address, uint16_t writeData) {
  int status = 0;
  
  /* Start tlv request task */
  startTask(session->memoryRwState);
  Try {
    /* Waiting reply from probe */
    while((status = tlvWriteDataInHalfWord(session, address, writeData)) == 0) {
      yield(session->memoryRwState);
    };
  } Catch(err) {
    return err;
  }
  /* End tlv request task */
  endTask(session->memoryRwState);
  
  return 1;
}

int memoryWriteByte(Tlv_Session *session, uint32_t address, uint8_t writeData) {
  int status = 0;
  
  /* Start tlv request task */
  startTask(session->memoryRwState);
  Try {
    /* Waiting reply from probe */
    while((status = tlvWriteDataInByte(session, address, writeData)) == 0) {
      yield(session->memoryRwState);
    };
  } Catch(err) {
    return err;
  }
  /* End tlv request task */
  endTask(session->memoryRwState);
  
  return 1;
}