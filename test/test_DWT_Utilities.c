#include "unity.h"
#include "DWT_Utilities.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "Emulator.h"
#include "MemoryReadWrite.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "Delay.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/*-----checkForValidDWTComparator-------------*/
void test_checkForValidDWTComparator_given_0_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidDWTComparator(0));
}

void test_checkForValidDWTComparator_given_1_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidDWTComparator(1));
}

void test_checkForValidDWTComparator_given_2_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidDWTComparator(2));
}

void test_checkForValidDWTComparator_given_3_should_return_3()
{
  TEST_ASSERT_EQUAL(1,checkForValidDWTComparator(3));
}

void test_checkForValidDWTComparator_given_4_5_negative1_should_return_negative_1()
{
  TEST_ASSERT_EQUAL(-1,checkForValidDWTComparator(-1));
  TEST_ASSERT_EQUAL(-1,checkForValidDWTComparator(4));
  TEST_ASSERT_EQUAL(-1,checkForValidDWTComparator(5));
}