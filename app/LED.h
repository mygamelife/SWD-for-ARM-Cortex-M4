#ifndef LED_H
#define LED_H

#include "configurePort.h"

#define ONE_SEC                   1000
#define ONE_HUNDRED_MILI_SEC      100
#define FIVE_HUNDRED_MILI_SEC     500

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
void blinkingLED(int delayMs);
#endif // LED_H
