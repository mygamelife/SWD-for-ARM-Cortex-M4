#ifndef FPB_Unit_H
#define FPB_Unit_H

#include "MemoryReadWrite.h"
#include "FPB_Utilities.h"
#include "CoreDebug.h"
#include "CodeStepping.h"

#define enableFPBUnit()   memoryWriteWord((uint32_t)&(FPB->FP_CTRL),ENABLE_FPB)
#define disableFPBUnit()  memoryWriteWord((uint32_t)&(FPB->FP_CTRL),DISABLE_FPB)


int manualSetInstructionBreakpoint(int instructionCOMPno,uint32_t instructionAddress,int matchingMode);
int manualSetInstructionRemapping(int instructionCOMPno,uint32_t instructionAddress, uint32_t remapAddress);
int manualSetLiteralRemapping(int literalCOMPno,uint32_t literalAddress, uint32_t remapAddress);

int autoSetInstructionBreakpoint(uint32_t instructionAddress,int matchingMode);
int autoSetInstructionRemapping(uint32_t instructionAddress,uint32_t machineCode);

uint32_t autoSetSoftwareBreakpoint(uint32_t instructionAddress);
void restoreSoftwareBreakpointOriginalInstruction(uint32_t instructionAddress,uint32_t machineCode);

int disableInstructionComparator(int instructionCOMPno);
int disableLiteralComparator(int literalCOMPno);
int disableFPComparatorLoadedWithAddress(uint32_t address,int comparatorType);
void disableAllFPComparator();

void readAndUpdateComparatorReadyFlag(int comparatorType);

void initialiseFPBUnit();

void removeAllFPComparatorSetToBreakpoint();
void stopAllFPRemapping();
#endif // FPB_Unit_H
