#include "swdStub.h"

extern int isr_vector_table;

int main(void)
{
	SCB->VTOR = &isr_vector_table;
	flashProgrammer();

	while(1)
	{}
}
