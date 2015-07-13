#include "swdProtocol.h"
#include "CoreDebug.h"
#include "Flash.h"
#include "SystemConfigure.h"
#include "Register_ReadWrite.h"

#define KBYTES 						1024  //1k byte
#define SWD_INSTRUCTION_ADDRESS		((uint32_t)0x20000000)
#define SWD_DATA_ADDRESS			((uint32_t)0x20000004)
#define SWD_INSTRUCTION_COPY		((uint32_t)0x00000010)
#define SWD_INSTRUCTION_ERASE		((uint32_t)0x00000020)
#define SWD_INSTRUCTION_CLEAR		((uint32_t)0xFFFFFFFF)

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
