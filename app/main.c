#include "main.h"

#define VECT_TAB_OFFSET 0x0000001C

int main(void)
{
	uint32_t ahbIDR = 0, ack = 0, cswBit = 0, readDataValue = 0;
	uint32_t *dataAddress = 0x20000000;
	uint32_t *data = 0xABCDABCD, start = 0, end = 0, status = 0;

	//configure_IOPorts();
	//resetTarget();
	//SWD_Initialisation();
	//readAhbIDR(&ahbIDR);

	/*
	cswBit = CSW_DEFAULT_MASK | CSW_WORD_SIZE;
	swdWriteCSW(&ack, cswBit);

	memoryAccessWrite((uint32_t)0x20000100, 0xDEADBEEF);
	memoryAccessWrite((uint32_t)0x20000104, 0xBEEFCAFE);
	loadCopyFromSRAMToFlashInstruction(0x20000100, 0x081C0000, 0x08);

	do  {
	    memoryAccessRead(SWD_TARGET_STATUS, &status);
	} while(status != TARGET_OK);
	memoryAccessRead((uint32_t)0x081C0000, &readDataValue);
	memoryAccessRead((uint32_t)0x081C0004, &readDataValue);

	loadEraseSectorInstruction(ADDR_FLASH_SECTOR_22, ADDR_FLASH_SECTOR_23);

	memoryAccessRead(0x20000018, &readDataValue);
	memoryAccessRead((uint32_t)0x081C0004, &readDataValue);*/
	SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;
	targetMain();

	while(1)
	{
	}
}
