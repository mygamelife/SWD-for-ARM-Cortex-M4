#ifndef CoreDebug_H
#define CoreDebug_H

#include "CoreDebug_Utilities.h"
#include "Register_ReadWrite.h"

#define enableDWTandITM()   {memoryWriteByte(DEMCR_REG+3,ENABLE_DWT_ITM);}
#define disableDWTandITM()  {memoryWriteByte(DEMCR_REG+3,DISABLE_DWT_ITM);}

#define enableVectorCatchCoreReset()  {enableSelectedVectorCatch(VC_CORERESET);}
#define enableVectorCatchMMERR()      {enableSelectedVectorCatch(VC_MMERR);}
#define enableVectorCatchNOCPERR()    {enableSelectedVectorCatch(VC_NOCPERR);}
#define enableVVectorCatchCHKERR()    {enableSelectedVectorCatch(VC_CHKERR);}
#define enableVectorCatchSTATERR()    {enableSelectedVectorCatch(VC_STATERR);}
#define enableVectorCatchBUSERR()     {enableSelectedVectorCatch(VC_BUSERR);}
#define enableVectorCatchINTERR()     {enableSelectedVectorCatch(VC_INTERR);}
#define enableVectorCatchHARDERR()    {enableSelectedVectorCatch(VC_HARDERR);}
#define disableAllVectorCatch()       {enableSelectedVectorCatch(VC_DISABLEALL);}

#define isExternalDebugEventOccured()     ((isSelectedDebugEventOccured(EXTERNAL_DEBUGEVENT)) ? 1 : 0)
#define isVectorCatchDebugEventOccured()  ((isSelectedDebugEventOccured(VCATCH_DEBUGEVENT)) ? 1 : 0)
#define isDWTTrapDebugEventOccured()      ((isSelectedDebugEventOccured(DWTTRAP_DEBUGEVENT)) ? 1 : 0)
#define isBreakpointDebugEventOccured()   ((isSelectedDebugEventOccured(BKPT_DEBUGEVENT)) ? 1 : 0)
#define isHaltedDebugEventOccured()       ((isSelectedDebugEventOccured(HALTED_DEBUGEVENT)) ? 1 : 0)

void setCoreMode(CoreMode mode);
CoreMode getCoreMode();

void writeCoreRegister(CoreRegister coreRegister,uint32_t data);
void readCoreRegister(CoreRegister coreRegister,uint32_t *dataRead);

void waitForCoreRegisterTransactionToComplete();

int isSelectedDebugEventOccured(DebugEvent debugEvent);

void enableSelectedVectorCatch(VectorCatch vectorCatch);

void enableSelectedVectorCatch(VectorCatch vectorCatch);

void performHaltOnReset();


#endif // CoreDebug_H
