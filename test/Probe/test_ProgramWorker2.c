#include "unity.h"
#include "Tlv.h"
#include "TlvEx.h"
#include "ProgramWorker.h"
#include "ErrorCode.h"
#include "CoreDebugEx.h"
#include "mock_IoOperations.h"
#include "mock_Uart.h"
#include "mock_CoreDebug.h"
#include "mock_FPBUnit.h"
#include "mock_DWTUnit.h"
#include "mock_stm32f4xx_hal_uart.h"
#include "mock_MemoryReadWrite.h"
#include "mock_SwdStub.h"
#include "mock_CodeStepping.h"
#include "mock_SystemTime.h"

void setUp(void)  {}

void tearDown(void) {}

void test_getDataType_should_return_word_if_word_address_and_size_is_larger_than_4(void)
{ 
  TEST_ASSERT_EQUAL(WORD_TYPE, getDataType(0x20000000, 4));
  TEST_ASSERT_EQUAL(WORD_TYPE, getDataType(0x20000000, 8));
}

void test_getDataType_should_return_halfword_if_word_address_and_size_is_less_than_4(void)
{ 
  TEST_ASSERT_EQUAL(HALFWORD_TYPE, getDataType(0x20000000, 3));
}

void test_getDataType_should_return_byte_if_word_address_and_size_is_less_than_2(void)
{ 
  TEST_ASSERT_EQUAL(BYTE_TYPE, getDataType(0x20000000, 1));
}

void test_getDataType_should_return_halfword_if_word_address_and_size_is_larger_than_4(void)
{ 
  TEST_ASSERT_EQUAL(HALFWORD_TYPE, getDataType(0x20000002, 12));
}

void test_getDataType_should_return_halfword_if_halfword_address_and_size_is_less_than_4(void)
{ 
  TEST_ASSERT_EQUAL(HALFWORD_TYPE, getDataType(0x20000002, 3));
}

void test_getDataType_should_return_halfword_if_halfword_address_and_size_is_2(void)
{ 
  TEST_ASSERT_EQUAL(HALFWORD_TYPE, getDataType(0x20000002, 2));
}

void test_getDataType_should_return_halfword_if_halfword_address_and_size_is_less_than_2(void)
{ 
  TEST_ASSERT_EQUAL(BYTE_TYPE, getDataType(0x20000002, 1));
}

void test_getDataType_should_return_byte_if_byte_address(void)
{ 
  TEST_ASSERT_EQUAL(BYTE_TYPE, getDataType(0x20000003, 1));
  TEST_ASSERT_EQUAL(BYTE_TYPE, getDataType(0x20000005, 10));
}

void test_writeDataWithCorrectDataType_should_write_0x12345678_to_word_address(void)
{ 
  int size = 5;
  uint32_t data[] = {0x12345678, 0xAB}, address = 0x20000000;
  uint8_t *dataPtr = (uint8_t *)data;
  
  memoryWriteWord_ExpectAndReturn(0x20000000, 0x12345678, SWD_NO_ERROR);
  
  writeDataWithCorrectDataType(&dataPtr, &address, &size);
  
  TEST_ASSERT_EQUAL_HEX8(0xAB, *dataPtr);
  TEST_ASSERT_EQUAL(0x20000004, address);
  TEST_ASSERT_EQUAL(1, size);
}

void test_writeDataWithCorrectDataType_should_write_0xBEEF_into_halfword_address(void)
{ 
  int size = 3;
  uint32_t data[] = {0xDEADBEEF}, address = 0x20000002;
  uint8_t *dataPtr = (uint8_t *)data;
  
  memoryWriteHalfword_ExpectAndReturn(0x20000002, 0xBEEF, SWD_NO_ERROR);
  
  writeDataWithCorrectDataType(&dataPtr, &address, &size);
  
  TEST_ASSERT_EQUAL_HEX8(0xAD, *dataPtr);
  TEST_ASSERT_EQUAL(0x20000004, address);
  TEST_ASSERT_EQUAL(1, size);
}

void test_writeTargetRam_should_write_correctly_when_address_start_at_4_byte_boundary(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  uint8_t buffer[] = {  0x00, 0x00, 0x00, 0x20,   //address 0x20000000
                        0x34, 0x12,               //0x1234
                        0xEF, 0xBE, 0xAD, 0xDE,   //0xDEADBEEF
                        0x99,                     //0x99
                        0xCD, 0xAB                //0xABCD
                     }; 
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, 13, buffer);
  
  memoryWriteWord_ExpectAndReturn(0x20000000, 0xBEEF1234, SWD_NO_ERROR);
  memoryWriteWord_ExpectAndReturn(0x20000004, 0xCD99DEAD, SWD_NO_ERROR);
  memoryWriteByte_ExpectAndReturn(0x20000008, 0xAB, SWD_NO_ERROR);
  
  writeTargetRam(session, &tlv->value[4], get4Byte(&tlv->value[0]), tlv->length - 5);
}

void test_writeTargetRam_should_write_correctly_when_address_start_at_2_byte_boundary(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  uint8_t buffer[] = {  0x02, 0x00, 0x00, 0x20,   //0x20000002
                        0xEF, 0xBE, 0xAD, 0xDE,   //0xDEADBEEF
                        0x78, 0x56, 0x34, 0x12,   //0x12345678
                        0x99, 0x88,               //0x8899
                        0x77, 0x66                //0x6677
                     }; 
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, 16, buffer);
  
  memoryWriteHalfword_ExpectAndReturn(0x20000002, 0xBEEF, SWD_NO_ERROR);
  memoryWriteWord_ExpectAndReturn(0x20000004, 0x5678DEAD, SWD_NO_ERROR);
  memoryWriteWord_ExpectAndReturn(0x20000008, 0x88991234, SWD_NO_ERROR);
  memoryWriteHalfword_ExpectAndReturn(0x2000000C, 0x6677, SWD_NO_ERROR);
  
  writeTargetRam(session, &tlv->value[4], get4Byte(&tlv->value[0]), tlv->length - 5);
}

void test_writeTargetRam_should_write_correctly_when_address_start_at_1_byte_boundary(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  uint8_t buffer[] = {  0x05, 0x00, 0x00, 0x20,   //0x20000002
                        0xEF, 0xBE, 0xAD, 0xDE,   //0xDEADBEEF
                        0x78, 0x56, 0x34, 0x12,   //0x12345678
                        0xFE, 0xCA, 0xAF, 0xDE,   //0xDEAFCAFE
                        0xAA                      //0xAA
                     }; 
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, 17, buffer);
  
  memoryWriteByte_ExpectAndReturn(0x20000005, 0xEF, SWD_NO_ERROR);
  memoryWriteHalfword_ExpectAndReturn(0x20000006, 0xADBE, SWD_NO_ERROR);
  memoryWriteWord_ExpectAndReturn(0x20000008, 0x345678DE, SWD_NO_ERROR);
  memoryWriteWord_ExpectAndReturn(0x2000000C, 0xAFCAFE12, SWD_NO_ERROR);
  memoryWriteHalfword_ExpectAndReturn(0x20000010, 0xAADE, SWD_NO_ERROR);
  
  writeTargetRam(session, &tlv->value[4], get4Byte(&tlv->value[0]), tlv->length - 5);
}