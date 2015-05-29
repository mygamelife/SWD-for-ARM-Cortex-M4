#ifndef Bit_ReadSend_H
#define Bit_ReadSend_H

#define SWCLK_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
#define SWCLK_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
#define SWDIO_High() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
#define SWDIO_Low() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);


void clockGenerator_1cycle();

void sendBit(int value);
void sendBits(long dataToSend,int numberOfBits);
int readBit();
void readBits(long *dataRead,int numberOfBits);

#endif // Bit_ReadSend_H
