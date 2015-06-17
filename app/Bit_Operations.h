#ifndef Bit_Operations_H
#define Bit_Operations_H

#include "IO_Operations.h"
#include <stdint.h>

void sendBit(int data);
void sendBits(uint32_t data,int numberOfBits);

int readBit();
void readBits(uint32_t *dataRead,int numberOfBits);

void send8bit(int dataToSend);
void send16bit(int dataToSend);
void send32bit(uint32_t dataToSend);

void read3bit(int *dataRead);
void read32bit(uint32_t *dataRead); 

#endif // Bit_Operations_H
