#include "swdProtocol.h"
#include "Register_ReadWrite.h"

int main(void)
{
	int ack = 0, parity = 0;

	uint32_t dummyRead = 0 ;

	uint32_t ctrlStatusRegData = 0x50000000  ,CTRLSTAT_READDATA =0, selectAPBank = 0xF0;
	uint32_t IDR = 0;
	uint32_t CSW_data = 0x22000002 ;
	uint32_t TAR_data = 0xE000EDF0 ;
	uint32_t DRW_data = 0xA05F0003 ;
	uint32_t DP_Select_data = 0x0 ;

	configure_IOPorts();
	//resetTarget();

	SWD_Initialisation();

	/* Access and read IDR
	SWDRegister_Write(0x04, DP, &ack, 0x50000000);
	SWDRegister_Read(0x04, DP, &ack, &parity, &CTRLSTAT_READDATA);

	SWDRegister_Write(0x08, DP, &ack, 0xF0); //Access SELECT register and select APBANK 0xF
	SWDRegister_Read(0x0C, AP, &ack, &parity, &IDR); //0x24770011
	SWDRegister_Read(0x0C, AP, &ack, &parity, &IDR);*/

	while(1)
	{
	}

}
