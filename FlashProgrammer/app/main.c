#include "main.h"

extern int isr_vector_table;

int main(void)
{
  /* Reallocate vector table to RAM */
  SCB->VTOR = &isr_vector_table;
  
  /* Initialize hardware and configure system clock */
  FlashSystemConfig();
  stubInit();

  //STUB->instruction = STUB_COPY;
  //STUB->flashAddress = 0x08040000;
  //STUB->sramAddress = 0x20000000;
  //STUB->dataSize = 12;

  //writeMemoryData(0x20000000, 0xdead);
  //writeMemoryData(0x20000002, 0xbeef);
  //writeMemoryData(0x20000004, 0x1234);
  //writeMemoryData(0x20000006, 0x5678);
  //writeMemoryData(0x20000008, 0xabcd);
  //writeMemoryData(0x2000000A, 0xcafe);

  while(1)
  {
	  swdStub(STUB->instruction);
  }
}
