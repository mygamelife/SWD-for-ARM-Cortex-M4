#include "unity.h"
#include <string.h>
#include <malloc.h>
#include "Tlv.h"
#include "TlvEx.h"
#include "Read_File.h"
#include "GetHeaders.h"
#include "ProgramElf.h"
#include "ProgramLoader.h"
#include "ErrorCode.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "SystemTime.h"
#include "Yield.h"
#include "mock_Uart.h"
#include "mock_Interface.h"

void setUp(void)  {}

void tearDown(void) {}

void test_writeRegister_should_send_register_address_0x12345678_data_0xDEADBEEF_and_wait_for_reply(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  int result = writeRegister(session, 0x12345678, 0xDEADBEEF);
  
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
}

void test_writeRegister_should_send_request_and_receive_reply(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  int result;
  
  /* Send request */
  result = writeRegister(session, 0x12345678, 0xDEADBEEF);
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  result = writeRegister(session, 0x12345678, 0xDEADBEEF);
  
  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL(TLV_OK, session->rxBuffer[0]);
  TEST_ASSERT_EQUAL(1, session->rxBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0, get4Byte(&session->rxBuffer[2]));
}

void test_readRegister_should_wait_response_after_send_packet(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  int result = readRegister(session, 0x88888888);
  
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(TLV_READ_REGISTER, session->txBuffer[0]); //type
  TEST_ASSERT_EQUAL(5, session->txBuffer[1]); //length
  TEST_ASSERT_EQUAL_HEX32(0x88888888, get4Byte(&session->txBuffer[2])); //value
  TEST_ASSERT_EQUAL_HEX8(0xE0, get4Byte(&session->txBuffer[6])); //chksum
}

void test_readRegister_should_receive_response_and_return_register_value(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  int result; uint32_t data = 0x88888888;
  
  /* Send request */
  result = readRegister(session, data);
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 5;
  session->rxBuffer[6] = tlvPackIntoBuffer(&session->rxBuffer[2], (uint8_t *)&data, 4);
  
  result = readRegister(session, data);
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL_HEX32(0x88888888, result);
}

void test_halt_send_request_and_wait_for_reply_should_return_0(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  int result = halt(session);
  
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);
}

void test_halt_should_return_1_after_request_and_received_OK_reply(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, halt(session));
  TEST_ASSERT_EQUAL(1, isYielding);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, halt(session));
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_tlvRunTarget_should_return_1_after_request_and_received_OK_reply(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, run(session));
  TEST_ASSERT_EQUAL(1, isYielding);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, run(session));
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_tlvMultipleStepTarget_should_receive_response_and_return_current_program_counter_address(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  int result; uint32_t data = 0x20000010;
  
  /* Send request */
  result = multipleStep(session, 10);
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 5;
  session->rxBuffer[6] = tlvPackIntoBuffer(&session->rxBuffer[2], (uint8_t *)&data, 4);
  
  result = multipleStep(session, 10);
  TEST_ASSERT_EQUAL_HEX32(0x20000010, result);
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_tlvSoftReset_should_return_1_after_request_and_received_OK_reply(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, softReset(session));
  TEST_ASSERT_EQUAL(1, isYielding);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, softReset(session));
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_tlvHardReset_should_return_1_after_request_and_received_OK_reply(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, hardReset(session));
  TEST_ASSERT_EQUAL(1, isYielding);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, hardReset(session));
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_tlvVectReset_should_return_1_after_request_and_received_OK_reply(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  TEST_ASSERT_EQUAL(0, vectorReset(session));
  TEST_ASSERT_EQUAL(1, isYielding);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  TEST_ASSERT_EQUAL(1, vectorReset(session));
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_tlvReadTargetMemory_should_request_read_memory_and_return_data_block(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  uint32_t data[] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
  uint8_t *dataBlock;
  
  int size  = 16;
  uint32_t address = 0x20000000;
  
  dataBlock = readMemory(session, &address, &size);

  TEST_ASSERT_NULL(dataBlock);
  TEST_ASSERT_EQUAL_HEX32(0x20000010, address);
  TEST_ASSERT_EQUAL(0, size);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 17;
  session->rxBuffer[18] = tlvPackIntoBuffer(&session->rxBuffer[2], (uint8_t *)&data, 16);
  
  dataBlock = readMemory(session, &address, &size);

  TEST_ASSERT_NOT_NULL(dataBlock);
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&dataBlock[0]));
  TEST_ASSERT_EQUAL_HEX32(0x22222222, get4Byte(&dataBlock[4]));
  TEST_ASSERT_EQUAL_HEX32(0x33333333, get4Byte(&dataBlock[8]));
  TEST_ASSERT_EQUAL_HEX32(0x44444444, get4Byte(&dataBlock[12]));
}

