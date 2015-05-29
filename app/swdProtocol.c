#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "Bit_ReadSend.h"
#include "swdProtocol.h"

void simpleDelay()
{
	int counter1 = 0,counter2 = 0 ;

	while(counter2 != 100000)
	{
		while (counter1 != 100000)
			counter1 ++ ;
		counter2++;
	}


}


void resetTarget()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_OD ;
	GpioInfo.Pin = GPIO_PIN_13; // PB13 as target reset pin
	GpioInfo.Pull = GPIO_NOPULL ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(GPIOB,&GpioInfo);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	simpleDelay();

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	simpleDelay();

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
