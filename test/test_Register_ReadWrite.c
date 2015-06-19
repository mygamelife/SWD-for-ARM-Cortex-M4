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