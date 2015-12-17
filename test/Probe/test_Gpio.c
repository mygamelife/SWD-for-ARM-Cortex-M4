#include "unity.h"
#include "Gpio.h"
#include "mock_Rcc.h"
#include "mock_MemoryReadWrite.h"

void setUp(void) {}

void tearDown(void) {}

void test_gpioSetAltFunction_should_set_AFRL_if_pin_is_2(void)
{
  GPIO portB;
  portB.AFRL = 0;
  
  memoryWriteWord_ExpectAndReturn((uint32_t)&portB.AFRL, AF1 << (PIN_2 * 4), 0);
  gpioSetAltFunction(&portB, PIN_2, AF1);
}

void test_gpioSetAltFunction_should_set_AFRL_if_pin_is_7(void)
{
  GPIO portB;
  portB.AFRL = 0;
  
  memoryWriteWord_ExpectAndReturn((uint32_t)&portB.AFRL, AF2 << (PIN_7 * 4), 0);
  gpioSetAltFunction(&portB, PIN_7, AF2);
}

void test_gpioSetAltFunction_should_set_AFRL_if_pin_is_8(void)
{
  GPIO portB;
  portB.AFRL = 0;
  
  memoryWriteWord_ExpectAndReturn((uint32_t)&portB.AFRH, (AF2 << ((PIN_8 - 8) * 4)), 0);
  gpioSetAltFunction(&portB, PIN_8, AF2);
}