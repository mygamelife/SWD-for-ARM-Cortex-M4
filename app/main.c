#include "Flash.h"
#include "swdStub.h"
#include "CoreDebug.h"
#include "swdProtocol.h"
#include "SystemConfigure.h"
#include "Register_ReadWrite.h"

int main(void)
{
	//CoreStatus corestatus;
	int i = 0 , j = 0;
	int Address = 0, ACK = 0, Parity = 0, status = 0;
	uint32_t FlashData = 0, readErrorCode = 0;
	uint32_t data_IDR = 0, readCSW = 0, sramDataAddr = 0, flashDataAddr = 0;
	uint32_t targetStatus = 1,  targetData = 0, targetFlashData = 0;

	/* Configure SWD I/O Port */
	configure_IOPorts();

	/* Hardware reset target board */
	resetTarget();

	/* Initialize SWD Protocol */
	SWD_Initialisation();

	powerUpSystemAndDebug();
	readAhbIDR(&data_IDR);

	swdSetMemorySize(CSW_WORD_SIZE);
	swdReadAP(CSW_REG, &ACK, &Parity, &readCSW);


	/* load flash start and end address to sram */
	//memoryAccessWrite(SWD_START_ADDRESS, ADDR_FLASH_SECTOR_12);
	//memoryAccessWrite(SWD_END_ADDRESS, ADDR_FLASH_SECTOR_16);

	/* load sector erase instruction to sram */
	//memoryAccessWrite(SWD_INSTRUCTION, INSTRUCTION_SECTOR_ERASE);

	/* Wait target ready for next instruction */
	//while(targetStatus != TARGET_OK)	{
		//memoryAccessRead(SWD_TARGET_STATUS, &targetStatus);
	//}

	/* Read from flash after erase */
	//memoryAccessRead(ADDR_FLASH_SECTOR_12, &targetFlashData);
	//memoryAccessRead(ADDR_FLASH_SECTOR_12 + 4, &targetFlashData);
	//memoryAccessRead(ADDR_FLASH_SECTOR_12 + 8, &targetFlashData);

	/* load data in sram */
	//memoryAccessWrite(SWD_DATA32, 0xDEADBEEF);
	//memoryAccessWrite(SWD_DATA32 + 4, 0xABCDABCD);
	//memoryAccessWrite(SWD_DATA32 + 8, 0xDCBADCBA);

	/* load length in sram */
	//memoryAccessWrite(SWD_DATA_LENGTH, 12);

	/* load copy instructoin in sram */
	//memoryAccessWrite(SWD_INSTRUCTION, INSTRUCTION_COPY);

	/* Wait target ready for next instruction */
	//while(targetStatus != TARGET_OK)	{
		//memoryAccessRead(SWD_TARGET_STATUS, &targetStatus);
	//}

	/* Read data from target flash */
	//memoryAccessRead(ADDR_FLASH_SECTOR_12, &targetFlashData);
	//memoryAccessRead(ADDR_FLASH_SECTOR_12 + 4, &targetFlashData);
	//memoryAccessRead(ADDR_FLASH_SECTOR_12 + 8, &targetFlashData);

	while(1)
	{
	}
}
