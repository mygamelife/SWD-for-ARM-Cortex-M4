#include "unity.h"
#include "swd_Utilities.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_calculateParity_SWDRequest_given_0000_should_return_0()
{
	
	TEST_ASSERT_EQUAL(0,calculateParity_SWDRequest(0,0,0,0));
}

void test_calculateParity_SWDRequest_given_0100_should_return_1()
{
	
	TEST_ASSERT_EQUAL(1,calculateParity_SWDRequest(0,1,0,0));
}

void test_calculateParity_SWDRequest_given_0111_should_return_1()
{
	
	TEST_ASSERT_EQUAL(1,calculateParity_SWDRequest(0,1,1,1));
}


void test_calculateParity_SWDRequest_given_1111_should_return_0()
{
	
	TEST_ASSERT_EQUAL(0,calculateParity_SWDRequest(1,1,1,1));
}

void test_calculateParity_32bitData_given_0x0_should_return_0()
{
	TEST_ASSERT_EQUAL(0,calculateParity_32bitData(0));
}

void test_calculateParity_32bitData_given_0x1_should_return_1()
{
	TEST_ASSERT_EQUAL(1,calculateParity_32bitData(1));
}

void test_calculateParity_32bitData_given_0xF1E2D3C4_should_return_1()
{
	//1111 0001 1110 0010 1101 0011 1100 0100
	
	TEST_ASSERT_EQUAL(1,calculateParity_32bitData(0xF1E2D3C4));
}

void test_getSWDAddress_Bit_given_0x2_should_return_0_for_bit3_0_for_bit2()
{
	int Address_bit3 , Address_bit2 ;
	
	getSWD_AddressBit(&Address_bit3,&Address_bit2,0x2);
	
	TEST_ASSERT_EQUAL(0,Address_bit3);
	TEST_ASSERT_EQUAL(0,Address_bit2);
}


void test_getSWDAddress_Bit_given_0x4_should_return_0_for_bit3_1_for_bit2()
{
	int Address_bit3 , Address_bit2 ;
	
	getSWD_AddressBit(&Address_bit3,&Address_bit2,0x4);
	
	TEST_ASSERT_EQUAL(0,Address_bit3);
	TEST_ASSERT_EQUAL(1,Address_bit2);
}

void test_getSWDAddress_Bit_given_0x8_should_return_1_for_bit3_0_for_bit2()
{
	int Address_bit3 , Address_bit2 ;
	
	getSWD_AddressBit(&Address_bit3,&Address_bit2,0x8);
	
	TEST_ASSERT_EQUAL(1,Address_bit3);
	TEST_ASSERT_EQUAL(0,Address_bit2);
}

void test_getSWDAddress_Bit_given_0xC_should_return_1_for_bit3_1_for_bit2()
{
	int Address_bit3 , Address_bit2 ;
	
	getSWD_AddressBit(&Address_bit3,&Address_bit2,0xC);
	
	TEST_ASSERT_EQUAL(1,Address_bit3);
	TEST_ASSERT_EQUAL(1,Address_bit2);
}


void test_getSWD_Request_given_Address_0x00_DP_Read_should_return_0xA5()
{
	//DP = 0 , AP = 1 
	//WRiTE = 0 ,READ = 1
	
	//**Note LSB
	//Start bit	|	APnDP	|	RW	|	Addr2	|	Addr3	|	Parity	|	Stop	|	Park	|
	//    1     |	0		|	1	| 	0		|	0		|	1		|	0		|	1		|
	
	TEST_ASSERT_EQUAL(0xA5,getSWD_Request(0x00,DP,READ));
}


void test_getSWD_Request_given_Address0x08_AP_Write_should_return_0x93()
{
	//DP = 0 , AP = 1 
	//WRiTE = 0 ,READ = 1
	
	//**Note LSB
	//Start bit	|	APnDP	|	RW	|	Addr2	|	Addr3	|	Parity	|	Stop	|	Park	|
	//    1     |	1		|	0	| 	0		|	1		|	0		|	0		|	1		|
	
	
	TEST_ASSERT_EQUAL(0x93,getSWD_Request(0x08,AP,WRITE));
	
}