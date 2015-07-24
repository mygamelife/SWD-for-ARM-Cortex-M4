#include "SRAM.h"

/**
  * sramWrite is a function to write data into SRAM address
  */
void sramWrite(uint32_t SRAM_Address, uint32_t data32) {
  *(__IO uint32_t*)SRAM_Address = data32;
}

/**
  * sramRead is a function to read data from SRAM address
  */
uint32_t sramRead(uint32_t SRAM_Address)  {
  __IO uint32_t data32 = 0;
  
  data32 = *(__IO uint32_t*)SRAM_Address;
  
  return data32;
}
