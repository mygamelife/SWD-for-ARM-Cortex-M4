#include "unity.h"
#include "FPB_Utilities.h"
#include "mock_FPB_Unit.h"
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

void test_checkForValidInstructionComparator_given_0_to_5_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(0));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(1));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(2));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(3));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(4));
  TEST_ASSERT_EQUAL(1,checkForValidInstructionComparator(5));

}


void test_checkForValidInstructionComparator_given_unkown_value_should_return_minus_1()
{
  TEST_ASSERT_EQUAL(-1,checkForValidInstructionComparator(0x100));
  TEST_ASSERT_EQUAL(-1,checkForValidInstructionComparator(-1));
  TEST_ASSERT_EQUAL(-1,checkForValidInstructionComparator(-3));
  TEST_ASSERT_EQUAL(-1,checkForValidInstructionComparator(6));
}

/*-------------------------checkForValidLiteralComparator-----------------------*/
void test_checkForValidLiteralComparator_given_LITERAL_COMP0_to_LITERAL_COMP1_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidLiteralComparator(LITERAL_COMP0));
  TEST_ASSERT_EQUAL(1,checkForValidLiteralComparator(LITERAL_COMP1));
}

void test_checkForValidLiteralComparator_given_0_to_1_should_return_1()
{
  TEST_ASSERT_EQUAL(1,checkForValidLiteralComparator(0));
  TEST_ASSERT_EQUAL(1,checkForValidLiteralComparator(1));
}

void test_checkForValidLiteralComparator_given_unkown_value_should_return_minus_1()
{
  TEST_ASSERT_EQUAL(-1,checkForValidLiteralComparator(0x100));
  TEST_ASSERT_EQUAL(-1,checkForValidLiteralComparator(-1));
  TEST_ASSERT_EQUAL(-1,checkForValidLiteralComparator(-3));
  TEST_ASSERT_EQUAL(-1,checkForValidLiteralComparator(3));
  
}

/*--------------------------selectNextFreeComparator-----------------------*/
//INSTRUCTION_TYPE
void test_selectNextFreeInstructionComparator_given_all_free_should_return_INSTRUCTION_COMP0()
{
  instructionComparatorReady[0] = COMP_READY ;
  instructionComparatorReady[1] = COMP_READY ;
  instructionComparatorReady[2] = COMP_READY ;
  instructionComparatorReady[3] = COMP_READY ;
  instructionComparatorReady[4] = COMP_READY ;
  instructionComparatorReady[5] = COMP_READY ;
  
  readAndUpdateComparatorReadyFlag_Ignore();
  
  TEST_ASSERT_EQUAL(INSTRUCTION_COMP0,selectNextFreeComparator(INSTRUCTION_TYPE));
}

void test_selectNextFreeInstructionComparator_given_COMP0_busy_should_return_INSTRUCTION_COMP1()
{
  instructionComparatorReady[0] = COMP_BUSY;
  instructionComparatorReady[1] = COMP_READY ;
  instructionComparatorReady[2] = COMP_READY ;
  instructionComparatorReady[3] = COMP_READY ;
  instructionComparatorReady[4] = COMP_READY ;
  instructionComparatorReady[5] = COMP_READY ;
  
  readAndUpdateComparatorReadyFlag_Ignore();
  
  TEST_ASSERT_EQUAL(INSTRUCTION_COMP1,selectNextFreeComparator(INSTRUCTION_TYPE));
}

void test_selectNextFreeInstructionComparator_given_all_busy_should_return_negative_1()
{
  instructionComparatorReady[0] = COMP_BUSY;
  instructionComparatorReady[1] = COMP_BUSY ;
  instructionComparatorReady[2] = COMP_BUSY ;
  instructionComparatorReady[3] = COMP_BUSY ;
  instructionComparatorReady[4] = COMP_BUSY ;
  instructionComparatorReady[5] = COMP_BUSY ;
  
  readAndUpdateComparatorReadyFlag_Ignore();
  
  TEST_ASSERT_EQUAL(-1,selectNextFreeComparator(INSTRUCTION_TYPE));
}

//LITERAL_TYPE
void test_selectNextFreeInstructionComparator_given_LiteralComp0_should_return_LiteralComp0()
{
  literalComparatorReady[0] = COMP_READY;
  literalComparatorReady[1] = COMP_BUSY;
  
  readAndUpdateComparatorReadyFlag_Ignore();
  
  TEST_ASSERT_EQUAL(LITERAL_COMP0,selectNextFreeComparator(LITERAL_TYPE));
}

void test_selectNextFreeInstructionComparator_given_LiteralComp1_should_return_LiteralComp1()
{
  literalComparatorReady[0] = COMP_BUSY;
  literalComparatorReady[1] = COMP_READY;
  
  readAndUpdateComparatorReadyFlag_Ignore();
  
  TEST_ASSERT_EQUAL(LITERAL_COMP1,selectNextFreeComparator(LITERAL_TYPE));
}

void test_selectNextFreeInstructionComparator_given_all_literal_COMP_busy_should_return_neg_1()
{
  literalComparatorReady[0] = COMP_BUSY;
  literalComparatorReady[1] = COMP_BUSY;
  
  readAndUpdateComparatorReadyFlag_Ignore();
  
  TEST_ASSERT_EQUAL(-1,selectNextFreeComparator(LITERAL_TYPE));
}

/*--------------------------swapHalfword-----------------------*/
void test_swapHalfword_given_0xAABBCCDD_should_return_0xCCDDAABB()
{
  TEST_ASSERT_EQUAL(0xCCDDAABB,swapHalfword(0xAABBCCDD));
}

void test_swapHalfword_given_0x1111AAAA_should_return_0xAAAA1111()
{
  TEST_ASSERT_EQUAL(0xAAAA1111,swapHalfword(0x1111AAAA));
}