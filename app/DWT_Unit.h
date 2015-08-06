#ifndef DWT_Unit_H
#define DWT_Unit_H

#include <stdint.h>
#include "DWT_Utilities.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"

typedef enum 
{
	Comparator_Disable = 0,
	Read = 5 ,
	Write ,
	ReadWrite 
}RWmode ;

int configure_DWTControl(DWTInfo *dwtInfo);

int read_DWTControl(DWTInfo *dwtInfo);
int read_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno);
int read_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno);
int read_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno);


int disable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno);
int reenable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno,DWTFunction function);

int configure_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t address);
int configure_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno,IgnoreMask ignoreMask);
int configure_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno,int firstLinkComp,int secondLinkComp,ComparisonMode mode,DATAVSIZE size,DWTFunction function);

int setWatchpoint_PC(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t address,IgnoreMask ignoreMask);
int setWatchpoint_DataAddr(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t address,IgnoreMask ignoreMask,RWmode mode);
int setWatchpoint_DataValue(DWTInfo *dwtInfo,uint32_t value,DATAVSIZE size,RWmode mode);

int setWatchpoint_LinkedComparison(DWTInfo *dwtInfo,uint32_t value,DATAVSIZE size,uint32_t first_COMPno,uint32_t firstAddress,IgnoreMask firstMask,uint32_t second_COMPno,uint32_t secondAddress,IgnoreMask secondMask,RWmode mode);

#endif // DWT_Unit_H
