#include "unity.h"
#include "Register_ReadWrite.h"
#include "Clock.h"
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
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1

	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //1
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

void test_writeDataToAbortReg_should_execute_same_sequence_as_wrote_in_the_function()
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

<<<<<<< HEAD
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
	
	SWDRegister_Read(0x4,AP,&ACK,&parity, &dataRead);
	
	TEST_ASSERT_EQUAL(1,ACK);
	TEST_ASSERT_EQUAL(0xFFFFFFFF,dataRead);
	TEST_ASSERT_EQUAL(1,parity);
=======
  for(i = 0; i < 8; i++)  {
    SWCLK_OFF_Expect();SWCLK_ON_Expect();
  }
  //-----------extraIdleClock-------------

  writeDataToAbortReg(WDERRCLR);
>>>>>>> 93be5a87effad50d608db3144d0e31fa798f07ac
}