#include "Tlv.h"
#include "unity.h"
#include "Tlv_ex.h"
#include <malloc.h>
#include "Read_File.h"
#include "GetHeaders.h"
#include "ProgramElf.h"
#include "mock_uart.h"
#include "ProgramLoader.h"
#include "ErrorCode.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "mock_Interface.h"
#include "GetTime.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvWriteTargetRegister_should_send_register_address_0x12345678_data_0xDEADBEEF_and_wait_for_reply(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data = 0xDEADBEEF;
  int result = tlvWriteTargetRegister(session, 0x12345678, &data);
  
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
}

void test_tlvWriteTargetRegister_should_send_request_and_receive_reply(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  int result;
  uint32_t data = 0xDEADBEEF;
  
  /* Send request */
  result = tlvWriteTargetRegister(session, 0x12345678, &data);
  TEST_ASSERT_EQUAL(0, result);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  result = tlvWriteTargetRegister(session, 0x12345678, &data);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL(TLV_OK, session->rxBuffer[0]);
  TEST_ASSERT_EQUAL(1, session->rxBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0, get4Byte(&session->rxBuffer[2]));
}

void test_tlvReadTargetRegister_should_wait_response_after_send_packet(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  int result = tlvReadTargetRegister(session, 0x88888888);
  
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL(TLV_READ_REGISTER, session->txBuffer[0]); //type
  TEST_ASSERT_EQUAL(5, session->txBuffer[1]); //length
  TEST_ASSERT_EQUAL_HEX32(0x88888888, get4Byte(&session->txBuffer[2])); //value
  TEST_ASSERT_EQUAL_HEX8(0xE0, get4Byte(&session->txBuffer[6])); //chksum
}

void test_tlvReadTargetRegister_should_receive_response_and_return_register_value(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  int result; uint32_t data = 0x88888888;
  
  /* Send request */
  result = tlvReadTargetRegister(session, data);
  TEST_ASSERT_EQUAL(0, result);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 5;
  session->rxBuffer[6] = tlvPackIntoBuffer(&session->rxBuffer[2], (uint8_t *)&data, 4);
  
  result = tlvReadTargetRegister(session, data);
  TEST_ASSERT_EQUAL_HEX32(0x88888888, result);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
}

void test_tlvHaltTarget_send_request_and_wait_for_reply_should_return_0(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  int result = tlvHaltTarget(session);
  
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
}

void test_tlvHaltTarget_should_return_1_after_request_and_received_OK_reply(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, tlvHaltTarget(session));
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, tlvHaltTarget(session));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
}

void test_tlvRunTarget_should_return_1_after_request_and_received_OK_reply(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, tlvRunTarget(session));
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, tlvRunTarget(session));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
}

void test_tlvMultipleStepTarget_should_receive_response_and_return_current_program_counter_address(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  int result; uint32_t data = 0x20000010;
  
  /* Send request */
  result = tlvMultipleStepTarget(session, 10);
  TEST_ASSERT_EQUAL(0, result);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 5;
  session->rxBuffer[6] = tlvPackIntoBuffer(&session->rxBuffer[2], (uint8_t *)&data, 4);
  
  result = tlvMultipleStepTarget(session, 10);
  TEST_ASSERT_EQUAL_HEX32(0x20000010, result);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
}

void test_tlvSoftReset_should_return_1_after_request_and_received_OK_reply(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, tlvSoftReset(session));
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, tlvSoftReset(session));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
}

void test_tlvHardReset_should_return_1_after_request_and_received_OK_reply(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, tlvHardReset(session));
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, tlvHardReset(session));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
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

