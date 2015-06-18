#include "unity.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"
#include "Bit_Operations.h"
#include "swd_Utilities.h"
#include "Clock.h"
#include "Delay.h"
#include "Reset.h"
#include "swdProtocol.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_switchJTAGtoSWD_should_call_SWDIO_Output_lineReset_send0xE79E_lineReset_extraIdleClock()
{
	int i = 0 ;	
	
	//SWDIO_OutputMode_Expect();
	
	//lineReset
	SWDIO_High_Expect();
	for (i = 0 ; i < 55 ; i ++)
	{	SWCLK_OFF_Expect();
		SWCLK_ON_Expect();
	}
	
	//0xE79E = 1110 0111 1001 1110
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	
	SWDIO_Low_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect();SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	

	//lineReset
	SWDIO_High_Expect();
	for (i = 0 ; i < 55 ; i ++)
	{	SWCLK_OFF_Expect();
		SWCLK_ON_Expect();
	}
	
	//extraIdleClock
	SWDIO_Low_Expect();
	
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	
	switchJTAGtoSWD();
}

void test_SWD_Initialisation_should_switchJTAGtoSWD_send0xA5_SWDIO_Input_turnAround_readACK_readID_readParity_turnAround_SWDIO_Output_IdleClock()
{
	int i ;
	
	
	//switch to OutputMode
	//SWDIO_OutputMode_Expect();
	
	//lineReset
	SWDIO_High_Expect();
	for (i = 0 ; i < 55 ; i ++)
	{	
		SWCLK_OFF_Expect();
		SWCLK_ON_Expect();
	}
	
	//0xE79E = 1110 0111 1001 1110
	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	
	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1

	
	//lineReset
	SWDIO_High_Expect();
	for (i = 0 ; i < 55 ; i ++)
	{	SWCLK_OFF_Expect();
		SWCLK_ON_Expect();
	}
	
	//extraIdleClock
	SWDIO_Low_Expect();
	
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	
	//send 0xA5
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0

	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	SWDIO_Low_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 0
	SWDIO_High_Expect()	;SWCLK_OFF_Expect();SWCLK_ON_Expect(); // 1
	
	//turnAround
	SWCLK_OFF_Expect();
	SWCLK_ON_Expect();
	
	SWDIO_InputMode_Expect();
  
	//read ACK	
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1); // LSB
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1); // E
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);

	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1); // E
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);

	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1); // 2
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);;

	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0); // 8
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);

	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0); // 0
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);

	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0); // 5
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);

	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1); // D
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1);

	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(1); // 4
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	
	//read Parity
	SWCLK_ON_Expect();SWCLK_OFF_Expect();	readSWDIO_Pin_ExpectAndReturn(0);
	
	//turnAround
	SWCLK_OFF_Expect();
	SWCLK_ON_Expect();
	
	//SWDIO_Output mode
	SWDIO_OutputMode_Expect();
	
	//extraIdleClock
	SWDIO_Low_Expect();
	
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();
	SWCLK_OFF_Expect();SWCLK_ON_Expect();

	
	SWD_Initialisation();
}