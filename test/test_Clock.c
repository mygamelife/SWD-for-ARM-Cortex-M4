#include "unity.h"
#include "Emulator.h"
#include "Clock.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"
void setUp(void)
{
}

void tearDown(void)
{
}

void test_clockGenerator_1cycle_should_turn_off_SWCLK_and_turn_on_SWCLK()
{
	SWCLK_OFF_Expect();
	SWCLK_ON_Expect();
	
	clockGenerator_1cycle();
}

void test_turnAround_ToRead_should_call_SWCLK_OFF()
{
	emulateTurnAroundRead();
	
	turnAround_ToRead();
}

void test_turnAround_ToWrite_should_call_SWCLK_ON_SWCLK_OFF_SWCLK_ON()
{
	emulateTurnAroundWrite();
	
	turnAround_ToWrite();
}


void test_extraIdleClock_given_1_clock_should_set_SWDIO_Low_and_turn_off_SWCLK_and_turn_on_SWCLK()
{
	emulateIdleClock(1);
	
	extraIdleClock(1);
}