void test_tlvReadTargetMemory_should_request_read_memory_and_return_data_block(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();

  uint32_t data[] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
  uint32_t address = 0x20000000; uint8_t *dataBlock;
  int size = 16;
  
  tlvReadTargetMemory(session, &address, &size);

  TEST_ASSERT_EQUAL(true, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, address);
  TEST_ASSERT_EQUAL(-232, size);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 17;
  session->rxBuffer[18] = tlvPackIntoBuffer(&session->rxBuffer[2], (uint8_t *)&data, 16);
  
  dataBlock = tlvReadTargetMemory(session, &address, &size);

  TEST_ASSERT_EQUAL(false, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&dataBlock[0]));
  TEST_ASSERT_EQUAL_HEX32(0x22222222, get4Byte(&dataBlock[4]));
  TEST_ASSERT_EQUAL_HEX32(0x33333333, get4Byte(&dataBlock[8]));
  TEST_ASSERT_EQUAL_HEX32(0x44444444, get4Byte(&dataBlock[12]));
}

void test_tlvReadTargetMemory_should_stop_request_tlv_read_register_when_size_is_0(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();

  uint32_t address = 0x20000000;
  int size = 400;
  
  uint8_t *result = tlvReadTargetMemory(session, &address, &size);

  TEST_ASSERT_NULL(result);
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, address);
  TEST_ASSERT_EQUAL(152, size);
}

void test_tlvWriteDataChunk_should_send_data_in_chunk_to_ram(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x11111111;
  uint32_t data[] = {0xDEADBEEF};
  tlvWriteDataChunk(session, (uint8_t *)data, address, 4, TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL_HEX8(TLV_WRITE_RAM, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX8(0x84, session->txBuffer[10]); //chksum
}

void test_tlvWriteDataChunk_should_send_data_in_chunk_to_flash(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x22222222;
  uint32_t data[] = {0xDEADBEEF};
  tlvWriteDataChunk(session, (uint8_t *)data, address, 4, TLV_WRITE_FLASH);
  
  TEST_ASSERT_EQUAL_HEX8(TLV_WRITE_FLASH, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x22222222, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX8(0x40, session->txBuffer[10]); //chksum
}

void test_tlvWriteDataChunk_should_send_data_in_chunk_with_specific_size(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x12345678;
  uint32_t data[] = {0x12345678, 0xDEADBEEF, 0xBEEFDEAD, 0xDEADDEAD};
  tlvWriteDataChunk(session, (uint8_t *)data, address, 20, TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, 25, address, (uint8_t *)data, (Tlv *)session->txBuffer);	
}

void test_tlvWriteTargetMemory_should_request_and_write_data_into_target_RAM(void)
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
  tlvWriteTargetMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, userSession.address);
  TEST_ASSERT_EQUAL(-228, userSession.size);
}

void test_tlvWriteTargetMemory_write_isr_vector_section_into_RAM(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  ElfData *elfData = openElfFile("test/ELF_File/blinkLed.elf");
  int index = getIndexOfSectionByName(elfData, ".isr_vector");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  int size = getSectionSize(elfData, index);

  tlvWriteTargetMemory(session, &dataAddress, &destAddress, &size, TLV_WRITE_RAM);
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, destAddress);
  TEST_ASSERT_EQUAL(180, size);
}
  
void test_tlvWriteTargetMemory_should_stop_request_when_size_is_0(void)
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
  tlvWriteTargetMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, userSession.address);
  TEST_ASSERT_EQUAL(252, userSession.size);
  
  /* Received reply */
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  tlvWriteTargetMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  tlvWriteTargetMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  TEST_ASSERT_EQUAL_HEX32(0x200001F0, userSession.address);
  TEST_ASSERT_EQUAL(4, userSession.size);
  
  /* Received reply */
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  tlvWriteTargetMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  tlvWriteTargetMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  TEST_ASSERT_EQUAL_HEX32(0x200002E8, userSession.address);
  TEST_ASSERT_EQUAL(-244, userSession.size);
}

void test_tlvLoadProgram_address_should_be_updated_after_call(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  TEST_ASSERT_EQUAL(TLV_LOAD_TEXT, session->loadProgramState);
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  closeElfFile();
}

