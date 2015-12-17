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
#include "Yield.h"
#include "LoadElf.h"
#include "mock_SystemTime.h"
#include "mock_Uart.h"
#include "mock_Interface.h"
#include "mock_ProgramVerifier.h"

void setUp(void)  {}

void tearDown(void) {}

void test_writeRegister_should_throw_if_timeout(void)
{
  CEXCEPTION_T err;
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  int result = 0;

  Try {
    getSystemTime_ExpectAndReturn(10);
    isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 1);
    result = writeRegister(session, 0x12345678, 0xDEADBEEF);
  }
  Catch(err) {
    TEST_ASSERT_EQUAL(PROBE_NOT_RESPONDING, err);
    TEST_ASSERT_EQUAL(0, isYielding);
    TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
    TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
  }
}

void test_writeRegister_should_send_request_and_receive_reply(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  int result;

  /* Send request */
  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
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

void test_readRegister_should_receive_response_and_return_register_value(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  int result; uint32_t data = 0x88888888;

  /* Send request */
  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
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

void test_halt_should_return_1_after_request_and_received_OK_reply(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
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

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);

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

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);

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

void test_tlvstepOver_should_receive_response_and_return_current_program_counter_address(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  int result; uint32_t data = 0x20000010;

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  
  /* Send request */
  result = stepOver(session);
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 5;
  session->rxBuffer[6] = tlvPackIntoBuffer(&session->rxBuffer[2], (uint8_t *)&data, 4);

  result = stepOver(session);
  TEST_ASSERT_EQUAL_HEX32(0x20000010, result);
  TEST_ASSERT_EQUAL(0, isYielding);
}


void test_tlvSoftReset_should_return_1_after_request_and_received_OK_reply(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);

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

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);

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

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);

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
  uint8_t *db;

  int size  = 16;
  uint32_t address = 0x20000000;

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  db = readMemory(session, address, size);

  TEST_ASSERT_EQUAL(1, isYielding);
  TEST_ASSERT_NULL(db);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 17;
  session->rxBuffer[18] = tlvPackIntoBuffer(&session->rxBuffer[2], (uint8_t *)&data, 16);

  db = readMemory(session, address, size);

  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_NOT_NULL(db);
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&db[0]));
  TEST_ASSERT_EQUAL_HEX32(0x22222222, get4Byte(&db[4]));
  TEST_ASSERT_EQUAL_HEX32(0x33333333, get4Byte(&db[8]));
  TEST_ASSERT_EQUAL_HEX32(0x44444444, get4Byte(&db[12]));

  delDataBlock(db);
}

