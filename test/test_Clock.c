#include "unity.h"
#include "Emulator.h"
#include "IoOperations.h"
#include "Delay.h"
#include "swd_Utilities.h"
#include "Register_ReadWrite.h"
#include "mock_LowLevelIO.h"
#include "mock_configurePort.h"
void setUp(void)
{
}

void tearDown(void)
{
}

void test_generateOneClockCycle_should_turn_off_SWCLK_and_turn_on_SWCLK()
{
	setLowSWCLK_Expect();
	setHighSWCLK_Expect();
	
	generateOneClockCycle();
}

void test_turnAroundRead_should_call_setLowSWCLK()
{
	emulateTurnAroundRead();
	
	turnAroundRead();
}

void test_turnAroundWrite_should_call_setHighSWCLK_setLowSWCLK_setHighSWCLK()
{
	emulateTurnAroundWrite();
	
	turnAroundWrite();
}


void test_extraIdleClock_given_1_clock_should_set_setLowSWDIO_and_turn_off_SWCLK_and_turn_on_SWCLK()
{
	emulateIdleClock(1);
	
	extraIdleClock(1);
}