#include "LED.h"

extern int isr_vector_table;

int main(void)
{
	State state = START;
	int enable = 1;
	// int i;
	// for (i=0;i<48;i++){ // Assume maximum number of exception is 48
	// Copy each vector table entry from flash to SRAM
	// HW32_REG((VTOR_NEW_ADDR + (i<<2))) = HW32_REG((i<<2));
	// }
	SCB->VTOR = &isr_vector_table;

	configureLED();
	turnOnLED3();
  	while(enable)	{
  	  LED3_Blink(&state, &enable);
  	}
	//flashProgrammer();

	while(1)
	{}
}
