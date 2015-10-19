#ifndef MemoryReadWrite_H
#define MemoryReadWrite_H

#include "ProgramLoader.h"
#include "Yield.h"

int memoryReadWord(Tlv_Session *session, uint32_t address, uint32_t *dataRead);
int memoryWriteWord(Tlv_Session *session, uint32_t address, uint32_t writeData);
int memoryWriteHalfWord(Tlv_Session *session, uint32_t address, uint16_t writeData);
int memoryWriteByte(Tlv_Session *session, uint32_t address, uint8_t writeData);

#endif // MemoryReadWrite_H