void test_writeMemory_should_request_and_write_data_into_target_RAM(void)
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

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  writeMemory(session, pData, userSession.address, userSession.size, TLV_WRITE_RAM);

  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  writeMemory(session, pData, userSession.address, userSession.size, TLV_WRITE_RAM);

  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_writeMemory_should_stop_request_when_size_is_0(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  User_Session userSession;

  userSession.data[0] = 0x12345678;
  userSession.data[1] = 0xDEADBEEF;
  userSession.data[2] = 0xBEEFDEAD;
  userSession.data[3] = 0xDEADDEAD;

  userSession.address = 0x20000000;
  userSession.size = 600;

  uint8_t *pData = (uint8_t *)userSession.data;

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  writeMemory(session, pData, userSession.address, userSession.size, TLV_WRITE_RAM);

  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  writeMemory(session, pData, userSession.address, userSession.size, TLV_WRITE_RAM);

  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  writeMemory(session, pData, userSession.address, userSession.size, TLV_WRITE_RAM);

  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  writeMemory(session, pData, userSession.address, userSession.size, TLV_WRITE_RAM);

  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_writeMemory_should_write_the_updated_data_instead_of_start_from_starting_position(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  uint8_t data[255] = { 0xaa, 0xaa, 0xaa, 0xaa,
                        0xbb, 0xbb, 0xbb, 0xbb,
                      };
  uint32_t address = 0x20000000;

  data[248] = 0xcc; data[249] = 0xcc; data[250] = 0xcc; data[251] = 0xcc;
  int size = 250;

  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  getSystemTime_ExpectAndReturn(10);
  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  writeMemory(session, data, address, size, TLV_WRITE_RAM);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  writeMemory(session, data, address, size, TLV_WRITE_RAM);

  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_loadProgram_should_load_all_section_of_the_program(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  int i = 0;

  printf("###################### LOADING PROGRAM ledRam.elf ######################\n");
  Program *p = getLoadableSection("test/ElfFiles/ledRam.elf");

  for(; i < 26; i++) {
    /* Received reply */
    SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
    session->rxBuffer[0] = TLV_OK;
    session->rxBuffer[1] = 1;
    session->rxBuffer[2] = 0;

    getSystemTime_IgnoreAndReturn(10);
    isTimeout_IgnoreAndReturn(0);
    /* Loading Program */
    loadProgram(session, p, TLV_WRITE_RAM);
  }
  TEST_ASSERT_EQUAL(0, isYielding);

  delProgram(p);
}

void test_loadRam_should_load_program_update_pc_and_run_the_program(void)
{
  int i = 0;

  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  Program *p = getLoadableSection("test/ElfFiles/ledRam.elf");

  for(; i < 28; i++) {
    /* Received reply */
    SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
    session->rxBuffer[0] = TLV_OK;
    session->rxBuffer[1] = 1;
    session->rxBuffer[2] = 0;

    getSystemTime_IgnoreAndReturn(10);
    isTimeout_IgnoreAndReturn(0);
    /* Load Data Section */
    loadRam(session, p);
  }

  TEST_ASSERT_EQUAL(0, isYielding);

  delProgram(p);
}

void test_tlvWaitDebugEvents_by_sending_specific_event_and_wait_for_reply(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  EventType event;

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  event = tlvWaitDebugEvents(session, BREAKPOINT_EVENT);

  TEST_ASSERT_EQUAL(0, event);
  TEST_ASSERT_EQUAL(TLV_DEBUG_EVENTS, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(2, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX8(BREAKPOINT_EVENT, session->txBuffer[2]);
  TEST_ASSERT_EQUAL_HEX8(0xFF, session->txBuffer[3]); //chksum
  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 2;
  session->rxBuffer[2] = BREAKPOINT_EVENT;
  session->rxBuffer[3] = 0xFF;

  event = tlvWaitDebugEvents(session, BREAKPOINT_EVENT);

  TEST_ASSERT_EQUAL(BREAKPOINT_EVENT, event);
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_setBreakpoint_should_send_tlv_set_breakpoint_request(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  setBreakpoint(session, 0xabcdabcd);
  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 2;
  session->rxBuffer[2] = BREAKPOINT_EVENT;
  session->rxBuffer[3] = 0xFF;

  setBreakpoint(session, 0xabcdabcd);

  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(TLV_BREAKPOINT, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0xabcdabcd, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX8(0x10, session->txBuffer[6]);
}

/** ==============================================================================
                      ######## Test For Host Interpreter ########
    ==============================================================================  */
void test_hostInterpreter_by_requesting_tlv_write_ram_memory(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  User_Session userSession;

  userSession.tlvCommand = TLV_WRITE_RAM;

  userSession.data[0] = 0x11111111;
  userSession.data[1] = 0x222222;
  userSession.data[2] = 0x3333;
  userSession.data[3] = 0x44444;

  userSession.address = 0x20000000;
  userSession.size = 16;

  printf(" ################ hostInterpreter ################\n");
  waitUserCommand_ExpectAndReturn(&userSession);

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);

  hostInterpreter(session);

  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(21, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX32(0x222222, get4Byte(&session->txBuffer[10]));
  TEST_ASSERT_EQUAL_HEX32(0x3333, get4Byte(&session->txBuffer[14]));
  TEST_ASSERT_EQUAL_HEX32(0x44444, get4Byte(&session->txBuffer[18]));
  TEST_ASSERT_EQUAL_HEX32(0x44, session->txBuffer[22]); //chksum
  TEST_ASSERT_EQUAL(1, isYielding);

  /* ################## Receive Tlv Acknowledgement ################## */

  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  delUserSession_Expect(&userSession);
  hostInterpreter(session);

  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_hostInterpreter_by_requesting_tlv_write_register(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  User_Session userSession;

  userSession.tlvCommand = TLV_WRITE_REGISTER;
  userSession.data[0] = 0x11111111;
  userSession.address = 0x20000000;
  userSession.size = 8;

  waitUserCommand_ExpectAndReturn(&userSession);

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);

  hostInterpreter(session);

  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0x11111111, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX32(0x9C, session->txBuffer[10]); //chksum
  TEST_ASSERT_EQUAL(1, isYielding);

  /* ################## Receive Tlv Acknowledgement ################## */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  delUserSession_Expect(&userSession);
  hostInterpreter(session);

  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_hostInterpreter_by_requesting_tlv_read_register(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  User_Session userSession;
  userSession.tlvCommand = TLV_READ_REGISTER;
  userSession.address = 0x20000000;

  waitUserCommand_ExpectAndReturn(&userSession);

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);

  hostInterpreter(session);

  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0x20000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xE0, session->txBuffer[6]); //chksum
  TEST_ASSERT_EQUAL(1, isYielding);

  /* ################## Receive Tlv Acknowledgement ################## */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  delUserSession_Expect(&userSession);
  hostInterpreter(session);

  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_hostInterpreter_should_call_flash_mass_erase_if_flash_programmer_is_loaded(void) {
  int i = 0;
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  User_Session userSession;
  userSession.tlvCommand = TLV_FLASH_MASS_ERASE;
  userSession.address = BOTH_BANK;

  waitUserCommand_ExpectAndReturn(&userSession);

  printf("######################## Host interpreter #################### \n");

  isProgramExist_IgnoreAndReturn(VERIFY_FAILED);
  for(; i < 77; i++) {
    SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
    session->rxBuffer[0] = TLV_OK;
    session->rxBuffer[1] = 1;
    session->rxBuffer[2] = 0;

    getSystemTime_IgnoreAndReturn(10);
    isTimeout_IgnoreAndReturn(0);
    hostInterpreter(session);
  }
  TEST_ASSERT_EQUAL(1, isYielding);

  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;

  delUserSession_Expect(&userSession);
  hostInterpreter(session);

  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(2, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(BOTH_BANK, session->txBuffer[2]);
}

void test_hostInterpreter_should_readMemory_and_stop_when_size_reached_0(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  User_Session us;

  us.tlvCommand = TLV_READ_MEMORY;
  us.address = 0x20000000;
  us.size = 400;

  waitUserCommand_ExpectAndReturn(&us);

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  
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

  getSystemTime_ExpectAndReturn(10);
  isTimeout_ExpectAndReturn(FIVE_SECOND, 10, 0);
  
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

  delUserSession_Expect(&us);

  hostInterpreter(session);

  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
}