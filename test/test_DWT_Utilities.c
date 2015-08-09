#include "unity.h"
#include "DWT_Utilities.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "Clock.h"
#include "Emulator.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "mock_configurePort.h"
#include "mock_IO_Operations.h"
void setUp(void)
{
}

void tearDown(void)
{
}

/*-----getDWTComparatorNumber-------------*/
void test_getDWTComparatorNumber_given_DWT_COMP0_should_return_0()
{
  TEST_ASSERT_EQUAL(0,getDWTComparatorNumber(DWT_COMP0));
}

void test_getDWTComparatorNumber_given_DWT_COMP1_should_return_1()
{
  TEST_ASSERT_EQUAL(1,getDWTComparatorNumber(DWT_COMP1));
}

void test_getDWTComparatorNumber_given_DWT_COMP2_should_return_2()
{
  TEST_ASSERT_EQUAL(2,getDWTComparatorNumber(DWT_COMP2));
}

void test_getDWTComparatorNumber_given_DWT_COMP3_should_return_3()
{
  TEST_ASSERT_EQUAL(3,getDWTComparatorNumber(DWT_COMP3));
}

void test_getDWTComparatorNumber_given_unknown_DWT_COMP_should_return_negative_1()
{
  TEST_ASSERT_EQUAL(-1,getDWTComparatorNumber(0));
  TEST_ASSERT_EQUAL(-1,getDWTComparatorNumber(10000));
  TEST_ASSERT_EQUAL(-1,getDWTComparatorNumber(00));
}