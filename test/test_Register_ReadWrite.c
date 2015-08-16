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
void test_swdRegisterWrite_given_Address_0x4_AP_Write_data_0xFFFFFFFF()
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

	swdRegisterWrite(0x4,AP,&ACK,0xFFFFFFFF);
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
  
	swdRegisterRead(0x4,AP,&ACK,&parity, &dataRead);

	TEST_ASSERT_EQUAL(1,ACK);
	TEST_ASSERT_EQUAL(1,parity);  
	TEST_ASSERT_EQUAL(0x1000000,dataRead);
}

void test_swdReadAP_should_call_swdRegisterRead_2times()
{
	int ACK = 0 , Parity = 0 ;
	uint32_t dataRead = 0 ;
	
	emulateswdRegisterRead(TAR_REG,AP,4,1,0x10);
	emulateswdRegisterRead(TAR_REG,AP,4,1,0x88112233);
	
	swdReadAP(TAR_REG,&ACK,&Parity,&dataRead);
	
  TEST_ASSERT_EQUAL(1,ACK);
	TEST_ASSERT_EQUAL(1,Parity);  
	TEST_ASSERT_EQUAL(convertMSB_LSB(0x88112233),dataRead);
}

void test_powerUpSystemAndDebug_should_send_0x50000000_to_CTRL_STATUS_register_and_check_error_flag() {
  emulateswdRegisterWrite(CTRLSTAT_REG, DP, 1, POWERUP_SYSTEM);

  powerUpSystemAndDebug();
}

void test_powerUpSystemAndDebug_should_send_0x50000000_to_CTRL_STATUS_register_then_check_error_flag_and_send_0x8_to_ABORT() {
  emulateswdRegisterWrite(CTRLSTAT_REG, DP, 1, POWERUP_SYSTEM);

  powerUpSystemAndDebug();
}

void test_readAhbIDR_should_clear_flags_after_readSelect_and_readAP()
{
  uint32_t data_IDR = 0;
	emulateswdRegisterWrite(CTRLSTAT_REG, DP, OK, POWERUP_SYSTEM);
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_F);
	emulateswdRegisterRead(IDR_REG, AP, OK, 1, convertMSB_LSB(0x24770011));
	emulateswdRegisterRead(IDR_REG, AP, OK, 1, convertMSB_LSB(0x24770011));
  
  readAhbIDR(&data_IDR);
  TEST_ASSERT_EQUAL(data_IDR, 0x24770011);
}

void test_readAhbIDR_return_WAIT_RESPONSE_should_retries_DPABORT_and_resend()
{
  uint32_t data_IDR = 0;
	emulateswdRegisterWrite(CTRLSTAT_REG, DP, OK, POWERUP_SYSTEM);
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_F);
	emulateswdRegisterRead(IDR_REG, AP, WAIT, 1, convertMSB_LSB(0x24770011));
	emulateswdRegisterRead(IDR_REG, AP, WAIT, 1, convertMSB_LSB(0x24770011));
  
  //Retries
  emulateswdRegisterRead(IDR_REG, AP, WAIT, 1, convertMSB_LSB(0x24770011));
	emulateswdRegisterRead(IDR_REG, AP, WAIT, 1, convertMSB_LSB(0x24770011));
  
  emulateswdRegisterRead(IDR_REG, AP, OK, 1, convertMSB_LSB(0x24770011));
	emulateswdRegisterRead(IDR_REG, AP, OK, 1, convertMSB_LSB(0x24770011));
  
  readAhbIDR(&data_IDR);
  TEST_ASSERT_EQUAL(data_IDR, 0x24770011);
}

void test_memoryReadWord_given_Address_0x12345678_should_write_address_to_TAR_and_read_data_from_DRW()
{
	uint32_t dataRead = 0 ;
	
  cswDataSize = CSW_WORD_SIZE ;
  
	//Write memory address to TAR
	emulateswdRegisterWrite(TAR_REG,AP,4,0x12345678);

	//Read and Discard dummy data
	emulateswdRegisterRead(DRW_REG,AP,4,1,0xAABBCCDD);
	
	//Read actual data from DRW
	emulateswdRegisterRead(DRW_REG,AP,4,1,0x10);
	
	memoryReadWord(0x12345678,&dataRead);
	
	TEST_ASSERT_EQUAL(convertMSB_LSB(0x10),dataRead);
}

//cswDataSize = CSW_WORD_SIZE
void test_memoryWriteByte_should_set_CSW_REG_to_byte_and_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_WORD_SIZE ;
  
  //Write BANK_0 to select register
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	
	//Write CSW_BYTE_SIZE to csw register
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
  
  //Write memory address to TAR
	emulateswdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	//Write data to DRW
	emulateswdRegisterWrite(DRW_REG,AP,4,0x21);
	
	//Write data to DRW
	memoryWriteByte(0x12345678,0x21);
  
}

//cswDataSize = CSW_BYTE_SIZE
void test_memoryWriteByte_should_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_BYTE_SIZE ;
  
  //Write memory address to TAR
	emulateswdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	//Write data to DRW
	emulateswdRegisterWrite(DRW_REG,AP,4,0x21);
	
	//Write data to DRW
	memoryWriteByte(0x12345678,0x21);
}

