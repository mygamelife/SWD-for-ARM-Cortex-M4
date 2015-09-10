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

void test_tlvWriteTargetRegister_should_send_register_address_0x12345678_and_data_0xDEADBEEF(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data = 0xDEADBEEF;
  tlvWriteTargetRegister(session, 0x12345678, &data);
  
  TEST_ASSERT_EQUAL(true, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->ongoingProcessFlag);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
}

void test_tlvReadTargetRegister_should_wait_response_after_send_packet(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  tlvReadTargetRegister(session, 0x88888888);
  
  TEST_ASSERT_EQUAL(true, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->ongoingProcessFlag);
  TEST_ASSERT_EQUAL_HEX32(0x88888888, get4Byte(&session->txBuffer[2]));
}

void test_tlvWriteDataChunk_should_send_data_in_chunk_with_correct_chksum(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x11111111;
  uint32_t data[] = {0xDEADBEEF};
  tlvWriteDataChunk(session, (uint8_t *)data, address, 4);
  
  TEST_ASSERT_EQUAL_HEX8(TLV_WRITE_RAM, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX8(0x84, session->txBuffer[10]); //chksum
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
  
  User_Session userSession;
  userSession.data = data;
  userSession.address = 0x20000000;
  userSession.size = 20;

  uint8_t *pData = (uint8_t *)userSession.data;
  tlvWriteTargetRam(session, &pData, &userSession.address, &userSession.size);
  
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, userSession.address);
  TEST_ASSERT_EQUAL(-228, userSession.size);
  TEST_ASSERT_EQUAL(false, session->ongoingProcessFlag);
}

void test_tlvWriteTargetRam_write_isr_vector_section_into_RAM(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  ElfData *elfData = openElfFile("test/ELF_File/blinkLed.elf");
  int index = getIndexOfSectionByName(elfData, ".isr_vector");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  int size = getSectionSize(elfData, index);

  tlvWriteTargetRam(session, &dataAddress, &destAddress, &size);
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, destAddress);
  TEST_ASSERT_EQUAL(180, size);
  TEST_ASSERT_EQUAL(true, session->ongoingProcessFlag);
}
  
void test_tlvWriteTargetRam_should_stop_request_when_size_is_0(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  uint32_t data[] = {0x12345678, 0xDEADBEEF, 0xBEEFDEAD, 0xDEADDEAD};
  
  User_Session userSession;
  userSession.data = data;
  userSession.address = 0x20000000;
  userSession.size = 500;

  uint8_t *pData = (uint8_t *)userSession.data;
  tlvWriteTargetRam(session, &pData, &userSession.address, &userSession.size);
  
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, userSession.address);
  TEST_ASSERT_EQUAL(252, userSession.size);
  TEST_ASSERT_EQUAL(true, session->ongoingProcessFlag);
  
  tlvWriteTargetRam(session, &pData, &userSession.address, &userSession.size);
  
  TEST_ASSERT_EQUAL_HEX32(0x200001F0, userSession.address);
  TEST_ASSERT_EQUAL(4, userSession.size);
  TEST_ASSERT_EQUAL(true, session->ongoingProcessFlag);
  
  tlvWriteTargetRam(session, &pData, &userSession.address, &userSession.size);
  
  TEST_ASSERT_EQUAL_HEX32(0x200002E8, userSession.address);
  TEST_ASSERT_EQUAL(-244, userSession.size);
  TEST_ASSERT_EQUAL(false, session->ongoingProcessFlag);
}

void test_tlvReadDataChunk_should_send_request_read_data_in_chunk(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  tlvReadDataChunk(session, 0x20001000, 255);
  
  TEST_ASSERT_EQUAL_HEX32(0x20001000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xFF, get4Byte(&session->txBuffer[6]));
}

void test_tlvReadTargetRam_should_stop_request_tlv_read_register_when_size_is_0(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();

  uint32_t address = 0x20000000;
  int size = 400;
  
  tlvReadTargetRam(session, &address, &size);

  TEST_ASSERT_EQUAL(true, session->ongoingProcessFlag);
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, address);
  TEST_ASSERT_EQUAL(152, size);
  
  tlvReadTargetRam(session, &address, &size);

  TEST_ASSERT_EQUAL(false, session->ongoingProcessFlag);
  TEST_ASSERT_EQUAL_HEX32(0x200001F0, address);
  TEST_ASSERT_EQUAL(-96, size);
}

void test_tlvLoadProgramToRam_should_open_file_and_load_isr_vector(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  tlvLoadProgramToRam(session, "test/ELF_File/blinkLed.elf");
  tlvLoadProgramToRam(session, "test/ELF_File/blinkLed.elf");
  TEST_ASSERT_EQUAL(TLV_LOAD_RO_DATA, session->loadProgramState);
  TEST_ASSERT_EQUAL(true, session->ongoingProcessFlag);
}

void test_hostInterpreter_by_requesting_tlv_write_register(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data32 = 0x11111111;
  
  User_Session userSession;
  userSession.tlvCommand = TLV_WRITE_REGISTER;
  userSession.data = &data32;
  userSession.address = 0x20000000;
  userSession.size = 8;

  waitUserCommand_ExpectAndReturn(&userSession);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(true, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX32(0x9C, session->txBuffer[10]); //chksum
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
}

void test_hostInterpreter_by_requesting_tlv_read_register(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  User_Session userSession;
  userSession.tlvCommand = TLV_READ_REGISTER;
  userSession.address = 0x20000000;

  waitUserCommand_ExpectAndReturn(&userSession);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(true, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xE0, session->txBuffer[6]); //chksum
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
}

void test_hostInterpreter_by_requesting_tlv_write_ram(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data32[] = {0x11111111, 0x22222222, 0x33333333};
  
  User_Session userSession;
  userSession.tlvCommand = TLV_WRITE_RAM;
  userSession.data = data32;
  userSession.address = 0x20000000;
  userSession.size = 12;

  waitUserCommand_ExpectAndReturn(&userSession);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(true, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX32(0x22222222, get4Byte(&session->txBuffer[10]));
  TEST_ASSERT_EQUAL_HEX32(0x33333333, get4Byte(&session->txBuffer[14]));
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
}

void test_hostInterpreter_should_stop_Interpret_write_ram_command_when_size_is_0(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();

  uint32_t data32[] = {0x11111111, 0x22222222, 0x33333333};
  
  User_Session userSession;
  userSession.tlvCommand = TLV_WRITE_RAM;
  userSession.data = data32;
  userSession.address = 0x20000000;
  userSession.size = 600;

  waitUserCommand_ExpectAndReturn(&userSession);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
}

void test_hostInterpreter_should_stop_Interpret_read_ram_command_when_size_is_0(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  User_Session userSession;
  userSession.tlvCommand = TLV_READ_RAM;
  userSession.address = 0x20000000;
  userSession.size = 600;

  waitUserCommand_ExpectAndReturn(&userSession);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = false;
  session->dataReceiveFlag = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(false, session->dataSendFlag);
  TEST_ASSERT_EQUAL(false, session->dataReceiveFlag);
}