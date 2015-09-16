#include "swdStub.h"
#include "configurePort.h"
#include "LED.h"

extern int isr_vector_table;

int main(void)
{
  __IO uint32_t swdInstruction = 0;

  /* Reallocate vector table to RAM */
  SCB->VTOR = &isr_vector_table;
  
  /* Initialize hardware and configure system clock */
  FlashSystemConfig();

  blinkingLED(ONE_SEC);
  
  /* Initialize target status */
  writeMemoryData(SWD_TARGET_STATUS, TARGET_OK);
  
  while(1)
  {
    swdInstruction = (__IO uint32_t)readMemoryData(SWD_INSTRUCTION);
    swdStub(swdInstruction);
  }
}
