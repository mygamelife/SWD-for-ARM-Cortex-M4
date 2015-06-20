#include "Bit_Operations.h"

/**
 * Send 1 bit of data through SWDIO pin by setting the SWDIO pin first
 * and generate 1 clock cycle through SWCLK pin
 *
 * Input : data  : 0 will set SWDIO pin low
 *		         : 1 will set SWDIO pin high	 
 * 
 */
void sendBit(int data)
{
	if (data != 0 )
		SWDIO_High();
	else
		SWDIO_Low();
	
	SWCLK_OFF();
	SWCLK_ON();
}

/**
 * Send multiple bits (max 32 bits)of data through SWDIO pin and 
 * generate the required amount of clock cycle
 * by calling multiple times of sendBit() 
 *
 * Input : dataToSend is the data going to be sent , maximum 32 bits
 *		   numberOfBits is used to determine how many bits to be sent
 * 
 */
void sendBits(uint32_t dataToSend,int numberOfBits)
{
	int i = 0  ;
	uint32_t bitValue = 0 ;
	
	for ( i = 0 ; i < numberOfBits ; i ++)
	{
		bitValue = dataToSend & (1 << i) ;
		bitValue = bitValue >> i ;
		sendBit(bitValue);
	}
}

/**
 * Read 1 bit of data through SWDIO pin during the falling edge of SWCLK
 *
 * Output : return the pin state read at SWDIO pin , either 1 for high 0 for low
 * 
 */
int readBit()
{
	int bitRead= 0 ;

	SWCLK_ON();
	SWCLK_OFF();
	bitRead = readSWDIO_Pin();
	
	return bitRead;
}

/**
 * Read multiple bits (max 32 bits) of data through SWDIO pin 
 * by calling multiple times of readBit() function
 *
 * Input : dataRead is the pointer to the place where data is stored (maximum 32 bits of data)
 *		   numberOfBits is used to determine how many bits to be read
 * 
 */
void readBits(uint32_t *dataRead,int numberOfBits)
{
	int i , bitValue ;
	
	for (i=0 ; i < numberOfBits ; i++)
	{
		bitValue = readBit();
		*dataRead = *dataRead | (bitValue << i);
	}
}

/**
 * Send 8 bits of data through SWDIO pin and 
 * generate the required amount of clock cycle
 *
 * Input : dataToSend is the data going to be sent , maximum 16 bits
 * 
 */
void send8bit(int dataToSend)
{
	sendBits(dataToSend,8);
}

/**
 * Send 16 bits of data through SWDIO pin and 
 * generate the required amount of clock cycle
 *
 * Input : dataToSend is the data going to be sent , maximum 16 bits
 * 
 */
void send16bit(int dataToSend)
{
	sendBits(dataToSend,16);
}


/**
 * Send 32 bits of data through SWDIO pin and 
 * generate the required amount of clock cycle
 *
 * Input : dataToSend is the data going to be sent , maximum 32 bits
 * 
 */
void send32bit(uint32_t dataToSend)
{
	sendBits(dataToSend,32);	
}

/**
 * Read 3 bits of data through SWDIO pin
 *
 * Input : dataRead is the pointer to the place where data is stored (maximum 16 bits of data)
 * 
 */
void read3bit(int *dataRead)
{
	readBits((uint32_t *)dataRead,3);
}

/**
 * Read 32 bits of data through SWDIO pin
 *
 * Input : dataRead is the pointer to the place where data is stored (maximum 32 bits of data)
 * 
 */
void read32bit(uint32_t *dataRead)
{
	readBits(dataRead,32);
}
