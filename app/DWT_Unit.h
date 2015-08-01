#ifndef DWT_Unit_H
#define DWT_Unit_H

#include <stdint.h>
#include "DWT_Utilities.h"

int configure_DWTControl(DWTInfo *dwtInfo);

int read_DWTControl(DWTInfo *dwtInfo);
int read_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno);
int read_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno);
int read_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno);


int disable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno);
int reenable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno);

int configure_DWTComparator(DWTInfo *dwtInfo,uint32_t address);
int configure_DWTMask(DWTInfo *dwtInfo,IgnoreMask ignoreMask);
int configure_DWTFunction(DWTInfo *dwtInfo,int firstLinkComp,int secondLinkComp,ComparisonMode mode,DATAVSIZE size,int EMITRANGE,DWTFunction function);


int setWatchpoint_DataValueRead(DWTInfo *dwtInfo,uint32_t value);
int setWatchpoint_DataValueWrite(DWTInfo *dwtInfo,uint32_t value);
int setWatchpoint_DataValueReadWrite(DWTInfo *dwtInfo,uint32_t value);

int setWatchpoint_DataAddrRead(DWTInfo *dwtInfo,uint32_t DWT_COMPno,IgnoreMask mask,uint32_t address);
int setWatchpoint_DataAddrWrite(DWTInfo *dwtInfo,uint32_t DWT_COMPno,IgnoreMask mask,uint32_t address);
int setWatchpoint_DataAddrReadWrite(DWTInfo *dwtInfo,uint32_t DWT_COMPno,IgnoreMask mask,uint32_t address);


int setWatchpoint_PC(DWTInfo *dwtInfo,uint32_t value);



#endif // DWT_Unit_H
