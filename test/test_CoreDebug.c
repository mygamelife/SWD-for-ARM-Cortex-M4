#include "unity.h"
#include <stdint.h>
#include "CoreDebug.h"
#include "Clock.h"
#include "Emulator.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "mock_configurePort.h"
#include "mock_IO_Operations.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_isCore_DebugMode_given_bit0_is_1_should_return_1_or_TRUE()
{
	TEST_ASSERT_EQUAL(TRUE,isCore_DebugMode(0xA05F0001));
}

void test_isCore_DebugMode_given_bit0_is_0_should_return_0_or_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,isCore_DebugMode(0x12345670));
}

void test_isCore_Halted_given_bit17_is_1should_return_1_or_TRUE()
{
	TEST_ASSERT_EQUAL(TRUE,isCore_Halted(0xA05F0003));
}

void test_isCore_Halted_given_bit17_0_should_return_0_for_FALSE()
{
	TEST_ASSERT_EQUAL(FALSE,isCore_Halted(0xA0500002));
}



void test_setCore_DebugMode_should_return_1_if_processor_enter_debug_mode()
{
	/*Memory_Write*/
	
	//Write DCHSR address to TAR
	emulateWrite(0x8B,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(DHCSR_REG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	//Write SET_CORE_DEBUG to DRW
	emulateWrite(0xBB,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(SET_CORE_DEBUG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	/*Memory_Read*/
	
	//Write DCHSR address to TAR
	emulateWrite(0x8B,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(DHCSR_REG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	//Discard dummy data
	emulateWrite(0x9F,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateRead(0xFF,32); 
	emulateRead(0x1,1); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
	
	//Read actual data from DRW
	emulateWrite(0x9F,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateRead(0x80000008,32); 
	emulateRead(0x1,1); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
	
	
	TEST_ASSERT_EQUAL(1,setCore_DebugMode());
}

void test_setCore_DebugMode_should_return_0_if_processor_doesnt_enter_debug_mode()
{
	/*Memory_Write*/
	
	//Write DCHSR address to TAR
	emulateWrite(0x8B,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(DHCSR_REG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	//Write SET_CORE_DEBUG to DRW
	emulateWrite(0xBB,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(SET_CORE_DEBUG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	/*Memory_Read*/
	
	//Write DCHSR address to TAR
	emulateWrite(0x8B,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(DHCSR_REG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	//Discard dummy data
	emulateWrite(0x9F,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateRead(0xFF,32); 
	emulateRead(0x1,1); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
	
	//Read actual data from DRW
	emulateWrite(0x9F,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateRead(0x80,32); 
	emulateRead(0x1,1); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
		
	TEST_ASSERT_EQUAL(0,setCore_DebugMode());
}

void test_setCore_Halt_should_return_1_if_processor_is_Halted()
{
	/*Memory_Write*/
	
	//Write DCHSR address to TAR
	emulateWrite(0x8B,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(DHCSR_REG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	//Write SET_CORE_DEBUG to DRW
	emulateWrite(0xBB,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(SET_CORE_HALT,32);
	emulateWrite(0,1);
	emulateIdleClock(8);
	
	/*Memory_Read*/
	
	//Write DCHSR address to TAR
	emulateWrite(0x8B,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(DHCSR_REG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	//Discard dummy data
	emulateWrite(0x9F,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateRead(0xFF,32); 
	emulateRead(0x1,1); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
	
	//Read actual data from DRW
	emulateWrite(0x9F,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateRead(MSB_LSB_Conversion(0x30030003),32); 
	emulateRead(0x1,1); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
		
	
	TEST_ASSERT_EQUAL(1,setCore_Halt());
}



void test_setCore_Halt_should_return_0_if_processor_is_not_Halted()
{
	printf("\n");
	
	/*Memory_Write*/
	
	//Write DCHSR address to TAR
	emulateWrite(0x8B,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(DHCSR_REG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	//Write SET_CORE_DEBUG to DRW
	emulateWrite(0xBB,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(SET_CORE_HALT,32);
	emulateWrite(0,1);
	emulateIdleClock(8);
	
	/*Memory_Read*/
	
	//Write DCHSR address to TAR
	emulateWrite(0x8B,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4, 3); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateWrite(DHCSR_REG,32);
	emulateWrite(1,1);
	emulateIdleClock(8);
	
	//Discard dummy data
	emulateWrite(0x9F,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateRead(0xFF,32); 
	emulateRead(0x1,1); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
	
	//Read actual data from DRW
	emulateWrite(0x9F,8);
	emulateTurnAroundRead();
	emulateSwdInput();
	emulateRead(0x4,3); 
	emulateRead(0xB0000000,32); 
	emulateRead(0x1,1); 
	emulateTurnAroundWrite();
	emulateSwdOutput();
	emulateIdleClock(8);
	
	TEST_ASSERT_EQUAL(0,setCore_Halt());
}