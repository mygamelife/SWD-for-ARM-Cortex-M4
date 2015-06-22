#include <stdint.h>
#include "unity.h"
#include "Emulator.h"
#include "Clock.h"
#include "Bit_Operations.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"

void setUp(void)
{
}

void tearDown(void)
{
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