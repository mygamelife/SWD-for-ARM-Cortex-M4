#include "swdStub.h"

/**
  * swdStub is small program routine take instruction from swd probe and response
  *
  * input   : swdInstruction is an instruction send by probe
  * return  : NONE
  */
/*
void swdStub(uint32_t swdInstruction) {
  switch(swdInstruction)  {
    case INSTRUCTION_COPY :
      stubCopy();
      break;
    
    case INSTRUCTION_SECTOR_ERASE :
      stubEraseSector();
      break;
  
    case INSTRUCTION_MASS_ERASE :
      stubMassErase();
      break;
  }
}*/

/**
  * swdCopy is small program routine to copy data in SRAM to Flash
  *
  * input   : NONE
  * return  : NONE
  */
void stubCopy(uint32_t SRAM_Start_Address, uint32_t Flash_Start_Address, int length) {
  /* Change target status to busy to prevent other function to interrupt */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_BUSY);
  
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
void stubEraseSector(uint32_t Flash_Start_Address, uint32_t Flash_End_Address)  {
  /* Change target status to busy to prevent other function to interrupt */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_BUSY);
  
  Flash_EraseSector(Flash_Start_Address, Flash_End_Address);
  
  /* Turn off verify checking LED */
  turnOffLED3();
  
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
void stubMassErase(uint32_t bankSelect)  {
  /* Change target status to busy to prevent other function to interrupt */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /* Perform mass erase here */
  Flash_MassErase(bankSelect);

  /* Turn off verify checking LED */
  turnOffLED3();
    
  /* Clear instruction prevent keep erase */
  SRAM_Write(SWD_INSTRUCTION, INSTRUCTION_CLEAR);

  /* Tell probe now target is ready for next instruction */
  SRAM_Write(SWD_TARGET_STATUS, TARGET_OK);
}
