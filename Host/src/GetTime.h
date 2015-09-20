#ifndef GetTime_H
#define GetTime_H

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <sys/time.h>
#include "CException.h"

extern double elapsedTime;

void InitTimer(void);
void startTimer(void);
void stopTimer(void);

int getElapsedTime(void);
#endif // GetTime_H
