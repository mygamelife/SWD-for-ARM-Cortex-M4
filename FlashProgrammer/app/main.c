#include "main.h"

extern int isr_vector_table;

int main(void)
{
  /* Reallocate vector table to RAM */
  SCB->VTOR = &isr_vector_table;
  
  /* Initialize hardware and configure system clock */
  FlashSystemConfig();

  while(1)
  {
	  svc(SVC_SERVICE_CALL);
  }
}
