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
#include "LoadElf.h"
#include "GetHeaders.h"
#include "Read_File.h"

void setUp(void)  {}

void tearDown(void) {}

void test_createNewUserSession_should_initialize_all_neccessary_information(void)
{
  int i = 0;

  User_Session *us = createNewUserSession();

  TEST_ASSERT_NOT_NULL(us);
  TEST_ASSERT_NULL(us->program);
  TEST_ASSERT_EQUAL(0, us->size);
  TEST_ASSERT_EQUAL(0, us->address);
  TEST_ASSERT_EQUAL(0, us->tlvCommand);

  for(; i < 2048; i++) {
    TEST_ASSERT_EQUAL(0, us->data[i]);
  }
}

void test_userWriteRegister_user_should_enter_wreg_register_address_and_value(void)
{
  String *str = stringNew("wreg R0 0xabcd");

  User_Session *us = createNewUserSession();
  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_WRITE_REGISTER, us->tlvCommand);
  TEST_ASSERT_EQUAL(R0, us->address);
  TEST_ASSERT_EQUAL_HEX32(0xabcd, us->data[0]);

  stringDel(str);
  delUserSession(us);
}

void test_userWriteRegister_should_throw_error_if_user_didnt_enter_register_address(void)
{
  CEXCEPTION_T err;
  User_Session *us = createNewUserSession();
  String *str = stringNew("wreg 0xabcd");

  Try {
    InterpreteCommand(us, str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_INCOMPLETE_COMMAND, err);
  }

  stringDel(str);
  delUserSession(us);
}

void test_userWriteRegister_should_throw_error_if_user_didnt_enter_anything(void)
{
  CEXCEPTION_T err;
  String *str = stringNew("   ");
  User_Session *us = createNewUserSession();

  Try {
    InterpreteCommand(us, str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_EMPTY_STRING, err);
  }

  stringDel(str);
  delUserSession(us);
}

void test_userReadRegister_should_throw_error_if_user_didnt_enter_register_address(void)
{
  CEXCEPTION_T err;
  String *str = stringNew("reg");
  User_Session *us = createNewUserSession();

  Try {
    InterpreteCommand(us, str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_INCOMPLETE_COMMAND, err);
  }

  stringDel(str);
  delUserSession(us);
}

void test_userReadRegister_should_acquire_info_enter_by_user(void)
{
  String *str = stringNew("reg R0");
  User_Session *us = createNewUserSession();

  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_READ_REGISTER, us->tlvCommand);
  TEST_ASSERT_EQUAL(R0, us->address);

  stringDel(str);
  delUserSession(us);
}

void test_userReadRegister_given_invalid_register_address_should_throw_an_error(void)
{
  CEXCEPTION_T err;
  String *str = stringNew("reg AAAAA");
  User_Session *us = createNewUserSession();

  Try {
    InterpreteCommand(us, str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER_ADDRESS, err);
  }

  stringDel(str);
  delUserSession(us);
}

void test_userStepTarget_should_throw_error_when_number_of_step_is_not_number(void)
{
  CEXCEPTION_T err;
  String *str = stringNew("step abc");
  User_Session *us = createNewUserSession();

  Try {
    InterpreteCommand(us, str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_EXPECT_NUMBER, err);
  }

  stringDel(str);
  delUserSession(us);
}

void test_userStepTarget_should_get_step_insturction_and_number_of_step(void)
{
  String *str = stringNew("step 1000");
  User_Session *us = createNewUserSession();

  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_STEP, us->tlvCommand);
  TEST_ASSERT_EQUAL(1000, us->data[0]);

  stringDel(str);
  delUserSession(us);
}

void test_userReadMemory_should_throw_error_when_address_is_not_in_number(void)
{
  CEXCEPTION_T err;
  String *str = stringNew("read abcdabcd 14");
  User_Session *us = createNewUserSession();

  Try {
    InterpreteCommand(us, str);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(ERR_EXPECT_NUMBER, err);
  }

  stringDel(str);
  delUserSession(us);
}

void test_userReadMemory_should_get_address_and_size(void)
{
  CEXCEPTION_T err;
  String *str = stringNew("read 0x20000000 255");
  User_Session *us = createNewUserSession();

  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL_HEX32(0x20000000, us->address);
  TEST_ASSERT_EQUAL(255, us->size);

  stringDel(str);
  delUserSession(us);
}

void test_userLoadProgram_should_throw_an_error_if_path_name_is_invalid(void)
{
  CEXCEPTION_T err;
  String *str = stringNew("load ram aaaa");
  User_Session *us = createNewUserSession();

  Try {
    InterpreteCommand(us, str);
  } Catch(err) {
    TEST_ASSERT_EQUAL(ERR_EXPECT_FILE_PATH, err);
  }

  stringDel(str);
  delUserSession(us);
}

