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
#include "MemoryReadWrite.h"

void setUp(void) {}

void tearDown(void) {}

void test_write_word_0xABCD1234_should_read_back_the_same_data(void) {
  Tlv_Session *session = tlvCreateSession();
  CEXCEPTION_T err;
  int result = 0; uint32_t dataRead;
  
  Try {
    do {
      tlvService(session);
      result = memoryWriteWord(session, 0x20000000, 0xABCD1234);
    } while(result == 0);
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  Try {
    do {
      tlvService(session);
      result = memoryReadWord(session, 0x20000000, &dataRead);
    } while(result == 0);
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  TEST_ASSERT_EQUAL_HEX32(0xABCD1234, dataRead);
  
  closeSerialPort((HANDLE *)session->handler);
}