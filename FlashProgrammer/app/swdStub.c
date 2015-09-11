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
      stubCopyFromSRAMToFlash();
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
void stubCopyFromSRAMToFlash(void) {
  __IO uint32_t flashStartAddress = 0;
  __IO uint32_t sramStartAddress = 0;

  uint32_t size = 0;
  
  /* Change target status to busy to prevent other function to interrupt */
  sramWrite(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /* Read SRAM source address from sram */
  sramStartAddress  = (__IO uint32_t)sramRead(SWD_SRAM_START_ADDRESS);
  /* Read flash destination address from sram */
  flashStartAddress = (__IO uint32_t)sramRead(SWD_FLASH_START_ADDRESS);
  /* Read Data Length from sram */
  size  = (__IO int)sramRead(SWD_DATA_SIZE);

  flashCopyFromSRAMToFlash(sramStartAddress, flashStartAddress, size);
  
  /* Clear instruction prevent keep erase */
  sramWrite(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  /* Tell probe now target is ready for next instruction */
  sramWrite(SWD_TARGET_STATUS, TARGET_OK);
}

/**
  * swdSectorErase is small program routine to erase specific 
  * flash sector address given by user
  *
  * input   : NONE
  * return  : NONE
  */
void stubEraseSector(void)  {
  __IO uint32_t flashStartAddress = 0;
  __IO uint32_t flashEndAddress = 0;

  /* Change target status to busy to prevent other function to interrupt */
  sramWrite(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /* Read user start address from sram */
  flashStartAddress = (__IO uint32_t)sramRead(SWD_FLASH_START_ADDRESS);
  
  /* Read user end address from sram */
  flashEndAddress = (__IO uint32_t)sramRead(SWD_FLASH_END_ADDRESS);
  
  flashEraseSector(flashStartAddress, flashEndAddress);
  
  /* Clear instruction prevent keep erase */
  sramWrite(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  
  /* Tell probe now target is ready for next instruction */
  sramWrite(SWD_TARGET_STATUS, TARGET_OK);
}

/**
  * stubMassErase is small program routine to erase specific 
  * bank according to the user selection
  *
  * input   : NONE
  * return  : NONE
  */
void stubMassErase(void)  {
  __IO uint32_t bankSelect = 0;

  /* Change target status to busy to prevent other function to interrupt */
  sramWrite(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /** Check MASS_ERASE_BANK_SELECT it
    * determine which bank need to be erase */
  bankSelect = (__IO uint32_t)sramRead(SWD_BANK_SELECT);
    
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
  flashMassErase(bankSelect);
    
  /* Clear instruction prevent keep erase */
  sramWrite(SWD_INSTRUCTION, INSTRUCTION_CLEAR);

  /* Tell probe now target is ready for next instruction */
  sramWrite(SWD_TARGET_STATUS, TARGET_OK);
}

/**
  * stubVerify is small program routine to verify the
  * program from sram to flash if correct LED will blink 10times
  *
  * input   : NONE
  * return  : NONE
  */
void stubVerify(void) {
  __IO uint32_t flashStartAddress = 0;
  __IO uint32_t sramStartAddress = 0;

  uint32_t size = 0;
  
  /* Change target status to busy to prevent other function to interrupt */
  sramWrite(SWD_TARGET_STATUS, TARGET_BUSY);
  
  /* Read SRAM source address from sram */
  sramStartAddress  = (__IO uint32_t)sramRead(SWD_SRAM_START_ADDRESS);
  /* Read flash destination address from sram */
  flashStartAddress = (__IO uint32_t)sramRead(SWD_FLASH_START_ADDRESS);
  /* Read Data Length from sram */
  size  = (__IO int)sramRead(SWD_DATA_SIZE);
  
  flashVerifyDataFromSRAMToFlash(sramStartAddress, flashStartAddress, size);
  
  /* Clear instruction prevent keep erase */
  sramWrite(SWD_INSTRUCTION, INSTRUCTION_CLEAR);
  /* Tell probe now target is ready for next instruction */
  sramWrite(SWD_TARGET_STATUS, TARGET_OK);
}

/**
  *
  */
void flashProgrammer(void) {
  __IO uint32_t swdInstruction = 0;
  
  /* Initialize hardware and configure system clock */
  #if !defined(TEST)
	FlashSystemConfig();
	configureLED();
  #endif

	/* Initialize target status */
	sramWrite(SWD_TARGET_STATUS, TARGET_OK);

  while(1)
  {
	turnOnLED3();
    swdInstruction = (__IO uint32_t)sramRead(SWD_INSTRUCTION);
    swdStub(swdInstruction);
    turnOffLED3();
  }
}
