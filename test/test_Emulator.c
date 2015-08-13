#include <stdint.h>
#include "unity.h"
#include "Emulator.h"
#include "Clock.h"
#include "Reset.h"
#include "Delay.h"
#include "Flash.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"
#include "mock_stm32f4xx_hal_flash.h"
#include "mock_stm32f4xx_hal_flash_ex.h"
#include "mock_stm32f4xx_hal_flash_ramfunc.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_convertMSB_LSB_given_0xEE2805D4_should_return_0x2BA01477()
{
	TEST_ASSERT_EQUAL(0x2BA01477,convertMSB_LSB(0xEE2805D4));
}

void test_convertMSB_LSB_given_0x2BA01477_should_return_0xEE2805D4()
{
	TEST_ASSERT_EQUAL(0xEE2805D4,convertMSB_LSB(0x2BA01477));
}


void test_emulateWrite_given_0x2_and_number_of_bits_2()
{
  emulateWrite(0x2, 2);
  
	sendBits(0x2, 2);
}

void test_emulateWrite_given_0x5231_and_number_of_bits_14()
{
  emulateWrite(0x5231, 14);
  
	sendBits(0x5231, 14);
}

void test_emulateWrite_given_0xA1B2C3D4_and_number_of_bits_32()
{
  emulateWrite(0xA1B2C3D4, 32);
  
	sendBits(0xA1B2C3D4, 32);
}

void test_emulateRead_given_0x4_and_number_of_bits_3()
{
  uint32_t readData = 0;
  emulateRead(0x4, 3);
  
	readBits(&readData, 3);
  
  TEST_ASSERT_EQUAL(0x1, readData);
}

void test_emulateRead_given_0xEE2805D4_and_number_of_bits_32()
{
  uint32_t readData = 0;
  emulateRead(0xEE2805D4, 32);
  
	readBits(&readData, 32);
  
  TEST_ASSERT_EQUAL(0x2BA01477, readData);
}

void test_emulateTurnAroundRead_clock_should_off_once()
{
  emulateTurnAroundRead();
  
	turnAround_ToRead();
}

void test_emulateTurnAroundWrite_clock_should_on_off_on()
{
  emulateTurnAroundWrite();
  
	turnAround_ToWrite();
}

void test_emulateSwdInput()
{
  emulateSwdInput();
  
  SWDIO_InputMode();
}

void test_emulateSwdOutput()
{
  emulateSwdOutput();
  
  SWDIO_OutputMode();
}

void test_emulateIdleClock_should_generate_SWDIO_low_and_SWDCLK_OFF_ON_8_times()
{
  emulateIdleClock(8);
  
  extraIdleClock(8);
}

void test_emulateLineReset_given_60clock_should_generate_clock_cycles_with_SWDIO_High()
{
	emulateLineReset(60);
	
	lineReset(60);
}

void test_emulateResetTarget_should_call_ResetPinLow_ResetPin_High()
{
	emulateResetTarget();
	
	resetTarget();
}

void test_emulateswdRegisterWrite_should_send_SWD_Request_readACK_and_Write_data()
{
	int ACK = 0 ;
	
	emulateswdRegisterWrite(TAR_REG,AP,4,0x2BA01477);
	swdRegisterWrite(TAR_REG,AP,&ACK,0x2BA01477);
	
	TEST_ASSERT_EQUAL(1,ACK);
}

void test_emulateswdRegisterRead_should_send_SWD_Request_readACK_and_readData_readParity()
{
	int ACK = 0 , Parity = 0;
	uint32_t dataRead ;
	
	emulateswdRegisterRead(TAR_REG,AP,4,1,0xEE2805D4);
	swdRegisterRead(TAR_REG,AP,&ACK,&Parity,&dataRead);
	
	TEST_ASSERT_EQUAL(0x2BA01477,dataRead);
	TEST_ASSERT_EQUAL(1,ACK);
	TEST_ASSERT_EQUAL(1,Parity);
}