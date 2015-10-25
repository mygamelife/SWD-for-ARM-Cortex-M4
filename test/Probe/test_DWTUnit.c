#include "unity.h"
#include <stdint.h>
#include "DWTUnit.h"
#include "DWTUnitEx.h"
#include "CoreDebug.h"
#include "CoreDebugEx.h"
#include "Emulator.h"
#include "MemoryReadWrite.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "Delay.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"
#include "core_cm4.h"

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

/*------------------setAddressWatchpoint--------------*/
void test_setAddressWatchpoint_given_DWT_COMP0_address_0x12345677_mask_WATCHPOINTMASK_BIT0_WATCHPOINT_READ()
{

  //Faking CSW to Byte Size
  cswDataSize = CSW_BYTE_SIZE ;

  //Enable Global enable for DWT
  emulateSwdRegisterWrite(TAR_REG,AP,4,DEMCR_REG+3);
  emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_DWT_ITM << 24);

  // Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  // Set CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  // Disable comparator first
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[0].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  // Program comparator 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[0].COMP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x12345677);
 
  // Program mask 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[0].MASK));
	emulateSwdRegisterWrite(DRW_REG,AP,4,WATCHPOINT_MASK_BIT0);
  
  // Program function
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[0].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,WATCHPOINT_READ);
  
  
  TEST_ASSERT_EQUAL(0,setAddressWatchpoint(COMPARATOR_0,0x12345677,WATCHPOINT_MASK_BIT0,WATCHPOINT_READ));
}

/*------------------setDataWatchpoint_MatchingOneComparator--------------*/

void test_setDataWatchpoint_MatchingOneComparator()
{
  uint32_t configData = 0 ;
  configData = (2 << 16) + (2 << 12) + (WATCHPOINT_WORD << 10) + (DATA_COMPARISON << 8) + WATCHPOINT_WRITE ;
  
  //Faking CSW to Byte Size
  cswDataSize = CSW_BYTE_SIZE ;

  //Enable Global enable for DWT
  emulateSwdRegisterWrite(TAR_REG,AP,4,0xE000EDFF);
  emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_DWT_ITM <<24 );

  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Set CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //Disable matching comparator
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[2].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  //Program matching comparator 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[2].COMP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xFFFFFFFF);
  
  //Program matching mask 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[2].MASK));
	emulateSwdRegisterWrite(DRW_REG,AP,4,WATCHPOINT_MASK_BIT14_BIT0);
  
  //Disable comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  //Program comparator 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].COMP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xABCDEF12);
  
  //Program mask 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].MASK));
	emulateSwdRegisterWrite(DRW_REG,AP,4,WATCHPOINT_MASK_NOTHING);
  
  //Program function
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,configData);
  
  TEST_ASSERT_EQUAL(0,setDataWatchpoint_MatchingOneComparator(COMPARATOR_2,0xFFFFFFFF,WATCHPOINT_MASK_BIT14_BIT0,
                                                              0xABCDEF12,WATCHPOINT_WORD,WATCHPOINT_WRITE));
}

