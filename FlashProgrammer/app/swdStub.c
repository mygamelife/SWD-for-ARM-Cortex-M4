#include "swdStub.h"

/**
  * swdStub is small program routine take instruction from swd probe and response
  *
  * input   : swdInstruction is an instruction send by probe
  * return  : NONE
  */
void swdStub(uint32_t swdInstruction) {
  switch(swdInstruction)  {
    case INSTRUCTION_COPY :
      stubCopy();
      break;
    
    case INSTRUCTION_ERASE_SECTOR :
      stubErase();
      break;
  
    case INSTRUCTION_MASS_ERASE :
      stubMassErase();
      break;
  }
}

/** stubCopy is small program routine to copy data in SRAM to Flash
  *
  * input   : NONE
  * return  : NONE
  */
void stubCopy(void) {
  __IO uint32_t flashAddress = 0, sramAddress = 0;
  __IO int size = 0;
  
  /* Change target status to busy to prevent other function to interrupt */
  writeMemoryData(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /* Read SRAM source address from sram */
  sramAddress  = (__IO uint32_t)readMemoryData(SWD_SRAM_START_ADDRESS);
  /* Read flash destination address from sram */
  flashAddress = (__IO uint32_t)readMemoryData(SWD_FLASH_START_ADDRESS);
  /* Read Data Length from sram */
  size = (__IO int)readMemoryData(SWD_DATA_SIZE);

  flashCopyFromSramToFlash(sramAddress, flashAddress, size);
  
  /* Clear instruction prevent keep erase */
  writeMemoryData(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  /* Tell probe now target is ready for next instruction */
  writeMemoryData(SWD_TARGET_STATUS, TARGET_OK);
}

/**
  * swdSectorErase is small program routine to erase specific 
  * flash sector address given by user
  *
  * input   : NONE
  * return  : NONE
  */
void stubErase(void)  {
  __IO uint32_t flashAddress = 0;
  __IO int flashSize = 0;

  /* Change target status to busy to prevent other function to interrupt */
  writeMemoryData(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /* Read user start address from sram */
  flashAddress = (__IO uint32_t)readMemoryData(SWD_FLASH_START_ADDRESS);
  
  /* Read user end address from sram */
  flashSize = (__IO uint32_t)readMemoryData(SWD_DATA_SIZE);
  
  flashErase(flashAddress, flashSize);
  
  /* Clear instruction prevent keep erase */
  writeMemoryData(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  
  /* Tell probe now target is ready for next instruction */
  writeMemoryData(SWD_TARGET_STATUS, TARGET_OK);
}

/** stubMassErase is small program routine to erase specific 
  * bank according to the user selection
  *
  * input   : NONE
  * return  : NONE
  */
void stubMassErase(void)  {
  __IO uint32_t bankSelect = 0;

  /* Change target status to busy to prevent other function to interrupt */
  writeMemoryData(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /** Check MASS_ERASE_BANK_SELECT it
    * determine which bank need to be erase */
  bankSelect = (__IO uint32_t)readMemoryData(SWD_BANK_SELECT);
    
  if(bankSelect == MASS_ERASE_BANK_1)
    bankSelect = FLASH_BANK_1;
  
  else if(bankSelect == MASS_ERASE_BOTH_BANK)
    bankSelect = FLASH_BANK_BOTH;
  
  else bankSelect = FLASH_BANK_2;
  
  /* Perform mass erase here */
  flashMassErase(bankSelect);
    
  /* Clear instruction prevent keep erase */
  writeMemoryData(SWD_INSTRUCTION, INSTRUCTION_CLEAR);

  /* Tell probe now target is ready for next instruction */
  writeMemoryData(SWD_TARGET_STATUS, TARGET_OK);
}