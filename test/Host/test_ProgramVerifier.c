#include "unity.h"
#include "ProgramVerifier.h"
#include "Tlv.h"
#include "TlvEx.h"
#include "Read_File.h"
#include "GetHeaders.h"
#include "ProgramElf.h"
#include "mock_ProgramLoader.h"
#include "ErrorCode.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "SystemTime.h"
#include "Yield.h"
#include "LoadElf.h"
#include "mock_Uart.h"
#include "mock_Interface.h"

void setUp(void) {}

void tearDown(void) {}

void test_verifyLoadedProgram_if_same_should_return_1(void) {
  int result = 0;
  uint8_t lp[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                  0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
          ap[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                  0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
                  
  result = verifyLoadedProgram(lp, ap, sizeof(ap));
  
  TEST_ASSERT_EQUAL(1, result);
}

void test_verifyLoadedProgram_if_not_same_should_return_0(void) {
  int result = 0;
  uint8_t lp[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                  0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
          ap[] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                  0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xef};
                  
  result = verifyLoadedProgram(lp, ap, sizeof(ap));
  
  TEST_ASSERT_EQUAL(0, result);
}

void test_isProgramExist_should_return_VERIFY_FAILED_if_program_is_not_same_as_actual(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  uint8_t data[] = {0xaa, 0xbb, 0xcc, 0xdd};
  Program *p = getLoadableSection("test/ElfFiles/ledRam.elf");  
  
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 0), getProgramSize(p, 0), data);
  delDataBlock_Expect(data);
  int result = isProgramExist(session, p);
  
  TEST_ASSERT_EQUAL(VERIFY_FAILED, result);
  TEST_ASSERT_EQUAL(0, isYielding);
  
  delProgram(p);
}

void test_isProgramExist_should_return_VERIFY_PASSED_if_program_is_same_as_actual(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  Program *p = getLoadableSection("test/ElfFiles/ledRam.elf");  
  
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 0), getProgramSize(p, 0), getProgramData(p, 0));
  delDataBlock_Expect(getProgramData(p, 0));
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 1), getProgramSize(p, 1), getProgramData(p, 1));
  delDataBlock_Expect(getProgramData(p, 1));
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 2), getProgramSize(p, 2), getProgramData(p, 2));
  delDataBlock_Expect(getProgramData(p, 2));
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 3), getProgramSize(p, 3), getProgramData(p, 3));
  delDataBlock_Expect(getProgramData(p, 3));
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 4), getProgramSize(p, 4), getProgramData(p, 4));
  delDataBlock_Expect(getProgramData(p, 4));
  
  int result = isProgramExist(session, p);
  
  TEST_ASSERT_EQUAL(VERIFY_PASSED, result);
  TEST_ASSERT_EQUAL(0, isYielding);
  
  delProgram(p);
}

void test_isProgramExist_load_flash_program_should_return_VERIFY_PASSED_if_program_is_same_as_actual(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();

  Program *p = getLoadableSection("test/ElfFiles/ledFlash.elf");  
  
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 0), getProgramSize(p, 0), getProgramData(p, 0));
  delDataBlock_Expect(getProgramData(p, 0));
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 1), getProgramSize(p, 1), getProgramData(p, 1));
  delDataBlock_Expect(getProgramData(p, 1));
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 2), getProgramSize(p, 2), getProgramData(p, 2));
  delDataBlock_Expect(getProgramData(p, 2));
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 3), getProgramSize(p, 3), getProgramData(p, 3));
  delDataBlock_Expect(getProgramData(p, 3));
  readMemory_ExpectAndReturn(session, getProgramAddress(p, 4), getProgramSize(p, 4), getProgramData(p, 4));
  delDataBlock_Expect(getProgramData(p, 4));
  
  int result = isProgramExist(session, p);
  
  TEST_ASSERT_EQUAL(VERIFY_PASSED, result);
  TEST_ASSERT_EQUAL(0, isYielding);
  
  delProgram(p);
}