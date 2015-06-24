#include "unity.h"
#include "Emulator.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"
#include "Bit_Operations.h"

void setUp(void){}

void tearDown(void){}

void test_sendBit_data_1_should_call_SWDIO_High_SWCLK_OFF_SWCLK_ON()
{
	emulateWrite(1,1);
	
	sendBit(1);
}

void test_sendBit_data_0_should_call_SWDIO_Low_SWCLK_OFF_SWCLK_ON()
{
	emulateWrite(0,1);
	
	sendBit(0);
}

void test_readBit_should_call_SWCLK_ON_SWCLK_OFF_readSWDIO_Pin_read_1_and_return_1()
{
	emulateRead(1,1);
	
	TEST_ASSERT_EQUAL(1,readBit());
}

void test_readBit_should_call_SWCLK_ON_SWCLK_OFF_readSWDIO_Pin_read_0_and_return_0()
{
	emulateRead(0,1);
	
	TEST_ASSERT_EQUAL(0,readBit());
}

void test_send8bit_0xA5_1010_0101_should_send_1010_0101()
{
	emulateWrite(0xA5,8);
	
	send8bit(0xA5);
}

void test_send8bit_0xb6_1011_0110_should_send_0110_1101()
{
	emulateWrite(0xb6,8);
	
	send8bit(0xB6);
}

void test_send16bit_0xE79E_1110_0111_1001_1110_should_send_0111_1001_1110_0111()
{
	emulateWrite(0xE79E,16);
	
	send16bit(0xE79E);
}

void test_send32bit_0xA1B2C3D4_1010_0001_1011_0010_1100_0011_1101_0100_should_send_0010_1011_1100_0011_0100_1101_1000_0101()
{
	emulateWrite(0xA1B2C3D4,32);
	
	send32bit(0xA1B2C3D4);
}

void test_read3bit_given_100_should_return_1()
{
	int data = 0 ;
	
	emulateRead(0x4,3);
	
	
	read3bit(&data);
	
	TEST_ASSERT_EQUAL(1,data);
}

void test_read3bit_given_010_should_return_2()
{
	int data = 0 ;
	
	emulateRead(0x2,3);
	
	read3bit(&data);
	
	TEST_ASSERT_EQUAL(2,data);
}

void test_read3bit_given_001_should_return_4()
{
	int data = 0 ;
	
	emulateRead(0x1,3);	
	
	read3bit(&data);
	
	TEST_ASSERT_EQUAL(4,data);
}

void test_read32bit_given_0xEE2805D4_should_read_0x2ba01477()
{
	uint32_t data = 0 ;
	
	emulateRead(0xEE2805D4,32);
	
	read32bit(&data);
	
	TEST_ASSERT_EQUAL(0x2ba01477,data);
}