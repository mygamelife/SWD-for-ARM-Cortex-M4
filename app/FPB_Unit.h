#ifndef FPB_Unit_H
#define FPB_Unit_H

#include "Reset.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"

#define FPB_REG 0xE0002000
#define ENABLE_FPB 0x00000003
#define FPB_MASK 1
#define TRUE 1
#define FALSE 0

int FPB_Enable();
int isFPB_Enabled(uint32_t data);

#endif // FPB_Unit_H
