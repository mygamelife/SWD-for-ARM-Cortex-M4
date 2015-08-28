#ifndef swdProtocol_H
#define swdProtocol_H

#include "Delay.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "IoOperations.h"
#include "Register_ReadWrite.h"

void switchJTAGtoSWD();
void SWD_Initialisation();

#endif // swdProtocol_H
