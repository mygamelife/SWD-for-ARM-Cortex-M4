#include "unity.h"
#include "Flash.h"
#include "mock_configurePort.h"
#include "mock_stm32f4xx_hal_flash.h"
#include "mock_stm32f4xx_hal_flash_ex.h"
#include "mock_stm32f4xx_hal_flash_ramfunc.h"

void setUp(void){}

void tearDown(void){}

void test_flash_Error_Handler_should_turn_on_LED4()  {
  turnOnLED4_Expect();
  Error_Handler();
}

void test_flash_GetSector_given_FLASH_SECTOR_0_should_select_FLASH_SECTOR_0()  {
  uint32_t sector = 0;
  
  sector = GetSector(ADDR_FLASH_SECTOR_0);
  TEST_ASSERT_EQUAL(FLASH_SECTOR_0, sector);
}

void test_flash_GetSector_given_flash_sector_23_should_return_23()  {
  uint32_t sector = 0;
  
  sector = GetSector(ADDR_FLASH_SECTOR_23);
  TEST_ASSERT_EQUAL(FLASH_SECTOR_23, sector);
}

void test_flash_GetSector_given_last_address_of_flash_sector_23_should_return_23()  {
  uint32_t sector = 0;
  
  sector = GetSector(0x081FFFFF);
  TEST_ASSERT_EQUAL(FLASH_SECTOR_23, sector);
}

void test_eraseFlashMemory_given_FLASH_TYPEERASE_SECTORS_should_perform_sector_erase()  {
  uint32_t sector = 0, sectorError = 0;
  uint32_t firstSector = 0, numOfSectors = 0;
  
  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASHEx_Erase_IgnoreAndReturn(HAL_OK);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  eraseFlashMemory(FLASH_TYPEERASE_SECTORS, 0, FLASH_VOLTAGE_RANGE_3);
}

void test_eraseFlashMemory_if_Flash_Erase_return_HAL_ERROR_func_should_fail()  {
  uint32_t sector = 0, sectorError = 0;
  uint32_t firstSector = 0, numOfSectors = 0;
  
  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASHEx_Erase_IgnoreAndReturn(HAL_ERROR);
  HAL_FLASH_GetError_ExpectAndReturn(HAL_ERROR);
  turnOnLED4_Expect();
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  eraseFlashMemory(FLASH_TYPEERASE_SECTORS, 0, FLASH_VOLTAGE_RANGE_3);
}

void test_writeToFlash_given_data_0xABCDABCD_should_write_into_address_0()  {
  uint32_t sector = 0, data = 0x11223344;

  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, 0, 0x11223344, HAL_OK);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  writeToFlash(0, 4, FLASH_TYPEPROGRAM_WORD, 0x11223344);
}

void test_writeToFlash_given_data_0xABCDABCD_should_write_into_address_0_until_address_36()  {
  int i = 0;
  uint32_t sector = 0, data = 0xFFFFFFFF, address = 0x0;

  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  
  for(i; i < 40; i += 4)  {
    HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, address, data, HAL_OK);
    address = address + 4;
  }
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  writeToFlash(0, 40, FLASH_TYPEPROGRAM_WORD, data);
}

void test_writeToFlash_given_data_0xABCDABCD_should_write_into_address_400_until_address_796()  {
  int i = 0, address = 400;
  uint32_t sector = 0, data = 0xFFFFFFFF;

  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  
  for(i = address; i < 800; i += 4)  {
    HAL_FLASH_Program_ExpectAndReturn(FLASH_TYPEPROGRAM_WORD, address, data, HAL_OK);
    address = address + 4;
  }
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  writeToFlash(400, 800, FLASH_TYPEPROGRAM_WORD, data);
}

void test_readFromFlash_given_pBEEF_address_should_get_the_data_0xBEEF()  {
  uint32_t *pBEEF;
  uint32_t beef = 0xBEEF, result = 0;
  
  pBEEF = &beef;
  
  result = readFromFlash((uint32_t)pBEEF);
  TEST_ASSERT_EQUAL(result, beef);
}