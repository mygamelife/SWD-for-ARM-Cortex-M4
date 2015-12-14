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

void configureTraceStartStopLogic(int traceStartStopLogicEnable,ResourceSelection startResource,ResourceSelection stopResource);
void configureTraceEnableEnablingEvent(ETMEvent_FunctionEncoding function,ETMEvent_Resources resourceA,ETMEvent_Resources resourceB);

#endif // ETM-M4_H
