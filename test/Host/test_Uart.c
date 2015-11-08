#include "unity.h"
#include "Uart.h"
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdint.h>
#include "CException.h"

void setUp(void) {}

void tearDown(void) {}

void xtest_getAvailableComPort_should_return_not_null_handler(void)
{
  void *hSerial;
  
  getAvailableComPort(&hSerial);
  uartConfig((HANDLE)hSerial);
  
  TEST_ASSERT_NOT_NULL(hSerial);
  
  CloseHandle((HANDLE)hSerial);
}

void test_uartInit(void)
{
  void *hSerial;
  
  uartInit(&hSerial);
}