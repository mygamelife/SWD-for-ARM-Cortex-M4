#ifndef MemoryReadWrite_H
#define MemoryReadWrite_H

#include <stdint.h>
// #include "Swd.h"
#include "Yield.h"
#include "CException.h"
// #include "IoOperations.h"
// #include "configurePort.h"
#include "ProgramLoader.h"

extern int cswDataSize ;

/* Used for mocking */
uint32_t memoryReadAndReturnWord(uint32_t address);

int memoryReadWord(uint32_t address,uint32_t *dataRead);
int memoryReadHalfword(uint32_t address,uint32_t *dataRead);
int memoryWriteByte(uint32_t address, uint8_t writeData);
int memoryWriteHalfword(uint32_t address, uint16_t writeData);
int memoryWriteWord(uint32_t address, uint32_t writeData);

uint32_t memoryWriteDataAlignment(uint32_t address, uint16_t writeData);

#endif // MemoryReadWrite_H
