#include "ILV_Protocol.h"

static uint32_t targetStatus = 0;

/**
  * load_SectorErase_Instruction is a function to load the sector erase
  * instruction into SRAM to tell the swdStub
  *
  * input   : startAddress is the address to begin erase
  *           endAddress is the address to end erase
  *
  * output  : NONE
  */
void load_SectorErase_Instruction(uint32_t startAddress, uint32_t endAddress)  {
  /* Continues wait for target to release */
  do  {
    memoryAccessRead(SWD_TARGET_STATUS, &targetStatus);
  } while(targetStatus != TARGET_OK);
  
  /* load flash start and end address to sram */
  memoryAccessWrite(SWD_START_ADDRESS, startAddress);
  memoryAccessWrite(SWD_END_ADDRESS, endAddress);
  
  /* load instruction to sram */
  memoryAccessWrite(SWD_INSTRUCTION, INSTRUCTION_SECTOR_ERASE);
}

/**
  * load_MassErase_Instruction is a function to load the mass erase
  * instruction into SRAM to tell the swdStub
  *
  * input   : bankSelect can be one of the following value
  *            + FLASH_BANK_1: Bank1 to be erased
  *            + FLASH_BANK_2: Bank2 to be erased
  *            + FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  * output  : NONE
  */
void load_MassErase_Instruction(uint32_t bankSelect)  {
  /* Continues wait for target to release */
  do  {
    memoryAccessRead(SWD_TARGET_STATUS, &targetStatus);
  } while(targetStatus != TARGET_OK);
  
  /* load bank select to sram */
  memoryAccessWrite(SWD_BANK_SELECT, bankSelect);
  
  /* load instruction to sram */
  memoryAccessWrite(SWD_INSTRUCTION, INSTRUCTION_MASS_ERASE);  
}
