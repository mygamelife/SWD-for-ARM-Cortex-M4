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
	uint32_t data_IDR = 0, readCSW = 0, sramDataAddr = 0, targetData = 0;

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

	//memoryAccessWrite(SWD_INSTRUCTION_ADDRESS, SWD_INSTRUCTION_ERASE);
	/* load data in sram */
	memoryAccessWrite(SWD_DATA_ADDRESS, 0xDEADBEEF);
	memoryAccessWrite(0x20000008, 0xABCDABCD);

	memoryAccessWrite(SWD_INSTRUCTION_ADDRESS, SWD_INSTRUCTION_COPY);
	memoryAccessRead(0x081A0000, &targetData);
	memoryAccessRead(0x081A0004, &targetData);

	while(1)
	{
	}
}
