#include "unity.h"
#include "mock_Tlv.h"
#include "TlvEx.h"
#include "SystemTime.h"
#include "MemoryReadWrite.h"
#include "CException.h"
#include "mock_Uart.h"
#include "mock_ProgramLoader.h"

void setUp(void) {}

void tearDown(void) {}

void test_memoryReadWord_should_continute_wait_until_data_is_return(void)
{
  int size = WORD_SIZE;
  uint32_t dataRead = 0, address = 0x20000000;
  uint8_t data[] = {0xEF, 0xBE, 0xAD, 0xDE};
  Storage s;
  
  s.data = data;
  
  readMemory_ExpectAndReturn(_session, address, size, 0);
  tlvService_Expect(_session);
  readMemory_ExpectAndReturn(_session, address, size, &s);
  
	int result = memoryRead(address, &dataRead, WORD_SIZE);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, dataRead);
}

void test_memoryReadWord_should_read_memory_and_return_data_in_word(void)
{
  int size = WORD_SIZE;
  uint32_t dataRead = 0, address = 0x20000000;
  uint8_t data[] = {0xEF, 0xBE, 0xAD, 0xDE};
  Storage s;
  
  s.data = data;
  
  readMemory_ExpectAndReturn(_session, address, size, &s);
  
	int result = memoryRead(address, &dataRead, WORD_SIZE);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, dataRead);
}

void test_memoryReadHalfword_should_read_memory_and_return_data_in_halfword(void)
{
  int size = HALFWORD_SIZE;
  uint32_t address = 0x20000000, dataRead = 0;
  uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
  Storage s;
  
  s.data = data;
  
  readMemory_ExpectAndReturn(_session, address, size, &s);
  
	int result = memoryRead(address, &dataRead, HALFWORD_SIZE);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX32(0xBBAA, dataRead);
}

void test_memoryReadByte_should_read_memory_and_return_data_in_byte(void)
{
  int size = BYTE_SIZE;
  uint32_t address = 0x20000000, dataRead = 0;
  uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
  Storage s;
  
  s.data = data;
  
  readMemory_ExpectAndReturn(_session, address, size, &s);
  
	int result = memoryRead(address, &dataRead, BYTE_SIZE);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL_HEX32(0xAA, dataRead);
}

void test_memoryWriteWord_should_write_memory_in_word_and_return_1_if_success(void)
{
  int size = WORD_SIZE;
  uint32_t writeData = 0xBEEFCAFE, address = 0x20000000;

  writeMemory_IgnoreAndReturn(PROCESS_DONE);
	int result = memoryWrite(address, writeData, WORD_SIZE);
  
  TEST_ASSERT_EQUAL(PROCESS_DONE, result);
}

// void test_memoryWriteHalfWord_should_write_memory_in_word_and_return_1_if_success(void)
// {
  // int size = HALFWORD_SIZE;
  // uint32_t writeData = 0xCAFE, address = 0x20000000;
  
  // uint8_t *p = (uint8_t *)&writeData;
  // writeMemory_ExpectAndReturn(_session, (uint8_t *)&writeData, &address, &size, TLV_WRITE_RAM, PROCESS_DONE);
	// int result = memoryWrite(address, writeData, HALFWORD_SIZE);
  
  // TEST_ASSERT_EQUAL(PROCESS_DONE, result);
// }

// void test_memoryWriteByte_should_write_memory_in_word_and_return_1_if_success(void)
// {
  // int size = BYTE_SIZE;
  // uint32_t writeData = 0xBE, address = 0x20000000;
  
  // uint8_t *p = (uint8_t *)&writeData;
  // writeMemory_ExpectAndReturn(_session, (uint8_t *)&writeData, &address, &size, TLV_WRITE_RAM, 0);
  // tlvService_Expect(_session);
  // writeMemory_ExpectAndReturn(_session, (uint8_t *)&writeData, &address, &size, TLV_WRITE_RAM, PROCESS_DONE);
  
	// int result = memoryWrite(address, writeData, BYTE_SIZE);
  
  // TEST_ASSERT_EQUAL(PROCESS_DONE, result);
// }

// void test_flashWrite_should_write_data_into_specified_flash_address(void)
// {
  // int size = BYTE_SIZE;
  // uint32_t writeData = 0xBEAFBABA, address = 0x8000000;
  
  // uint8_t *p = (uint8_t *)&writeData;
  // writeMemory_ExpectAndReturn(_session, (uint8_t *)&writeData, &address, &size, TLV_WRITE_FLASH, 0);
  // tlvService_Expect(_session);
  // writeMemory_ExpectAndReturn(_session, (uint8_t *)&writeData, &address, &size, TLV_WRITE_FLASH, PROCESS_DONE);

	// int result = _flashWrite(address, writeData, BYTE_SIZE);
  
  // TEST_ASSERT_EQUAL(PROCESS_DONE, result);
// }

// void test_flashErase_should_erase_according_to_size_and_address(void)
// {
  // int size = 20000;
  // uint32_t address = 0x8000000;
  
  // eraseSection_ExpectAndReturn(_session, address, size, 0);
  // tlvService_Expect(_session);
  // eraseSection_ExpectAndReturn(_session, address, size, PROCESS_DONE);

	// int result = _flashErase(address, size);
  
  // TEST_ASSERT_EQUAL(PROCESS_DONE, result);
// }