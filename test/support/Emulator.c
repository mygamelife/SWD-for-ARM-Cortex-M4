#include <stdio.h>
#include "Emulator.h"


uint32_t interconvertMSBandLSB(uint32_t input)
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
      setHighSWDIO_Expect();

    else  setLowSWDIO_Expect();

    setLowSWCLK_Expect(); setHighSWCLK_Expect();
	}
}

void emulateRead(int data, int numOfBits)  {
	int i, bitValue = 0;

	for (i = 0; i < numOfBits; i++)
	{
    setHighSWCLK_Expect(); setLowSWCLK_Expect();
		bitValue = data & (1 << (numOfBits - 1 - i));

    if (bitValue != 0 ) {
      //printf("1");
      readSWDIO_ExpectAndReturn(1);
    }
      

    else {
      //printf("0");
      readSWDIO_ExpectAndReturn(0);
    } 
	}
}

void emulateTurnAroundRead()  {
  setLowSWCLK_Expect();
}

void emulateTurnAroundWrite() {
	setHighSWCLK_Expect();
	setLowSWCLK_Expect();
	setHighSWCLK_Expect();
}

void emulateSwdOutput() {
  SWDIO_OutputMode_Expect();
}

void emulateSwdInput()  {
  SWDIO_InputMode_Expect();
}

void emulateIdleClock(int numOfClocks)  {
  int i;
  
  setLowSWDIO_Expect();

	for(i = 0; i < numOfClocks; i++)  {
		setLowSWCLK_Expect();setHighSWCLK_Expect();
	}
}

void emulateLineReset(int numOfClocks)
{
	int i ;
	
	if (numOfClocks < 50 )
		numOfClocks = 50 ;
	
	//lineReset
	setHighSWDIO_Expect();
	for (i = 0 ; i < numOfClocks ; i ++)
	{	setLowSWCLK_Expect();
		setHighSWCLK_Expect();
	}
}

void emulateResetTarget()
{
	setLowNRST_Expect();
	setHighNRST_Expect();
}

void emulateSwdRegisterWrite(int address, int pointType, int ack, uint32_t data)
{
	int swdRequest, parity;
  
	swdRequest = getSWD_Request(address, pointType, WRITE);
	parity = calculateParity_32bitData(data);
	
	emulateWrite(swdRequest, 8);
	emulateTurnAroundRead();
	emulateSwdInput();

	emulateRead(ack, 3);

	emulateTurnAroundWrite();
	emulateSwdOutput();

	emulateWrite(data, 32);
	emulateWrite(parity, 1);

	emulateIdleClock(8);
}

void emulateSwdRegisterRead(int address, int pointType, int ack, int parity, uint32_t data)
{
	int swdRequest;
	swdRequest = getSWD_Request(address, pointType, READ);

	emulateWrite(swdRequest, 8);

	emulateTurnAroundRead();
	emulateSwdInput();

	emulateRead(ack, 3);
	emulateRead(data, 32);
	emulateRead(parity, 1);
	emulateTurnAroundWrite();
	emulateSwdOutput();

	emulateIdleClock(8);
}