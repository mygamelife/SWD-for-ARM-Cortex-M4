#include "unity.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "mock_IO_Operations.h"

void setUp(void)  {}

void tearDown(void) {}

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

void test_setCtrlStatusReg_should_send_0x50000000_set_CSYSPWRUPRED_and_CDBGPWRUPREQ(void)
{
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

  SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1 CDBGPWRUPREQ
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0
  SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); //1 CSYSPWRUPRED
  SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect();  //0

	setCtrlStatusReg(0x50000000);
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

	readIDCODEReg(READ);
}
