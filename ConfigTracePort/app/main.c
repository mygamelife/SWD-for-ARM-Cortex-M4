#include "main.h"

extern int isr_vector_table;

int main(void)
{
  /* Reallocate vector table to RAM */
  SCB->VTOR = (uint32_t)&isr_vector_table;
  
  /* Initialize hardware and configure system clock */
  SystemClock_Config();

  while(1) {}
}
