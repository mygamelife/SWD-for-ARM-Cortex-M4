#include "unity.h"
#include "Flash.h"
#include "mock_configurePort.h"
#include "mock_stm32f4xx_hal_flash.h"
#include "mock_stm32f4xx_hal_flash_ex.h"
#include "mock_stm32f4xx_hal_flash_ramfunc.h"
#include "mock_Register_ReadWrite.h"

void setUp(void){}

void tearDown(void){}

void test_flashGetSector_given_FLASH_SECTOR_0_should_select_FLASH_SECTOR_0()  {
  uint32_t sector = 0;
  
  sector = flashGetSector(ADDR_FLASH_SECTOR_0);
  TEST_ASSERT_EQUAL(FLASH_SECTOR_0, sector);
}

void test_flashGetSector_given_flash_sector_23_should_return_23()  {
  uint32_t sector = 0;
  
  sector = flashGetSector(ADDR_FLASH_SECTOR_23);
  TEST_ASSERT_EQUAL(FLASH_SECTOR_23, sector);
}

void test_flashGetSector_given_last_address_of_flash_sector_23_should_return_23()  {
  uint32_t sector = 0;
  
  sector = flashGetSector(0x081FFFFF);
  TEST_ASSERT_EQUAL(FLASH_SECTOR_23, sector);
}

void test_flashMassErase_given_bank2_should_erase_whole_bank_2() {
  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASHEx_Erase_IgnoreAndReturn(HAL_OK);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  flashMassErase(FLASH_BANK_2);
}

void test_flashErase_given_StartSector_15_and_20() {
  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASHEx_Erase_IgnoreAndReturn(HAL_OK);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  flashErase(ADDR_FLASH_SECTOR_15, 20);
}

void test_flashWriteProgram_given_FLASH_TYPEPROGRAM_WORD() {
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0x081FFFFF, 0xABCDABCD, HAL_OK);
  
  flashWriteProgram(FLASH_TYPEPROGRAM_WORD, 0x081FFFFF, 0xABCDABCD);
}

void test_flashWrite_given_data_0xABCDABCD_should_write_into_ADDR_FLASH_SECTOR_19()  {
  uint32_t data = 0xABCDABCD;

  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, ADDR_FLASH_SECTOR_19, data, HAL_OK);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  flashWrite(&data, ADDR_FLASH_SECTOR_19, 1);
}

void test_flashWrite_given_data_and_size_5_should_write_into_Flash()  {
  uint32_t data[] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555};

  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0x08000000, data[0], HAL_OK);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0x08000004, data[1], HAL_OK);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0x08000008, data[2], HAL_OK);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0x0800000C, data[3], HAL_OK);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0x08000010, data[4], HAL_OK);

  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  flashWrite(data, 0x08000000, 20);
}

void test_flashCopyFromSramToFlash_given_0x20000000_sram_address_0x08000000_flash_address_and_size_8(void) {
  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASHEx_Erase_IgnoreAndReturn(HAL_OK); //Erase
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  readMemoryData_ExpectAndReturn(0x20000000, 0xDEADBEEF);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0x08000000, 0xDEADBEEF, HAL_OK);
  
  readMemoryData_ExpectAndReturn(0x20000004, 0xBEEFCAFE);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0x08000004, 0xBEEFCAFE, HAL_OK);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  flashCopyFromSramToFlash(0x20000000, 0x08000000, 8);
}

void test_flashVerify_given_0x20000000_sram_address_0x08000000_flash_address_and_size_16(void) {
  
  readMemoryData_ExpectAndReturn(0x20000000, 0x11111111);
  readMemoryData_ExpectAndReturn(0x08000000, 0x11111111);
  
  readMemoryData_ExpectAndReturn(0x20000004, 0x22222222);
  readMemoryData_ExpectAndReturn(0x08000004, 0x22222222);
  
  readMemoryData_ExpectAndReturn(0x20000008, 0x33333333);
  readMemoryData_ExpectAndReturn(0x08000008, 0x33333333);
  
  readMemoryData_ExpectAndReturn(0x2000000C, 0x44444444);
  readMemoryData_ExpectAndReturn(0x0800000C, 0x44444444);
  
  flashVerify(0x20000000, 0x08000000, 16);
}