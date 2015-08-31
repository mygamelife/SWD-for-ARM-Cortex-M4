#include "unity.h"
#include <stdint.h>
#include "CoreDebug_Utilities.h"
#include "Misc_Utilities.h"

void setUp(void)  {}

void tearDown(void) {}

/*-------------------------------determineCoreModeFromDataRead---------------------------------*/
void test_determineCoreModeFromDataRead_given_0x0_should_return_CORE_NORMAL_MODE()
{
  int result = 0;
  result = doesCoreModeRequiresHaltedAndDebug(0);
  TEST_ASSERT_EQUAL(CORE_NORMAL_MODE, result);
}

void test_determineCoreModeFromDataRead_given_0x8_should_return_CORE_NORMAL_MASKINT()
{
  TEST_ASSERT_EQUAL_HEX32(CORE_NORMAL_MODE, 123);
  TEST_ASSERT_EQUAL(CORE_NORMAL_MASKINT,determineCoreModeFromDataRead(0x8));
}

void test_determineCoreModeFromDataRead_given_0x1_should_return_CORE_DEBUG_MODE()
{
  TEST_ASSERT_EQUAL(CORE_DEBUG_MODE,determineCoreModeFromDataRead(1));
}

void test_determineCoreModeFromDataRead_given_0x1_should_return_CORE_DEBUG_HALT()
{
  TEST_ASSERT_EQUAL(CORE_DEBUG_HALT,determineCoreModeFromDataRead(0x20003));
}


/*-------------------------------doesCoreModeRequiresHaltedAndDebug---------------------------------*/
void test_isCoreModeRequiresHaltedAndDebug_given_CORE_NORMAL_MASKINT_should_return_1()
{
  TEST_ASSERT_EQUAL(1,doesCoreModeRequiresHaltedAndDebug(CORE_NORMAL_MASKINT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SINGLE_STEP_should_return_1()
{
  TEST_ASSERT_EQUAL(1,doesCoreModeRequiresHaltedAndDebug(CORE_SINGLE_STEP));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SINGLE_STEP_MASKINT_should_return_1()
{
  TEST_ASSERT_EQUAL(1,doesCoreModeRequiresHaltedAndDebug(CORE_SINGLE_STEP_MASKINT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_NORMAL_MODE_should_return_0()
{
  TEST_ASSERT_EQUAL(0,doesCoreModeRequiresHaltedAndDebug(CORE_NORMAL_MODE));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_DEBUG_MODE_should_return_0()
{
  TEST_ASSERT_EQUAL(0,doesCoreModeRequiresHaltedAndDebug(CORE_DEBUG_MODE));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_DEBUG_HALT_should_return_0()
{
  TEST_ASSERT_EQUAL(0,doesCoreModeRequiresHaltedAndDebug(CORE_DEBUG_HALT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SNAPSTALL_should_return_0()
{
  TEST_ASSERT_EQUAL(0,doesCoreModeRequiresHaltedAndDebug(CORE_SNAPSTALL));
}

