#include "unity.h"
#include "Tlv.h"
#include "TlvEx.h"
#include "Uart.h"
#include "SystemTime.h"
#include "ErrorCode.h"
#include "Read_File.h"
#include "GetHeaders.h"
#include "Interface.h"
#include "ProgramElf.h"
#include "ProgramLoader.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "StringObject.h"
#include "Token.h"
#include "IdentifierToken.h"
#include "NumberToken.h"
#include "OperatorToken.h"
#include "FileToken.h"

void setUp(void) {}

void tearDown(void) {}

// void test_tlvLoadToRam_should_load_program_into_target_Ram(void) {
  // Tlv_Session *session = tlvCreateSession();
  // CEXCEPTION_T err;
  // int result = 0;
  
  // Try {
    // do {
      // tlvService(session);
      // tlvLoadToRam(session, "test/ElfFiles/ledRam.elf");
    // } while(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_SET);
  // } Catch(err) {
    // displayErrorMessage(err);
  // }
  
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  // Try {
    // do {
      // tlvService(session);
      // tlvHardReset(session);
    // } while(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_SET);
  // } Catch(err) {
    // displayErrorMessage(err);
  // }
  
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  // TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  // closePort(session);
// }

void test_tlvLoadToFlash_should_load_program_into_target_flash(void) {
  Tlv_Session *session = tlvCreateSession();
  CEXCEPTION_T err;
  int result = 0;
  
  Try {
    do {
      tlvService(session);
      tlvLoadToFlash(session, "test/ElfFiles/ledFlash.elf");
    } while(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_SET);
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  Try {
    do {
      tlvService(session);
      tlvMassEraseTargetFlash(session, BANK_1);
    } while(GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG) == FLAG_SET);    
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_ONGOING_PROCESS_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  TEST_ASSERT_EQUAL(0, session->hresetState);
  closePort(session);
}