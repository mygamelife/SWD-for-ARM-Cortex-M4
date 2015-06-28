#include "unity.h"
#include "Clock.h"
#include "Emulator.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "mock_configurePort.h"
#include "mock_IO_Operations.h"

void setUp(void){}
void tearDown(void){}

 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    0      |    0     |     1      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************/
void test_writeCtrlStatusReg_request_10010101_should_be_send(void)
{
  //DP = 0, Write = 0, Addr = 0x04
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1

	ctrlStatusReg(WRITE);
}

 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    0      |   1     |     1      |     0     |     0       |    0      |     1     |
 ******************************************************************************************************/
void test_readCtrlStatusReg_request_10010101_should_be_sent(void)
{
  //DP = 0, Read = 1, Addr = 0x04
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1

	ctrlStatusReg(READ);
}

 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    0      |    1     |     0      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************/
void test_readIDCODEReg_request_10010101_should_be_send(void)
{
  //DP = 0, Read = 1, Addr = 0x00
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1

	readIDCODEReg();
}

 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    0      |    0     |     0      |     0     |     0       |    0      |     1     |
 ******************************************************************************************************/
void test_writeAbortReg_request_10000001_should_be_send(void)
{
  //DP = 0, Write = 0, Addr = 0x00
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1

	writeAbortReg();
}

void test_writeDataToSelectReg_should_execute_same_sequence_as_wrote_in_the_function()
{
  int i = 0;
	//send 0x81
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1

	//turnAround_ToRead
	SWCLK_OFF_Expect();

	//Switch to Input
	SWDIO_InputMode_Expect();

	//read ACK
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);

	//turnAround_ToWrite
	SWCLK_ON_Expect();
	SWCLK_OFF_Expect();
	SWCLK_ON_Expect();

	//SWDIO_Output mode
	SWDIO_OutputMode_Expect();

	//Data
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // LSB
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // 8
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // 0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // 0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // 0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // 0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // 0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // 0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  // 0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	//read Parity is 1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();

	//-----------extraIdleClock-------------
	SWDIO_Low_Expect();

	for(i = 0; i < 8; i++)  {
		SWCLK_OFF_Expect();SWCLK_ON_Expect();
	}
	//-----------extraIdleClock-------------

  writeDataToSelectReg(WDERRCLR);
}


 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    1      |    0     |     1      |     0     |     0       |    0      |     1     |
 ******************************************************************************************************/
void test_SWDRegister_Write_given_Address_0x4_AP_Write_data_0xFFFFFFFF()
{
	int i = 0 , ACK = 0;
	uint32_t dataToSend = 0xFFFFFFFF ;

	//send 0x8B
	emulateWrite(0x8B,8);

	//turnAround_ToRead
	emulateTurnAroundRead();

	//Switch to Input
	emulateSwdInput();

	//read ACK
	emulateRead(4,3);

	//turnAround_ToWrite
	emulateTurnAroundWrite();

	//SWDIO_Output mode
	emulateSwdOutput();

	//send 32bits data 0xFFFFFFFF
	emulateWrite(0xFFFFFFFF,32);

	//send parity
	emulateWrite(0,1);

	//-----------extraIdleClock-------------
	emulateIdleClock(8);

	SWDRegister_Write(0x4,AP,&ACK,0xFFFFFFFF);
	TEST_ASSERT_EQUAL(1,ACK);
}

 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    1      |   1     |     1      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************/
void test_SWDRegisterRead_given_Address_0x4_AP_READ() {
	int parity = 0 , ACK = 0;
	uint32_t dataRead = 0 ;
  
	//send 0xAF
	emulateWrite(0xAF, 8); //SWD 8bit protocol
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); //Acknowledgement
	emulateRead(0x80, 32); //Read DATA (LSB)
	emulateRead(0x1, 1); //Parity
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
  
	SWDRegister_Read(0x4,AP,&ACK,&parity, &dataRead);

	TEST_ASSERT_EQUAL(1,ACK);
	TEST_ASSERT_EQUAL(1,parity);  
	TEST_ASSERT_EQUAL(0x1000000,dataRead);
}

void test_SWD_ReadAP_should_call_SWDRegister_Read_2times()
{
	int ACK = 0 , Parity = 0 ;
	uint32_t dataRead = 0 ;
	
	emulateSWDRegister_Read(TAR_REG,AP,4,1,0x10);
	emulateSWDRegister_Read(TAR_REG,AP,4,1,0x88112233);
	
	SWD_ReadAP(TAR_REG,&ACK,&Parity,&dataRead);
	
	TEST_ASSERT_EQUAL(MSB_LSB_Conversion(0x88112233),dataRead);
	TEST_ASSERT_EQUAL(1,ACK);
	TEST_ASSERT_EQUAL(1,Parity);
}


void test_Memory_Read_given_Address_0x12345678_should_write_address_to_TAR_and_read_data_from_DRW()
{
	uint32_t dataRead = 0 ;
	
	//Write memory address to TAR
	emulateSWDRegister_Write(TAR_REG,AP,4,0x12345678);

	//Read and Discard dummy data
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0xAABBCCDD);
	
	//Read actual data from DRW
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x10);
	
	Memory_Read(0x12345678,&dataRead);
	
	TEST_ASSERT_EQUAL(MSB_LSB_Conversion(0x10),dataRead);
}

void test_Memory_Write_given_Address_0x12345678_Data_0x87654321_should_Write_Address_to_TAR_and_data_to_DRW()
{
	//Write memory address to TAR
	emulateSWDRegister_Write(TAR_REG,AP,4,0x12345678);
	
	//Write data to DRW
	emulateSWDRegister_Write(DRW_REG,AP,4,0x87654321);
	
	//Write data to DRW
	Memory_Write(0x12345678,0x87654321);
}
