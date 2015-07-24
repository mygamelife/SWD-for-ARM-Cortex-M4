#ifndef SRAM_H
#define SRAM_H

#include <stdint.h>
#include "swdStub.h"

#define __IO  volatile

void sramWrite(uint32_t SRAM_Address, uint32_t data32);
uint32_t sramRead(uint32_t SRAM_Address);

#endif // SRAM_H
