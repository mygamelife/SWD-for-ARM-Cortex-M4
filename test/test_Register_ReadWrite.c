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
 |      1       |    1      |    0     |     1      |     0     |     0       |    0      |     1     |
 ******************************************************************************************************/
void test_SWDRegister_Write_given_Address_0x4_AP_Write_data_0xFFFFFFFF_should_send_SWD_Request_and_WriteData()
{
	int i = 0 , ACK = 0;
	uint32_t dataToSend = 0xFFFFFFFF ;

	//send 0x8B
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //1

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

	//send 32bits data 0xFFFFFFFF
	for (i = 0 ; i < 32 ; i ++)
	{
		SWDIO_High_Expect();
		SWCLK_OFF_Expect();SWCLK_ON_Expect();
	}

	//send parity
	SWDIO_Low_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();

	//-----------extraIdleClock-------------
	SWDIO_Low_Expect();

	for(i = 0; i < 8; i++)
	{
		SWCLK_OFF_Expect();SWCLK_ON_Expect();
	}

	SWDRegister_Write(0x4,AP,&ACK,0xFFFFFFFF);
	TEST_ASSERT_EQUAL(1,ACK);
}

 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    1      |   1     |     1      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************/
void test_SWDRegister_RW_given_Address_0x4_AP_READ_data_0xFFFFFFFF_should_send_SWD_Request_and_ReadData()
{
	int i , parity = 0 , ACK = 0;
	uint32_t dataRead = 0 ;

	//send 0xAF
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //1

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //1
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


	//read 32bits data 0xFFFFFFFF
	for (i = 0 ; i < 32 ; i ++)
	{
		SWCLK_ON_Expect();SWCLK_OFF_Expect();
		readSWDIO_Pin_ExpectAndReturn(1);
	}

	//read 1 bit parity
	SWCLK_ON_Expect();SWCLK_OFF_Expect();
	readSWDIO_Pin_ExpectAndReturn(1);

	//turnAround_ToWrite
	SWCLK_ON_Expect(); SWCLK_OFF_Expect(); SWCLK_ON_Expect();

	//switch to output mode
	SWDIO_OutputMode_Expect();

	//-----------extraIdleClock-------------
	SWDIO_Low_Expect();

	for(i = 0; i < 8; i++)
	{
		SWCLK_OFF_Expect();SWCLK_ON_Expect();
	}
  //-----------extraIdleClock-------------

	SWDRegister_Read(0x4,AP,&ACK,&parity, &dataRead);

	TEST_ASSERT_EQUAL(1,ACK);
	TEST_ASSERT_EQUAL(0xFFFFFFFF,dataRead);
	TEST_ASSERT_EQUAL(1,parity);
}

void test_SWDRegisterRead() {
	int parity = 0 , ACK = 0;
	uint32_t dataRead = 0 ;
  
  //send 0xB1
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

void test_powerUpSystemAndDebug_should_send_0x50000000_to_CTRL_STATUS_register_and_check_error_flag() {
  //Write CTR/STATUS 
  emulateWrite(0xA9, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateTurnAroundWrite();
  emulateSwdOutput();  
  emulateWrite(POWERUP_SYSTEM, 32); //Write DATA (LSB)
  emulateWrite(0x0, 1); //Parity
  emulateIdleClock(8);
  
  //Check error flag
  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0xA, 32); //Read DATA (LSB)
  emulateRead(0x0, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);

  powerUpSystemAndDebug();
}

void test_powerUpSystemAndDebug_should_send_0x50000000_to_CTRL_STATUS_register_then_check_error_flag_and_send_0x8_to_ABORT() {
  //Write CTR/STATUS 
  emulateWrite(0xA9, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateTurnAroundWrite();
  emulateSwdOutput();  
  emulateWrite(POWERUP_SYSTEM, 32); //Write DATA (LSB)
  emulateWrite(0x0, 1); //Parity
  emulateIdleClock(8);
  
  //Check error flag <-- Expect error flag happen here
  emulateWrite(0x8D, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateRead(0x100000A, 32); //Read DATA (LSB)
  emulateRead(0x1, 1); //Parity
  emulateTurnAroundWrite();
  emulateSwdOutput();
  emulateIdleClock(8);
  
  //Write ABORT
  emulateWrite(0x81, 8); //SWD 8bit protocol
  emulateTurnAroundRead();
  emulateSwdInput();
  emulateRead(0x4, 3); //Acknowledgement
  emulateTurnAroundWrite();
  emulateSwdOutput();  
  emulateWrite(WDERRCLR, 32); //Write DATA (LSB)
  emulateWrite(0x1, 1); //Parity
  emulateIdleClock(8);

  powerUpSystemAndDebug();
}