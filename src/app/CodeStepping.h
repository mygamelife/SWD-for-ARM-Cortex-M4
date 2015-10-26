#ifndef CodeStepping_H
#define CodeStepping_H

#include "CoreDebug.h"
#include "FPBUnit.h"

#define BUFFER_SIZE    100

#define POP_R7andLR_MACHINECODE     0xBD80
#define BX_LR_MACHINECODE           0x4770
#define MOV_LRtoPC_MACHINECODE      0x46F7

#define INITIALMAINCODE_SP          0x2002ffe8

#define ENCODING_32BIT_CASE1        0xE800
#define ENCODING_32BIT_CASE2        0xF000
#define ENCODING_32BIT_CASE3        0xF800
#define ENCODING_32BIT_MASK         0xF800

#define BL_INSTRUCTION_MASK         0xF800D000
#define BL_ENCODING                 0xF000D000

int isStackPointerPointingToDefaultLocation();

int isSelectedAddressContains32bitsInstruction(uint32_t address);
int isSelectedAddressContains32bitsInstructionExtended(uint32_t address,uint32_t *machineCode);

int isSelectedAddressContainsCallingSubroutineInstruction(uint32_t address);

int isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(uint32_t machineCode);

uint32_t stepIntoOnce();
uint32_t stepOver();
uint32_t stepOut();

#endif // CodeStepping_H




