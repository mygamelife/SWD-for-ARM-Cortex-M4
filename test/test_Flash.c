#include "unity.h"
#include "Flash.h"
#include "mock_stm32f429i_discovery.h"
#include "mock_stm32f4xx_hal_flash.h"
#include "mock_stm32f4xx_hal_flash_ex.h"
#include "mock_stm32f4xx_hal_flash_ramfunc.h"

void setUp(void){}

void tearDown(void){}

void test_flash_Error_Handler_should_turn_on_LED4()  {
  
  BSP_LED_On_Expect(LED4);
  Error_Handler();
}

void test_flash_GetSector_given_FLASH_SECTOR_0_should_select_FLASH_SECTOR_0()  {
  uint32_t sector = 0;
  
  sector = GetSector(ADDR_FLASH_SECTOR_0);
  TEST_ASSERT_EQUAL(FLASH_SECTOR_0, sector);
}

void test_flash_GetSector_given_flash_address_out_of_the_sector_boundary_should_return_0()  {
  uint32_t sector = 0;
  
  BSP_LED_On_Expect(LED4);
  sector = GetSector(0x8200003);
  TEST_ASSERT_EQUAL(0, sector);
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
  BSP_LED_On_Expect(LED4);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  eraseFlashMemory(FLASH_TYPEERASE_SECTORS, 0, FLASH_VOLTAGE_RANGE_3);
}

void test_writeToFlash_given_data_0xABCDABCD_should_write_into_address_SECTOR13_until_SECTOR14()  {
  uint32_t sector = 0;

  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASH_Program_IgnoreAndReturn(HAL_OK);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  writeToFlash(FLASH_TYPEPROGRAM_WORD, 0x11223344);
}

void test_writeToFlash_if_Flash_Program_return_HAL_ERROR_program_should_stop_and_go_to_Error_Handler()  {
  uint32_t sector = 0;

  HAL_FLASH_Unlock_ExpectAndReturn(HAL_OK);
  HAL_FLASH_Program_IgnoreAndReturn(HAL_ERROR);
  HAL_FLASH_GetError_ExpectAndReturn(HAL_ERROR);
  BSP_LED_On_Expect(LED4);
  HAL_FLASH_Lock_ExpectAndReturn(HAL_OK);
  
  writeToFlash(FLASH_TYPEPROGRAM_WORD, 0xDEADBEEF);
}