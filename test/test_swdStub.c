#include "unity.h"
#include "Delay.h"
#include "swdStub.h"
#include "Emulator.h"
#include "mock_SRAM.h"
#include "mock_Flash.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "Register_ReadWrite.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"

void setUp(void)  {}

void tearDown(void) {}

void test_stubEraseSector_should_get_start_and_end_flash_address_and_call_flashEraseSector_func()  {
  cswDataSize = CSW_WORD_SIZE ;
  sramWrite_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  sramRead_ExpectAndReturn(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_12);
  sramRead_ExpectAndReturn(SWD_FLASH_END_ADDRESS, ADDR_FLASH_SECTOR_14);
  
  flashEraseSector_Expect(ADDR_FLASH_SECTOR_12, ADDR_FLASH_SECTOR_14);
  
  sramWrite_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  sramWrite_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubEraseSector();
}

void test_stubMassErase_should_get_bank_select_from_SRAM_and_call_flashMassErase_func()  {
  
  sramWrite_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  sramRead_ExpectAndReturn(SWD_BANK_SELECT, MASS_ERASE_BANK_1);
  
  flashMassErase_Expect(FLASH_BANK_1);
  
  sramWrite_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  sramWrite_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubMassErase();
}

void test_stubCopy_should_get_flash_sram_start_address_length_and_call_Flash_Copy_func()  {
  
  sramWrite_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  sramRead_ExpectAndReturn(SWD_SRAM_START_ADDRESS, SWD_SRAM_DATA32_ADDRESS);
  sramRead_ExpectAndReturn(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_22);
  sramRead_ExpectAndReturn(SWD_DATA_SIZE, 2048);
  
  flashCopyFromSRAMToFlash_Ignore();
  
  sramWrite_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  sramWrite_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubCopyFromSRAMToFlash();
}