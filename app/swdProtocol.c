#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "Bit_ReadSend.h"
#include "configurePort.h"
#include "swdProtocol.h"



void resetTarget()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_OD ;
	GpioInfo.Pin = GPIO_PIN_13; // PB13 as target reset pin
	GpioInfo.Pull = GPIO_PULLDOWN ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(GPIOB,&GpioInfo);

	simpleDelay();

	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_13);
}



void lineReset()
{
	int i ;

	SWDIO_High();
	for (i = 0 ; i < 52 ; i ++)
	{
		clockGenerator_1cycle();
	}
}



int check_Parity(int APnDP, int RnW, int addrBit3, int addrBit2)	{
	int sum = 0;

	sum = APnDP + RnW + addrBit3 + addrBit2;

	if((sum % 2) != 0) //if odd num 1's return 1
		return 1;
	else return 0; // if even numm  1's return 0
}

int SWD_Request(int APnDP,int ReadWrite,int Address)
{
	int SWD_RequestData = 0, startBit = 1 , stopBit = 0 , parkBit = 1 ;
	int Address_bit2 , Address_bit3, ParityBit ;

	Address_bit2 = checkAddressbit(Address,2);
	Address_bit3 = checkAddressbit(Address,3);

	ParityBit = check_Parity(APnDP,ReadWrite,Address_bit3,Address_bit2);

	SWD_RequestData = SWD_RequestData | startBit << 0 ;
	SWD_RequestData = SWD_RequestData | APnDP << 1 ;
	SWD_RequestData = SWD_RequestData | ReadWrite << 2 ;
	SWD_RequestData = SWD_RequestData | Address_bit2 << 3;
	SWD_RequestData = SWD_RequestData | Address_bit3 << 4;
	SWD_RequestData = SWD_RequestData | ParityBit << 5;
	SWD_RequestData = SWD_RequestData | stopBit << 6;
	SWD_RequestData = SWD_RequestData | parkBit << 7 ;

	return SWD_RequestData ;
}

int checkAddressbit(int address,int bitNumber)
{
	int address_bit =0 ;

	address_bit = address & ( 1 << bitNumber);

	if (address_bit !=0)
		return 1 ;
	else
		return 0 ;
}

int ABORT_CLEAR_ERRFLAG(int DAPabort,ErrorFlag errflag)
{
	long data = 0x00000000;
	int SWD_RequestData = 0 ;
	int status = 0 ;

	if (DAPabort == 1)
		data = data | DAPabort ;
	else
	{
		switch(errflag)
		{
			case	STICKYORUN :
								data = data | 1 << 4 ;
								break ;
			case	WDATAERR:
								data = data | 1 << 3 ;
								break ;
			case	STICKYERR:
								data = data | 1 << 2 ;
								break ;
			case	STICKYCMP:
								data = data | 1 << 1 ;
								break ;
		}
	}

	SWD_RequestData = SWD_Request(DP,WRITE,0x00);
	sendSWDRequest(SWD_RequestData);
	status = checkACK_RWData(&data,SWD_RequestData,WRITE);

	return status ;
}

int AP_Select(int APnDP,int BankNo,int APSEL)
{
	long data = 0x00000000 ;
	int SWD_RequestData = 0 ;
	int status = 0 ;


	if (APnDP == DP) //debug port select modification
		data = data | (BankNo & 0x00000001) ; //modify bit 0
	else //access port select modification
	{
		if (BankNo == 0x0 || BankNo == 0x1 || BankNo == 0xF)
			data = data | BankNo << 4 ; //modify bit [7:4]
		else // unknown/non existing BankNo
			return 0 ;
	}

	SWD_RequestData = SWD_Request(DP,WRITE,0x08);
	sendSWDRequest(SWD_RequestData);
	status = checkACK_RWData(&data,SWD_RequestData,WRITE);
	return status ;
}

int checkACK_RWData(long *data, int SWD_RequestData, int ReadWrite)
{
	int i = 0 , ACK = 0 ;
	readBits(&ACK,3);

	if (ACK == OK)
	{
		if (ReadWrite)
			readBits(data,32);
		else
		{
			SWDIO_OutputMode();
			clockGenerator_1cycle();//turn around
			sendBits(*data,32);
		}
		return OK ;
	}
	else if (ACK == WAIT)
	{
		for ( i = 0 ; i < 3 ; i ++) //retry for maximum 3 times
		{
			sendSWDRequest(SWD_RequestData);
			readBits(&ACK,3);
			if (ACK == OK)
			{
				if (ReadWrite)
					readBits(data,32);
				else
				{
					SWDIO_OutputMode();
					clockGenerator_1cycle(); //turn around
					sendBits(*data,32);
				}

				return OK ;
			}
		}
		return WAITED_TOOLONG ;
	}
	else
		return FAULT ; //FAULT response or no response
}

void sendSWDRequest(int SWD_RequestData)
{
	SWDIO_OutputMode();
	sendBits(SWD_RequestData,8);
	SWDIO_InputMode();
	//simpleDelay();
}

/*
void initialisation()
{
	int SWD_RequestData ;
	long IDCODE ;
	int ACK = 0 ;

	SWDIO_OutputMode();

	SWD_RequestData = SWD_Request(DP,READ,0x00);

	resetTarget();
	lineReset();
	sendBits(0xE79E,16);
	lineReset();

	sendSWDRequest(SWD_RequestData);
	readBits(&ACK,3);
	//checkACK_RWData(&IDCODE,SWD_RequestData,READ);
}*/



