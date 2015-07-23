#ifndef ILV_Protocol_H
#define ILV_Protocol_H

#include "swdStub.h"

void Load_EraseSector_Instruction(uint32_t startAddress, uint32_t endAddress);
void Load_MassErase_Instruction(uint32_t bankSelect);
void Load_Copy_Instruction(uint32_t sramStartAddr, uint32_t flashStartAddr, int length);
#endif // ILV_Protocol_H
