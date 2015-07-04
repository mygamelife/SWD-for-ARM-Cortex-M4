#include "swdProtocol.h"
#include "Register_ReadWrite.h"

// MSC memory range 0x400C0000 – 0x400C03FF
#define MSC	0x400C0000

#define MSC_WRITECTRL	(MSC + 0x008)
#define MSC_ADDRB		(MSC + 0x010)
#define MSC_WRITECMD	(MSC + 0x00C)
#define MSC_WDATA		(MSC + 0x018)
#define MSC_STATUS		(MSC + 0x01C)

int main(void)
{
 	int ack = 0, parity = 0;
	uint32_t data_IDR = 0, readData_CSW = 0, readDummy = 0, read_DHCSR = 0;

	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	readAhbIDR(&data_IDR);

	swdWriteSelect(&ack, BANK_0);
	SWDRegister_Write(CSW_REG,AP,&ack, 0x23000042);
	swdReadAP(CSW_REG, &ack, &parity, &readData_CSW);

	SWDRegister_Write(TAR_REG, AP, &ack, DHCSR);
	SWDRegister_Write(DRW_REG,AP,&ack,0xA05F0003);

	swdReadAP(TAR_REG, &ack, &parity, &readDummy);
	swdReadAP(DRW_REG, &ack, &parity, &read_DHCSR);

	SWDRegister_Write(TAR_REG, AP, &ack, 0x2000004C);
	//SWDRegister_Write(DRW_REG,AP,&ack,0x11111111);

	swdReadAP(DRW_REG, &ack, &parity, &readDummy);

	while(1)
	{
	}

}
