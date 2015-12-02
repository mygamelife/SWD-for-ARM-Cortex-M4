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

void test_readMemory_should_read_without_pointer_address_and_size(void) {
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

void Xtest_isProgramExit(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  isProgramExist(session, "test/ElfFiles/ledRam.elf");

  /* Received reply */
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  isProgramExist(session, "test/ElfFiles/ledRam.elf");
  
  TEST_ASSERT_EQUAL(0, isYielding);
  // TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
  // TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
}