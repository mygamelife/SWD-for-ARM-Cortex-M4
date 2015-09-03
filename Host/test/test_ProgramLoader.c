#include "Tlv.h"
#include "unity.h"
#include "Tlv_ex.h"
#include <malloc.h>
#include "Read_File.h"
#include "GetHeaders.h"
#include "ProgramElf.h"
#include "mock_uart.h"
#include "ProgramLoader.h"
#include "Tlv_ErrorCode.h"
#include "CException.h"
#include "CustomAssertion.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvWriteTargetRegister_should_send_register_address_and_data_to_probe_using_UART(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x12345678;
  uint32_t data = 0xDEADBEEF;
  
  session->writeRegisterState = SEND_PACKET;
  
  tlvWriteTargetRegister(session, &address, &data);
  
  TEST_ASSERT_EQUAL(WAIT_RESPONSE, session->writeRegisterState);
  TEST_ASSERT_EQUAL(TLV_WRITE_REGISTER, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
}

void test_tlvWriteTargetRegister_should_throw_exception_error_code_if_receive_NACK(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  CEXCEPTION_T err;
  
  Try {
    uint32_t address = 0x12345678;
    uint32_t data = 0xDEADBEEF;
    
    session->DATA_ARRIVE_FLAG = true;
    session->writeRegisterState = WAIT_RESPONSE;
    session->rxBuffer[0] = TLV_NOT_OK;
    session->rxBuffer[1] = 2;
    session->rxBuffer[2] = TLV_CORRUPTED_DATA;
  
    tlvWriteTargetRegister(session, &address, &data);
    printf("Should Throw TLV_CORRUPTED_DATA ERROR!");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(TLV_CORRUPTED_DATA, err);
  }
}

void test_tlvWriteTargetRegister_should_throw_invalid_command_if_type_is_unorganized(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  CEXCEPTION_T err;
  
  Try {
    uint32_t address = 0x12345678;
    uint32_t data = 0xDEADBEEF;
    
    session->DATA_ARRIVE_FLAG = true;
    session->writeRegisterState = WAIT_RESPONSE;
    session->rxBuffer[0] = 0xFF;
    session->rxBuffer[1] = 0;
  
    tlvWriteTargetRegister(session, &address, &data);
    printf("Should Throw TLV_INVALID_COMMAND ERROR!");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(TLV_INVALID_COMMAND, err);
  }
}

void test_tlvWriteTargetRegister_should_throw_checksum_error(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  CEXCEPTION_T err;
  
  Try {
    uint32_t address = 0x12345678;
    uint32_t data = 0xDEADBEEF;
    
    session->DATA_ARRIVE_FLAG = true;
    session->writeRegisterState = WAIT_RESPONSE;
    session->rxBuffer[0] = TLV_WRITE_REGISTER;
    session->rxBuffer[1] = 2;
    session->rxBuffer[2] = 0x12;
  
    tlvWriteTargetRegister(session, &address, &data);
    printf("Should Throw TLV_CHECKSUM_ERROR!");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(TLV_CHECKSUM_ERROR, err);
  }
}

void test_tlvReadTargetRegister_should_wait_response_after_send_packet(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x12345678;
  uint32_t buffer[] = {0x12345678};
  Tlv *tlv = tlvCreatePacket(TLV_READ_REGISTER, 4, (uint8_t *)buffer);
  session->readRegisterState = SEND_PACKET;
  
  tlvReadTargetRegister(session, &address);
  TEST_ASSERT_EQUAL(WAIT_RESPONSE, session->readRegisterState);
}

void test_tlvWriteDataChunk_should_send_data_in_chunk_with_specific_size(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x12345678;
  uint32_t data[] = {0x12345678, 0xDEADBEEF, 0xBEEFDEAD, 0xDEADDEAD};
  tlvWriteDataChunk(session, (uint8_t *)data, address, 20);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, 25, address, (uint8_t *)data, (Tlv *)session->txBuffer);	
}

void test_tlvWriteTargetRam_should_request_and_write_data_into_target_RAM(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x10000000;
  uint32_t data[] = {0x12345678, 0xDEADBEEF, 0xBEEFDEAD, 0xDEADDEAD};
  int size = 400;
  tlvWriteTargetRam(session, (uint8_t *)data, &address, &size);
  TEST_ASSERT_EQUAL_HEX32(0x100000F8, address);
  TEST_ASSERT_EQUAL(152, size);
}

void test_tlvWriteTargetRam_should_write_data_into_target_RAM_and_Update_dataAddress_destAddress_and_size(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x10000000;
  uint32_t data[] = {0x12345678, 0xDEADBEEF, 0xBEEFDEAD, 0xDEADDEAD};
  int size = 400;
  tlvWriteTargetRam(session, (uint8_t *)data, &address, &size);
  TEST_ASSERT_EQUAL_HEX32(0x100000F8, address);
  TEST_ASSERT_EQUAL(152, size);
  TEST_ASSERT_EQUAL(WAIT_RESPONSE, session->writeRAMState);
  
  session->DATA_ARRIVE_FLAG = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  tlvWriteTargetRam(session, (uint8_t *)data, &address, &size);
  TEST_ASSERT_EQUAL(SEND_PACKET, session->writeRAMState);
  
  tlvWriteTargetRam(session, (uint8_t *)data, &address, &size);
  TEST_ASSERT_EQUAL_HEX32(0x100001F0, address);
  TEST_ASSERT_EQUAL(-96, size);
}