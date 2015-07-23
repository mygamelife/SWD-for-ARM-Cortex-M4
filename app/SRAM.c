#include "SRAM.h"

/**
  * SRAM_Write is a function to write data into SRAM address
  */
void SRAM_Write(uint32_t SRAM_Address, uint32_t data32) {
  *(__IO uint32_t*)SRAM_Address = data32;
}

/**
  * SRAM_Read is a function to read data from SRAM address
  */
uint32_t SRAM_Read(uint32_t SRAM_Address)  {
  __IO uint32_t data32 = 0;
  
  data32 = *(__IO uint32_t*)SRAM_Address;
  
  return data32;
}
