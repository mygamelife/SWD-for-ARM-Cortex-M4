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

void test_tlvWriteTargetRegister_should_send_register_address_0x12345678_and_data_0xDEADBEEF(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t data = 0xDEADBEEF;
  tlvWriteTargetRegister(session, 0x12345678, &data);
  
  TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
}

void test_tlvReadTargetRegister_should_wait_response_after_send_packet(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  tlvReadTargetRegister(session, 0x88888888);
  
  TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  TEST_ASSERT_EQUAL_HEX32(0x88888888, get4Byte(&session->txBuffer[2]));
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
  
  tlvWriteTargetMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL_HEX32(0x200001F0, userSession.address);
  TEST_ASSERT_EQUAL(4, userSession.size);
  
  tlvWriteTargetMemory(session, &pData, &userSession.address, &userSession.size, TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL_HEX32(0x200002E8, userSession.address);
  TEST_ASSERT_EQUAL(-244, userSession.size);
}

void test_tlvSetBreakpoint_should_send_tlv_set_breakpoint_request(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  tlvSetBreakpoint(session, 0xabcdabcd);
  
  TEST_ASSERT_EQUAL(TLV_BREAKPOINT, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0xabcdabcd, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX8(0x10, session->txBuffer[6]);
}

void test_tlvLoadProgram_address_should_be_updated_after_call(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  TEST_ASSERT_EQUAL(TLV_LOAD_RO_DATA, session->loadProgramState);
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL(FLAG_SET, session->ongoingProcessFlag);
}

void test_tlvLoadProgram_address_should_load_ro_data_after_isr_vector(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  /* Load ISR VECTOR */
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  /* Load Read Only Data */
  tlvLoadProgram(session, "test/ELF_File/blinkLedx.elf", TLV_WRITE_FLASH);
  
  TEST_ASSERT_EQUAL(TLV_LOAD_DATA, session->loadProgramState);
  TEST_ASSERT_EQUAL(FLAG_SET, session->ongoingProcessFlag);
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
  
  tlvReadTargetMemory(session, &address, &size);

  TEST_ASSERT_EQUAL(true, session->ongoingProcessFlag);
  TEST_ASSERT_EQUAL_HEX32(0x200000F8, address);
  TEST_ASSERT_EQUAL(152, size);
  
  tlvReadTargetMemory(session, &address, &size);

  TEST_ASSERT_EQUAL(false, session->ongoingProcessFlag);
  TEST_ASSERT_EQUAL_HEX32(0x200001F0, address);
  TEST_ASSERT_EQUAL(-96, size);
}

void test_tlvLoadToRam_should_set_ongoing_process_flag_when_program_is_still_loading(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");

  TEST_ASSERT_EQUAL(TLV_LOAD_PROGRAM, session->ramState);
  TEST_ASSERT_EQUAL(FLAG_SET, session->ongoingProcessFlag);
}

void test_tlvLoadToRam_should_update_PC_after_finish_loading_elf_file(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");

  TEST_ASSERT_EQUAL(TLV_UPDATE_PC, session->ramState);
  TEST_ASSERT_EQUAL(FLAG_SET, session->ongoingProcessFlag);
  
  tlvLoadToRam(session, "test/ELF_File/blinkLedx.elf");
  
  TEST_ASSERT_EQUAL(TLV_LOAD_PROGRAM, session->ramState);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->ongoingProcessFlag);
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
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX32(0x9C, session->txBuffer[10]); //chksum
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = FLAG_CLEAR;
  session->dataReceiveFlag = FLAG_SET;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
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
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  
  /* ################## Send Tlv Write Register Request ################## */
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xE0, session->txBuffer[6]); //chksum
  
  /* ################## Receive Tlv Acknowledgement ################## */
  
  session->dataSendFlag = FLAG_CLEAR;
  session->dataReceiveFlag = FLAG_SET;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_WAIT_USER_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
}

void test_hostInterpreter_should_change_state_if_isr_vector_is_finish_transmit(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  
  User_Session userSession;
  userSession.tlvCommand = TLV_WRITE_RAM;
  userSession.fileName = "C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/Host/test/ELF_File/blinkLedx.elf";

  waitUserCommand_ExpectAndReturn(&userSession);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  
  /* ################## Interpret Tlv Command ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(TLV_LOAD_PROGRAM, session->ramState);
  
  /* ################## Sending first 248 bytes of ISR_VECTOR ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(TLV_LOAD_ISR_VECTOR, session->loadProgramState);
  TEST_ASSERT_EQUAL(FLAG_SET, session->ongoingProcessFlag);
  
  session->dataReceiveFlag = FLAG_SET;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  
  /* ################## Sending last 248 bytes of ISR_VECTOR ################## */
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_WAITING_RESPONSE, session->hostState);
  TEST_ASSERT_EQUAL(TLV_LOAD_RO_DATA, session->loadProgramState);
  TEST_ASSERT_EQUAL(FLAG_SET, session->ongoingProcessFlag);
  
  session->dataReceiveFlag = FLAG_SET;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  hostInterpreter(session);
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
}

void test_tlvFlashErase_should_send_flash_erase_request_if_flash_programmer_is_loaded(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  
  session->fPFlag = RUNNING;
  tlvFlashErase(session, 0x081C0000, 255);
  
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->ongoingProcessFlag);
  TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  TEST_ASSERT_EQUAL(TLV_FLASH_ERASE, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x081C0000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL(255, get4Byte(&session->txBuffer[6]));
}

void test_tlvFlashMassErase_should_send_flash_mass_erase_request_if_flash_programmer_is_loaded(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  fileStatus = FILE_CLOSED;
  
  session->fPFlag = RUNNING;
  tlvFlashMassErase(session, BANK_1);
  
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->ongoingProcessFlag);
  TEST_ASSERT_EQUAL(FLAG_SET, session->dataSendFlag);
  TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(BANK_1, get4Byte(&session->txBuffer[2]));
}

void test_hostInterpreter_should_call_flash_mass_erase(void) {
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  int bankSelection = BOTH_BANK;
  User_Session userSession;
  userSession.tlvCommand = TLV_FLASH_MASS_ERASE;
  userSession.data = &bankSelection;

  waitUserCommand_ExpectAndReturn(&userSession);
  hostInterpreter(session);
  
  TEST_ASSERT_EQUAL(HOST_INTERPRET_COMMAND, session->hostState);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
  
  session->fPFlag = RUNNING;
  
  /* ################## Run flash mass erase request ################## */
  hostInterpreter(session);
}