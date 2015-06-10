#include "Delay.h"

void delay(int ms,int numberOfLoops, int division)
{
	int i = 0 , counter1 = 0;

	if (division == 0 )
		division = 1;

	for ( i = 0 ; i < numberOfLoops; i ++)
	{
		while (counter1 != ((100 / division) * ms ))
			counter1 ++ ;
	}
}
