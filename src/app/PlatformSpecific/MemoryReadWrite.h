#ifndef MemoryReadWrite_H
#define MemoryReadWrite_H

#include <stdint.h>
#include "Swd.h"
#include "IoOperations.h"
#include "IoOperationsEx.h"
#include "configurePort.h"

extern int cswDataSize ;

/* Used for mocking */
uint32_t memoryReadAndReturnWord(uint32_t address);
uint8_t memoryReadAndReturnByte(uint32_t address);

int memoryReadByte(uint32_t address, uint32_t *dataRead);
int memoryReadWord(uint32_t address, uint32_t *dataRead);
int memoryReadHalfword(uint32_t address, uint32_t *dataRead);

SwdError memoryWriteByte(uint32_t address, uint8_t writeData);
SwdError memoryWriteHalfword(uint32_t address, uint16_t writeData);
SwdError memoryWriteWord(uint32_t address, uint32_t writeData);

uint32_t memoryWriteDataAlignment(uint32_t address, uint16_t writeData);

#endif // MemoryReadWrite_H
