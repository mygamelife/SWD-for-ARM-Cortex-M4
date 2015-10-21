#ifndef Swd_H
#define swd_H

#include "swd_Utilities.h"
#include "configurePort.h"
#include "IoOperations.h"
#include "MemoryReadWrite.h"

void switchJTAGtoSWD(void);
SwdError swdInit(void);

#endif // Swd_H
