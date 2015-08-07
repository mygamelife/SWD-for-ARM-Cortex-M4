#ifndef DWT_Unit_H
#define DWT_Unit_H

#include <stdint.h>
#include "DWT_Utilities.h"


typedef enum 
{
	WATCHPOINT_READ = 5 ,
	WATCHPOINT_WRITE = 6 ,
	WATCHPOINT_READWRITE 
}Watchpoint_AccessMode ;


int setWatchpoint(int comparatorNumber,uint32_t address,IgnoreMask addressMask,Watchpoint_AccessMode accessMode);
int setWatchpoint_MatchingOneComparator(int comparatorNumber,uint32_t address,IgnoreMask addressMask,uint32_t matchedData,Watchpoint_AccessMode accessMode);
int setWatchpoint_MatchingTwoComparator(int comparatorNumber_1,uint32_t address_1,IgnoreMask addressMask_1,
										int comparatorNumber_2,uint32_t address_2,IgnoreMask addressMask_2,
										uint32_t matchedData,Watchpoint_AccessMode accessMode);

int disable_WatchpointComparator(int comparatorNumber);
int disable_AllWatchpoint();







int setWatchpoint_PC(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t address,IgnoreMask ignoreMask);
int setWatchpoint_DataAddr(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t address,IgnoreMask ignoreMask,RWmode mode);
int setWatchpoint_DataValue(DWTInfo *dwtInfo,uint32_t value,DATAVSIZE size,RWmode mode);

int setWatchpoint_LinkedComparison(DWTInfo *dwtInfo,uint32_t value,DATAVSIZE size,uint32_t first_COMPno,uint32_t firstAddress,IgnoreMask firstMask,uint32_t second_COMPno,uint32_t secondAddress,IgnoreMask secondMask,RWmode mode);

#endif // DWT_Unit_H
