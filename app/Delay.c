#include "Delay.h"

void delay(int ms,int numberOfLoops)
{
	int i = 0 , counter1 = 0;

	for ( i = 0 ; i < numberOfLoops; i ++)
	{
		while (counter1 != (100 * ms))
			counter1 ++ ;
	}
}
