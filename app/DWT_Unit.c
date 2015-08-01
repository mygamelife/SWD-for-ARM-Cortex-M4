#include "DWT_Unit.h"

int read_DWTControl(DWTInfo *dwtInfo)
{
	uint32_t dataRead = 0;
	int status = 0 ;
	
	status = memoryAccessRead(DWT_CTRL,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_DWTControlData(dwtInfo->dwtControl,dataRead);
	
	return ERR_NOERROR;
}

int read_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno)
{
	uint32_t dataRead = 0;
	int status = 0,number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_COMPno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	status = memoryAccessRead(DWT_COMPno,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_DWTComparatorData(dwtInfo->dwtCompInfo[number],dataRead);
	
	return ERR_NOERROR;
}

int read_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno)
{
	uint32_t dataRead = 0;
	int status = 0,number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_MASKno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	status = memoryAccessRead(DWT_MASKno,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_DWTMaskData(dwtInfo->dwtCompInfo[number],dataRead);
	
	return ERR_NOERROR;
}

int read_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno)
{
	uint32_t dataRead = 0;
	int status = 0,number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_FUNCno);
	
	if(number == ERR_INVALID_DWTREGISTER)
		return number ;
	
	status = memoryAccessRead(DWT_FUNCno,&dataRead);
	if (status != ERR_NOERROR)
		return status;
	
	process_DWTFunctionData(dwtInfo->dwtCompInfo[number]->dwtFunctionInfo,dataRead);
	
	return ERR_NOERROR;
}

int disable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno)
{
	uint32_t dataRead = 0;
	int status = 0,number = 0 ;

	
	return ERR_NOERROR;
}

int configure_DWTComparator(DWTInfo *dwtInfo,uint32_t address)
{
	
}

int configure_DWTMask(DWTInfo *dwtInfo,IgnoreMask ignoreMask)
{
	
}
/*
int configure_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno,int firstLinkComp,int secondLinkComp,ComparisonMode mode,DATAVSIZE size,int EMITRANGE,DWTFunction function)
{
	uint32_t dataToWrite = 0 ;
	int status = 0 ,number = 0 ;
	
	number = get_DWTComparatorInfoNumber(DWT_FUNCno);
	
	if(number == ERR_INVALID_DWTREGISTER);
		return number ;
	
	dataToWrite = get_DWTFunction_WriteValue(firstLinkComp,secondLinkComp,mode,size,EMITRANGE,function);
	
	memoryAccessWrite(DWT_FUNCno,dataToWrite);
	//status= memoryAccessRead(DWT_FUNCno,);
}*/