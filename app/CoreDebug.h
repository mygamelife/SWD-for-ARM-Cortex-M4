#ifndef CoreDebug_H
#define CoreDebug_H

#include "Clock.h"
#include "Reset.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"

#define DHCSR_REG 0xE000EDF0
#define SET_CORE_DEBUG 0xA05F0001
#define SET_CORE_HALT 0xA05F0003

#define CORE_DEBUG_MASK 0x1
#define CORE_HALT_MASK 0x00020000

#define TRUE 1
#define FALSE 0

int setCore_DebugMode();
int setCore_Halt();

int isCore_DebugMode();
int isCore_Halted();

#endif // CoreDebug_H