//cswDataSize = CSW_BYTE_SIZE
void test_memoryWriteHalfword_should_set_CSW_REG_to_haflword_and_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_BYTE_SIZE ;
  
  //Write BANK_0 to select register
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	
	//Write CSW_HALFWORD_SIZE_SIZE to csw register
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
  
  //Write memory address to TAR
	emulateswdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	//Write data to DRW
	emulateswdRegisterWrite(DRW_REG,AP,4,0x4321);
	
	//Write data to DRW
	memoryWriteHalfword(0x12345678,0x4321);
}

//cswDataSize = CSW_HALFWORD_SIZE
void test_memoryWriteHalfword_should_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_HALFWORD_SIZE ;
  
  //Write memory address to TAR
	emulateswdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	//Write data to DRW
	emulateswdRegisterWrite(DRW_REG,AP,4,0x4321);
	
	//Write data to DRW
	memoryWriteHalfword(0x12345678,0x4321);
}

//cswDataSize = CSW_BYTE_SIZE
void test_memoryWriteWord_should_set_CSW_REG_to_word_and_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_BYTE_SIZE ;
  
	//Write BANK_0 to select register
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	
	//Write CSW_WORD_SIZE to csw register
	emulateswdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  //Write memory address to TAR
	emulateswdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	//Write data to DRW
	emulateswdRegisterWrite(DRW_REG,AP,4,0x87654321);
	
	//Write data to DRW
	memoryWriteWord(0x12345678,0x87654321);
}

//cswDataSize = CSW_WORD_SIZE
void test_memoryWriteWord_should_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_WORD_SIZE ;
  
  //Write memory address to TAR
	emulateswdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	//Write data to DRW
	emulateswdRegisterWrite(DRW_REG,AP,4,0x87654321);
	
	//Write data to DRW
	memoryWriteWord(0x12345678,0x87654321);
}

void test_swdWriteCSW_given_CSW_WORD_SIZE_should_select_BANK_0_and_set_CSW_register()
{
  int ack = 0;
  uint32_t CSW_BIT_SET = 0;
  
  CSW_BIT_SET = CSW_DEFAULT_MASK | CSW_WORD_SIZE;
  
	//Write BANK_0 to select register
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	
	//Write CSW_WORD_SIZE to csw register
	emulateswdRegisterWrite(CSW_REG, AP, OK, CSW_BIT_SET);
	
	swdWriteCSW(&ack, CSW_BIT_SET);
  
  TEST_ASSERT_EQUAL(1, ack);
}

void test_swdWriteCSW_given_CSW_WORD_SIZE_and_Enable_ADDR_INC()
{
  int ack = 0;
  uint32_t CSW_BIT_SET = 0;
  
  CSW_BIT_SET = CSW_DEFAULT_MASK | CSW_WORD_SIZE | CSW_ENABLE_ADDR_INC_PACKED;
  
	//Write BANK_0 to select register
	emulateswdRegisterWrite(SELECT_REG, DP, OK, BANK_0);
	
	//Write CSW_WORD_SIZE to csw register
	emulateswdRegisterWrite(CSW_REG, AP, OK, CSW_BIT_SET);
	
	swdWriteCSW(&ack, CSW_BIT_SET);
  
  TEST_ASSERT_EQUAL(1, ack);
  TEST_ASSERT_EQUAL(0x23000062, CSW_BIT_SET);
}

void test_memoryWriteDataAlignment_given_address_0_data_0xFF_should_return_0xFF()
{
  TEST_ASSERT_EQUAL(0xFF,memoryWriteDataAlignment(0,0xFF));
}

void test_memoryWriteDataAlignment_given_address_1_data_0xFF_should_return_0xFF00()
{
  TEST_ASSERT_EQUAL(0xFF00,memoryWriteDataAlignment(1,0xFF));
}

void test_memoryWriteDataAlignment_given_address_2_data_0xFF_should_return_0xFF0000()
{
  TEST_ASSERT_EQUAL(0xFF0000,memoryWriteDataAlignment(2,0xFF));
}

void test_memoryWriteDataAlignment_given_address_3_data_0xFF_should_return_0xFF000000()
{
  TEST_ASSERT_EQUAL(0xFF000000,memoryWriteDataAlignment(3,0xFF));
}

void test_memoryWriteDataAlignment_given_address_4_data_0xABCD_should_return_0xABCD()
{
  TEST_ASSERT_EQUAL(0xABCD,memoryWriteDataAlignment(4,0xABCD));
}

void test_memoryWriteDataAlignment_given_address_5_data_0xABCD_should_return_0xABCD00()
{
  TEST_ASSERT_EQUAL(0xABCD00,memoryWriteDataAlignment(5,0xABCD));
}

void test_memoryWriteDataAlignment_given_address_6_data_0xABCD_should_return_0xABCD0000()
{
  TEST_ASSERT_EQUAL(0xABCD0000,memoryWriteDataAlignment(6,0xABCD));
}
