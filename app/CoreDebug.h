#ifndef CoreDebug_H
#define CoreDebug_H

#include "Clock.h"
#include "Reset.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"
#include "CoreDebug_Utilities.h"

#define DHCSR_REG	0xE000EDF0
#define DFSR_REG	0xE000ED30
#define DEMCR_REG	0xE000EDFC	
#define DCRSR_REG	0xE000EDF4	  
#define DCRDR_REG 	0xE000EDF8

int setCore(CoreControl coreControl,CoreStatus *coreStatus);
int setCore_Exception(CoreControl coreControl,CoreStatus *coreStatus);

void configure_DEMCR(DebugMonitorControl debugmonitorcontrol,VectorCatch *vectorCatch);

int write_CoreRegister(Core_RegisterSelect coreRegister,CoreStatus *coreStatus,uint32_t data);
int read_CoreRegister(Core_RegisterSelect coreRegister,CoreStatus *coreStatus,uint32_t *dataRead);

void check_CoreStatus(CoreStatus *coreStatus);
void check_DebugEvent(DebugEvent *debugEvent);
void check_VectorCatch(VectorCatch *vectorCatch);

void enable_VectorCatch(DebugMode debugMode,VectorCatch *vectorCatch);


int wait_CoreRegisterTransaction(CoreStatus *coreStatus, int numberOfTries);




#endif // CoreDebug_H
