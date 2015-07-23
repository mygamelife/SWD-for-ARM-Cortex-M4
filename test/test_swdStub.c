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

void test_sstubEraseSector_should_set_target_busy_and_call_Flash_EraseSector_func_when_done_set_target_to_Ok()  {
  
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  Flash_EraseSector_Expect(ADDR_FLASH_SECTOR_12, ADDR_FLASH_SECTOR_14);
  
  turnOffLED3_Expect();
  
  SRAM_Write_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
	stubEraseSector(ADDR_FLASH_SECTOR_12, ADDR_FLASH_SECTOR_14);
}

void test_stubMassErase_should_set_target_busy_and_call_Flash_MassErase_func_when_done_set_target_to_Ok()  {
  
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  Flash_MassErase_Expect(FLASH_BANK_1);
  
  turnOffLED3_Expect();
  
  SRAM_Write_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
	stubMassErase(FLASH_BANK_1);
}

void test_stubMassErase_should_set_target_busy_and_call_Flash_Copy_func_when_done_set_target_to_Ok()  {
  
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  Flash_CopyFromSramToFlash_Expect(SWD_SRAM_DATA32_ADDRESS, ADDR_FLASH_SECTOR_18, 2048);
  
  SRAM_Write_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  SRAM_Write_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
	stubCopy(SWD_SRAM_DATA32_ADDRESS, ADDR_FLASH_SECTOR_18, 2048);
}