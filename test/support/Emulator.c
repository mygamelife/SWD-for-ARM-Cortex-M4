#include "stdio.h"
#include "Emulator.h"


uint32_t MSB_LSB_Conversion(uint32_t input)
{
	// swap odd and even bits
    input = (((input & 0xaaaaaaaa) >> 1) | ((input & 0x55555555) << 1));
	// swap consecutive pairs
    input = (((input & 0xcccccccc) >> 2) | ((input & 0x33333333) << 2));
	// swap nibble
    input = (((input & 0xf0f0f0f0) >> 4) | ((input & 0x0f0f0f0f) << 4));
	// swap bytes
    input = (((input & 0xff00ff00) >> 8) | ((input & 0x00ff00ff) << 8));
	// swap 2-byte long pairs
    return((input >> 16) | (input << 16));

}

void emulateWrite(int data, int numOfBits)  {
  int i, bitValue = 0;

	for ( i = 0; i < numOfBits; i ++)
	{
		bitValue = data & (1 << i) ;
		bitValue = bitValue >> i;

    if (bitValue != 0 )
      SWDIO_High_Expect();

    else  SWDIO_Low_Expect();

    SWCLK_OFF_Expect(); SWCLK_ON_Expect();
	}
}

void emulateRead(int data, int numOfBits)  {
	int i, bitValue = 0;

	for (i = 0; i < numOfBits; i++)
	{
    SWCLK_ON_Expect(); SWCLK_OFF_Expect();
		bitValue = data & (1 << (numOfBits - 1 - i));

    if (bitValue != 0 ) {
      //printf("1");
      readSWDIO_Pin_ExpectAndReturn(1);
    }
      

    else {
      //printf("0");
      readSWDIO_Pin_ExpectAndReturn(0);
    } 
	}
}

void emulateTurnAroundRead()  {
  SWCLK_OFF_Expect();
}

void emulateTurnAroundWrite() {
	SWCLK_ON_Expect();
	SWCLK_OFF_Expect();
	SWCLK_ON_Expect();
}

void emulateSwdOutput() {
  SWDIO_OutputMode_Expect();
}

void emulateSwdInput()  {
  SWDIO_InputMode_Expect();
}

void emulateIdleClock(int numOfClocks)  {
  int i;
  
  SWDIO_Low_Expect();

	for(i = 0; i < numOfClocks; i++)  {
		SWCLK_OFF_Expect();SWCLK_ON_Expect();
	}
}

void emulateLineReset(int numOfClocks)
{
	int i ;
	
	if (numOfClocks < 50 )
		numOfClocks = 50 ;
	
	//lineReset
	SWDIO_High_Expect();
	for (i = 0 ; i < numOfClocks ; i ++)
	{	SWCLK_OFF_Expect();
		SWCLK_ON_Expect();
	}
}

void emulateResetTarget()
{
	ResetPin_Low_Expect();
	ResetPin_High_Expect();
}

void emulateSWDRegister_Write(int Address,int APnDP,int emulateACK, uint32_t data)
{
	int SWD_Request = 0 , parity = 0;
	SWD_Request = getSWD_Request(Address,APnDP,WRITE);
	parity = calculateParity_32bitData(data);
	
	emulateWrite(SWD_Request,8);
	emulateTurnAroundRead();
	emulateSwdInput();

	emulateRead(emulateACK,3);

	emulateTurnAroundWrite();
	emulateSwdOutput();

	emulateWrite(data,32);
	emulateWrite(parity,1);

	emulateIdleClock(8);
}

void emulateSWDRegister_Read(int Address,int APnDP,int emulateACK,int emulateParity, uint32_t emulateData)
{
	int SWD_Request = 0 ;
	SWD_Request = getSWD_Request(Address,APnDP,READ);

	emulateWrite(SWD_Request,8);

	emulateTurnAroundRead();
	emulateSwdInput();

	emulateRead(emulateACK,3);
	emulateRead(emulateData,32);
	emulateRead(emulateParity,1);
	emulateTurnAroundWrite();
	emulateSwdOutput();

	emulateIdleClock(8);
}