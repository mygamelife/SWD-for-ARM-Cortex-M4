#include "LED.h"

void LED3_Blink(State *state, int *pTimer)	{
	static uint32_t previousTime = 0, counter = 0;

	switch (*state)
	{
		case START:
			turnOffLED3();
			*state = ON_LED3;
			break;

		case ON_LED3:
			if(oneHundredMiliSecHasNotExpired())	{
				turnOnLED3();
				*state = OFF_LED3;
			}
			break;

		case OFF_LED3:
			if(oneHundredMiliSecHasNotExpired())	{
				turnOffLED3();
				*state = COUNTER;
			}
			break;

		case COUNTER:
			counter++;
			if(counter == 10)	{
				*pTimer = 0;
				break;
			}

			else *state = ON_LED3;
			break;

		default:
			break;
	}
}

int oneHundredMiliSecHasNotExpired()	{
	static int previousTime = 0;

	if((getCurrentTime() - previousTime) > 50)	{
		previousTime = getCurrentTime();
		return 1;
	}
	return 0;
}

uint32_t getCurrentTime(){
	return HAL_GetTick()/2;
}
