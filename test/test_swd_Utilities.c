#include "stdint.h"
#include "unity.h"
#include "Clock.h"
#include "Emulator.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"
#include "mock_configurePort.h"
#include "mock_IO_Operations.h"

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

void test_getSWD_Request_given_Address0x04_DP_READ_should_return_0x8D()
{
	//DP = 0 , AP = 1
	//WRiTE = 0 ,READ = 1

	//**Note LSB
	//Start bit	|	APnDP	|	RW	|	Addr2	|	Addr3	|	Parity	|	Stop	|	Park	|
	//    1     |	0		|	1	| 	1		|	0		|	0		|	0		|	1		|


	TEST_ASSERT_EQUAL(0x8D,getSWD_Request(0x04,DP,READ));
}

void test_checkAckResponse_given_ackValue_OK_should_return_OK_RESPONSE(){

  TEST_ASSERT_EQUAL(OK_RESPONSE, checkAckResponse(OK));
}

void test_checkAckResponse_given_ackValue_WAIT_should_return_WAIT_RESPONSE(){

  TEST_ASSERT_EQUAL(WAIT_RESPONSE, checkAckResponse(WAIT));
}

void test_checkAckResponse_given_ackValue_FAULT_should_return_FAULT_RESPONSE(){

  TEST_ASSERT_EQUAL(FAULT_RESPONSE, checkAckResponse(FAULT));
}

void test_checkAckResponse_given_ackValue_0x9_should_return_NO_RESPONSE(){

  TEST_ASSERT_EQUAL(NO_RESPONSE, checkAckResponse(0x9));
}

void test_checkErrorFlag_should_return_0x8_when_bit_7_of_the_readData_is_set_to_1() {
  int i = 0, data = 0;

  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0x1000000, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);

  data = checkErrorFlag();

  TEST_ASSERT_EQUAL(0x8, data);
}


void test_checkErrorFlag_should_return_0x4_when_bit_5_of_the_readData_is_set_to_1() {
  int i = 0, data = 0;

  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0x4000000, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);

  data = checkErrorFlag();

  TEST_ASSERT_EQUAL(0x4, data);
}

void test_checkErrorFlag_should_return_0x2_when_bit_4_of_the_readData_is_set_to_1() {
  int i = 0, data = 0;

  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0x8000000, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);

  data = checkErrorFlag();

  TEST_ASSERT_EQUAL(0x2, data);
}

void test_checkErrorFlag_should_return_0x10_when_bit_1_of_the_readData_is_set_to_1() {
  int i = 0, data = 0;

  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0x40000000, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);

  data = checkErrorFlag();

  TEST_ASSERT_EQUAL(0x10, data);
}

void test_checkErrorFlag_should_return_0xC_when_bit_7_5_of_the_readData_is_set_to_1() {
  int i = 0, data = 0;

  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0x5000000, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);

  data = checkErrorFlag();

  TEST_ASSERT_EQUAL(0xC, data);
}

void test_checkErrorFlag_should_return_0xE_when_bit_7_5_4_of_the_readData_is_set_to_1() {
  int i = 0, data = 0;

  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0xD000000, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);
  data = checkErrorFlag();

  TEST_ASSERT_EQUAL(0xE, data);
}

void test_checkErrorFlag_should_return_0x1E_when_bit_7_5_4_1_of_the_readData_is_set_to_1() {
  int i = 0, data = 0;

  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0x4D000000, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);
  data = checkErrorFlag();

  TEST_ASSERT_EQUAL(0x1E, data);
}

/*******************************************************************************************************
 * Read CTRL/STAT Register (0x8D)
 * *****************************************************************************************************
 * |  Start bit   |  APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 * -----------------------------------------------------------------------------------------------------
 * |      1      |    0      |    1     |     1      |     0     |     0       |    0      |     1     |
 * *****************************************************************************************************
 *
 * Write AP ABORT Register (0x81)
 * *****************************************************************************************************
 * |  Start bit   |  APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 * -----------------------------------------------------------------------------------------------------
 * |      1      |    0      |    0     |     0      |     0     |     0       |    0      |     1     |
 * *****************************************************************************************************
 */
/*
void test_takeActionToAckResponse_given_WDERRCLR_bit_set_to_1_should_write_0x1000000_to_AP_ABORT_register() {
  int i = 0, data = 0;

  //Read CTRL/STAT Register
  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0x1000000, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);
  
  //Write AP ABORT Register
  emulateWrite(0x81, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateTurnAroundWrite();
  emulateSwdOutput();  
  emulateWrite(0x8, 32); //Write DATA (LSB)
  emulateWrite(0x1, 1); //Parity
  emulateIdleClock(8);
  
  takeActionToAckResponse(FAULT_RESPONSE);
}*/

/*
void test_takeActionToAckResponse_given_WAIT_RESPOND_should_write_CLRDAPABOT_to_AP_ABORT_Register() {
  int i = 0, data = 0;

  //Write AP ABORT Register
  emulateWrite(0x81, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateTurnAroundWrite();
  emulateSwdOutput();  
  emulateWrite(0x1, 32); //Write DATA (LSB)
  emulateWrite(0x1, 1); //Parity
  emulateIdleClock(8);
  
  takeActionToAckResponse(WAIT_RESPONSE);
}*/