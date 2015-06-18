#ifndef Bit_ReadSend_H
#define Bit_ReadSend_H

#include "stm32f4xx_hal_gpio.h"
#include "IO_Operations.h"
#include "Delay.h"

#if !defined(TEST)

#include "stm32f4xx_hal.h"

#endif // !defined(TEST)

void sendBit(int value);
void sendBits(uint32_t dataToSend,int numberOfBits);
int readBit();
void readBits(uint32_t *dataRead,int numberOfBits);

#endif // Bit_ReadSend_H
