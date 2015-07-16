#include "swdStub.h"

/**
  * swdStub is small program routine take instruction from swd probe and response
  *
  * input   : swdInstruction is an instruction send by probe
  * return  : NONE
  */
void swdStub(uint32_t swdInstruction) {
  if(swdInstruction == INSTRUCTION_COPY)	{
    *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_BUSY;
    copyFromRamToFlash(SWD_DATA32, ADDR_FLASH_SECTOR_21, TWO_KBYTES);
    *(__IO uint32_t*)SWD_INSTRUCTION    = INSTRUCTION_CLEAR;
    *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_OK;
  }
  
  else if(swdInstruction == INSTRUCTION_SECTOR_ERASE)	{
    *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_BUSY;
    sectorErase(ADDR_FLASH_SECTOR_21, ADDR_FLASH_SECTOR_23);
    /* Turn off verify checking LED */
    turnOffLED3();
    *(__IO uint32_t*)SWD_INSTRUCTION    = INSTRUCTION_CLEAR;
    *(__IO uint32_t*)SWD_TARGET_STATUS  = TARGET_BUSY;
  }
}