#ifndef Register_ReadWrite_H
#define Register_ReadWrite_H

#include <stdint.h>
#include "Bit_Operations.h"
#include "swd_Utilities.h"

//-------------------------- SWD-DP Register --------------------------------

//IDCODE Register
void readIDCODEReg(int RnW);

//Control/Status Register
void ctrlStatusReg(int RnW);
void setCtrlStatusReg(uint32_t data);
//-------------------------- SWD-DP Register --------------------------------
#endif // Register_ReadWrite_H
