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

void test_lineReset_should_generate_at_least_50clocks_with_setHighSWDIO()
{
	emulateLineReset(50);
	
	lineReset(10);
}

void test_lineReset_given_60_should_generate_at_60clocks_with_setHighSWDIO()
{
	emulateLineReset(60);
	
	lineReset(60);
}

void test_hardResetTarget_should_set_ResetPinLow_ResetPinHigh()
{
	emulatehardResetTarget();
	
	hardResetTarget();
}