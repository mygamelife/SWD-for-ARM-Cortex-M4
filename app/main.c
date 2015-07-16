#include "Flash.h"
#include "swdStub.h"
#include "CoreDebug.h"
#include "swdProtocol.h"
#include "SystemConfigure.h"
#include "Register_ReadWrite.h"

int main(void)
{
  int i = 0;
  uint32_t dataAddr = 0, swdInstruction = 0;

  FlashSystemConfig();
  sectorErase(ADDR_FLASH_SECTOR_21, ADDR_FLASH_SECTOR_23);
  
  dataAddr = SWD_DATA32;
  for(i; i < TWO_KBYTES; i += 4)  {
    *(__IO uint32_t*)dataAddr = 0xABCDABCD;
    dataAddr = dataAddr + 4;
  }
  
  //*(__IO uint32_t*)dataAddr = 0xABCDABCD;
  while(1)
  {
    //swdInstruction = *(__IO uint32_t*)SWD_INSTRUCTION_ADDRESS;
    swdInstruction = INSTRUCTION_COPY;
    
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
}
