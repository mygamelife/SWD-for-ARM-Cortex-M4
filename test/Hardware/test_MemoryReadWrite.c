#include "unity.h"
#include "Tlv.h"
#include "Tlv_ex.h"
#include "GetTime.h"
#include "MemoryReadWrite.h"
#include "CException.h"
#include "mock_uart.h"
#include "mock_ProgramLoader.h"

void setUp(void) {}

void tearDown(void) {}

void test_memoryReadWord_should_read_memory_and_return_data_in_word(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  int size = 1;
  uint32_t dataRead = 0, address = 0x20000000;
  uint8_t data[] = {0x00, 0x00, 0x00, 0x20, 0xEF, 0xBE, 0xAD, 0xDE};
  
  tlvReadTargetMemory_ExpectAndReturn(session, &address, &size, data);
	int result = memoryReadWord(session, address, &dataRead);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, dataRead);
}

void test_memoryWriteWord_should_write_memory_in_word_and_return_1_if_success(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  int size = 1;
  uint32_t writeData = 0xBEEFCAFE, address = 0x20000000;
  
  tlvWriteDataInWord_ExpectAndReturn(session, address, writeData, PROCESS_DONE);
	int result = memoryWriteWord(session, address, writeData);
  
  TEST_ASSERT_EQUAL(PROCESS_DONE, result);
}

void test_memoryWriteHalfWord_should_write_memory_in_word_and_return_1_if_success(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  int size = 1;
  uint32_t writeData = 0xBEEFCAFE, address = 0x20000000;
  
  tlvWriteDataInHalfWord_ExpectAndReturn(session, address, writeData, PROCESS_DONE);
	int result = memoryWriteHalfWord(session, address, writeData);
  
  TEST_ASSERT_EQUAL(PROCESS_DONE, result);
}

void test_memoryWriteByte_should_write_memory_in_word_and_return_1_if_success(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  int size = 1;
  uint32_t writeData = 0xBEEFCAFE, address = 0x20000000;
  
  tlvWriteDataInByte_ExpectAndReturn(session, address, writeData, PROCESS_DONE);
	int result = memoryWriteByte(session, address, writeData);
  
  TEST_ASSERT_EQUAL(PROCESS_DONE, result);
}