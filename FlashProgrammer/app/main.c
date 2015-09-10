#include "swdStub.h"
#include "configurePort.h"
extern int isr_vector_table;

int main(void)
{
	int counter = 100;
	State state = START;

	SCB->VTOR = &isr_vector_table;
	//HAL_Init();
	//configureLED();

	flashProgrammer();

	while(1)
	{
		//if(counter != 0)
			//blinkLED3(&state, &counter);
	}
}
