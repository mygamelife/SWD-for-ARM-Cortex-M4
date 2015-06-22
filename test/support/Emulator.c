#include "stdio.h"
#include "Emulator.h"

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