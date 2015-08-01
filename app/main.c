#include "SRAM.h"
#include "Flash.h"
#include "swdStub.h"
#include "configurePort.h"
#include "CoreDebug.h"
#include "swdProtocol.h"
#include "SystemConfigure.h"
#include "Register_ReadWrite.h"

uint8_t buffer[] = "Hello World";

int main(void)
{
  int Address = 0, ACK = 0, Parity = 0, status = 0, length = 0, i = 0, j = 0;
  uint32_t FlashData = 0, readErrorCode = 0, CSW_BIT_SET = 0;
  uint32_t data_IDR = 0, readCSW = 0, sramDataAddr = 0, flashDataAddr = 0;
  uint32_t targetStatus = 1,  targetData = 0, targetFlashData = 0, targetSramData = 0;

  targetMain();

  while(1)
  {
  }
}
