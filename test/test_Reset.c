#include "unity.h"
#include "Emulator.h"
#include "Clock.h"
#include "Reset.h"
#include "Delay.h"
#include "swd_Utilities.h"
#include "Register_ReadWrite.h"
#include "Bit_Operations.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"


void setUp(void)
{
}

void tearDown(void)
{
}

void test_lineReset_should_generate_at_least_50clocks_with_SWDIO_High()
{
	emulateLineReset(50);
	
	lineReset(10);
}

void test_lineReset_given_60_should_generate_at_60clocks_with_SWDIO_High()
{
	emulateLineReset(60);
	
	lineReset(60);
}

void test_ResetTarget_should_set_ResetPinLow_ResetPinHigh()
{
	emulateResetTarget();
	
	resetTarget();
}