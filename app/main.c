#include "swdProtocol.h"
#include "Register_ReadWrite.h"

int main(void)
{
	int ack = 0, parity = 0;

	uint32_t dummyRead = 0 ;
	uint32_t dataRead = 0, DHCSR_Read = 0 , DEMCR_Read = 0 ,DFSR_Read_1= 0, DFSR_Read_2=0, FP_COMP0_Read = 0 ;

	uint32_t ctrlStatusRegData = 0x50000000  ,CTRLSTAT_READDATA =0, selectAPBank = 0xF0;
	uint32_t IDR = 0;
	uint32_t CSW_data = 0x23000042 , CSW_READDATA = 0;
	uint32_t DRW_data = 0xA05F0003 ;
	uint32_t DP_Select_data = 0x0 ;
	uint32_t RDBUFF_DATA = 0 ;

	configure_IOPorts();
	resetTarget();

	SWD_Initialisation();

	//Note : implement code to check system and debug power up status here just in case, value expected 0xF0000000

	//Configure Control/Status Word Register CSW
	SWDRegister_Write(CSW_REG,AP,&ack,CSW_data);


	/*Enable CPU debug mode*/
	SWDRegister_Write(TAR_REG,AP,&ack,0xE000EDF0);
	SWDRegister_Write(DRW_REG,AP,&ack,0xA05F0001);


	//Read DHCSR
	SWDRegister_Write(TAR_REG,AP,&ack,0xE000EDF0);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dummyRead);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&DHCSR_Read);

	//Read DEMCR
	SWDRegister_Write(TAR_REG,AP,&ack,0xE000EDFC);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dummyRead);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&DEMCR_Read);

	/* Read DFSR to check whether breakpoint event occur */
	SWDRegister_Write(TAR_REG,AP,&ack,0xE000ED30);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dummyRead);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&DFSR_Read_1);



	/*---------------------------------*/

	/*Enable CPU debug mode*/
	//Select address of DHCSR in TAR
	//SWDRegister_Write(TAR_REG,AP,&ack,0xE000EDF0);

	//Write 0xA05F0001 to DRW
	//SWDRegister_Write(DRW_REG,AP,&ack,0xA05F0001);
	//SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dummyRead);
	//SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dataRead);
	/*---------------------------------*/

	/*Enable FPB unit */
	//Select address of FP_CTRL in TAR
	SWDRegister_Write(TAR_REG,AP,&ack,0xE0002000);

	//Write 0x3 to DRW
	SWDRegister_Write(DRW_REG,AP,&ack,0x00000003);

	/*Check if FPB is enabled
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dummyRead);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dataRead);
	*/

	/*---------------------------------*/

	/*Configure FP Comparator #0 (Instruction Comparator)*/
	//Select address of FP_COMP0 in TAR
	SWDRegister_Write(TAR_REG,AP,&ack,0xE0002008);

	//Write to DRW
	SWDRegister_Write(DRW_REG,AP,&ack,0x080002F1);

	//Test reading comparator
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dummyRead);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&FP_COMP0_Read);

	/*---------------------------------*/

	/* Read DFSR to check whether breakpoint event occur */
	SWDRegister_Write(TAR_REG,AP,&ack,0xE000ED30);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dummyRead);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&DFSR_Read_2);

	/*---------------------------------*/
	//Disable FP Comparator #0
	//Select address of FP_COMP0 in TAR
	SWDRegister_Write(TAR_REG,AP,&ack,0xE0002008);

	//Write to DRW
	SWDRegister_Write(DRW_REG,AP,&ack,0x080002F0);
	/*---------------------------------*/

	/*Halt CPU and enter debug state
	//Select address of DHCSR in TAR
	SWDRegister_Write(TAR_REG,AP,&ack,0xE000EDF0);

	//Write 0xA05F0003 to DRW
	SWDRegister_Write(DRW_REG,AP,&ack,0xA05F0003);

	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dummyRead);
	SWDRegister_Read(DRW_REG,AP,&ack,&parity,&dataRead);
	*/

	/* Access and read IDR
	SWDRegister_Write(0x04, DP, &ack, 0x50000000);
	SWDRegister_Read(0x04, DP, &ack, &parity, &CTRLSTAT_READDATA);

	SWDRegister_Write(0x08, DP, &ack, 0xF0); //Access SELECT register and select APBANK 0xF
	SWDRegister_Read(0x0C, AP, &ack, &parity, &IDR); //0x24770011
	SWDRegister_Read(0x0C, AP, &ack, &parity, &IDR);*/




	//Disable CPU debug mode
	SWDRegister_Write(TAR_REG,AP,&ack,0xE000EDF0);
	SWDRegister_Write(DRW_REG,AP,&ack,0xA05F0000);

	while(1)
	{
	}

}
