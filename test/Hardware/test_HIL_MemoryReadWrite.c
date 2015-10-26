#include "unity.h"
#include "Tlv.h"
#include "uart.h"
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

void setUp(void) {
  initMemoryReadWrite();
}

void tearDown(void) {}

void test_write_word_0xABCD1234_should_read_back_the_same_data(void) {
  int result = 0;
  uint32_t dataRead;
  
  result = memoryWriteWord(0x20000000, 0xABCD1234);
  
  TEST_ASSERT_EQUAL(1, result);
  
  result = memoryReadWord(0x20000000, &dataRead);
  
  TEST_ASSERT_EQUAL_HEX32(0xABCD1234, dataRead);
}

void test_write_halfword_0xBEEF_should_read_back_the_same_data(void) {
  int result = 0;
  uint32_t dataRead;
  
  result = memoryWriteHalfword(0x20000000, 0xBEEF);
  TEST_ASSERT_EQUAL(1, result);
  
  result = memoryReadHalfword(0x20000000, &dataRead);
  TEST_ASSERT_EQUAL_HEX16(0xBEEF, dataRead);
}

void test_write_word_0xDEADBEEF_and_read_halfword_should_get_0xBEEF(void) {
  int result = 0;
  uint32_t dataRead;
  
  result = memoryWriteWord(0x20000000, 0xDEADBEEF);
  TEST_ASSERT_EQUAL(1, result);

  result = memoryReadHalfword(0x20000000, &dataRead);
  TEST_ASSERT_EQUAL_HEX16(0xBEEF, dataRead);
}
