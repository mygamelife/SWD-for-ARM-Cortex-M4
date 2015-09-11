#include "swdStub.h"
#include "configurePort.h"
#include "LED.h"

extern int isr_vector_table;

int main(void)
{
	int counter = 10;
	State state = START;

	SCB->VTOR = &isr_vector_table;

	//while(counter != 0)	{
		//blinkLED3(&state, &counter);
	//}
	flashProgrammer();

	while(1)
	{}
}
