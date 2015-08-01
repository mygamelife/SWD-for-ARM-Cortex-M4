#include "unity.h"
#include "TLV_Host.h"
#include "TLV_Protocol.h"
#include "mock_Serial.h"
#include "GetHeaders.h"
#include "Read_File.h"
#include "CException.h"
#include "ErrorCode.h"
#include "ProgramElf.h"
#include <stdlib.h>

void setUp(void)  {}

void tearDown(void) {}

void test_tlvHost_given_TLV_START_should_create_packet_and_change_state_to_TLV_SEND_TYPE()
{
  /*
  typedef struct {
  TlvState *pState;
  HANDLE hSerial;
  ElfData *pElfData;
} TLVSession;*/
}

/*
void test_tlvHost_given_TLV_WAIT_REPLY_should_wait_reply_from_probe()
{
  const char *comPort = "COM7";
  TlvState state = TLV_WAIT_REPLY;
  HANDLE hSerial;
  
  uint8_t rxBuffer[1024] = {0x10};
  serialGetBytes_ExpectAndReturn(hSerial, rxBuffer, ONE_BYTE, sizeof(rxBuffer));
  
  tlvHost(&state, hSerial);
  TEST_ASSERT_EQUAL(state, TLV_TRANSMIT_DATA);
}*/
