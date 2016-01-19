#include "unity.h"
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <unistd.h>
#include "SystemPath.h"

void setUp(void) {}

void tearDown(void) {}

void test_isCurrentDirectory_should_return_true_if_string_contain_build_release(void)
{
	TEST_ASSERT_EQUAL(1, isCurrentDirectory("c:\\User\\build\\release"));
}

void test_isCurrentDirectory_should_return_false_if_string_doesnt_contain_build_release(void)
{
	TEST_ASSERT_EQUAL(0, isCurrentDirectory("c:\\User\\"));
}

void test_isCurrentDirectory_should_return_false_if_string_contain_build(void)
{
	TEST_ASSERT_EQUAL(0, isCurrentDirectory("c:\\User\\build\\"));
}

void test_getFpPath_should_return_FP_INSIDE_CWD_if_inside_current_directory(void)
{
  TEST_ASSERT_EQUAL_STRING(FP_INSIDE_CWD, getFpPath("c:\\User\\build\\release"));
}

void test_getFpPath_should_return_FP_OUTSIDE_CWD_if_outside_of_current_directory(void)
{
  TEST_ASSERT_EQUAL_STRING(FP_OUTSIDE_CWD, getFpPath(getCurrentDirectory()));
}