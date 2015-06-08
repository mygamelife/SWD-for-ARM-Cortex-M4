#include "Bit_ReadSend.h"
#include "swdProtocol.h"


void clockGenerator_1cycle()
{
	SWCLK_OFF()
	SWCLK_ON()
}

/* include two IDLE clock cycles with SWDIO low for each packet
 * to ensure the operation is in stable mode
 */
void extraIdleClock(int numberOfClocks)
{
	int i;

	for(i = 0 ; i < numberOfClocks ; i ++)
		sendBit(0);
}


void sendBit(int value)
{
	SWCLK_OFF() ;

	if (value == 1)
		SWDIO_High()
	else
		SWDIO_Low()

	SWCLK_ON();
}

void sendBits(uint32_t dataToSend,int numberOfBits)
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
	SWCLK_OFF();
	bitRead  = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);

	if (bitRead  == 1)
		return 1 ;
	else
		return 0 ;
}

void readBits(uint32_t *dataRead,int numberOfBits)
{
	int i ;
	int bitRead = 0;

	for ( i = 0 ; i < numberOfBits ; i ++)
	{
		bitRead = readBit();
		*dataRead = *dataRead | (bitRead << i ) ;
	}


}
