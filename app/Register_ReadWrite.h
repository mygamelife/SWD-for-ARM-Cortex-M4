#ifndef Register_ReadWrite_H
#define Register_ReadWrite_H

#include <stdint.h>
#include "Bit_Operations.h"
#include "swd_Utilities.h"

//-------------------------- SWD-DP Register --------------------------------

//IDCODE Register
void readIDCODEReg(int RnW);

//CONTROL/STATUS Register
void ctrlStatusReg(int RnW);
void setCtrlStatusReg(uint32_t data);

//ABORT Register
void writeAbortReg();
void setAbortReg(uint32_t data);

//SELECT Register
void writeSelectReg();
//-------------------------- SWD-DP Register --------------------------------
#endif // Register_ReadWrite_H
