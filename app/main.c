#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#define SWDIO

#define SWCLK_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
#define SWCLK_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

void configurePort()
{
	GPIO_InitTypeDef GpioInfo;
	__GPIOB_CLK_ENABLE();

	GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
	GpioInfo.Pin = GPIO_PIN_10 | GPIO_PIN_12; // Init PB10 clock and PG14
	GpioInfo.Pull = GPIO_NOPULL ;
	GpioInfo.Speed = GPIO_SPEED_HIGH ;

	HAL_GPIO_Init(GPIOB,&GpioInfo);
}

void lineReset()
{
	int i ;

	sendBit(1);
	for (i = 0 ; i < 50 ; i ++)
	{
		//clockGenerator_1cycle();
	}
}

void sendBit(int value)
{
	if ((value & 1) == 1)
		;
	else
		;
}

void sendBits(long dataToSend,int numberOfBits)
{
	int i ;
	for (i=0 ; i < numberOfBits ; i ++)
	{
		//sendBit();
	}
}

int readBit()
{
	 //HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	//GPIO_PinState readBit ;

	//readBit =

	if (readBit == 1)
		return 1 ;
	else
		return 0 ;
}

void readBits(long *dataRead,int numberOfBits)
{
	int i = 0 ;

	for ( i = 0 ; i < numberOfBits ; i ++)
	{
		readBit();

	}


}

void clockGenerator_1cycle()
{
	SWCLK_ON()
	SWCLK_OFF()

}

int check_Parity(int APnDP, int RnW, int addrBit3, int addrBit2)	{
	int sum = 0;

	sum = APnDP + RnW + addrBit3 + addrBit2;

	if((sum % 2) != 0) //if odd num 1's return 1
		return 1;
	else return 0; // if even numm  1's return 0
}

int main(void)
{
	configurePort();
    while(1)
    {
    	clockGenerator_1cycle();
    }
}
