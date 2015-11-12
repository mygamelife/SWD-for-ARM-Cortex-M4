#ifndef MemoryReadWrite_H
#define MemoryReadWrite_H

#include <stdint.h>
#include "Yield.h"
#include "CException.h"
#include "ProgramLoader.h"

extern Tlv_Session *_session;
extern int cswDataSize;

#define BYTE_SIZE           1
#define HALFWORD_SIZE       2
#define WORD_SIZE           4

// #define memoryReadByte(address, dataRead)         memoryRead(address, dataRead, BYTE_SIZE       )
// #define memoryReadHalfword(address, dataRead)     memoryRead(address, dataRead, HALFWORD_SIZE   )
// #define memoryReadWord(address, dataRead)         memoryRead(address, dataRead, WORD_SIZE       )

// #define memoryWriteByte(address, dataWrite)       memoryWrite(address, dataWrite, BYTE_SIZE     )
// #define memoryWriteHalfword(address, dataWrite)   memoryWrite(address, dataWrite, HALFWORD_SIZE )
// #define memoryWriteWord(address, dataWrite)       memoryWrite(address, dataWrite, WORD_SIZE     )

/* Used for mocking */
uint32_t memoryReadAndReturnWord(uint32_t address);
uint8_t memoryReadAndReturnByte(uint32_t address);

/* Initialize memory read write by creating tlv session */
void initMemoryReadWrite(void);
int memoryRead(uint32_t address, uint32_t *dataRead, int size);
int memoryWrite(uint32_t address, uint32_t dataWrite, int size);

/*********** ############ MemoryReadWrite ########## **********/
int memoryReadByte(uint32_t address, uint32_t *dataRead);
int memoryReadHalfword(uint32_t address, uint32_t *dataRead);
int memoryReadWord(uint32_t address, uint32_t *dataRead);

int memoryWriteByte(uint32_t address, uint32_t dataWrite);
int memoryWriteHalfword(uint32_t address, uint32_t dataWrite);
int memoryWriteWord(uint32_t address, uint32_t dataWrite);

int _flashWrite(uint32_t address, uint32_t writeData, int size);
int _flashErase(uint32_t address, int size);

uint32_t memoryWriteDataAlignment(uint32_t address, uint16_t writeData);

#endif // MemoryReadWrite_H
