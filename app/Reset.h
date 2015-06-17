#ifndef Reset_H
#define Reset_H

#include "Delay.h"
#include "IO_Operations.h"
#include "Clock.h"

void lineReset(int numberOfClock);
void resetTarget();
#endif // Reset_H
