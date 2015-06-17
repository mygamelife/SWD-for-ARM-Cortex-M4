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
 * Generate turn around period by clocking 1 cycle
 * and to be used when the host/target change I/O mode
 * 
 */
void turnAround()
{
	SWCLK_OFF();
	SWCLK_ON();
}
