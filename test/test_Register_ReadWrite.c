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

void test_swdReadAP_should_call_SWDRegister_Read_2times()
{
	int ACK = 0 , Parity = 0 ;
	uint32_t dataRead = 0 ;
	
	emulateSWDRegister_Read(TAR_REG,AP,4,1,0x10);
	emulateSWDRegister_Read(TAR_REG,AP,4,1,0x88112233);
	
	swdReadAP(TAR_REG,&ACK,&Parity,&dataRead);
	
  TEST_ASSERT_EQUAL(1,ACK);
	TEST_ASSERT_EQUAL(1,Parity);  
	TEST_ASSERT_EQUAL(MSB_LSB_Conversion(0x88112233),dataRead);
}

void test_powerUpSystemAndDebug_should_send_0x50000000_to_CTRL_STATUS_register_and_check_error_flag() {
  emulateSWDRegister_Write(CTRLSTAT_REG, DP, 1, POWERUP_SYSTEM);

  powerUpSystemAndDebug();
}

void test_powerUpSystemAndDebug_should_send_0x50000000_to_CTRL_STATUS_register_then_check_error_flag_and_send_0x8_to_ABORT() {
  emulateSWDRegister_Write(CTRLSTAT_REG, DP, 1, POWERUP_SYSTEM);

  powerUpSystemAndDebug();
}

void test_readAhbIDR_should_clear_flags_after_readSelect_and_readAP()
{
  uint32_t data_IDR = 0;
	emulateSWDRegister_Write(CTRLSTAT_REG, DP, OK, POWERUP_SYSTEM);
	emulateSWDRegister_Write(SELECT_REG, DP, OK, BANK_F);
	emulateSWDRegister_Read(IDR_REG, AP, OK, 1, MSB_LSB_Conversion(0x24770011));
	emulateSWDRegister_Read(IDR_REG, AP, OK, 1, MSB_LSB_Conversion(0x24770011));
  
  readAhbIDR(&data_IDR);
  TEST_ASSERT_EQUAL(data_IDR, 0x24770011);
}

void test_readAhbIDR_return_WAIT_RESPONSE_should_retries_DPABORT_and_resend()
{
  uint32_t data_IDR = 0;
	emulateSWDRegister_Write(CTRLSTAT_REG, DP, OK, POWERUP_SYSTEM);
	emulateSWDRegister_Write(SELECT_REG, DP, OK, BANK_F);
	emulateSWDRegister_Read(IDR_REG, AP, WAIT, 1, MSB_LSB_Conversion(0x24770011));
	emulateSWDRegister_Read(IDR_REG, AP, WAIT, 1, MSB_LSB_Conversion(0x24770011));
  
  //Retries
  emulateSWDRegister_Read(IDR_REG, AP, WAIT, 1, MSB_LSB_Conversion(0x24770011));
	emulateSWDRegister_Read(IDR_REG, AP, WAIT, 1, MSB_LSB_Conversion(0x24770011));
  //Retries
  emulateSWDRegister_Read(IDR_REG, AP, WAIT, 1, MSB_LSB_Conversion(0x24770011));
	emulateSWDRegister_Read(IDR_REG, AP, WAIT, 1, MSB_LSB_Conversion(0x24770011));
  //Retries
  emulateSWDRegister_Read(IDR_REG, AP, WAIT, 1, MSB_LSB_Conversion(0x24770011));
	emulateSWDRegister_Read(IDR_REG, AP, WAIT, 1, MSB_LSB_Conversion(0x24770011));
  
  emulateSWDRegister_Write(ABORT_REG, DP, OK, SWD_DAPABORT_CLEAR_FLAG);
  emulateSWDRegister_Read(IDR_REG, AP, OK, 1, MSB_LSB_Conversion(0x24770011));
	emulateSWDRegister_Read(IDR_REG, AP, OK, 1, MSB_LSB_Conversion(0x24770011));
  
  readAhbIDR(&data_IDR);
  TEST_ASSERT_EQUAL(data_IDR, 0x24770011);
}

void test_memoryAccessRead_given_Address_0x12345678_should_write_address_to_TAR_and_read_data_from_DRW()
{
	uint32_t dataRead = 0 ;
	
	//Write memory address to TAR
	emulateSWDRegister_Write(TAR_REG,AP,4,0x12345678);

	//Read and Discard dummy data
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0xAABBCCDD);
	
	//Read actual data from DRW
	emulateSWDRegister_Read(DRW_REG,AP,4,1,0x10);
	
	memoryAccessRead(0x12345678,&dataRead);
	
	TEST_ASSERT_EQUAL(MSB_LSB_Conversion(0x10),dataRead);
}

void test_memoryAccessWrite_given_Address_0x12345678_Data_0x87654321_should_Write_Address_to_TAR_and_data_to_DRW()
{
	//Write memory address to TAR
	emulateSWDRegister_Write(TAR_REG,AP,4,0x12345678);
	
	//Write data to DRW
	emulateSWDRegister_Write(DRW_REG,AP,4,0x87654321);
	
	//Write data to DRW
	memoryAccessWrite(0x12345678,0x87654321);
}

void test_swdSetMemorySize_given_CSW_WORD_SIZE_should_select_BANK_0_and_set_CSW_register()
{
	//Write BANK_0 to select register
	emulateSWDRegister_Write(SELECT_REG, DP, OK, BANK_0);
	
	//Write CSW_WORD_SIZE to csw register
	emulateSWDRegister_Write(CSW_REG, AP, OK, CSW_WORD_SIZE);
	
	swdSetMemorySize(CSW_WORD_SIZE);
}