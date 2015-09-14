#include "unity.h"
#include "CException.h"
#include "StringObject.h"
#include "Token.h"
#include "IdentifierToken.h"
#include "NumberToken.h"
#include "OperatorToken.h"
#include "Interface.h"

void setUp(void)  {}

void tearDown(void) {}

void test_userWriteRegister_user_should_enter_wreg_register_address_and_value(void)
{
  String *str = stringNew("wreg R0 0xabcd");
  
  User_Session *session = userInputInterpreter(str);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_REGISTER, session->tlvCommand);
  TEST_ASSERT_EQUAL(R0, session->address);
  TEST_ASSERT_EQUAL_HEX32(0xabcd, get4Byte(&session->data[0]));
}
