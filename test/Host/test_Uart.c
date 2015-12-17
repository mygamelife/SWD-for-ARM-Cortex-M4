#include "unity.h"
#include "Uart.h"
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdint.h>
#include "CException.h"
#include "ErrorCode.h"

void setUp(void) {}

void tearDown(void) {}

void test_openComPort_should_return_NULL(void)
{ 
  TEST_ASSERT_NULL(openComPort("COM20", UART_BAUD_RATE));
}

void test_uartInit_should_throw_error_if_no_COMPORT_is_found_else_hSerial_shouldnt_null(void)
{
  CEXCEPTION_T err;
  void *hSerial = NULL;
  
  Try {
    uartInit(&hSerial);
    TEST_ASSERT_NOT_NULL(hSerial);
  } Catch(err) {
    displayErrorMessage(err);
    TEST_ASSERT_NULL(hSerial);
  }
}