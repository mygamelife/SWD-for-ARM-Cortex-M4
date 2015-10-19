#include <malloc.h>
#include "Tlv.h"
#include "uart.h"
#include "Tlv_ex.h"
#include "unity.h"
#include "GetTime.h"
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

void test_tlvLoadToFlash_should_load_program_into_target_flash(void) {
  Tlv_Session *session = tlvCreateSession();
  CEXCEPTION_T err;
  int result = 0;
  
  Try {
    do {
      tlvService(session);
      tlvLoadToFlash(session, "scripts/led.elf");
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
  
  Try {
    do {
      tlvService(session);
      result = tlvHardReset(session);
      printf("yo\n");
    } while(result != PROCESS_DONE);
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  TEST_ASSERT_EQUAL(0, session->hresetState);
  closeSerialPort((HANDLE *)session->handler);
}

// void test_tlvMassErase_should_reply_after_erase_bank_1_is_done(void) {
  // Tlv_Session *session = tlvCreateSession();
  // CEXCEPTION_T err;
  

    
  // closeSerialPort((HANDLE *)session->handler);
// }

// void test_tlvHardReset_should_hard_reset_target(void) {
  // Tlv_Session *session = tlvCreateSession();
  // CEXCEPTION_T err;
  // int result = 0;
  

    
  // closeSerialPort((HANDLE *)session->handler);
// }