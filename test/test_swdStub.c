#include "unity.h"
#include "swdStub.h"
#include "Emulator.h"
#include "mock_SRAM.h"
#include "mock_Flash.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "Register_ReadWrite.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"

uint32_t readDummy = 0xFFFFFFFF;

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
  sramRead_ExpectAndReturn(SWD_DATA_LENGTH, 2048);
  
  flashCopyFromSramToFlash_Expect(SWD_SRAM_DATA32_ADDRESS, ADDR_FLASH_SECTOR_22, 2048);
  
  sramWrite_Expect(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  sramWrite_Expect(SWD_TARGET_STATUS, TARGET_OK);
  
  stubCopy();
}

void test_load_SectorErase_Instruction_should_wait_untill_target_response_OK_before_load_instruction(void)
{
  /* Read target status */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_TARGET_STATUS);
  emulateSwdRegisterRead(DRW_REG,AP,OK,0,readDummy);
  emulateSwdRegisterRead(DRW_REG,AP,OK,1,TARGET_BUSY);
  
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_TARGET_STATUS);
  emulateSwdRegisterRead(DRW_REG,AP,OK,0,readDummy);
  emulateSwdRegisterRead(DRW_REG,AP,OK,0,TARGET_OK);
  
  /* Load start addres */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_FLASH_START_ADDRESS);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,ADDR_FLASH_SECTOR_20);
  
  /* Load end addres */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_FLASH_END_ADDRESS);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,ADDR_FLASH_SECTOR_22);
  
  /* Load Instruction */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_INSTRUCTION);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,INSTRUCTION_ERASE_SECTOR);
  
  loadEraseSectorInstruction(ADDR_FLASH_SECTOR_20, ADDR_FLASH_SECTOR_22);
}

void test_loadMassEraseInstruction_should_wait_untill_target_response_OK_before_load_instruction(void)
{
  /* Read target status */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_TARGET_STATUS);
  emulateSwdRegisterRead(DRW_REG,AP,OK,0,readDummy);
  emulateSwdRegisterRead(DRW_REG,AP,OK,0,TARGET_OK);
  
  /* Load bank select */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_BANK_SELECT);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,FLASH_BANK_BOTH);
  
  /* Load Instruction */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_INSTRUCTION);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,INSTRUCTION_MASS_ERASE);
  
  loadMassEraseInstruction(FLASH_BANK_BOTH);
}

void test_loadCopyInstruction_should_load_src_address_dest_address_and_length_into_SRAM_instruction_address(void)
{
  /* Read target status */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_TARGET_STATUS);
  emulateSwdRegisterRead(DRW_REG,AP,OK,0,readDummy);
  emulateSwdRegisterRead(DRW_REG,AP,OK,0,TARGET_OK);
  
  /* Load Source address */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_SRAM_START_ADDRESS);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,SWD_SRAM_DATA32_ADDRESS);
  
  /* Load Destination address */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_FLASH_START_ADDRESS);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,ADDR_FLASH_SECTOR_18);
  
  /* Load length */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_DATA_LENGTH);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,2000);
  
  /* Load Instruction */
  emulateSwdRegisterWrite(TAR_REG,AP,OK,SWD_INSTRUCTION);
  emulateSwdRegisterWrite(DRW_REG,AP,OK,INSTRUCTION_COPY);
  
  loadCopyInstruction(SWD_SRAM_DATA32_ADDRESS, ADDR_FLASH_SECTOR_18, 2000);
}