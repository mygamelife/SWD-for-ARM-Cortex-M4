#include "unity.h"
#include "FPB_Utilities.h"
#include <stdint.h>
#include <string.h>
void setUp(void)
{
}

void tearDown(void)
{
}

/*-------------------------checkForValidInstructionComparator-----------------------*/
void test_checkForValidInstructionComparator_given_INSTRUCTION_COMP0_to_INSTRUCTION_COMP5_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(INSTRUCTION_COMP0));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(INSTRUCTION_COMP1));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(INSTRUCTION_COMP2));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(INSTRUCTION_COMP3));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(INSTRUCTION_COMP4));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(INSTRUCTION_COMP5));

}

void test_checkForValidInstructionComparator_given_unkown_value_should_return_minus_1()
{
  TEST_ASSERT_EQUAL(-1,checkForValidInstructionComparator(0x100));
  TEST_ASSERT_EQUAL(-1,checkForValidInstructionComparator(0x1));
  TEST_ASSERT_EQUAL(-1,checkForValidInstructionComparator(0x2));
  TEST_ASSERT_EQUAL(-1,checkForValidInstructionComparator(0));
  
}

/*-------------------------checkForValidLiteralComparator-----------------------*/
void test_given_LITERAL_COMP0_to_LITERAL_COMP1_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidLiteralComparator(LITERAL_COMP0));
  TEST_ASSERT_EQUAL(1,checkForValidLiteralComparator(LITERAL_COMP1));
}

void test_checkForValidLiteralComparator_given_unkown_value_should_return_minus_1()
{
  TEST_ASSERT_EQUAL(-1,checkForValidLiteralComparator(0x100));
  TEST_ASSERT_EQUAL(-1,checkForValidLiteralComparator(0x1));
  TEST_ASSERT_EQUAL(-1,checkForValidLiteralComparator(0x2));
  TEST_ASSERT_EQUAL(-1,checkForValidLiteralComparator(0));
  
}

/*--------------------------checkForValidFPComparator-----------------------*/
void test_given_INSTRUCTION_COMP0_to_INSTRUCTION_COMP5_and_LITERAL_COMP0_to_LITERAL_COMP1_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidFPComparator(INSTRUCTION_COMP0));
  TEST_ASSERT_EQUAL(1,checkForValidFPComparator(INSTRUCTION_COMP1));
  TEST_ASSERT_EQUAL(1,checkForValidFPComparator(INSTRUCTION_COMP2));
  TEST_ASSERT_EQUAL(1,checkForValidFPComparator(INSTRUCTION_COMP3));
  TEST_ASSERT_EQUAL(1,checkForValidFPComparator(INSTRUCTION_COMP4));
  TEST_ASSERT_EQUAL(1,checkForValidFPComparator(INSTRUCTION_COMP5));
  TEST_ASSERT_EQUAL(1,checkForValidFPComparator(LITERAL_COMP0));
  TEST_ASSERT_EQUAL(1,checkForValidFPComparator(LITERAL_COMP1));
}