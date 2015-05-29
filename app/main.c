#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#define SWDIO

#define SWCLK_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
#define SWCLK_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
#define SWDIO_High() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
#define SWDIO_Low() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
#define READ 1
#define WRITE 0
#define DP 0
#define AP 1

void clockGenerator_1cycle();
void configurePort();
void SWDIO_OutputMode();
void SWDIO_InputMode();
void lineReset();
void sendBit(int value);
void sendBits(long dataToSend,int numberOfBits);
int readBit();
void readBits(long *dataRead,int numberOfBits);
void clockGenerator_1cycle();
int check_Parity(int ApnDp,int RnW,int AddrBit3,int AddrBit2);
void resetTarget();


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


void configurePort()
{
	GPIO_InitTypeDef GpioInfo;
	__GPIOB_CLK_ENABLE();

	GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
	GpioInfo.Pin = GPIO_PIN_10 ; // PB10 as clock output
	GpioInfo.Pull = GPIO_NOPULL ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(GPIOB,&GpioInfo);


}


void SWDIO_OutputMode()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
	GpioInfo.Pin = GPIO_PIN_12; // PB12 as output
	GpioInfo.Pull = GPIO_NOPULL ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(GPIOB,&GpioInfo);
}

void SWDIO_InputMode()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_INPUT ;
	GpioInfo.Pin = GPIO_PIN_12; // PB12 as input
	GpioInfo.Pull = GPIO_NOPULL;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(GPIOB,&GpioInfo);
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

void sendBit(int value)
{
	if (value == 1)
		SWDIO_High()
	else
		SWDIO_Low()

	clockGenerator_1cycle();

}

void sendBits(long dataToSend,int numberOfBits)
{
	//LSB first

	int i , bitValue = 0;
	for (i=0 ; i < numberOfBits ; i ++)
	{
		bitValue = dataToSend & (1 << i ) ;

		if (bitValue != 0)
			sendBit(1);
		else
			sendBit(0);
	}

}

int readBit()
{
	GPIO_PinState bitRead ;

	SWCLK_ON();
	bitRead  = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
	SWCLK_OFF();

	if (bitRead  == 1)
		return 1 ;
	else
		return 0 ;
}

void readBits(long *dataRead,int numberOfBits)
{
	int i , bitRead;

	for ( i = 0 ; i < numberOfBits ; i ++)
	{
		bitRead = readBit();
		*dataRead = *dataRead | (bitRead << i ) ;
	}


}

int check_Parity(int APnDP, int RnW, int addrBit3, int addrBit2)	{
	int sum = 0;

	sum = APnDP + RnW + addrBit3 + addrBit2;

	if((sum % 2) != 0) //if odd num 1's return 1
		return 1;
	else return 0; // if even numm  1's return 0
}

int SWD_Protocol(int APnDP,int ReadWrite,int Address)
{
	int SWD_Protocol = 0, startBit = 1 , stopBit = 0 , parkBit = 1 ;
	int Address_bit2 , Address_bit3, ParityBit ;

	Address_bit3 = Address & ( 1 << 3) ;
	if (Address_bit3 != 0)
		Address_bit3 = 1 ;
	else
		Address_bit3 = 0 ;

	Address_bit2 = Address & ( 1 << 2) ;

	if (Address_bit2 != 0)
			Address_bit2 = 1 ;
		else
			Address_bit2 = 0 ;

	ParityBit = check_Parity(APnDP,ReadWrite,Address_bit3,Address_bit2);

	SWD_Protocol = SWD_Protocol | startBit << 0 ;
	SWD_Protocol = SWD_Protocol | APnDP << 1 ;
	SWD_Protocol = SWD_Protocol | ReadWrite << 2 ;
	SWD_Protocol = SWD_Protocol | Address_bit2 << 3;
	SWD_Protocol = SWD_Protocol | Address_bit3 << 4;
	SWD_Protocol = SWD_Protocol | ParityBit << 5;
	SWD_Protocol = SWD_Protocol | stopBit << 6;
	SWD_Protocol = SWD_Protocol | parkBit << 7 ;

	return SWD_Protocol ;
}

void clockGenerator_1cycle()
{
	SWCLK_ON()
	SWCLK_OFF()
}



int main(void)
{
	long ACK =0;
	long IDCODE=0 ;
	long SWD_Request ;

	configurePort();
	SWDIO_OutputMode();
	SWD_Request = SWD_Protocol(DP,READ,0x00);

	resetTarget();
	lineReset();
	sendBits(0xE79E,16);
	lineReset();
	sendBits(SWD_Request,8);
	clockGenerator_1cycle();
	SWDIO_InputMode();
	clockGenerator_1cycle();
	readBits(&ACK,3);
	readBits(&IDCODE,32);
	resetTarget();
	while(1)
	{
	}

}
