#include "unity.h"
#include "CoreDebug_Utilities.h"
#include "Misc_Utilities.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/*-------------------------------determineCoreModeFromDataRead---------------------------------*/
void test_determineCoreModeFromDataRead_given_0x0_should_return_CORE_NORMAL_MODE()
{
  TEST_ASSERT_EQUAL(CORE_NORMAL_MODE,determineCoreModeFromDataRead(0));
}

void test_determineCoreModeFromDataRead_given_0x8_should_return_CORE_NORMAL_MASKINT()
{
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


/*-------------------------------isCoreModeRequiresHaltedAndDebug---------------------------------*/
void test_isCoreModeRequiresHaltedAndDebug_given_CORE_NORMAL_MASKINT_should_return_1()
{
  TEST_ASSERT_EQUAL(1,isCoreModeRequiresHaltedAndDebug(CORE_NORMAL_MASKINT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SINGLE_STEP_should_return_1()
{
  TEST_ASSERT_EQUAL(1,isCoreModeRequiresHaltedAndDebug(CORE_SINGLE_STEP));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SINGLE_STEP_MASKINT_should_return_1()
{
  TEST_ASSERT_EQUAL(1,isCoreModeRequiresHaltedAndDebug(CORE_SINGLE_STEP_MASKINT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_NORMAL_MODE_should_return_0()
{
  TEST_ASSERT_EQUAL(0,isCoreModeRequiresHaltedAndDebug(CORE_NORMAL_MODE));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_DEBUG_MODE_should_return_0()
{
  TEST_ASSERT_EQUAL(0,isCoreModeRequiresHaltedAndDebug(CORE_DEBUG_MODE));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_DEBUG_HALT_should_return_0()
{
  TEST_ASSERT_EQUAL(0,isCoreModeRequiresHaltedAndDebug(CORE_DEBUG_HALT));
}

void test_isCoreModeRequiresHaltedAndDebug_given_CORE_SNAPSTALL_should_return_0()
{
  TEST_ASSERT_EQUAL(0,isCoreModeRequiresHaltedAndDebug(CORE_SNAPSTALL));
}
/*-------------------------------getCoreModeConfiguration-----------------------------------------*/

void test_getCoreModeConfiguration_given_CORE_NORMAL_MODE_should_return_0xA05F0000_or_SET_CORE_NORMAL()
{
	TEST_ASSERT_EQUAL(0xA05F0000,getCoreModeConfiguration(CORE_NORMAL_MODE));
	TEST_ASSERT_EQUAL(SET_CORE_NORMAL,getCoreModeConfiguration(CORE_NORMAL_MODE));
}

void test_getCoreModeConfiguration_given_CORE_NORMAL_MODE_should_return_0xA05F0008_or_SET_CORE_NORMAL_MASKINT()
{
	TEST_ASSERT_EQUAL(0xA05F0008,getCoreModeConfiguration(CORE_NORMAL_MASKINT));
	TEST_ASSERT_EQUAL(SET_CORE_NORMAL_MASKINT,getCoreModeConfiguration(CORE_NORMAL_MASKINT));
}

void test_getCoreModeConfiguration_given_CORE_DEBUG_MODE_should_return_0xA05F0001_or_SET_CORE_DEBUG()
{
	TEST_ASSERT_EQUAL(0xA05F0001,getCoreModeConfiguration(CORE_DEBUG_MODE));
	TEST_ASSERT_EQUAL(SET_CORE_DEBUG,getCoreModeConfiguration(CORE_DEBUG_MODE));
}

void test_getCoreModeConfiguration_given_CORE_DEBUG_HALT_should_return_0xA05F0003_or_SET_CORE_DEBUG_HALT()
{
	TEST_ASSERT_EQUAL(0xA05F0003,getCoreModeConfiguration(CORE_DEBUG_HALT));
	TEST_ASSERT_EQUAL(SET_CORE_DEBUG_HALT,getCoreModeConfiguration(CORE_DEBUG_HALT));
}

void test_getCoreModeConfiguration_given_CORE_SINGLE_STEP_should_return_0xA05F0005_or_SET_CORE_STEP()
{
	TEST_ASSERT_EQUAL(0xA05F0005,getCoreModeConfiguration(CORE_SINGLE_STEP));
	TEST_ASSERT_EQUAL(SET_CORE_STEP,getCoreModeConfiguration(CORE_SINGLE_STEP));
}

void test_getCoreModeConfiguration_given_CORE_SINGLE_STEP_MASKINT_should_return_0xA05F000D_or_SET_CORE_STEP_MASKINT()
{
	TEST_ASSERT_EQUAL(0xA05F000D,getCoreModeConfiguration(CORE_SINGLE_STEP_MASKINT));
	TEST_ASSERT_EQUAL(SET_CORE_STEP_MASKINT,getCoreModeConfiguration(CORE_SINGLE_STEP_MASKINT));
}

void test_getCoreModeConfiguration_given_CORE_SNAPSTALL_should_return_0xA05F00023_or_SET_CORE_SNAPSTALL()
{
	TEST_ASSERT_EQUAL(0xA05F0023,getCoreModeConfiguration(CORE_SNAPSTALL));
	TEST_ASSERT_EQUAL(SET_CORE_SNAPSTALL,getCoreModeConfiguration(CORE_SNAPSTALL));
}