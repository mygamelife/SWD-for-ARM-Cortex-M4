#ifndef memoryRW_H
#define memoryRW_H

#include <stdint.h>

#define __IO volatile

uint32_t readMemoryData(uint32_t address);
void writeMemoryData(uint32_t address, uint32_t data);

#endif // memoryRW_H
