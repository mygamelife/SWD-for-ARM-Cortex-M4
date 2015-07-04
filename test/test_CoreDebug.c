#include "unity.h"
#include <stdint.h>
#include "CoreDebug.h"
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

void test_get_Core_WriteValue_given_CORE_DEBUG_MODE_should_return_0xA05F0001_or_SET_CORE_DEBUG()
{
	TEST_ASSERT_EQUAL(0xA05F0001,Get_Core_WriteValue(CORE_DEBUG_MODE));
	TEST_ASSERT_EQUAL(SET_CORE_DEBUG,Get_Core_WriteValue(CORE_DEBUG_MODE));
}

void test_get_Core_WriteValue_given_CORE_DEBUG_HALT_should_return_0xA05F0003_or_SET_CORE_DEBUG_HALT()
{
	TEST_ASSERT_EQUAL(0xA05F0003,Get_Core_WriteValue(CORE_DEBUG_HALT));
	TEST_ASSERT_EQUAL(SET_CORE_DEBUG_HALT,Get_Core_WriteValue(CORE_DEBUG_HALT));
}

void test_get_Core_WriteValue_given_CORE_SINGLE_STEP_should_return_0xA05F0007_or_SET_CORE_STEP()
{
	TEST_ASSERT_EQUAL(0xA05F0007,Get_Core_WriteValue(CORE_SINGLE_STEP));
	TEST_ASSERT_EQUAL(SET_CORE_STEP,Get_Core_WriteValue(CORE_SINGLE_STEP));
}

void test_get_Core_WriteValue_given_CORE_SINGLE_STEP_should_return_0xA05F0007_or_SET_CORE_MASKINT()
{
	TEST_ASSERT_EQUAL(0xA05F000B,Get_Core_WriteValue(CORE_MASK_INTERRUPT));
	TEST_ASSERT_EQUAL(SET_CORE_MASKINT,Get_Core_WriteValue(CORE_MASK_INTERRUPT));
}

/*
void test_get_Core_WriteValue_given_CORE_SNAP_STALL_should_return_0xA05F0007_or_SET_CORE_SNAPSTALL()
{
	TEST_ASSERT_EQUAL(0xA05F0023,get_Core_WriteValue(CORE_SNAP_STALL));
	TEST_ASSERT_EQUAL(SET_CORE_SNAPSTALL,get_Core_WriteValue(CORE_SNAP_STALL));
}
*/

/*------------------------------------------------------------------------------*/
 /******************************************************************************************************
	Debug Halting and Control Status Register , DHCSR
 
	Bits[31:26] --- RESERVED
	
	Bit[26]		--- S_RESET_ST
	Bit[25]		--- S_RETIRE_ST
	
	Bits[23:20]	--- RESERVED
	
	Bit[19]		--- S_LOCKUP
	Bit[18]		--- S_SLEEP
	Bit[17]		--- S_HALT
	Bit[16]		--- S_REGRDY
	
	Bits[15:6]	--- RESERVED
	
	Bit[5]		--- C_SNAPSTALL
	
	Bit[4]		--- RESERVED
	
	Bit[3]		--- C_MASKINTS
	Bit[2]		--- C_STEP
	Bit[1]		--- C_HALT
	Bit[0]		--- C_DEBUGEN
	
 ******************************************************************************************************/
//CORE_DEBUG_MODE
// Testing Bit[0]
void test_isCore_given_CORE_DEBUG_MODE_data_0x1_should_return_TRUE()
{
	TEST_ASSERT_EQUAL(TRUE,IsCore(CORE_DEBUG_MODE,0x1));
}

void test_isCore_given_CORE_DEBUG_MODE_data_0x10_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_DEBUG_MODE,0x10));
}

//CORE_DEBUG_HALT
//Testing Bit[17], Bits[1], Bit[0]
void test_isCore_given_CORE_DEBUG_HALT_data_0x20003_should_return_TRUE()
{
	TEST_ASSERT_EQUAL(TRUE,IsCore(CORE_DEBUG_HALT,0x20003));
}

void test_isCore_given_CORE_DEBUG_HALT_data_0x20000_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_DEBUG_HALT,0x20000));
}

void test_isCore_given_CORE_DEBUG_HALT_data_0x20001_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_DEBUG_HALT,0x20001));
}

void test_isCore_given_CORE_DEBUG_HALT_data_0x3_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_DEBUG_HALT,0x3));
}

void test_isCore_given_CORE_DEBUG_HALT_data_0x0_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_DEBUG_HALT,0x0));
}

//CORE_SINGLE_STEP
// Testing Bit[2] , Bit[1] , Bit[0]
void test_isCore_given_CORE_SINGLE_STEP_data_0x7_should_return_TRUE()
{
	TEST_ASSERT_EQUAL(TRUE,IsCore(CORE_SINGLE_STEP,0x7));
}

void test_isCore_given_CORE_SINGLE_STEP_data_0x0_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_SINGLE_STEP,0x0));
}

void test_isCore_given_CORE_SINGLE_STEP_data_0x1_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_SINGLE_STEP,0x1));
}

void test_isCore_given_CORE_SINGLE_STEP_data_0x3_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_SINGLE_STEP,0x3));
}

void test_isCore_given_CORE_SINGLE_STEP_data_0x4_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_SINGLE_STEP,0x4));
}

//CORE_MASK_INTERRUPT
//Testing Bit[17], Bit[4], Bit[1], Bit[0]
void test_isCore_given_CORE_MASK_INTERRUPT_data_0x2000B_should_return_TRUE()
{
	TEST_ASSERT_EQUAL(TRUE,IsCore(CORE_MASK_INTERRUPT,0x2000B));
}

void test_isCore_given_CORE_MASK_INTERRUPT_data_0x20000_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_MASK_INTERRUPT,0x20000));
}

void test_isCore_given_CORE_MASK_INTERRUPT_data_0x20001_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_MASK_INTERRUPT,0x20001));
}

void test_isCore_given_CORE_MASK_INTERRUPT_data_0x20002_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_MASK_INTERRUPT,0x20002));
}

void test_isCore_given_CORE_MASK_INTERRUPT_data_0xB_should_return_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,IsCore(CORE_MASK_INTERRUPT,0xB));
}

/*------------------------------------------------------------------------------*/

void test_setCore_CORE_DEBUG_HALT_should_write_0xA05F0003_to_DHCSR_and_return_true_if_successful()
{
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Write(DRW_REG,AP,4,SET_CORE_DEBUG_HALT);
	
	emulateSWDRegister_Write(TAR_REG,AP,4,DHCSR_REG);
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x1234) ;
	emulateSWDRegister_Read(DRW_REG,AP,4,1,MSB_LSB_Conversion(0x03030003));
	TEST_ASSERT_EQUAL(TRUE,SetCore(CORE_DEBUG_HALT));
}