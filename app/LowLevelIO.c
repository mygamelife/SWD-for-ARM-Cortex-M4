#include "LowLevelIO.h"

/**
 * Read the current pin state of SWDIO pin
 *
 * Output : 1 for GPIO_PIN_SET / High
 *          0 for GPIO_PIN_RESET / Low
 */
int readSWDIO()
{
	GPIO_PinState bitRead ;
	bitRead  = HAL_GPIO_ReadPin(SWD_PORT,SWDIO_PIN);

	return bitRead ;
}