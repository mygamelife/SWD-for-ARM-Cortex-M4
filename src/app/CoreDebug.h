#ifndef CoreDebug_H
#define CoreDebug_H

#include "CoreDebug_Utilities.h"

#define enableDWTandITM()                   {memoryWriteByte((DEMCR_REG+3),ENABLE_DWT_ITM);}
#define disableDWTandITM()                  {memoryWriteByte((DEMCR_REG+3),DISABLE_DWT_ITM);}

#define enableVectorCatchCoreReset()        {memoryWriteByte(DEMCR_REG,VC_CORERESET);}
#define enableVectorCatchMMERR()            {memoryWriteHalfword(DEMCR_REG,VC_MMERR);}
#define enableVectorCatchNOCPERR()          {memoryWriteHalfword(DEMCR_REG,VC_NOCPERR);}
#define enableVVectorCatchCHKERR()          {memoryWriteHalfword(DEMCR_REG,VC_CHKERR);}
#define enableVectorCatchSTATERR()          {memoryWriteHalfword(DEMCR_REG,VC_STATERR);}
#define enableVectorCatchBUSERR()           {memoryWriteHalfword(DEMCR_REG,VC_BUSERR);}
#define enableVectorCatchINTERR()           {memoryWriteHalfword(DEMCR_REG,VC_INTERR);}
#define enableVectorCatchHARDERR()          {memoryWriteHalfword(DEMCR_REG,VC_HARDERR);}

#define disableAllVectorCatch()             {memoryWriteHalfword(DEMCR_REG,VC_DISABLEALL);}
#define disableVectorCatchCoreReset()       {memoryWriteByte(DEMCR_REG,0);}

#define hasExternalDebugEventOccured()      (readDebugEventRegister() & EXTERNAL_DEBUGEVENT)
#define hasVectorCatchDebugEventOccured()   (readDebugEventRegister() & VCATCH_DEBUGEVENT)
#define hasDWTTrapDebugEventOccured()       (readDebugEventRegister() & DWTTRAP_DEBUGEVENT)
#define hasBreakpointDebugEventOccured()    (readDebugEventRegister() & BKPT_DEBUGEVENT)
#define hasHaltedDebugEventOccured()        (readDebugEventRegister() & HALTED_DEBUGEVENT)

#define clearBreakpointDebugEvent()         {memoryWriteWord(DFSR_REG,BKPT_DEBUGEVENT);}
#define clearDWTTrapDebugEvent()            {memoryWriteWord(DFSR_REG,DWTTRAP_DEBUGEVENT);}
#define clearVectorCatchDebugEvent()        {memoryWriteWord(DFSR_REG,VCATCH_DEBUGEVENT);}
#define clearExternalDebugEvent()           {memoryWriteWord(DFSR_REG,EXTERNAL_DEBUGEVENT);}

#define softResetTarget()                   {memoryWriteWord(AIRCR_REG,REQUEST_SYSTEM_RESET);}             
#define vectorResetTarget()                 {memoryWriteWord(AIRCR_REG,REQUEST_VECTOR_RESET);}


int doesCoreModeRequiresHaltedAndDebug(CoreMode mode);
void setCoreMode(CoreMode mode);

CoreMode determineCoreModeFromDataRead(uint32_t dataRead);
CoreMode getCoreMode();

void stepOnly(int nInstructions);

void writeCoreRegister(CoreRegister coreRegister,uint32_t data);
uint32_t readCoreRegister(CoreRegister coreRegister);

void waitForCoreRegisterTransactionToComplete();

uint32_t readDebugEventRegister();
void clearDebugEvent(uint32_t debugEvent);

void performHaltOnReset();
#endif // CoreDebug_H
