#ifndef Bit_ReadSend_H
#define Bit_ReadSend_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "Delay.h"
#include "IO_Operations.h"

void sendBit(int value);
void sendBits(uint32_t dataToSend,int numberOfBits);
int readBit();
void readBits(uint32_t *dataRead,int numberOfBits);

#endif // Bit_ReadSend_H
