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
#include "SystemPath.h"
#include "mock_Uart.h"
#include "mock_SystemTime.h"
#include "mock_Interface.h"
#include "mock_ProgramVerifier.h"

void setUp(void)  {}

void tearDown(void) {}

void test_readMemory_should_read_store_the_finished_read_block_and_cont_until_size_is_0(void) {
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  uint32_t address = 0x20000000;
  uint8_t *db = NULL;
  int size = 280, i = 0;

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 249;
  //0xdeadbeef
  session->rxBuffer[2] = 0xef;
  session->rxBuffer[3] = 0xbe;
  session->rxBuffer[4] = 0xad;
  session->rxBuffer[5] = 0xde;
  session->rxBuffer[6] = 0xC8;

  getSystemTime_IgnoreAndReturn(10);
  isTimeout_IgnoreAndReturn(0);
  db = readMemory(session, address, size);

  TEST_ASSERT_EQUAL(1, isYielding);
  TEST_ASSERT_NULL(db);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 33;
  session->rxBuffer[2] = 0xaa;
  session->rxBuffer[3] = 0xaa;
  session->rxBuffer[4] = 0xaa;
  session->rxBuffer[5] = 0xaa;
  session->rxBuffer[6] = 0x58;

  db = readMemory(session, address, size);

  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_NOT_NULL(db);
  TEST_ASSERT_EQUAL_HEX32(0xdeadbeef, get4Byte(&db[0]));
  TEST_ASSERT_EQUAL_HEX32(0xaaaaaaaa, get4Byte(&db[248]));

  delDataBlock(db);
}

void test_eraseSection_should_load_flash_programmer_if_it_is_not_loaded_before_sending_request(void)
{
  int i = 0;
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  printf(" ####################### Erase Section #######################\n");

  for(; i < 80; i++) {
    /* Received reply */
    SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
    session->rxBuffer[0] = TLV_OK;
    session->rxBuffer[1] = 1;
    session->rxBuffer[2] = 0;

    getSystemTime_IgnoreAndReturn(10);
    isTimeout_IgnoreAndReturn(0);

    eraseSection(session, 0x081C0000, 20000);
  }

  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_reactiveProgram_should_update_pc_and_run_the_program(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  Program *p = getLoadableSection("test/ElfFiles/ledRam.elf");

  getSystemTime_IgnoreAndReturn(10);
  isTimeout_IgnoreAndReturn(0);
  int result = reactiveProgram(session, p);

  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  /* Set PC to entry Address */
  result = reactiveProgram(session, p);

  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL(1, isYielding);

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  /* Send request to run the program */
  result = reactiveProgram(session, p);

  TEST_ASSERT_EQUAL(1, result);
  TEST_ASSERT_EQUAL(0, isYielding);

  delProgram(p);
}

void test_eraseAll_should_send_reactive_flashProgrammer_instead_of_reload_and_send_request(void)
{
  int i = 0;
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  printf(" ####################### Mass Erase #######################\n");
  for(; i < 80; i++) {
    /* Received reply */
    SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
    session->rxBuffer[0] = TLV_OK;
    session->rxBuffer[1] = 1;
    session->rxBuffer[2] = 0;

    getSystemTime_IgnoreAndReturn(10);
    isTimeout_IgnoreAndReturn(0);
    eraseAll(session, BOTH_BANK);
  }

  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_loadFlash_should_request_flash_erase_section_and_program_Size(void)
{
  int i = 0;
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  Program *p = getLoadableSection("test/ElfFiles/ledFlash.elf");

  printf(" ####################### Load Flash #######################\n");
  for(; i < 108; i++) {
    /* Received reply */
    SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
    session->rxBuffer[0] = TLV_OK;
    session->rxBuffer[1] = 1;
    session->rxBuffer[2] = 0;

    getSystemTime_IgnoreAndReturn(10);
    isTimeout_IgnoreAndReturn(0);
    loadFlash(session, p);
  }

  TEST_ASSERT_EQUAL(0, isYielding);

  delProgram(p);
}