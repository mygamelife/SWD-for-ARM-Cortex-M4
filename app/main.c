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

	uint32_t dummyRead = 0 ;

	uint32_t ctrlStatusRegData = 0x50000000  ,CTRLSTAT_READDATA =0, selectAPBank = 0xF0;
	uint32_t IDR = 0, CSW = 0, TAR = 0, DRW = 0;
	uint32_t CSW_data = 0x22000002;
	uint32_t TAR_addr = 0xE000EDF0;
	uint32_t DRW_data = 0xA05F0003;
	//uint32_t DP_Select_data = 0x0 ;

	configure_IOPorts();
	//resetTarget();

	SWD_Initialisation();

	//selectRegisterBank(0x00);
	//SWDRegister_Write(0x00, AP, &ack, CSW_data); //Select 32bit memory access size in CSW register
	//SWDRegister_Read(0x00, AP, &ack, &parity, &dummyRead); //discard previous AP
	//SWDRegister_Read(0x00, AP, &ack, &parity, &CSW); //CSW

	/*
	//Step 1 enable MSC_WRITECTRL
	SWDRegister_Write(0x04, AP, &ack, MSC_WRITECTRL); //TAR
	SWDRegister_Write(0x0C, AP, &ack, 0x1); //DRW

	//Step 2 write MSC_ADDRB
	SWDRegister_Write(0x04, AP, &ack, MSC_ADDRB); //TAR
	SWDRegister_Write(0x0C, AP, &ack, 0x400C0080); //DRW

	//Step 3 Load MSC_ADDRB into ADDR
	SWDRegister_Write(0x04, AP, &ack, MSC_WRITECMD); //TAR
	SWDRegister_Write(0x0C, AP, &ack, 0x1); //DRW

	//Step 4 Write the word to MSC_WDATA
	SWDRegister_Write(0x04, AP, &ack, MSC_WDATA); //TAR
	SWDRegister_Write(0x0C, AP, &ack, 0xABCD000); //DRW

	//Step 5 write a 1 to bit WRITEONCE in MSC_WRITECMD
	SWDRegister_Write(0x04, AP, &ack, MSC_WRITECMD); //TAR
	SWDRegister_Write(0x0C, AP, &ack, 0x8); //DRW
	*/

	/*
	SWDRegister_Write(0x04, AP, &ack, 0x00020000); //TAR
	SWDRegister_Read(0x04, AP, &ack, &parity, &dummyRead);
	SWDRegister_Read(0x04, AP, &ack, &parity, &TAR);

	SWDRegister_Write(0x0C, AP, &ack, 0x55667788); //DRW
	SWDRegister_Read(0x0C, AP, &ack, &parity, &dummyRead);
	SWDRegister_Read(0x0C, AP, &ack, &parity, &DRW);*/

	//SWDRegister_Read(0x0C, DP, &ack, &parity, &TAR); //TAR




	while(1)
	{
	}

}
