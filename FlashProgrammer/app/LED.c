#include "LED.h"

void blinkLED3(State *state, int *counter)	{
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
			*counter -= 1;
			if(*counter == 0)	{
				*state = HALT;
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
