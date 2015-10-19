#include "memoryRW.h"

/** ################################ Used by SwdStub mocking purpose ################################
  * readMemoryData is a function to read the data from the corresponding address and return the value
  *
  * input   :   address is the starting address to program it is define by user
  *
  * output  :   data32 is the value store inside the address
  */
uint32_t readMemoryData(uint32_t address) {
	__IO uint32_t data32 = 0;
  
  data32 = *(__IO uint32_t*)address;
  return data32;
}

void writeMemoryData(uint32_t address, uint32_t data) {
  *(__IO uint32_t*)address = data;
}