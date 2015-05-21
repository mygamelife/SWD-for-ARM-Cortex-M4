#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#define SWDIO

#define SWCLK_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
#define SWCLK_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
#define SWDIO_High() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
#define SWDIO_Low() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

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

void configurePort()
{
	GPIO_InitTypeDef GpioInfo;
	__GPIOB_CLK_ENABLE();

	GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
	GpioInfo.Pin = GPIO_PIN_10 ; // PB10 as clock output
	GpioInfo.Pull = GPIO_NOPULL ;
	GpioInfo.Speed = GPIO_SPEED_HIGH ;

	HAL_GPIO_Init(GPIOB,&GpioInfo);
}

void SWDIO_OutputMode()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
	GpioInfo.Pin = GPIO_PIN_12; // PB12 as output
	GpioInfo.Pull = GPIO_NOPULL ;
	GpioInfo.Speed = GPIO_SPEED_HIGH ;

	HAL_GPIO_Init(GPIOB,&GpioInfo);
}

void SWDIO_InputMode()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_INPUT ;
	GpioInfo.Pin = GPIO_PIN_12; // PB12 as input
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
		clockGenerator_1cycle();
	}
	sendBit(0);
}

void sendBit(int value)
{
	if (value == 1)
		SWDIO_High()
	else
		SWDIO_Low()
}

void sendBits(long dataToSend,int numberOfBits)
{
	//LSB first

	SWDIO_OutputMode();

	int i , bitValue = 0;
	for (i=0 ; i < numberOfBits ; i ++)
	{
		bitValue = dataToSend & (1 << i ) ;

		if (bitValue == 1)
			sendBit(1);
		else
			sendBit(0);
	}
}

int readBit()
{
	GPIO_PinState bitRead ;

	bitRead  = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);

	if (bitRead  == 1)
		return 1 ;
	else
		return 0 ;
}

void readBits(long *dataRead,int numberOfBits)
{
	SWDIO_InputMode();

	int i , bitRead;

	for ( i = 0 ; i < numberOfBits ; i ++)
	{
		bitRead = readBit();
		*dataRead = *dataRead | (bitRead << i ) ;
	}


}

void clockGenerator_1cycle()
{
	SWCLK_ON();
	SWCLK_OFF();

}

int main(void)
{
	configurePort();
    while(1)
    {
    	clockGenerator_1cycle();
    }
}
