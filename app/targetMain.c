/*
#include "swdProtocol.h"
#include "CoreDebug.h"
#include "Flash.h"
#include "SystemConfigure.h"
#include "Register_ReadWrite.h"

#define KBYTES 						1024  //2k byte
#define SWD_INSTRUCTION_ADDRESS		((uint32_t)0x20000000)
#define SWD_DATA_ADDRESS			((uint32_t)0x20000004)
#define SWD_INSTRUCTION_COPY		((uint32_t)0x00000010)
#define SWD_INSTRUCTION_ERASE		((uint32_t)0x00000020)
#define SWD_INSTRUCTION_CLEAR		((uint32_t)0xFFFFFFFF)

int main(void)
{
	__IO uint32_t FlashData = 0, swdInstruction = 0;

	FlashSystemConfig();

	while(1)
	{
		swdInstruction = *(__IO uint32_t*)SWD_INSTRUCTION_ADDRESS;

		if(SWD_INSTRUCTION_COPY == swdInstruction)	{
			copyFromRamToFlash(SWD_DATA_ADDRESS, FLASH_USER_START_ADDR, KBYTES);
			*(__IO uint32_t*)SWD_INSTRUCTION_ADDRESS = SWD_INSTRUCTION_CLEAR;
		}

		else if(SWD_INSTRUCTION_ERASE == swdInstruction)	{
			sectorErase();
			turnOffLED3();
			*(__IO uint32_t*)SWD_INSTRUCTION_ADDRESS = SWD_INSTRUCTION_CLEAR;
		}
	}
}*/
