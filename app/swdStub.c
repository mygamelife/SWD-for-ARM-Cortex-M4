#include "swdStub.h"

static __IO uint32_t Flash_Start_Address = 0;
static __IO uint32_t Flash_End_Address = 0;
static __IO uint32_t SRAM_Start_Address = 0;
static int DataLength = 0;

/**
  * swdStub is small program routine take instruction from swd probe and response
  *
  * input   : swdInstruction is an instruction send by probe
  * return  : NONE
  */
void swdStub(uint32_t swdInstruction) {
  if(swdInstruction == INSTRUCTION_COPY)	{
    stubCopy();
  }
  
  else if(swdInstruction == INSTRUCTION_SECTOR_ERASE)	{
    stubSectorErase();
  }
  
  else if(swdInstruction == INSTRUCTION_MASS_ERASE)	{
    stubMassErase();
  }
}

/**
  * swdCopy is small program routine to copy data in SRAM to Flash
  *
  * input   : NONE
  * return  : NONE
  */
void stubCopy(void) {
  /* Inform probe now target is busy */
  *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_BUSY;
  
  /* Extract flash destination address from sram */
  Flash_Start_Address = *(__IO uint32_t*)SWD_FLASH_START_ADDRESS;
  
  /* Extract SRAM source address from sram */
  SRAM_Start_Address = *(__IO uint32_t*)SWD_SRAM_START_ADDRESS;
  
  /* Extract DataLength from sram */
  DataLength  = *(__IO int*)SWD_DATA_LENGTH;
  
  copyFromRamToFlash(SRAM_Start_Address, Flash_Start_Address, DataLength);
  
  /* Clear instruction prevent keep copy */
  *(__IO uint32_t*)SWD_INSTRUCTION    = INSTRUCTION_CLEAR;
  
  /* Tell probe now target is ready for next instruction */
  *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_OK;
}

/**
  * swdSectorErase is small program routine to erase specific 
  * flash sector address given by user
  *
  * input   : NONE
  * return  : NONE
  */
void stubSectorErase(void)  {
  *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_BUSY;
  
  /* Extract user start address from sram */
  Flash_Start_Address = *(__IO uint32_t*)SWD_FLASH_START_ADDRESS;
  
  /* Extract user start address from sram */
  Flash_End_Address = *(__IO uint32_t*)SWD_FLASH_END_ADDRESS;
  
  /* Perform sector erase here */
  sectorErase(Flash_Start_Address, Flash_End_Address);
  
  /* Turn off verify checking LED */
  turnOffLED3();
  
  /* Clear instruction prevent keep erase */
  *(__IO uint32_t*)SWD_INSTRUCTION    = INSTRUCTION_CLEAR;
  
  /* Tell probe now target is ready for next instruction */
  *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_OK;  
}

void stubMassErase(void)  {
  static __IO uint32_t bankSelect = 0;
  
  *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_BUSY;
  
  /** Check MASS_ERASE_BANK_SELECT it
    * determine which bank need to be erase */
  bankSelect = *(__IO uint32_t*)SWD_BANK_SELECT;
    
  if(bankSelect == MASS_ERASE_BANK_1){
    bankSelect = FLASH_BANK_1;
  }
  
  else if(bankSelect == MASS_ERASE_BOTH_BANK) {
    bankSelect = FLASH_BANK_BOTH;  
  }
  
  else  {
    bankSelect = FLASH_BANK_2;
  }
  
  /* Perform mass erase here */
  massErase(bankSelect);
  
  /* Turn off verify checking LED */
  turnOffLED3();
    
  /* Clear instruction prevent keep erase */
  *(__IO uint32_t*)SWD_INSTRUCTION    = INSTRUCTION_CLEAR;

  /* Tell probe now target is ready for next instruction */
  *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_OK;
}