void test_userRunTarget_should_get_run_command(void)
{
  String *str = stringNew("run");
  User_Session *us = createNewUserSession();

  InterpreteCommand(us, str);
  
  TEST_ASSERT_EQUAL(TLV_RUN_TARGET, us->tlvCommand);

  stringDel(str);
  delUserSession(us);
}

void test_userHaltTarget_should_get_halt_command(void)
{
  String *str = stringNew("halt");
  User_Session *us = createNewUserSession();

  InterpreteCommand(us, str);
  
  TEST_ASSERT_EQUAL(TLV_HALT_TARGET, us->tlvCommand);

  stringDel(str);
  delUserSession(us);
}

void test_userWriteMemory_should_get_data_address_and_size(void)
{
  String *str = stringNew("write ram 0x20000000 0xabc 0x123 0xdeaf 0xbeef");
  User_Session *us = createNewUserSession();

  InterpreteCommand(us, str);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, us->tlvCommand);
  TEST_ASSERT_EQUAL(16, us->size);
  TEST_ASSERT_EQUAL_HEX32(0xabc, us->data[0]);
  TEST_ASSERT_EQUAL_HEX32(0x123, us->data[1]);
  TEST_ASSERT_EQUAL_HEX32(0xdeaf, us->data[2]);
  TEST_ASSERT_EQUAL_HEX32(0xbeef, us->data[3]);

  stringDel(str);
  delUserSession(us);
}

void test_userSetBreakpoint_should_get_address_need_to_be_set_break_point(void)
{
  String *str = stringNew("brkpt 0x20000000");
  User_Session *us = createNewUserSession();
  
  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_BREAKPOINT, us->tlvCommand);
  TEST_ASSERT_EQUAL(0x20000000, us->address);
  
  stringDel(str);
  delUserSession(us);
}

void test_userErase_should_get_erase_section_option(void)
{
  String *str = stringNew("erase section 0x08000000 20");
  User_Session *us = createNewUserSession();
  
  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_FLASH_ERASE, us->tlvCommand);
  TEST_ASSERT_EQUAL(0x08000000, us->address);
  TEST_ASSERT_EQUAL(20, us->size);
  
  stringDel(str);
  delUserSession(us);
}

void test_userErase_should_get_erase_bank_1_option(void)
{
  String *str = stringNew("erase bank1");
  User_Session *us = createNewUserSession();
    
  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, us->tlvCommand);
  TEST_ASSERT_EQUAL(BANK_1, us->address);
  
  stringDel(str);
  delUserSession(us);
}

void test_userErase_should_get_erase_bank_2_option(void)
{
  String *str = stringNew("erase bank2");
  User_Session *us = createNewUserSession();
  
  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, us->tlvCommand);
  TEST_ASSERT_EQUAL(BANK_2, us->address);
  
  stringDel(str);
  delUserSession(us);
}

void test_userErase_should_get_erase_full_option(void)
{
  String *str = stringNew("erase full");
  User_Session *us = createNewUserSession();
  
  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_FLASH_MASS_ERASE, us->tlvCommand);
  TEST_ASSERT_EQUAL(BOTH_BANK, us->address);
  
  stringDel(str);
  delUserSession(us);
}

void test_userErase_should_throw_error(void)
{
  CEXCEPTION_T err;
  String *str = stringNew("erase aaxxx");
  User_Session *us = createNewUserSession();
  
  Try {
    InterpreteCommand(us, str);
  } Catch(err) {
    TEST_ASSERT_EQUAL(ERR_INVALID_BANK_SELECTION, err);
  }
  
  stringDel(str);
  delUserSession(us);
}

void test_userReset_should_get_reset_option(void)
{
  String *str = stringNew("reset hard");
  User_Session *us = createNewUserSession();
  
  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_HARD_RESET, us->tlvCommand);
  
  stringDel(str);
  delUserSession(us);
}

void test_userExit_should_get_exit_command(void)
{
  String *str = stringNew("exit");
  User_Session *us = createNewUserSession();
  
  InterpreteCommand(us, str);

  TEST_ASSERT_EQUAL(TLV_EXIT, us->tlvCommand);
  
  stringDel(str);
  delUserSession(us);
}

void test_displayMemoryMap_given_address_and_value(void)
{
  uint32_t data[] = {0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd,
                     0xabcdabcd, 0xabcdabcd, 0xabcdabcd, 0xabcdabcd};

  displayMemoryMap((uint8_t *)data, 0x08000000, 160);
}