/*------------------setDataWatchpoint_MatchingTwoComparator--------------*/
void test_setDataWatchpoint_MatchingTwoComparator()
{
  uint32_t configData = 0 ;
  configData = (3 << 16) + (2 << 12) + (WATCHPOINT_BYTE << 10) + (DATA_COMPARISON << 8) + WATCHPOINT_READWRITE ;

  //Faking CSW to Byte Size
  cswDataSize = CSW_BYTE_SIZE ;

  //Enable Global enable for DWT
  emulateSwdRegisterWrite(TAR_REG,AP,4,0xE000EDFF);
  emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_DWT_ITM <<24);

  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Set CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //Disable 1st matching comparator
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[2].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  //Program 1st matching comparator 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[2].COMP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xBEEEEEEF);
  
  //Program 1st matching mask 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[2].MASK));
	emulateSwdRegisterWrite(DRW_REG,AP,4,WATCHPOINT_MASK_BIT0);
  
  //Set CSW to Byte Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
  
  //Enable Global enable for DWT
  emulateSwdRegisterWrite(TAR_REG,AP,4,0xE000EDFF);
  emulateSwdRegisterWrite(DRW_REG,AP,4,ENABLE_DWT_ITM <<24);
  
  //Set CSW to Word Size
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Set CORE_DEBUG_MODE
  emulateSwdRegisterWrite(TAR_REG,AP,4,DHCSR_REG);
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA05F0001);
  
  //Disable 2nd matching comparator
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[3].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  //Program 2nd matching comparator 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[3].COMP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xA55AA55A);
  
  //Program 2nd matching mask 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[3].MASK));
	emulateSwdRegisterWrite(DRW_REG,AP,4,WATCHPOINT_MASK_NOTHING);
  
  //Disable comparator
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  //Program comparator 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].COMP));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0xABCDEF12);
  
  //Program mask 
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].MASK));
	emulateSwdRegisterWrite(DRW_REG,AP,4,WATCHPOINT_MASK_NOTHING);
  
  //Program function
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,configData);
  
  TEST_ASSERT_EQUAL(0,setDataWatchpoint_MatchingTwoComparator(COMPARATOR_2,0xBEEEEEEF,WATCHPOINT_MASK_BIT0,
                                                              COMPARATOR_3,0xA55AA55A, WATCHPOINT_MASK_NOTHING,
                                                              0xABCDEF12,WATCHPOINT_BYTE,WATCHPOINT_READWRITE));
}

/*------------------hasDataWatchpointOccurred--------------*/
void test_hasDataWatchpointOccurred_given_DWTTRAP_DEBUGEVENT_1_MATCHED_should_return_1()
{
  //Faking CSW to Byte Size
  cswDataSize = CSW_WORD_SIZE ;
  
  //DWTTRAP_DEBUGEVENT = 1
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x4));
  
  //read DWT_FUNC1 and MATCHED bit set
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].FUNCTION));
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x1000000));
  
  TEST_ASSERT_EQUAL(1,hasDataWatchpointOccurred());
}

void test_hasDataWatchpointOccurred_given_DWTTRAP_DEBUGEVENT_1_NOT_MATCHED_should_return_0()
{
  //DWTTRAP_DEBUGEVENT = 1
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x4));
  
  //read DWT_FUNC1 and MATCHED bit not set
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].FUNCTION));
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,0,interconvertMSBandLSB(0xA));
  
  TEST_ASSERT_EQUAL(0,hasDataWatchpointOccurred());
}

void test_hasDataWatchpointOccurred_given_DWTTRAP_DEBUGEVENT_0_should_return_0()
{
  //DWTTRAP_DEBUGEVENT = 0
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x0));
  
  TEST_ASSERT_EQUAL(0,hasDataWatchpointOccurred());
}

/*------------------hasAddressWatchpointOccurred--------------*/
void test_hasAddressWatchpointOccurred_given_DWT_COMP3__DWTTRAP_DEBUGEVENT_1_MATCHED_should_return_1()
{
  //DWTTRAP_DEBUGEVENT = 1
  emulateSwdRegisterWrite(TAR_REG,AP,4,DFSR_REG);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x4));
  
  //read DWT_FUNC3 and MATCHED bit set
  emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[3].FUNCTION));
  emulateSwdRegisterRead(DRW_REG,AP,4,1,0x1234);
  emulateSwdRegisterRead(DRW_REG,AP,4,1,interconvertMSBandLSB(0x1000000));
  
  TEST_ASSERT_EQUAL(1,hasAddressWatchpointOccurred(COMPARATOR_3));
}


/*------------------disableDWTComparator--------------*/
void test_disableDWTComparator_given_DWT_COMP1_should_write_0_to_DWT_FUNC1()
{
  //Faking CSW to Word Size
  cswDataSize = CSW_WORD_SIZE ;
  
  //Program function
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[1].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  TEST_ASSERT_EQUAL(0,disableDWTComparator(COMPARATOR_1));
}

void test_disableDWTComparator_given_DWT_COMP3_should_write_0_to_DWT_FUNC3()
{ 
  //Program function
	emulateSwdRegisterWrite(TAR_REG,AP,4,(uint32_t)(&DWT_COMP[3].FUNCTION));
	emulateSwdRegisterWrite(DRW_REG,AP,4,0);
  
  TEST_ASSERT_EQUAL(0,disableDWTComparator(COMPARATOR_3));
}

