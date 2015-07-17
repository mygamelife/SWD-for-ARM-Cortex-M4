#ifndef ILV_Protocol_H
#define ILV_Protocol_H

#include "swdStub.h"

void load_SectorErase_Instruction(uint32_t startAddress, uint32_t endAddress);
void load_MassErase_Instruction(uint32_t bankSelect);
#endif // ILV_Protocol_H
