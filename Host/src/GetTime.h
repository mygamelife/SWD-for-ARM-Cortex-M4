#ifndef GetTime_H
#define GetTime_H

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <sys/time.h>
#include <stdlib.h>
#include "CException.h"

#define TIMEOUT_SECOND    5000

uint16_t getSystemTime(void);
uint16_t getElapsedTime(void);
void resetSystemTime(void);
int isTimeOut(void);

#endif // GetTime_H