void test_tlvLoadProgram_address_should_load_text_after_isr_vector(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  /* Load ISR VECTOR */
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  /* Load ISR VECTOR */
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  /* Load Text */
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  TEST_ASSERT_EQUAL(TLV_LOAD_TEXT, session->loadProgramState);
  TEST_ASSERT_EQUAL_HEX32(0x200001AC, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  closeElfFile();
}

void test_tlvLoadToRam_should_set_ongoing_process_flag_when_program_is_still_loading(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");

  TEST_ASSERT_EQUAL(TLV_LOAD_PROGRAM, session->lramState);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  closeElfFile();
}

void test_tlvLoadToRam_should_update_PC_and_run_the_program_after_finish_loading(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0; 
  
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf"); 
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  TEST_ASSERT_EQUAL(TLV_UPDATE_PC, session->lramState);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  TEST_ASSERT_EQUAL(TLV_RUN_PROGRAM, session->lramState);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  TEST_ASSERT_EQUAL(TLV_LOAD_PROGRAM, session->lramState);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
}

void test_tlvEraseTargetFlash_should_request_erase_target_memory_and_return_1_if_successfully_request_and_receive_ack(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  uint32_t address = 0x8000000; int size = 255;
  int result = 0;
  
  result = tlvRequestFlashErase(session, address, size);

  TEST_ASSERT_EQUAL(PROCESS_BUSY, result);
  TEST_ASSERT_EQUAL(true, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x8000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL(255, get4Byte(&session->txBuffer[6]));

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  result = tlvRequestFlashErase(session, address, size);

  TEST_ASSERT_EQUAL(PROCESS_DONE, result);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
}

void test_tlvEraseTargetFlash_should_send_flash_erase_request_if_flash_programmer_is_loaded(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  session->eraseState = TLV_REQUEST_ERASE;
  
  tlvEraseTargetFlash(session, 0x081C0000, 20000);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  tlvEraseTargetFlash(session, 0x081C0000, 20000);
  
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(TLV_FLASH_ERASE, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x081C0000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0x4E20, get4Byte(&session->txBuffer[6]));
}

// void test_tlvMassEraseTargetFlash_should_send_flash_mass_erase_request(void)
// {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  // session->mEraseState = TLV_LOAD_FLASH_PROGRAMMER;
  // session->ramState = TLV_RUN_PROGRAM;
  
  // tlvMassEraseTargetFlash(session, BANK_1);
  // tlvMassEraseTargetFlash(session, BANK_1);
  
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->ongoingProcessFlag);
  // TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(TLV_LOAD_FLASH_PROGRAMMER, session->mEraseState);
  // TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX32(BANK_1, session->txBuffer[2]);
// }

// void test_tlvLoadToFlash_should_request_flash_erase_section_and_program_Size(void)
// {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  // session->eraseState = TLV_REQUEST_ERASE;
  
  // tlvLoadToFlash(session, "C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/Host/test/ELF_File/led.elf");
  
  // TEST_ASSERT_EQUAL(TLV_LOAD_ACTUAL_PROGRAM, session->flashState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(TLV_FLASH_ERASE, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX32(0x08000000, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL(4176, get4Byte(&session->txBuffer[6]));
// }

// void test_tlvLoadToFlash_should_update_program_counter_to_entry_address_after_loaded_actual_program(void)
// {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // session->flashState = TLV_UPDATE_PC;
  // tlvLoadToFlash(session, "C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/Host/test/ELF_File/led.elf");
  
  // TEST_ASSERT_EQUAL(TLV_RUN_PROGRAM, session->flashState);
// }

// void test_tlvLoadToFlash_should_run_the_program_after_update_program_counter_to_entry_address(void)
// {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // session->flashState = TLV_RUN_PROGRAM;
  // tlvLoadToFlash(session, "C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/Host/test/ELF_File/led.elf");
  
  // TEST_ASSERT_EQUAL(TLV_REQUEST_ERASE, session->flashState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->ongoingProcessFlag);
// }

// void test_tlvSetBreakpoint_should_send_tlv_set_breakpoint_request(void)
// {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // tlvSetBreakpoint(session, 0xabcdabcd);
  
  // TEST_ASSERT_EQUAL(TLV_BREAKPOINT, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX32(0xabcdabcd, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX8(0x10, session->txBuffer[6]);
// }

void test_hostInterpreter_by_requesting_tlv_write_ram_memory(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data32[] = {0x11111111, 0x222222, 0x3333, 0x44444};
  
  User_Session userSession;
  userSession.tlvCommand = TLV_WRITE_RAM;
  userSession.data = data32;
  userSession.address = 0x20000000;
  userSession.size = 16;

  waitUserCommand_ExpectAndReturn(&userSession);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX32(0x222222, get4Byte(&session->txBuffer[10]));
  TEST_ASSERT_EQUAL_HEX32(0x3333, get4Byte(&session->txBuffer[14]));
  TEST_ASSERT_EQUAL_HEX32(0x44444, get4Byte(&session->txBuffer[18]));
  TEST_ASSERT_EQUAL_HEX32(0x44, session->txBuffer[22]); //chksum
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
}

// void test_hostInterpreter_by_requesting_tlv_write_register(void)
// {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // uint32_t data32 = 0x11111111;
  
  // User_Session userSession;
  // userSession.tlvCommand = TLV_WRITE_REGISTER;
  // userSession.data = &data32;
  // userSession.address = 0x20000000;
  // userSession.size = 8;

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  
  // /* ################## Send Tlv Write Register Request ################## */
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  // TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  // TEST_ASSERT_EQUAL_HEX32(0x9C, session->txBuffer[10]); //chksum
  
  // /* ################## Receive Tlv Acknowledgement ################## */
  
  // session->dataSendFlag = FLAG_CLEAR;
  // GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
// }

// void test_hostInterpreter_by_requesting_tlv_read_register(void)
// {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // User_Session userSession;
  // userSession.tlvCommand = TLV_READ_REGISTER;
  // userSession.address = 0x20000000;

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  
  // /* ################## Send Tlv Write Register Request ################## */
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  // TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX32(0xE0, session->txBuffer[6]); //chksum
  
  // /* ################## Receive Tlv Acknowledgement ################## */
  
  // session->dataSendFlag = FLAG_CLEAR;
  // GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
// }

// void test_hostInterpreter_should_change_state_if_isr_vector_is_finish_transmit(void) {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  
  // User_Session userSession;
  // userSession.tlvCommand = TLV_WRITE_RAM;
  // userSession.fileName = "C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/Host/test/ELF_File/blinkLedx.elf";

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  
  // /* ################## Interpret Tlv Command ################## */
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(TLV_LOAD_PROGRAM, session->ramState);
  
  // /* ################## Sending first 248 bytes of ISR_VECTOR ################## */
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  // TEST_ASSERT_EQUAL(TLV_LOAD_ISR_VECTOR, session->loadProgramState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  
  // /* ################## Sending last 248 bytes of ISR_VECTOR ################## */
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  // TEST_ASSERT_EQUAL(TLV_LOAD_TEXT, session->loadProgramState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  
  // closeElfFile();
// }

// void test_hostInterpreter_should_call_flash_mass_erase_if_flash_programmer_is_loaded(void) {
  // HANDLE hSerial;
  // uartInit_IgnoreAndReturn(hSerial);
	// Tlv_Session *session = tlvCreateSession();
  
  // User_Session userSession;
  // userSession.tlvCommand = TLV_FLASH_MASS_ERASE;
  // userSession.address = BOTH_BANK;

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  
  // /* ################## Run flash mass erase request ################## */
  // session->ramState = TLV_RUN_PROGRAM;
  
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(TLV_REQUEST_ERASE, session->mEraseState);
  
  // /* ################## Probe response with acknowledgment ################## */
  // GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->ongoingProcessFlag);
  // TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  // TEST_ASSERT_EQUAL(TLV_LOAD_FLASH_PROGRAMMER, session->mEraseState);
  // TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX32(BOTH_BANK, session->txBuffer[2]);
// }