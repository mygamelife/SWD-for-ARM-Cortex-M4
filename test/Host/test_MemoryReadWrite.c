#include "unity.h"
#include "Tlv.h"
#include "TlvEx.h"
#include "SystemTime.h"
#include "MemoryReadWrite.h"
#include "CException.h"
#include "mock_Uart.h"
#include "mock_ProgramLoader.h"

void setUp(void) {
  uartInit_Ignore();
	initMemoryReadWrite();
}

void tearDown(void) {}

void test_memoryReadWord_should_read_memory_and_return_data_in_word(void)
{
  uint32_t dataRead = 0, address = 0x20000000;
  uint8_t data[] = {0x00, 0x00, 0x00, 0x20, 0xEF, 0xBE, 0xAD, 0xDE};
  
  tlvReadTargetMemory_IgnoreAndReturn(data);
	int result = memoryReadWord(address, &dataRead);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, dataRead);
}

void test_memoryReadHalfword_should_read_memory_and_return_data_in_halfword(void)
{
  uint16_t dataRead = 0;
  uint32_t address = 0x20000000;
  uint8_t data[] = {0x00, 0x00, 0x00, 0x20, 0xCD, 0xAB, 0x34, 0x12};
  
  tlvReadTargetMemory_IgnoreAndReturn(data);
	int result = memoryReadHalfword(address, &dataRead);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX16(0xABCD, dataRead);
}

void test_memoryWriteWord_should_write_memory_in_word_and_return_1_if_success(void)
{
  uint32_t writeData = 0xBEEFCAFE, address = 0x20000000;
  
  tlvWriteDataInWord_IgnoreAndReturn(PROCESS_DONE);
	int result = memoryWriteWord(address, writeData);
  
  TEST_ASSERT_EQUAL(PROCESS_DONE, result);
}

void test_memoryWriteHalfWord_should_write_memory_in_word_and_return_1_if_success(void)
{
  uint32_t writeData = 0xBEEFCAFE, address = 0x20000000;
  
  tlvWriteDataInHalfWord_IgnoreAndReturn(PROCESS_DONE);
	int result = memoryWriteHalfWord(address, writeData);
  
  TEST_ASSERT_EQUAL(PROCESS_DONE, result);
}

void test_memoryWriteByte_should_write_memory_in_word_and_return_1_if_success(void)
{
  uint32_t writeData = 0xBEEFCAFE, address = 0x20000000;
  
  tlvWriteDataInByte_IgnoreAndReturn(PROCESS_DONE);
	int result = memoryWriteByte(address, writeData);
  
  TEST_ASSERT_EQUAL(PROCESS_DONE, result);
}