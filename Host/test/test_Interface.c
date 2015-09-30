#include "unity.h"
#include "CException.h"
#include "StringObject.h"
#include "Token.h"
#include "IdentifierToken.h"
#include "NumberToken.h"
#include "OperatorToken.h"
#include "FileToken.h"
#include "Interface.h"
#include "ErrorCode.h"

void setUp(void)  {}

void tearDown(void) {}

void test_userWriteRegister_user_should_enter_wreg_register_address_and_value(void)
{
  String *str = stringNew("wreg R0 0xabcd");
  
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_REGISTER, session->tlvCommand);
  TEST_ASSERT_EQUAL(R0, session->address);
  TEST_ASSERT_EQUAL_HEX32(0xabcd, get4Byte(&session->data[0]));
}

void test_userWriteRegister_should_throw_error_if_user_didnt_enter_register_address(void)
{
  CEXCEPTION_T err;
  
  Try {
    String *str = stringNew("wreg 0xabcd");
    User_Session *session = InterpreteCommand(str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_INCOMPLETE_COMMAND, err);
  }
}

void test_userWriteRegister_should_throw_error_if_user_didnt_enter_anything(void)
{
  CEXCEPTION_T err;
  
  Try {
    String *str = stringNew("   ");
    User_Session *session = InterpreteCommand(str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_EMPTY_STRING, err);
  }
}

void test_userReadRegister_should_throw_error_if_user_didnt_enter_register_address(void)
{
  CEXCEPTION_T err;
  
  Try {
    String *str = stringNew("reg");
    User_Session *session = InterpreteCommand(str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_INCOMPLETE_COMMAND, err);
  }
}

void test_userReadRegister_should_acquire_info_enter_by_user(void)
{
  String *str = stringNew("reg R0");
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_READ_REGISTER, session->tlvCommand);
  TEST_ASSERT_EQUAL(R0, session->address);
}

void test_userReadRegister_given_invalid_register_address_should_throw_an_error(void)
{
  CEXCEPTION_T err;
  
  Try {
    String *str = stringNew("reg AAAAA");
    User_Session *session = InterpreteCommand(str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER_ADDRESS, err);
  }
}

void test_userStepTarget_should_throw_error_when_number_of_step_is_not_number(void)
{
  CEXCEPTION_T err;
  
  Try {
    String *str = stringNew("step abc");
    User_Session *session = InterpreteCommand(str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_EXPECT_NUMBER, err);
  }
}

void test_userReadMemory_should_throw_error_when_address_is_not_in_number(void)
{
  CEXCEPTION_T err;
  
  Try {
    String *str = stringNew("rmem abcdabcd 14");
    User_Session *session = InterpreteCommand(str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_EXPECT_NUMBER, err);
  }
}

void test_userLoadProgram_should_get_load_instruction_memory_selection_and_file_path(void)
{
  String *str = stringNew("load ram C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/Host/test");
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, session->tlvCommand);
  TEST_ASSERT_EQUAL_STRING("C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/Host/test", session->fileName);
}

void test_userLoadProgram_should_throw_an_error_if_path_name_is_invalid(void)
{
  CEXCEPTION_T err;
  
  Try {
    String *str = stringNew("load ram aaaa");
    User_Session *session = InterpreteCommand(str);    
  } Catch(err) {
    TEST_ASSERT_EQUAL(ERR_EXPECT_FILE_PATH, err);
  }
}

void test_userSetBreakpoint_should_get_address_need_to_be_set_break_point(void)
{
  String *str = stringNew("brkpt 0x20000000");
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_BREAKPOINT, session->tlvCommand);
  TEST_ASSERT_EQUAL(0x20000000, session->address);
}

void test_userErase_should_get_erase_section_option(void)
{
  String *str = stringNew("erase section 0x08000000 20");
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_FLASH_ERASE, session->tlvCommand);
  TEST_ASSERT_EQUAL(0x08000000, session->address);
  TEST_ASSERT_EQUAL(20, session->size);
}

void test_userErase_should_get_erase_bank_1_option(void)
{
  String *str = stringNew("erase bank1");
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->tlvCommand);
  TEST_ASSERT_EQUAL(BANK_1, session->address);
}

void test_userErase_should_get_erase_bank_2_option(void)
{
  String *str = stringNew("erase bank2");
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->tlvCommand);
  TEST_ASSERT_EQUAL(BANK_2, session->address);
}

void test_userErase_should_get_erase_full_option(void)
{
  String *str = stringNew("erase full");
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, session->tlvCommand);
  TEST_ASSERT_EQUAL(BOTH_BANK, session->address);
}

void test_userErase_should_throw_error(void)
{
  CEXCEPTION_T err;
  
  Try {
    String *str = stringNew("erase aaxxx");
    User_Session *session = InterpreteCommand(str);    
  } Catch(err) {
    TEST_ASSERT_EQUAL(ERR_INVALID_BANK_SELECTION, err);
  }
}

void test_userReset_should_get_reset_option(void)
{
  String *str = stringNew("reset hard");
  User_Session *session = InterpreteCommand(str);
  
  TEST_ASSERT_EQUAL(TLV_HARD_RESET, session->tlvCommand);
}