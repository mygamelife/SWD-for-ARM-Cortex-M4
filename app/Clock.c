#include "Clock.h"

/**
 * Generate 1 clock cycle at SWDCLK pin by clocking low followed by clocking high
 *
 */
void clockGenerator_1cycle()
{
	SWCLK_OFF();
	SWCLK_ON();
}

/**
 * Generate idle clock cycle by setting SWDIO pin low and clock
 * before starting a new transaction 
 *
 * Note : After line reset there should be at least 2 idle clock cyle
 *
 * Input : numberOfClocks is the amount of idle clock to be generated
 */
void extraIdleClock(int numberOfClocks)
{
	int i;

	SWDIO_Low();
	for(i = 0 ; i < numberOfClocks ; i ++)
		clockGenerator_1cycle();
}

/**
 * Generate 0.5 clock cycle of turn around period 
 *
 * Host : change from output mode to input mode
 * Target : change from input mode to output mode
 * 
 */
void turnAround_ToRead()
{
	SWCLK_OFF(); // z impedance on both line starts here
}

/**
 * Generate 1.5 clock cycle of turn around period 
 *
 * Host : change from input mode to output mode
 * Target : change output mode to input mode
 * 
 */
void turnAround_ToWrite()
{
	SWCLK_ON(); // z impedance on both line starts here
	SWCLK_OFF();
	SWCLK_ON();
}
