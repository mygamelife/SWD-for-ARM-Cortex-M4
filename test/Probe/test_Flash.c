#include "unity.h"
#include "Flash.h"
#include "mock_configurePort.h"
#include "mock_stm32f4xx_hal_flash.h"
#include "mock_stm32f4xx_hal_flash_ex.h"
#include "mock_memoryRW.h"

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

void test_flashCopyFromSramToFlash_given_0x20000000_sram_address_0x08000000_flash_address_and_size_8(void) {
  
  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);

  readMemoryData_ExpectAndReturn(0x20000000, 0xEF);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_BYTE, 0x08000000, 0xEF, HAL_OK);
  
  readMemoryData_ExpectAndReturn(0x20000001, 0xBE);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_BYTE, 0x08000001, 0xBE, HAL_OK);
  
  readMemoryData_ExpectAndReturn(0x20000002, 0xAD);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_BYTE, 0x08000002, 0xAD, HAL_OK);
  
  readMemoryData_ExpectAndReturn(0x20000003, 0xDE);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_BYTE, 0x08000003, 0xDE, HAL_OK);

  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  flashCopyFromSramToFlash(0x20000000, 0x08000000, 4);
}