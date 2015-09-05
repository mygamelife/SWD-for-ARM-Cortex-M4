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
#include "mock_Interface.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvWriteTargetRegister_should_send_register_address_and_data_to_probe_using_UART(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  tlvWriteTargetRegister(session, 0x12345678, 0xDEADBEEF);
  
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
}

void test_tlvWriteTargetRegister_should_send_register_address_0x0_and_data_0xDEADBEEF(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();

  tlvWriteTargetRegister(session, 0x0, 0xDEADBEEF);
  
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
}

void test_tlvReadTargetRegister_should_wait_response_after_send_packet(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  tlvReadTargetRegister(session, 0x12345678);
  
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
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
  
  uint32_t data[] = {0x12345678, 0xDEADBEEF, 0xBEEFDEAD, 0xDEADDEAD};
  uint32_t destAddress = 0x20000000;
  int size = 20;

  tlvWriteTargetRam(session, data, &destAddress, &size);
  
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, destAddress);
  TEST_ASSERT_EQUAL(-228, size);
  TEST_ASSERT_EQUAL(false, session->ONGOING_PROCESS_FLAG);
}

void test_tlvReadTargetRam_should_request_tlv_read_register_with_address_0x12345678_and_size_255(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();

  tlvReadTargetRam(session, 0x12345678, 255);

  TEST_ASSERT_EQUAL(false, session->ONGOING_PROCESS_FLAG);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xFF, get4Byte(&session->txBuffer[6]));
}

void test_hostInterpreter_given_write_register_command_should_execute_and_wait_respond(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data[] = {0xABCDABCD, 0xBEEFCAFE};
  Tlv *command = tlvCreatePacket(TLV_WRITE_REGISTER, 8, (uint8_t *)data);
  
  waitUserCommand_ExpectAndReturn(command);
  session->userCommand = command;

  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  session->DATA_RECEIVE_FLAG = 1;
  
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
}

void test_hostInterpreter_with_tlvService_by_requesting_tlv_write_register(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data[] = {0xABCDABCD, 0xBEEFCAFE};
  Tlv *command = tlvCreatePacket(TLV_WRITE_REGISTER, 8, (uint8_t *)data);
  
  /* #### mocking function ##### */
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive
  
  tlvService(session);
  /* #### mocking function ##### */
  waitUserCommand_ExpectAndReturn(command);
  
  session->userCommand = command;
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  
  /* #### mocking function ##### */
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive
  
  tlvService(session);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);

  /* #### mocking function ##### */
  sendBytes_ExpectAndReturn(session->handler, session->txBuffer, 11, 0x00); //send request packet
  
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  /* #### mocking function ##### */
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x00); //data arrive
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 1, 0x00); //receive tlv response
  
  tlvService(session);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
}

void test_hostInterpreter_with_tlvService_by_requesting_tlv_read_register(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data[] = {0xDEAFCAFE};
  Tlv *command = tlvCreatePacket(TLV_READ_REGISTER, 4, (uint8_t *)data);

  /* #### mocking function ##### */
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive
  
  tlvService(session);
  /* #### mocking function ##### */
  waitUserCommand_ExpectAndReturn(command);
  
  session->userCommand = command;
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  
  /* #### mocking function ##### */
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive
  
  tlvService(session);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);

  /* #### mocking function ##### */
  sendBytes_ExpectAndReturn(session->handler, session->txBuffer, 7, 0x00); //send request packet
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 5;
  session->rxBuffer[2] = 0xDD;
  session->rxBuffer[3] = 0xCC;
  session->rxBuffer[4] = 0xBB;
  session->rxBuffer[5] = 0xAA;
  session->rxBuffer[6] = 0xF2;
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x00); //data arrive
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 5, 0x00); //receive tlv response
  
  tlvService(session);

  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
}

void test_hostInterpreter_with_tlvService_by_requesting_tlv_write_ram(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data[] = {0x20000000, 12, 0x11111111, 0x22222222, 0x33333333};

  Tlv *command = tlvCreatePacket(TLV_WRITE_RAM, 20, (uint8_t *)data);

  /* #### mocking function ##### */
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive
  
  tlvService(session);
  /* #### mocking function ##### */
  waitUserCommand_ExpectAndReturn(command);
  session->userCommand = command;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  
  /* #### mocking function ##### */
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive
  
  tlvService(session);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);

  /* #### mocking function ##### */
  sendBytes_ExpectAndReturn(session->handler, session->txBuffer, 19, 0x00); //send request packet
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x00); //data arrive
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 1, 0x00); //receive tlv response
  
  tlvService(session);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);  
}