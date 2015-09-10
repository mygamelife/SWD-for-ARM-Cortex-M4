#include "main.h"

extern int isr_vector_table;

int main(void)
{
  State state = START;  int counter = 20;

  SCB->VTOR = &isr_vector_table;

  HAL_Init();
  configureLED();

  while(state != HALT)	{
    blinkLED3(&state, &counter);
  }
  
	flashProgrammer();

	while(1)  {}
}