#include "Reset.h"
#include "Delay.h"

void resetTarget()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	delay(10,1,1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	delay(2600,1,1);
}

void lineReset()
{
	int i ;

	SWDIO_High();

	for (i = 0 ; i < 53 ; i ++)
		clockGenerator_1cycle();
}
