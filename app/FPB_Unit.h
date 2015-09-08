#ifndef FPB_Unit_H
#define FPB_Unit_H

#include "Register_ReadWrite.h"
#include "FPB_Utilities.h"
#include "CoreDebug.h"

#define enableFPBUnit()   memoryWriteWord((uint32_t)&(FPB->FP_CTRL),ENABLE_FPB)
#define disableFPBUnit()  memoryWriteWord((uint32_t)&(FPB->FP_CTRL),DISABLE_FPB)


int manualSetInstructionBreakpoint(int instructionCOMPno,uint32_t instructionAddress,int matchingMode);
int manualSetInstructionRemapping(int instructionCOMPno,uint32_t instructionAddress, uint32_t remapAddress);
int manualSetLiteralRemapping(int literalCOMPno,uint32_t literalAddress, uint32_t remapAddress);

int autoSetInstructionBreakpoint(uint32_t instructionAddress,int matchingMode);

int disableInstructionComparator(int instructionCOMPno);
int disableLiteralComparator(int literalCOMPno);

void readAndUpdateComparatorReadyFlag(int comparatorType);
int getEnabledComparatorLoadedWithAddress(uint32_t data);
#endif // FPB_Unit_H
