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
#include "CoreDebugEx.h"
#include "CoreDebug.h"
#include "Yield.h"
#include "LoadElf.h"
#include "SystemPath.h"
#include "ProgramVerifier.h"

int initFlag = 0;

void setUp(void) {
  if(initFlag == 0) {
    initFlag = 1;
    initMemoryReadWrite();
  }
}

void tearDown(void) {}

void test_write_word_data_0xDEADBEEF_should_read_back_the_same_data(void) {
  int result = 0;
  uint32_t dataWrite = 0xDEADBEEF, dataRead = 0;

  result = memoryWrite(0x20010000, dataWrite, 4);
  
  TEST_ASSERT_EQUAL(1, result);
  
  result = memoryRead(0x20010000, &dataRead, 4);
  
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, dataRead);
}

void test_read_halfword_data_should_read_0x1234_and_0x5678(void) {
  int result = 0;
  uint32_t dataWrite = 0x12345678, dataRead = 0;

  result = memoryWrite(0x20010000, dataWrite, 4);
  TEST_ASSERT_EQUAL(1, result);
  
  result = memoryRead(0x20010000, &dataRead, 2);
  TEST_ASSERT_EQUAL_HEX16(0x5678, dataRead);
  
  result = memoryRead(0x20010002, &dataRead, 2);
  TEST_ASSERT_EQUAL_HEX16(0x1234, dataRead);
}

void test_read_byte_data_should_read_0x89_0xAB_0xCD_0xEF(void) {
  int result = 0;
  uint32_t dataWrite = 0x89ABCDEF, dataRead = 0;

  result = memoryWrite(0x20010000, dataWrite, 4);
  TEST_ASSERT_EQUAL(1, result);
  
  result = memoryRead(0x20010000, &dataRead, 1);
  TEST_ASSERT_EQUAL_HEX8(0xEF, dataRead);
  
  result = memoryRead(0x20010001, &dataRead, 1);
  TEST_ASSERT_EQUAL_HEX8(0xCD, dataRead);
  
  result = memoryRead(0x20010002, &dataRead, 1);
  TEST_ASSERT_EQUAL_HEX8(0xAB, dataRead);
  
  result = memoryRead(0x20010003, &dataRead, 1);
  TEST_ASSERT_EQUAL_HEX8(0x89, dataRead);
}

void test_write_word_0xABCD1234_should_read_back_the_same_data(void) {
  int result = 0;
  uint32_t dataRead;
  
  result = memoryWriteWord(0x20010000, 0xABCD1234);
  
  TEST_ASSERT_EQUAL(1, result);
  
  result = memoryReadWord(0x20010000, &dataRead);
  
  TEST_ASSERT_EQUAL_HEX32(0xABCD1234, dataRead);
}

void test_write_halfword_0xBEEF_should_read_back_the_same_data(void) {
  int result = 0;
  uint32_t dataRead;
  
  result = memoryWriteHalfword(0x20010000, 0xBEEF);
  TEST_ASSERT_EQUAL(1, result);
  
  result = memoryReadHalfword(0x20010000, &dataRead);
  TEST_ASSERT_EQUAL_HEX16(0xBEEF, dataRead);
}

void test_write_word_0xDEADBEEF_and_read_halfword_should_get_0xBEEF(void) {
  int result = 0;
  uint32_t dataRead;
  
  result = memoryWriteByte(0x20000000, 0xAD);
  result = memoryWriteByte(0x20000001, 0xDE);
  result = memoryWriteByte(0x20000002, 0x11);
  result = memoryWriteByte(0x20000003, 0x22);
  TEST_ASSERT_EQUAL(1, result);

  result = memoryReadHalfword(0x20000000, &dataRead);
  TEST_ASSERT_EQUAL_HEX8(0xAD, dataRead);
  result = memoryReadHalfword(0x20000001, &dataRead);
  TEST_ASSERT_EQUAL_HEX8(0xDE, dataRead);
  result = memoryReadHalfword(0x20000002, &dataRead);
  TEST_ASSERT_EQUAL_HEX8(0x11, dataRead);
  result = memoryReadHalfword(0x20000003, &dataRead);
  TEST_ASSERT_EQUAL_HEX8(0x22, dataRead);
}

void test_tlvWriteToFlash_write_0xDEADBEEF_into_0x8000000_and_should_read_back_the_same_value(void) {
  int result = 0;
  uint32_t dataRead = 0;
  
  uint8_t data[] = {0xef, 0xbe, 0xad, 0xde, 
                    0xfe, 0xca, 0xcd, 0xab};
                    
  result = _flashWrite(0x080E0000, data, sizeof(data));
  TEST_ASSERT_EQUAL(1, result);
    
  result = memoryReadWord(0x080E0000, &dataRead);
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, dataRead);
  
  result = memoryReadWord(0x080E0004, &dataRead);
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX32(0xABCDCAFE, dataRead);
}

void test_flashWrite_write_multiple_halfword_should_read_back_same_result(void) {
  int i, result = 0;
  uint32_t dataRead = 0;
  uint32_t address = 0x080003BE;
  
  uint16_t data[] = { 0xaaaa, 0xbbbb, 0xcccc, 0xdddd,
                      0xeeee, 0xffff, 0x1111, 0x2222,
                      0x3333, 0x4444, 0x5555, 0x6666
                    };
  
  result = _flashWrite(0x080003BE, (uint8_t *)data, sizeof(data));
  TEST_ASSERT_EQUAL(1, result);
  
  for(i = 0; i < 12; i++, address += 2) {
    result = memoryReadHalfword(address, &dataRead);
    TEST_ASSERT_EQUAL(1, result);
    TEST_ASSERT_EQUAL_HEX16(data[i], dataRead);
  }
}