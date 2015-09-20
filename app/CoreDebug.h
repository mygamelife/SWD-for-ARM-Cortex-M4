#ifndef CoreDebug_H
#define CoreDebug_H

#include "CoreDebug_Utilities.h"

#define enableDWTandITM()                   {memoryWriteByte((DEMCR_REG+3),ENABLE_DWT_ITM);}
#define disableDWTandITM()                  {memoryWriteByte((DEMCR_REG+3),DISABLE_DWT_ITM);}

#define enableVectorCatchCoreReset()        {enableVectorCatch(VC_CORERESET);}
#define enableVectorCatchMMERR()            {enableVectorCatch(VC_MMERR);}
#define enableVectorCatchNOCPERR()          {enableVectorCatch(VC_NOCPERR);}
#define enableVVectorCatchCHKERR()          {enableVectorCatch(VC_CHKERR);}
#define enableVectorCatchSTATERR()          {enableVectorCatch(VC_STATERR);}
#define enableVectorCatchBUSERR()           {enableVectorCatch(VC_BUSERR);}
#define enableVectorCatchINTERR()           {enableVectorCatch(VC_INTERR);}
#define enableVectorCatchHARDERR()          {enableVectorCatch(VC_HARDERR);}
#define disableAllVectorCatch()             {enableVectorCatch(VC_DISABLEALL);}

#define hasExternalDebugEventOccured()      (readDebugEventRegister() & EXTERNAL_DEBUGEVENT)
#define hasVectorCatchDebugEventOccured()   (readDebugEventRegister() & VCATCH_DEBUGEVENT)
#define hasDWTTrapDebugEventOccured()       (readDebugEventRegister() & DWTTRAP_DEBUGEVENT)
#define hasBreakpointDebugEventOccured()    (readDebugEventRegister() & BKPT_DEBUGEVENT)
#define hasHaltedDebugEventOccured()        (readDebugEventRegister() & HALTED_DEBUGEVENT)

#define clearBreakpointDebugEvent()         {clearDebugEvent(BKPT_DEBUGEVENT);}
#define clearDWTTrapDebugEvent()            {clearDebugEvent(DWTTRAP_DEBUGEVENT);}

#define softResetTarget()                   {memoryWriteWord(AIRCR_REG,REQUEST_SYSTEM_RESET);}             
#define vectorResetTarget()                 {memoryWriteWord(AIRCR_REG,REQUEST_VECTOR_RESET);}

void setCoreMode(CoreMode mode);
CoreMode getCoreMode();

void stepOnly(int nInstructions);

void writeCoreRegister(CoreRegister coreRegister,uint32_t data);
void readCoreRegister(CoreRegister coreRegister,uint32_t *dataRead);

void waitForCoreRegisterTransactionToComplete();

uint32_t readDebugEventRegister();
void clearDebugEvent(uint32_t debugEvent);

void enableVectorCatch(uint32_t vectorCatch);

void performHaltOnReset();


#endif // CoreDebug_H
