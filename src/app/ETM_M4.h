#ifndef ETM_M4_H
#define ETM_M4_H

#include "ETMEx.h"
#include "MemoryReadWrite.h"

#define unlockETM()   memoryWriteWord((uint32_t)&(ETM->ETMLAR),ETM_UNLOCK_KEY)
#define lockETM()     memoryWriteWord((uint32_t)&(ETM->ETMLAR),ETM_LOCK)


uint32_t getETMID();
void getETMConfiguration();

int isETMLocked();
int isETMPoweredUp();
int isETMProgrammingBitSet();

void powerUpETM();
void powerDownETM();

void setETMProgrammingBit();
void clearETMProgrammingBit();

#endif // ETM-M4_H
