#include "unity.h"
#include "Delay.h"
#include "IoOperations.h"
#include "Emulator.h"
#include "Register_ReadWrite.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"

void setUp(void){}
void tearDown(void){}

 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    1      |    0     |     1      |     0     |     0       |    0      |     1     |
 ******************************************************************************************************/
void test_swdRegisterWrite_given_Address_0x4_AP_Write_data_0xFFFFFFFF()
{
	emulateSwdRegisterWrite(TAR_REG, AP, OK, 0xFFFFFFFF);
	swdRegisterWrite(TAR_REG, AP, 0xFFFFFFFF);
}

 /******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    1      |   1     |     1      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************/
void test_SWDRegisterRead_given_Address_0x4_AP_READ() {
	int parity = 0, ack = 0;
	uint32_t dataRead = 0 ;
  
	emulateSwdRegisterRead(0x4, AP, OK, 1, interconvertMSBandLSB(0x1000000));
	swdRegisterRead(0x4, AP, &dataRead);

	TEST_ASSERT_EQUAL(0x1000000,dataRead);
}

void test_swdReadAP_should_call_swdRegisterRead_2times()
{
	uint32_t dataRead = 0 ;
	
	emulateSwdRegisterRead(TAR_REG, AP, OK, 1, 0x10);
	emulateSwdRegisterRead(TAR_REG, AP, OK, 1, 0x88112233);
	
	swdReadAP(TAR_REG, &dataRead);
	TEST_ASSERT_EQUAL(interconvertMSBandLSB(0x88112233),dataRead);
}

void test_readAhbIDR_should_clear_flags_after_readSelect_and_readAP()
{
  uint32_t data_IDR = 0;
	emulateSwdRegisterWrite(CTRLSTAT_REG, DP, OK, POWERUP_SYSTEM);
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANKF);
	emulateSwdRegisterRead(IDR_REG, AP, OK, 1, interconvertMSBandLSB(0x24770011));
	emulateSwdRegisterRead(IDR_REG, AP, OK, 1, interconvertMSBandLSB(0x24770011));
  
  readAhbIDR(&data_IDR);
  TEST_ASSERT_EQUAL(data_IDR, 0x24770011);
}

void test_readAhbIDR_return_WAIT_RESPONSE_should_retries_DPABORT_and_resend()
{
  uint32_t data_IDR = 0;
	emulateSwdRegisterWrite(CTRLSTAT_REG, DP, OK, POWERUP_SYSTEM);
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANKF);
	emulateSwdRegisterRead(IDR_REG, AP, WAIT, 1, interconvertMSBandLSB(0x24770011));
	emulateSwdRegisterRead(IDR_REG, AP, WAIT, 1, interconvertMSBandLSB(0x24770011));
  
  // Retries
  emulateSwdRegisterRead(IDR_REG, AP, WAIT, 1, interconvertMSBandLSB(0x24770011));
	emulateSwdRegisterRead(IDR_REG, AP, WAIT, 1, interconvertMSBandLSB(0x24770011));
  
  emulateSwdRegisterRead(IDR_REG, AP, OK, 1, interconvertMSBandLSB(0x24770011));
	emulateSwdRegisterRead(IDR_REG, AP, OK, 1, interconvertMSBandLSB(0x24770011));
  
  readAhbIDR(&data_IDR);
  TEST_ASSERT_EQUAL(data_IDR, 0x24770011);
}

void test_swdSelectMemorySize_given_CSW_WORD_SIZE_should_select_BANK_0_and_set_CSW_register()
{
  int ack = 0;
  uint32_t CSW_BIT_SET = 0;
  
  CSW_BIT_SET = CSW_DEFAULT_MASK | CSW_WORD_SIZE;
  
	// Write BANK_0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	
	// Write CSW_WORD_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, CSW_BIT_SET);
	
	swdSelectMemorySize(CSW_BIT_SET);
}

void test_swdSelectMemorySize_given_CSW_WORD_SIZE_and_Enable_ADDR_INC()
{
  int ack = 0;
  uint32_t CSW_BIT_SET = 0;
  
  CSW_BIT_SET = CSW_DEFAULT_MASK | CSW_WORD_SIZE | CSW_ENABLE_ADDR_INC_PACKED;
  
	// Write BANK_0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	
	// Write CSW_WORD_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, CSW_BIT_SET);
	
	swdSelectMemorySize(CSW_BIT_SET);
  
  TEST_ASSERT_EQUAL(0x23000062, CSW_BIT_SET);
}

void test_memoryReadWord_given_Address_0x12345678_should_write_address_to_TAR_and_read_data_from_DRW()
{
	uint32_t dataRead = 0 ;
	
  cswDataSize = CSW_WORD_SIZE ;
  
	// Write memory address to TAR
	emulateSwdRegisterWrite(TAR_REG, AP, OK, 0x12345678);

	// Read and Discard dummy data
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0xAABBCCDD);
	
	// Read actual data from DRW
	emulateSwdRegisterRead(DRW_REG, AP, OK, 1, 0x10);
	
	memoryReadWord(0x12345678, &dataRead);
	
	TEST_ASSERT_EQUAL(interconvertMSBandLSB(0x10),dataRead);
}

void test_memoryWriteByte_should_set_CSW_REG_to_byte_and_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_WORD_SIZE;
  
  // Write BANK_0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	
	// Write CSW_BYTE_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
  
  // Write memory address to TAR
	emulateSwdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	// Write data to DRW
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x21);
	
	// Write data to DRW
	memoryWriteByte(0x12345678,0x21);
}

void test_memoryWriteByte_should_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_BYTE_SIZE;
  
  // Write memory address to TAR
	emulateSwdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	// Write data to DRW
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x21);
	
	// Write data to DRW
	memoryWriteByte(0x12345678,0x21);
}

void test_memoryWriteHalfword_should_set_CSW_REG_to_haflword_and_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_BYTE_SIZE;
  
  // Write BANK_0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	
	// Write CSW_HALFWORD_SIZE_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
  
  // Write memory address to TAR
	emulateSwdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	// Write data to DRW
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x4321);
	
	// Write data to DRW
	memoryWriteHalfword(0x12345678,0x4321);
}

void test_memoryWriteHalfword_should_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_HALFWORD_SIZE;
  
  // Write memory address to TAR
	emulateSwdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	// Write data to DRW
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x4321);
	
	// Write data to DRW
	memoryWriteHalfword(0x12345678,0x4321);
}

void test_memoryWriteWord_should_set_CSW_REG_to_word_and_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_BYTE_SIZE;
  
	// Write BANK_0 to select register
	emulateSwdRegisterWrite(SELECT_REG, DP, OK, SELECT_BANK0);
	
	// Write CSW_WORD_SIZE to csw register
	emulateSwdRegisterWrite(CSW_REG, AP, OK, (CSW_DEFAULT_MASK | CSW_WORD_SIZE));
  
  // Write memory address to TAR
	emulateSwdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	// Write data to DRW
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x87654321);
	
	// Write data to DRW
	memoryWriteWord(0x12345678,0x87654321);
}

void test_memoryWriteWord_should_write_address_to_TAR_and_data_to_DRW()
{
  cswDataSize = CSW_WORD_SIZE;
  
  // Write memory address to TAR
	emulateSwdRegisterWrite(TAR_REG,AP,4,0x12345678);
	
	// Write data to DRW
	emulateSwdRegisterWrite(DRW_REG,AP,4,0x87654321);
	
	// Write data to DRW
	memoryWriteWord(0x12345678,0x87654321);
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
