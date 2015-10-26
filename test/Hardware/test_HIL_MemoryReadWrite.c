#include "unity.h"
#include "Tlv.h"
#include "Uart.h"
#include "TlvEx.h"
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
#include "MemoryReadWrite.h"

int initFlag = 0;

void setUp(void) {
  if(initFlag == 0) {
    initFlag = 1;
    initMemoryReadWrite();
  }
}

void tearDown(void) {}

// void test_write_word_0xABCD1234_should_read_back_the_same_data(void) {
  // int result = 0;
  // uint32_t dataRead;
  
  // result = memoryWriteWord(0x20010000, 0xABCD1234);
  
  // TEST_ASSERT_EQUAL(1, result);
  
  // result = memoryReadWord(0x20010000, &dataRead);
  
  // TEST_ASSERT_EQUAL_HEX32(0xABCD1234, dataRead);
// }

// void test_write_halfword_0xBEEF_should_read_back_the_same_data(void) {
  // int result = 0;
  // uint32_t dataRead;
  
  // result = memoryWriteHalfword(0x20000000, 0xBEEF);
  // TEST_ASSERT_EQUAL(1, result);
  
  // result = memoryReadHalfword(0x20000000, &dataRead);
  // TEST_ASSERT_EQUAL_HEX16(0xBEEF, dataRead);
// }

// void test_write_word_0xDEADBEEF_and_read_halfword_should_get_0xBEEF(void) {
  // int result = 0;
  // uint32_t dataRead;
  
  // result = memoryWriteWord(0x20000000, 0xDEADBEEF);
  // TEST_ASSERT_EQUAL(1, result);

  // result = memoryReadHalfword(0x20000000, &dataRead);
  // TEST_ASSERT_EQUAL_HEX16(0xBEEF, dataRead);
// }

void test_tlvWriteToFlash_write_0xDEADBEEF_into_0x8000000_and_should_read_back_the_same_value(void) {
  int result = 0;
  uint32_t dataRead = 0;
  
  // result = _flashWriteWord(0x8000000, 0xDEADBEEF);
  result = _flashWriteWord(0x8000004, 0x1234);
  TEST_ASSERT_EQUAL(1, result);
  result = _flashWriteWord(0x8000006, 0x5678);
  TEST_ASSERT_EQUAL(1, result);

  // result = memoryReadWord(0x8000004, &dataRead);
  // TEST_ASSERT_EQUAL(1, result);
  // TEST_ASSERT_EQUAL_HEX32(0x1234, dataRead);
}