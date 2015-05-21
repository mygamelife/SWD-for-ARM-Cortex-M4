#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#define SWDIO
#define SWCLK
//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);



int main(void)
{

    while(1)
    {
    }
}

void configurePort()
{
	GPIO_InitTypeDef GpioInfo ;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
	GpioInfo.Pin = GPIO_PIN_13 | GPIO_PIN_14; // Init PG13 and PG14
	GpioInfo.Pull = GPIO_NOPULL ;
	GpioInfo.Speed = GPIO_SPEED_HIGH ;

	//_GPIOA_CLK_ENABLE();
	//HAL_GPIO_Init(GPIOA,&GpioInfo);

}

void lineReset()
{
	int i ;

	sendBit(1);
	for (i = 0 ; i < 50 ; i ++)
	{
		clockGenerator_1cycle();
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
		sendBit();
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
	//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

}
}

