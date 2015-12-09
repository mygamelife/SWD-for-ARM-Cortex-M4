#include "unity.h"
#include "Yield.h"
#include "Tlv.h"
#include "TlvEx.h"
#include "Uart.h"
#include "SystemTime.h"
#include "ErrorCode.h"
#include "Read_File.h"
#include "GetHeaders.h"
#include "Interface.h"
#include "ProgramElf.h"
#include "ProgramLoader.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "StringObject.h"
#include "Token.h"
#include "IdentifierToken.h"
#include "NumberToken.h"
#include "OperatorToken.h"
#include "FileToken.h"
#include "LoadElf.h"
#include "ProgramVerifier.h"
#include "MemoryReadWrite.h"
#include "Gpio.h"
#include "Rcc.h"

static int flag = 0;

void setUp(void) {
  if(flag == 0) {
    flag = 1;
    initMemoryReadWrite();
  }
}

void tearDown(void) {}

void test_gpioConfigPort_should_configure_port_alternate_function_register(void) {
  uint32_t result = 0;
  
  GPIO_Init init;
  
  init.mode = ALTFUNC_MODE;
  init.pin = PIN_2;
  init.af = AF0;
  
  gpioConfigPort(PORTE, &init);
  
  /* PORTE clock should enable */
  memoryReadWord((uint32_t)&rcc->RCC_AHB1ENR, &result);
  TEST_ASSERT_EQUAL_HEX32(0x10, result);
  /* Get GPIO Mode */
  memoryReadWord((uint32_t)&PORTE->MODER, &result);
  TEST_ASSERT_EQUAL_HEX32(0x20, result);
  /* Get GPIO Output Mode */
  memoryReadWord((uint32_t)&PORTE->OTYPER, &result);
  TEST_ASSERT_EQUAL_HEX32(0xFFFB, result);
  /* Get GPIO Speed */
  memoryReadWord((uint32_t)&PORTE->OSPEED, &result);
  TEST_ASSERT_EQUAL_HEX32(0x30, result);
  
  /* Get GPIO PE2 Alternate Function Register */
  memoryReadWord((uint32_t)&PORTE->AFRL, &result);
  TEST_ASSERT_EQUAL_HEX32(0x0, result);
}