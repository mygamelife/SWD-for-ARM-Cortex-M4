#ifndef LED_H
#define LED_H

#include "configurePort.h"

typedef enum{
	START,
	LED_BLINKING_ON,
	LED_BLINKING_OFF,
	ON_LED3,
	OFF_LED3,
	COUNTER,
  HALT,
	FINAL
}State;

uint32_t getCurrentTime();
int oneHundredMiliSecHasNotExpired();
void LED3_Blink(State *state, int *enable);
void blinkLED3(State *state, int *counter);
#endif // LED_H
