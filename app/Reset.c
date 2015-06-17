#include "Reset.h"

/**
 * Perform a line reset by clocking at least 50 cycles of SWDIO high to 
 * ensure that JTAG/SWD interface is at reset state
 *
 * Input : numberOfClock is the number of clock with SWDIO high to be generated
 */
void lineReset(int numberOfClock)
{
	int i  = 0 ;
	
	if (numberOfClock < 50 )
		numberOfClock = 50 ;
	
	SWDIO_High();
	
	for ( i = 0 ; i < numberOfClock ; i ++)
	{
		clockGenerator_1cycle();
	}
}

/**
 * Perform a hard reset on the target device by setting low and high 
 * on the nRST pin of the target
 *
 * Note : It is recommended to perform a hard reset as the target device might not respond
 */
void resetTarget()
{
	ResetPin_Low();
	delay(500,1,1);
	ResetPin_High();
	delay(2500,1,1);

}
