#include "unity.h"
#include "swdStub.h"
#include "mock_Flash.h"
#include "mock_SRAM.h"
#include "mock_configurePort.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_stubEraseSector_should_get_start_and_end_flash_address_and_call_Flash_EraseSector_func()  {
  
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  SRAM_Read_ExpectAndReturn(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_12);
  SRAM_Read_ExpectAndReturn(SWD_FLASH_END_ADDRESS, ADDR_FLASH_SECTOR_14);
  
  Flash_EraseSector_Expect(ADDR_FLASH_SECTOR_12, ADDR_FLASH_SECTOR_14);
  
  SRAM_Write_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubEraseSector();
}

void test_stubMassErase_should_get_bank_select_from_SRAM_and_call_Flash_MassErase_func()  {
  
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  SRAM_Read_ExpectAndReturn(SWD_BANK_SELECT, MASS_ERASE_BANK_1);
  
  Flash_MassErase_Expect(FLASH_BANK_1);
  
  SRAM_Write_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubMassErase();
}

void test_stubCopy_should_get_flash_sram_start_address_length_and_call_Flash_Copy_func()  {
  
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  SRAM_Read_ExpectAndReturn(SWD_SRAM_START_ADDRESS, SWD_SRAM_DATA32_ADDRESS);
  SRAM_Read_ExpectAndReturn(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_22);
  SRAM_Read_ExpectAndReturn(SWD_DATA_LENGTH, 2048);
  
  Flash_CopyFromSramToFlash_Expect(SWD_SRAM_DATA32_ADDRESS, ADDR_FLASH_SECTOR_22, 2048);
  
  SRAM_Write_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubCopy();
}