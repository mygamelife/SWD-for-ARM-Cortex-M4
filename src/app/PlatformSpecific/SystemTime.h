#ifndef SystemTime_H
#define SystemTime_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "CException.h"

#define FIVE_SECOND		5000
#define ONE_SECOND		1000

uint16_t getSystemTime(void);
uint16_t getElapsedTime(void);
void resetSystemTime(void);
int isTimeOut(int timeOut);

#endif // SystemTime_H
