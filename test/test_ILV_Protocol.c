#include "unity.h"
#include "Clock.h"
#include "Emulator.h"
#include "mock_Flash.h"
#include "ILV_Protocol.h"
#include "swd_Utilities.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"
#include "mock_configurePort.h"
#include "mock_IO_Operations.h"

uint32_t readDummy = 0xFFFFFFFF;

void setUp(void)
{
  
}

void tearDown(void)
{
}

void test_load_SectorErase_Instruction_should_wait_untill_target_response_OK_before_load_instruction(void)
{
  /* Read target status */
	emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_TARGET_STATUS);
	emulateSWDRegister_Read(DRW_REG,AP,OK,0,readDummy);
	emulateSWDRegister_Read(DRW_REG,AP,OK,1,TARGET_BUSY);
  
  emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_TARGET_STATUS);
	emulateSWDRegister_Read(DRW_REG,AP,OK,0,readDummy);
	emulateSWDRegister_Read(DRW_REG,AP,OK,1,TARGET_BUSY);
  
  emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_TARGET_STATUS);
	emulateSWDRegister_Read(DRW_REG,AP,OK,0,readDummy);
	emulateSWDRegister_Read(DRW_REG,AP,OK,0,TARGET_OK);
  
  /* Load start addres */
	emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_FLASH_START_ADDRESS);
	emulateSWDRegister_Write(DRW_REG,AP,OK,ADDR_FLASH_SECTOR_20);
  
  /* Load end addres */
  emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_FLASH_END_ADDRESS);
	emulateSWDRegister_Write(DRW_REG,AP,OK,ADDR_FLASH_SECTOR_22);
  
  /* Load Instruction */
  emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_INSTRUCTION);
	emulateSWDRegister_Write(DRW_REG,AP,OK,INSTRUCTION_ERASE_SECTOR);
  
  Load_SectorErase_Instruction(ADDR_FLASH_SECTOR_20, ADDR_FLASH_SECTOR_22);
}

void test_load_MassErase_Instruction_should_wait_untill_target_response_OK_before_load_instruction(void)
{
  /* Read target status */
	emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_TARGET_STATUS);
	emulateSWDRegister_Read(DRW_REG,AP,OK,0,readDummy);
	emulateSWDRegister_Read(DRW_REG,AP,OK,0,TARGET_OK);
  
  /* Load bank select */
	emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_BANK_SELECT);
	emulateSWDRegister_Write(DRW_REG,AP,OK,FLASH_BANK_BOTH);
  
  /* Load Instruction */
  emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_INSTRUCTION);
	emulateSWDRegister_Write(DRW_REG,AP,OK,INSTRUCTION_MASS_ERASE);
  
  Load_MassErase_Instruction(FLASH_BANK_BOTH);
}

void test_Load_Copy_Instruction_should_load_src_address_dest_address_and_length_into_SRAM_instruction_address(void)
{
  /* Read target status */
	emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_TARGET_STATUS);
	emulateSWDRegister_Read(DRW_REG,AP,OK,0,readDummy);
	emulateSWDRegister_Read(DRW_REG,AP,OK,0,TARGET_OK);
  
  /* Load Source address */
	emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_SRAM_START_ADDRESS);
	emulateSWDRegister_Write(DRW_REG,AP,OK,SWD_SRAM_DATA32_ADDRESS);
  
  /* Load Destination address */
	emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_FLASH_START_ADDRESS);
	emulateSWDRegister_Write(DRW_REG,AP,OK,ADDR_FLASH_SECTOR_18);
  
  /* Load length */
	emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_DATA_LENGTH);
	emulateSWDRegister_Write(DRW_REG,AP,OK,2000);
  
  /* Load Instruction */
  emulateSWDRegister_Write(TAR_REG,AP,OK,SWD_INSTRUCTION);
	emulateSWDRegister_Write(DRW_REG,AP,OK,INSTRUCTION_COPY);
  
  Load_Copy_Instruction(SWD_SRAM_DATA32_ADDRESS, ADDR_FLASH_SECTOR_18, 2000);
}