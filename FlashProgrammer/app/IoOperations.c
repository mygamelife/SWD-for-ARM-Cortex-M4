#include "IoOperations.h"

/**
 * Generate 1 clock cycle at SWDCLK pin by clocking low followed by clocking high
 *
 */
void generateOneClockCycle()
{
	setLowSWCLK();
	setHighSWCLK();
}

/**
 * Generate idle clock cycle by setting SWDIO pin low and clock
 * before starting a new transaction 
 *
 * Note : After line reset there should be at least 2 idle clock cyle
 *
 * Input : numberOfClocks is the amount of idle clock to be generated
 */
void extraIdleClock(int numberOfClocks)
{
	int i;

	setLowSWDIO();
	for(i = 0 ; i < numberOfClocks ; i ++)
		generateOneClockCycle();
}

/**
 * Generate 0.5 clock cycle of turn around period 
 *
 * Host : change from output mode to input mode
 * Target : change from input mode to output mode
 * 
 */
void turnAroundRead()
{
	setLowSWCLK(); // z impedance on both line starts here
}

/**
 * Generate 1.5 clock cycle of turn around period 
 *
 * Host : change from input mode to output mode
 * Target : change output mode to input mode
 * 
 */
void turnAroundWrite()
{
	setHighSWCLK(); // z impedance on both line starts here
	setLowSWCLK();
	setHighSWCLK();
}

/**
 * Perform a line reset by clocking at least 50 cycles of SWDIO high to 
 * ensure that JTAG/SWD interface is at reset state
 *
 * Input : numberOfClock is the number of clock with SWDIO high to be generated
 */
void lineReset(int numberOfClock)
{
	int i  = 0 ;
	
	if (numberOfClock < 50 )
		numberOfClock = 50 ;
	
	setHighSWDIO();
	
	for ( i = 0 ; i < numberOfClock ; i ++)
	{
		generateOneClockCycle();
	}
}

/**
 * Perform a hard reset on the target device by setting low and high 
 * on the nRST pin of the target
 *
 * Note : It is recommended to perform a hard reset as the target device might not respond
 */
void hardResetTarget()
{
	setLowNRST();
	delay(500,1,1);
	setHighNRST();
	delay(2500,1,1);
}


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
		setHighSWDIO();
	else
		setLowSWDIO();
	
	setLowSWCLK();
	setHighSWCLK();
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

	setHighSWCLK();
	setLowSWCLK();
	bitRead = readSWDIO();
	
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
	*dataRead = 0 ; // clear to make sure it is 0 first
	
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
