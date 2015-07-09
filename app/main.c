#include "swdProtocol.h"
#include "CoreDebug.h"
#include "stm32f429i_discovery.h"
#include "Flash.h"
#include "SystemConfigure.h"
#include "Register_ReadWrite.h"

int main(void)
{
	/*
 	int ack = 0, parity = 0, status = 0;
	uint32_t data_IDR = 0, readData_CSW = 0, readDummy = 0, read_DHCSR = 0;

	configure_IOPorts();
	resetTarget();
	SWD_Initialisation();

	readAhbIDR(&data_IDR);
	swdWriteSelect(&ack, BANK_0);
	SWDRegister_Write(CSW_REG,AP,&ack, 0x23000042);
*/
	FlashSystemConfig();
	sectorErase();
	writeToFlash(FLASH_TYPEPROGRAM_WORD, 0xABCDABCD);

	while(1)
	{
	}
}
