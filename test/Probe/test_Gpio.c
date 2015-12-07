#include "unity.h"
#include "Gpio.h"
#include "mock_Rcc.h"

void setUp(void) {}

void tearDown(void) {}

void test_gpioSetAltFunction_should_set_AFRL_if_pin_is_2(void)
{
  GPIO portB;
  portB.AFRL = 0;
  
  gpioSetAltFunction(&portB, PIN_2, AF1);
  
  TEST_ASSERT_EQUAL_HEX32(0x100, portB.AFRL);
}

void test_gpioSetAltFunction_should_set_AFRL_if_pin_is_7(void)
{
  GPIO portB;
  portB.AFRL = 0;
  
  gpioSetAltFunction(&portB, PIN_7, AF2);
  
  TEST_ASSERT_EQUAL_HEX32(0x20000000, portB.AFRL);
}

void test_gpioSetAltFunction_should_set_AFRL_if_pin_is_7(void)
{
  GPIO portB;
  portB.AFRL = 0;
  
  gpioSetAltFunction(&portB, PIN_7, AF2);
  
  TEST_ASSERT_EQUAL_HEX32(0x20000000, portB.AFRL);
}