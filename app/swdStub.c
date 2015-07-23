#include "swdStub.h"

static __IO uint32_t Flash_Start_Address = 0;
static __IO uint32_t Flash_End_Address = 0;
static __IO uint32_t SRAM_Start_Address = 0;
static __IO uint32_t bankSelect = 0;

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
      stubEraseSector();
      break;
  
    case INSTRUCTION_MASS_ERASE :
      stubMassErase();
      break;
  }
}

/**
  * swdCopy is small program routine to copy data in SRAM to Flash
  *
  * input   : NONE
  * return  : NONE
  */
void stubCopy(void) {
  uint32_t length = 0;
  
  /* Change target status to busy to prevent other function to interrupt */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /* Read SRAM source address from sram */
  SRAM_Start_Address  = (__IO uint32_t)SRAM_Read(SWD_SRAM_START_ADDRESS);
  
  /* Read flash destination address from sram */
  Flash_Start_Address = (__IO uint32_t)SRAM_Read(SWD_FLASH_START_ADDRESS);
  
  /* Read Data Length from sram */
  length  = (__IO int)SRAM_Read(SWD_DATA_LENGTH);
  
  Flash_CopyFromSramToFlash(SRAM_Start_Address, Flash_Start_Address, length);
  
  /* Clear instruction prevent keep erase */
  SRAM_Write(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  
  /* Tell probe now target is ready for next instruction */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_OK);
}

/**
  * swdSectorErase is small program routine to erase specific 
  * flash sector address given by user
  *
  * input   : NONE
  * return  : NONE
  */
void stubEraseSector(void)  {
  /* Change target status to busy to prevent other function to interrupt */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /* Read user start address from sram */
  Flash_Start_Address = (__IO uint32_t)SRAM_Read(SWD_FLASH_START_ADDRESS);
  
  /* Read user end address from sram */
  Flash_End_Address = (__IO uint32_t)SRAM_Read(SWD_FLASH_END_ADDRESS);
  
  Flash_EraseSector(Flash_Start_Address, Flash_End_Address);
  
  /* Clear instruction prevent keep erase */
  SRAM_Write(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  
  /* Tell probe now target is ready for next instruction */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_OK);
}

/**
  * stubMassErase is small program routine to erase specific 
  * bank according to the user selection
  *
  * input   : NONE
  * return  : NONE
  */
void stubMassErase(void)  {
  /* Change target status to busy to prevent other function to interrupt */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /** Check MASS_ERASE_BANK_SELECT it
    * determine which bank need to be erase */
  bankSelect = (__IO uint32_t)SRAM_Read(SWD_BANK_SELECT);
    
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
  Flash_MassErase(bankSelect);
    
  /* Clear instruction prevent keep erase */
  SRAM_Write(SWD_INSTRUCTION, INSTRUCTION_CLEAR);

  /* Tell probe now target is ready for next instruction */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_OK);
}
