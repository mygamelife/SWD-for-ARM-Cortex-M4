#ifndef MemoryReadWrite_H
#define MemoryReadWrite_H

#include <stdint.h>
#include "Yield.h"
#include "CException.h"
#include "ProgramLoader.h"

#define BYTE_SIZE                1
#define HALFWORD_SIZE            2
#define WORD_SIZE                4

extern Tlv_Session *_session ;
extern int cswDataSize ;

/* Used for mocking */
uint32_t memoryReadAndReturnWord(uint32_t address);

/* Initialize memory read write by creating tlv session */
void initMemoryReadWrite(void);

int memoryReadWord(uint32_t address, uint32_t *dataRead);
int memoryReadHalfword(uint32_t address, uint32_t *dataRead);
int memoryWriteByte(uint32_t address, uint8_t writeData);
int memoryWriteHalfword(uint32_t address, uint16_t writeData);
int memoryWriteWord(uint32_t address, uint32_t writeData);

uint32_t memoryWriteDataAlignment(uint32_t address, uint16_t writeData);

#endif // MemoryReadWrite_H
