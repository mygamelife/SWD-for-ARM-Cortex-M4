#ifndef Clock_H
#define Clock_H

#include "IO_Operations.h"

void clockGenerator_1cycle();
void extraIdleClock(int numberOfClocks);
void turnAround_ToRead();
void turnAround_ToWrite();

#endif // Clock_H
