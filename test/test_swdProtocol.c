#include "unity.h"
#include "swd.h"
#include "Emulator.h"
#include "IoOperations.h"
#include "swd_Utilities.h"
#include "Delay.h"
#include "Register_ReadWrite.h"
#include "mock_LowLevelIO.h"
#include "mock_configurePort.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_switchJTAGtoSWD_should_call_SWDIO_Output_lineReset_send0xE79E_lineReset_extraIdleClock()
{
	//lineReset
	emulateLineReset(55);

	//0xE79E = 1110 0111 1001 1110
	emulateWrite(0xE79E,16);


	//lineReset
	emulateLineReset(55);

	//extraIdleClock
	emulateIdleClock(3);

	switchJTAGtoSWD();
}

void test_SWD_Initialisation_should_switchJTAGtoSWD_send0xA5_SWDIO_Input_turnAround_readACK_readID_readParity_turnAround_SWDIO_Output_IdleClock()
{
  //Reset Target
  emulatehardResetTarget();
  
	//lineReset
	emulateLineReset(55);

	//0xE79E = 1110 0111 1001 1110
	emulateWrite(0xE79E,16);

	//lineReset
	emulateLineReset(55);

	//extraIdleClock
	emulateIdleClock(3);

	//send 0xA5
	emulateWrite(0xA5,8);

	//turnAroundRead
	emulateTurnAroundRead();
	
	//SWDIO Input mode
	emulateSwdInput();

	//read ACK
	emulateRead(4,3);

	//read 32 bits IDCODE
	emulateRead(0xEE2805D4, 32);

	//read Parity
	emulateRead(0,1);

	//turnAroundWrite
	emulateTurnAroundWrite();
	
	//SWDIO_Output mode
	emulateSwdOutput();

	//extraIdleClock
	emulateIdleClock(8);
  
	swdInit();
}
