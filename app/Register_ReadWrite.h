#ifndef Register_ReadWrite_H
#define Register_ReadWrite_H

#include <stdint.h>
#include "Clock.h"
#include "Bit_Operations.h"
#include "configurePort.h"
#include "swd_Utilities.h"
#include "IO_Operations.h"

//-------------------------- SWD-DP Register --------------------------------
#define WDERRCLR 0x08

//IDCODE Register
void readIDCODEReg();

//CONTROL/STATUS Register
void ctrlStatusReg(int RnW);

//ABORT Register
void writeAbortReg();
void writeDataToSelectReg(uint32_t data);

//SELECT Register
void writeSelectReg();
//-------------------------- SWD-DP Register --------------------------------
#endif // Register_ReadWrite_H
