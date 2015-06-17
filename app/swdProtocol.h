#ifndef swdProtocol_H
#define swdProtocol_H

#include "configurePort.h"
#include "Bit_Operations.h"
#include "swd_Utilities.h"
#include "Clock.h"
#include "Reset.h"

void switchJTAGtoSWD();
void SWD_Initialisation();

#endif // swdProtocol_H
