#ifndef swdProtocol_H
#define swdProtocol_H

#include "Clock.h"
#include "Reset.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"

void switchJTAGtoSWD();
void SWD_Initialisation();

#endif // swdProtocol_H
