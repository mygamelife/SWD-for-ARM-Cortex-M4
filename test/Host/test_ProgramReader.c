#include "unity.h"
#include "ProgramReader.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_getSelectiveSection_should_able_get_all_selective_section(void)
{
	Program *p = getSelectiveSection("ElfFiles\z");
  
  TEST_ASSERT_NOT_NULL(p);
}
