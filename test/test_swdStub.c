#include "unity.h"
#include "Delay.h"
#include "swdStub.h"
#include "Emulator.h"
#include "mock_Flash.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "mock_Register_ReadWrite.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"

void setUp(void)  {}

void tearDown(void) {}

void test_stubErase_should_get_flash_address_and_flash_size_and_call_flashErase_func(void) {
  writeMemoryData_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  readMemoryData_ExpectAndReturn(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_12);
  readMemoryData_ExpectAndReturn(SWD_DATA_SIZE, 16);
  
  flashErase_Expect(ADDR_FLASH_SECTOR_12, 16);
  
  writeMemoryData_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  writeMemoryData_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubErase();
}

void test_stubMassErase_should_get_bank_select_from_SRAM_and_call_flashMassErase_func()  {
  
  writeMemoryData_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  readMemoryData_ExpectAndReturn(SWD_BANK_SELECT, MASS_ERASE_BANK_1);
  
  flashMassErase_Expect(FLASH_BANK_1);
  
  writeMemoryData_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  writeMemoryData_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubMassErase();
}

void test_stubCopy_should_get_flash_sram_start_address_length_and_call_Flash_Copy_func()  {
  
  writeMemoryData_Expect(SWD_TARGET_STATUS, TARGET_BUSY);
  
  readMemoryData_ExpectAndReturn(SWD_SRAM_START_ADDRESS, 0x20000000);
  readMemoryData_ExpectAndReturn(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_22);
  readMemoryData_ExpectAndReturn(SWD_DATA_SIZE, 2048);
  
  flashCopyFromSramToFlash_Expect(0x20000000, ADDR_FLASH_SECTOR_22, 2048);
  
  writeMemoryData_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  writeMemoryData_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubCopy();
}