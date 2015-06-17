#include "unity.h"
#include "mock_IO_Operations.h"
#include "Clock.h"
#include "Delay.h"
#include "Reset.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_lineReset_should_generate_at_least_50clocks_with_SWDIO_High()
{
	int i = 0 ;
	
	SWDIO_High_Expect();
	
	for ( i = 0 ; i < 50 ; i ++)
	{
		SWCLK_OFF_Expect();
		SWCLK_ON_Expect();
	}
	
	
	lineReset(10);
}

void test_lineReset_given_60_should_generate_at_60clocks_with_SWDIO_High()
{
	int i = 0 ;
	
	SWDIO_High_Expect();
	
	for ( i = 0 ; i < 60 ; i ++)
	{
		SWCLK_OFF_Expect();
		SWCLK_ON_Expect();
	}
	
	lineReset(60);
}

void test_ResetTarget_should_set_ResetPinLow_ResetPinHigh()
{
	ResetPin_Low_Expect();
	ResetPin_High_Expect();
	
	resetTarget();
}