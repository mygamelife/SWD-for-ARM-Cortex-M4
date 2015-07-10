#include "swdProtocol.h"
#include "CoreDebug.h"
#include "stm32f429i_discovery.h"
#include "Flash.h"
#include "SystemConfigure.h"
#include "Register_ReadWrite.h"

int main(void)
{
	uint32_t FlashData = 0, readErrorCode = 0;

	FlashSystemConfig();
	sectorErase();
	writeToFlash(FLASH_TYPEPROGRAM_WORD, 0xABCDABCD);
	FlashData = readFromFlash(FLASH_USER_START_ADDR);
	while(1)
	{
	}
}
