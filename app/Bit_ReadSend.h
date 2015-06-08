#ifndef Bit_ReadSend_H
#define Bit_ReadSend_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#define SWCLK_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
#define SWCLK_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
#define SWDIO_High() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
#define SWDIO_Low() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);


void clockGenerator_1cycle();
void extraIdleClock(int numberOfClocks);

void sendBit(int value);
void sendBits(uint32_t dataToSend,int numberOfBits);
int readBit();
void readBits(uint32_t *dataRead,int numberOfBits);

#endif // Bit_ReadSend_H
