#ifndef FPB_Unit_H
#define FPB_Unit_H

#include "Register_ReadWrite.h"
#include "FPB_Utilities.h"
#include "CoreDebug.h"

#define enableFPBUnit()   memoryWriteWord(FP_CTRL,ENABLE_FPB)
#define disableFPBUnit()  memoryWriteWord(FP_CTRL,DISABLE_FPB)


int setInstructionBreakpoint(uint32_t instructionCOMPno,uint32_t instructionAddress,int matchingMode);
int setInstructionRemapping(uint32_t instructionCOMPno,uint32_t instructionAddress, uint32_t remapAddress);
int setLiteralRemapping(uint32_t literalCOMPno,uint32_t literalAddress, uint32_t remapAddress);

int disableFPComparator(uint32_t compNo);


#endif // FPB_Unit_H
