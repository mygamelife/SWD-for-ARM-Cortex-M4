#ifndef LED_H
#define LED_H

#include "configurePort.h"

typedef enum{
	START,
	PRESS,
	RELEASE,
	LED_ON,
	LED_OFF,
	LED_BLINKING_ON,
	LED_BLINKING_OFF,
	ON_LED3,
	OFF_LED3,
	BUTTON_STAT,
	COUNTER,
	HALT,
	CHECK_DELAY,
	FINAL
}State;

uint32_t getCurrentTime();
int oneHundredMiliSecHasNotExpired();
void LED3_Blink(State *state, int *pTimer);
#endif // LED_H
