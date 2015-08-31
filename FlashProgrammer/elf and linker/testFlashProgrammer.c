#include "main.h"

int main(void)
{
	uint32_t data_IDR = 0, dataRead = 0, ack = 0, parity = 0;
	uint32_t targetStatus = 1,  targetData = 0, targetFlashData = 0, targetSramData = 0;

	uint32_t word1 = 0, word2= 0, word3 = 0, word4 = 0, word5 = 0;

	/* Configure SWD I/O Port */
	configure_IOPorts();
	/* Hardware reset target board */
	resetTarget();
	/* Initialize SWD Protocol */
	SWD_Initialisation();
	/* Power Up AHB Port */
	readAhbIDR(&data_IDR);

	//swdWriteCSW(&ack, CSW_DEFAULT_MASK | CSW_WORD_SIZE);

	//swdWriteAP(TAR_REG, &ack, 0x2000001C);
	//swdWriteAP(DRW_REG, &ack, 0xCAFE0000);

	//swdRegisterWrite(TAR_REG,AP,&ack,0x2000001C);
	//swdReadAP(DRW_REG,&ack,&parity,&dataRead);
	memoryWriteWord(0x20000024, 0xCAFEDEAD);
	memoryWriteWord(0x20000028, 0xDEADBEEF);
	memoryWriteWord(0x2000002C, 0xBEEFCAFE);
	memoryWriteWord(0x20000030, 0xCAFECAFE);
	memoryWriteWord(0x20000034, 0xFAFAFAFA);

	loadCopyFromSRAMToFlashInstruction(0x20000024, 0x081C0000, 16);

	memoryReadWord(0x20000024, &word1);
	memoryReadWord(0x20000028, &word2);
	memoryReadWord(0x2000002C, &word3);
	memoryReadWord(0x20000030, &word4);
	memoryReadWord(0x20000034, &word5);

	while(1)
	{}
}
