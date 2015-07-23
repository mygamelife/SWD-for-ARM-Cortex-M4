#ifndef SRAM_H
#define SRAM_H

#include <stdint.h>
#include "swdStub.h"

#define __IO  volatile

void SRAM_Write(uint32_t SRAM_Address, uint32_t data32);
uint32_t SRAM_Read(uint32_t SRAM_Address);

#endif // SRAM_H
