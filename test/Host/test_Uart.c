#include "unity.h"
#include "Uart.h"
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdint.h>
#include "CException.h"

void setUp(void) {}

void tearDown(void) {}

void test_createHandler(void)
{
  CEXCEPTION_T err;
  void *handler;
  
  // Try {
    // uartInit(&handler);
  // } Catch (err) {
    // printf("err %d\n", err);
  // }
	TEST_ASSERT_NOT_NULL(createHandler());
}
