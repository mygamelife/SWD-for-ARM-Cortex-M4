#ifndef DWT_Unit_H
#define DWT_Unit_H

#include <stdint.h>
#include "DWT_Utilities.h"


int setAddressWatchpoint(uint32_t dwtCompNumber,uint32_t address,Watchpoint_AddressMask addressMask,Watchpoint_AccessMode accessMode);
int setDataWatchpoint_MatchingOneComparator(uint32_t dwtCompNumber,uint32_t address,Watchpoint_AddressMask addressMask,
                                            uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode);

int setDataWatchpoint_MatchingTwoComparator(uint32_t dwtCompNumber1,uint32_t address1,Watchpoint_AddressMask addressMask1,
                                            uint32_t dwtCompNumber2,uint32_t address2,Watchpoint_AddressMask addressMask2,
                                            uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode);

int isDataWatchpointOccurred();
int isAddressWatchpointOccurred(uint32_t dwtCompNumber);
                             
int disableDWTComparator(uint32_t dwtCompNumber);
void disableAllDWTComparators();

#endif // DWT_Unit_H
