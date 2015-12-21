#ifndef SystemTime_H
#define SystemTime_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "CException.h"

#define ONE_SECOND          1000
#define TWO_SECOND          2000
#define THREE_SECOND        3000
#define FOUR_SECOND         4000
#define FIVE_SECOND         5000
#define TEN_SECOND          10000
#define FIFTEEN_SECOND      15000
#define FORTY_SECOND        40000
#define ONE_MINUTE          60000

uint16_t getSystemTime(void);
int isTimeout(int timeout, uint32_t previousTime);

#endif // SystemTime_H