void test_tlvWriteTargetMemory_should_request_and_write_data_into_target_RAM(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  User_Session userSession;
  
  userSession.data[0] = 0x12345678;
  userSession.data[1] = 0xDEADBEEF;
  userSession.data[2] = 0xBEEFDEAD;
  userSession.data[3] = 0xDEADDEAD;

  userSession.address = 0x20000000;
  userSession.size = 20;

  uint8_t *pData = (uint8_t *)userSession.data;
  writeMemory(session, pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL_HEX32(0x20000014, userSession.address);
  TEST_ASSERT_EQUAL(0, userSession.size);
  TEST_ASSERT_EQUAL(1, isYielding);
}

// void test_tlvWriteTargetMemory_write_isr_vector_section_into_RAM(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // ElfData *elfData = openElfFile("test/ElfFiles/ledRam.elf");
  // int index = getIndexOfSectionByName(elfData, ".isr_vector");
  // uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  // uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  // int size = getSectionSize(elfData, index);

  // writeMemory(session, &dataAddress, &destAddress, &size, TLV_WRITE_RAM);
  // TEST_ASSERT_EQUAL_HEX32(0x200000F8, destAddress);
  // TEST_ASSERT_EQUAL(180, size);
// }
  
// void test_tlvWriteTargetMemory_should_stop_request_when_size_is_0(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  // User_Session userSession;
  
  // userSession.data[0] = 0x12345678;
  // userSession.data[1] = 0xDEADBEEF;
  // userSession.data[2] = 0xBEEFDEAD;
  // userSession.data[3] = 0xDEADDEAD;
  
  // userSession.address = 0x20000000;
  // userSession.size = 500;

  // uint8_t *pData = (uint8_t *)userSession.data;
  // writeMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  
  // TEST_ASSERT_EQUAL_HEX32(0x200000F8, userSession.address);
  // TEST_ASSERT_EQUAL(252, userSession.size);
  
  // /* Received reply */
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // writeMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // writeMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  // TEST_ASSERT_EQUAL_HEX32(0x200001F0, userSession.address);
  // TEST_ASSERT_EQUAL(4, userSession.size);
  
  // /* Received reply */
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // writeMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // writeMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  // TEST_ASSERT_EQUAL_HEX32(0x200002E8, userSession.address);
  // TEST_ASSERT_EQUAL(-244, userSession.size);
// }

// void test_tlvLoadProgram_address_should_be_updated_after_call(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  // tlvLoadProgram(session, "test/ElfFiles/ledRam.elf", TLV_WRITE_FLASH);
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  // tlvLoadProgram(session, "test/ElfFiles/ledRam.elf", TLV_WRITE_FLASH);
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  // tlvLoadProgram(session, "test/ElfFiles/ledRam.elf", TLV_WRITE_FLASH);
  
  // TEST_ASSERT_EQUAL(TLV_LOAD_TEXT, session->loadProgramState);
  // TEST_ASSERT_EQUAL_HEX32(0x200000F8, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // closeElfFile();
// }

// void test_tlvLoadProgram_address_should_load_text_after_isr_vector(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  // /* Load ISR VECTOR */
  // tlvLoadProgram(session, "test/ElfFiles/ledRam.elf", TLV_WRITE_FLASH);

  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  // tlvLoadProgram(session, "test/ElfFiles/ledRam.elf", TLV_WRITE_FLASH);
  
  // /* Load ISR VECTOR */
  // tlvLoadProgram(session, "test/ElfFiles/ledRam.elf", TLV_WRITE_FLASH);
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  // tlvLoadProgram(session, "test/ElfFiles/ledRam.elf", TLV_WRITE_FLASH);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // /* Load Text */
  // tlvLoadProgram(session, "test/ElfFiles/ledRam.elf", TLV_WRITE_FLASH);
  
  // TEST_ASSERT_EQUAL(TLV_LOAD_TEXT, session->loadProgramState);
  // TEST_ASSERT_EQUAL_HEX32(0x200001AC, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // closeElfFile();
// }

// void test_tlvLoadToRam_should_set_ongoing_process_flag_when_program_is_still_loading(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");

  // TEST_ASSERT_EQUAL(TLV_LOAD_PROGRAM, session->lramState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // closeElfFile();
// }

// void test_tlvLoadToRam_should_update_PC_and_run_the_program_after_finish_loading(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0; 
  
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf"); 
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // TEST_ASSERT_EQUAL(TLV_UPDATE_PC, session->lramState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // TEST_ASSERT_EQUAL(TLV_RUN_PROGRAM, session->lramState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
  // TEST_ASSERT_EQUAL(TLV_LOAD_PROGRAM, session->lramState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
// }

// void test_tlvRequestFlashErase_should_request_erase_target_memory_and_return_1_if_successfully_request_and_receive_ack(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  // uint32_t address = 0x8000000; int size = 255;
  // int result = 0;
  
  // result = tlvRequestFlashErase(session, address, size);

  // TEST_ASSERT_EQUAL(PROCESS_BUSY, result);
  // TEST_ASSERT_EQUAL(true, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL_HEX32(0x8000000, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL(255, get4Byte(&session->txBuffer[6]));

  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // result = tlvRequestFlashErase(session, address, size);

  // TEST_ASSERT_EQUAL(PROCESS_DONE, result);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
// }

// void test_tlvEraseTargetFlash_should_send_flash_erase_request_if_flash_programmer_is_loaded(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  // session->eraseState = TLV_REQUEST_ERASE;
  
  // tlvEraseTargetFlash(session, 0x081C0000, 20000);
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // tlvEraseTargetFlash(session, 0x081C0000, 20000);
  
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(TLV_FLASH_ERASE, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX32(0x081C0000, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX32(0x4E20, get4Byte(&session->txBuffer[6]));
// }

// void test_tlvRequestFlashMassErase_should_request_erase_target_memory_and_return_1_if_successfully_request_and_receive_ack(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  // int result = 0;
  
  // result = tlvRequestFlashMassErase(session, BOTH_BANK);

  // TEST_ASSERT_EQUAL(PROCESS_BUSY, result);
  // TEST_ASSERT_EQUAL(true, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL_HEX32(0x000000003, get4Byte(&session->txBuffer[2]));

  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // result = tlvRequestFlashMassErase(session, BOTH_BANK);

  // TEST_ASSERT_EQUAL(PROCESS_DONE, result);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
// }

// void test_tlvMassEraseTargetFlash_should_send_flash_mass_erase_request(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  // session->mEraseState = TLV_REQUEST_ERASE;
  
  // tlvMassEraseTargetFlash(session, BOTH_BANK);
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // tlvMassEraseTargetFlash(session, BOTH_BANK);
  
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX32(0x00000003, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX8(0xFD, session->txBuffer[6]); //checksum
// }

// void test_tlvLoadToFlash_should_request_flash_erase_section_and_program_Size(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  // session->eraseState = TLV_REQUEST_ERASE;
  
  // tlvLoadToFlash(session, "test/ElfFiles/ledFlash.elf");
  
  // TEST_ASSERT_EQUAL(TLV_REQUEST_ERASE, session->flashState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(TLV_FLASH_ERASE, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX32(0x08000000, get4Byte(&session->txBuffer[2]));
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // tlvLoadToFlash(session, "test/ElfFiles/ledFlash.elf");
  // TEST_ASSERT_EQUAL(TLV_LOAD_ACTUAL_PROGRAM, session->flashState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
// }

// void test_tlvLoadToFlash_should_update_program_counter_to_entry_address_after_loaded_actual_program(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // session->flashState = TLV_UPDATE_PC;
  
  // tlvLoadToFlash(session, "test/ElfFiles/ledFlash.elf");
  // TEST_ASSERT_EQUAL(TLV_UPDATE_PC, session->flashState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // tlvLoadToFlash(session, "test/ElfFiles/ledFlash.elf");
  // TEST_ASSERT_EQUAL(TLV_RUN_PROGRAM, session->flashState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
// }

// void test_tlvLoadToFlash_should_run_the_program_after_update_program_counter_to_entry_address(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // session->flashState = TLV_RUN_PROGRAM;
  // tlvLoadToFlash(session, "test/ElfFiles/ledFlash.elf");
  // TEST_ASSERT_EQUAL(TLV_RUN_PROGRAM, session->flashState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // tlvLoadToFlash(session, "test/ElfFiles/ledFlash.elf");
  // TEST_ASSERT_EQUAL(0, session->runState);
  // TEST_ASSERT_EQUAL(TLV_REQUEST_ERASE, session->flashState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
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

// void test_tlvWaitDebugEvents_by_sending_specific_event_and_wait_for_reply(void) {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  // EventType event;
  
  // event = tlvWaitDebugEvents(session, BREAKPOINT_EVENT);
  
  // TEST_ASSERT_EQUAL(TLV_DEBUG_EVENTS, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(2, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX8(BREAKPOINT_EVENT, session->txBuffer[2]);
  // TEST_ASSERT_EQUAL_HEX8(0xFF, session->txBuffer[3]); //chksum
  
  // /* Received reply */
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 2;
  // session->rxBuffer[2] = BREAKPOINT_EVENT;
  // session->rxBuffer[3] = 0xFF;
  
  // event = tlvWaitDebugEvents(session, BREAKPOINT_EVENT);
  
  // TEST_ASSERT_EQUAL(BREAKPOINT_EVENT, event);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
// }

// /**
  // ==============================================================================
                      // ##### Test For Host Interpreter #####
  // ==============================================================================  
  // */
// void test_hostInterpreter_by_requesting_tlv_write_ram_memory(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  // User_Session userSession;
  
  // userSession.tlvCommand = TLV_WRITE_RAM;
  
  // userSession.data[0] = 0x11111111;
  // userSession.data[1] = 0x222222;
  // userSession.data[2] = 0x3333;
  // userSession.data[3] = 0x44444;
  
  // userSession.address = 0x20000000;
  // userSession.size = 16;

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  // /* ################## Send Tlv Write Register Request ################## */
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  // TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  // TEST_ASSERT_EQUAL_HEX32(0x222222, get4Byte(&session->txBuffer[10]));
  // TEST_ASSERT_EQUAL_HEX32(0x3333, get4Byte(&session->txBuffer[14]));
  // TEST_ASSERT_EQUAL_HEX32(0x44444, get4Byte(&session->txBuffer[18]));
  // TEST_ASSERT_EQUAL_HEX32(0x44, session->txBuffer[22]); //chksum
  
  // /* ################## Receive Tlv Acknowledgement ################## */
  
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
// }

// void test_hostInterpreter_by_requesting_tlv_write_register(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  // User_Session userSession;
  
  // userSession.tlvCommand = TLV_WRITE_REGISTER;
  // userSession.data[0] = 0x11111111;
  // userSession.address = 0x20000000;
  // userSession.size = 8;

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  // /* ################## Send Tlv Write Register Request ################## */
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  // TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  // TEST_ASSERT_EQUAL_HEX32(0x9C, session->txBuffer[10]); //chksum
  
  // /* ################## Receive Tlv Acknowledgement ################## */
  
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
// }

// void test_hostInterpreter_by_requesting_tlv_read_register(void)
// {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // User_Session userSession;
  // userSession.tlvCommand = TLV_READ_REGISTER;
  // userSession.address = 0x20000000;

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  // /* ################## Send Tlv Write Register Request ################## */
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  // TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX32(0xE0, session->txBuffer[6]); //chksum
  
  // /* ################## Receive Tlv Acknowledgement ################## */
  
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
// }

// void test_hostInterpreter_should_change_state_if_isr_vector_is_finish_transmit(void) {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // fileStatus = FILE_CLOSED;
  
  // User_Session userSession;
  // userSession.tlvCommand = TLV_LOAD_RAM;
  // userSession.fileName = "test/ElfFiles/ledRam.elf";

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  // /* ################## Sending first 248 bytes of ISR_VECTOR ################## */
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(TLV_LOAD_ISR_VECTOR, session->loadProgramState);
  
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  
  // /* ################## Sending last 248 bytes of ISR_VECTOR ################## */
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(TLV_LOAD_TEXT, session->loadProgramState);
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  
  // closeElfFile();
// }

// void test_hostInterpreter_should_call_flash_mass_erase_if_flash_programmer_is_loaded(void) {
  // uartInit_Ignore();
	// Tlv_Session *session = tlvCreateSession();
  
  // User_Session userSession;
  // userSession.tlvCommand = TLV_FLASH_MASS_ERASE;
  // userSession.address = BOTH_BANK;

  // waitUserCommand_ExpectAndReturn(&userSession);
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  // /* ################## Run flash mass erase request ################## */
  // session->lramState = TLV_RUN_PROGRAM;
  
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(TLV_REQUEST_ERASE, session->mEraseState);
  
  // /* ################## Probe response with acknowledgment ################## */
  // CLEAR_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
  // SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  // session->rxBuffer[0] = TLV_OK;
  // session->rxBuffer[1] = 1;
  // session->rxBuffer[2] = 0;
  
  // hostInterpreter(session);
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  
  // TEST_ASSERT_EQUAL(TLV_LOAD_FLASH_PROGRAMMER, session->mEraseState);
  // TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->txBuffer[0]);
  // TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  // TEST_ASSERT_EQUAL_HEX32(BOTH_BANK, session->txBuffer[2]);
// }

void test_hostInterpreter_should_readMemory_and_stop_when_size_reached_0(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  User_Session us;
  
  us.tlvCommand = TLV_READ_MEMORY;
  us.address = 0x20000000;
  us.size = 400;
  
  waitUserCommand_ExpectAndReturn(&us);
  
  hostInterpreter(session);

  TEST_ASSERT_EQUAL(1, isYielding);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL(248, session->txBuffer[6]);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(1, isYielding);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL(152, session->txBuffer[6]);
  
  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  deleteUserSession_Expect(&us);
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
}