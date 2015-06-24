#include "unity.h"
#include "Emulator.h"
#include "Bit_Operations.h"
#include "swd_Utilities.h"
#include "Clock.h"
#include "Delay.h"
#include "Reset.h"
#include "swdProtocol.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_switchJTAGtoSWD_should_call_SWDIO_Output_lineReset_send0xE79E_lineReset_extraIdleClock()
{
	int i = 0 ;


	//lineReset
	emulateLineReset(55);

	//0xE79E = 1110 0111 1001 1110
	emulateWrite(0xE79E,16)


	//lineReset
	emulateLineReset(55);

	//extraIdleClock
	emulateIdleClock(3);

	switchJTAGtoSWD();
}

void test_SWD_Initialisation_should_switchJTAGtoSWD_send0xA5_SWDIO_Input_turnAround_readACK_readID_readParity_turnAround_SWDIO_Output_IdleClock()
{
	int i ;

	//lineReset
	emulateLineReset(55);

	//0xE79E = 1110 0111 1001 1110
	emulateWrite(0xE79E,16)


	//lineReset
	emulateLineReset(55);

	//extraIdleClock
	emulateIdleClock(3);

	//send 0xA5
	emulateWrite(0xA5,8)

	//turnAround_ToRead
	emulateTurnAroundRead();
	
	//SWDIO Input mode
	emulateSwdInput();

	//read ACK
	emulateRead(4,3);

	//read 32 bits IDCODE
	emulateRead(0xEE2805D4, 32);

	//read Parity
	emulateRead(0,1);

	//turnAround_ToWrite
	emulateTurnAroundWrite();
	
	//SWDIO_Output mode
	emulateSwdOutput();

	//extraIdleClock
	emulateIdleClock(8);


	SWD_Initialisation();
}