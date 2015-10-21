#ifndef IoOperations_H
#define IoOperations_H

#include "stm32f4xx_hal_gpio.h"
#include <stdint.h>
#include "configurePort.h"
#include "IoOperationsEx.h"

/* ############################ Swd I/O Operation Functions ############################### */

void generateOneClockCycle();
void extraIdleClock(int numberOfClocks);
void turnAroundRead();
void turnAroundWrite();

void delay(int ms,int numberOfLoops, int division);
void lineReset(int numberOfClock);
void hardResetTarget();

void sendBit(int data);
void sendBits(uint32_t data,int numberOfBits);

int readBit();
void readBits(uint32_t *dataRead,int numberOfBits);

void send8bit(int dataToSend);
void send16bit(int dataToSend);
void send32bit(uint32_t dataToSend);

void read3bit(int *dataRead);
void read32bit(uint32_t *dataRead); 
#endif // IoOperations_H
