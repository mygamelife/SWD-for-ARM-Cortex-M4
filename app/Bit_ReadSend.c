#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "Bit_ReadSend.h"


void clockGenerator_1cycle()
{
	SWCLK_ON()
	SWCLK_OFF()
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
