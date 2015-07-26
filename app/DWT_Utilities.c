#include "DWT_Utilities.h"

void init_DWTInfo(DWT_Info *dwtInfo)
{
	static DWT_Control dwtCtrl ;
	static DWT_8bitCounter cpi,exceptionOverhead,sleep,lsu,folderInstruction ;
	static DWT_32bitCounter cycle,pcSampling ;
	static DWT_ComparatorInfo dwtComp[4] ;
	static DWT_FunctionInfo dwtFunction[4];
	
	init_DWTControl(&dwtCtrl);
	init_DWT32bitCounter(&cycle);
	init_DWT8bitCounter(&cpi);
	init_DWT8bitCounter(&exceptionOverhead);
	init_DWT8bitCounter(&sleep);
	init_DWT8bitCounter(&lsu);
	init_DWT8bitCounter(&folderInstruction);
	init_DWT32bitCounter(&pcSampling);
	
	init_DWTComparatorInfo(&(dwtComp[0]),&(dwtFunction[0]));
	init_DWTComparatorInfo(&(dwtComp[1]),&(dwtFunction[1]));
	init_DWTComparatorInfo(&(dwtComp[2]),&(dwtFunction[2]));
	init_DWTComparatorInfo(&(dwtComp[3]),&(dwtFunction[3]));
	
	
	dwtInfo->dwtControl 			= &dwtCtrl;
	
	dwtInfo->CycleCount 			= &cycle;
	dwtInfo->CPICount 				= &cpi;
	dwtInfo->ExceptionOverheadCount = &exceptionOverhead ;
	dwtInfo->SleepCount 			= &sleep;
	dwtInfo->LSUCount 				= &lsu;
	dwtInfo->FoldedInstructionCount = &folderInstruction;
	dwtInfo->PCSample 				= &pcSampling;
	
	dwtInfo->dwtCompInfo[0] 		= &(dwtComp[0]) ;	
	dwtInfo->dwtCompInfo[1]			= &(dwtComp[1]) ;	
	dwtInfo->dwtCompInfo[2] 		= &(dwtComp[2]) ;	
	dwtInfo->dwtCompInfo[3] 		= &(dwtComp[3]) ;	
}

void init_DWTControl(DWT_Control *dwtControl)
{
	static EventStatus event ;
	
	init_EventStatus(&event);
	
	dwtControl->numberComparator 				= 4 ;
	dwtControl->support_Tracing 				= 0 ;
	dwtControl->support_ExternalTrigger 		= 0 ;
	dwtControl->support_CycleCount 				= 0 ;
	dwtControl->support_ProfilingCount			= 0 ;
	dwtControl->eventStatus 					= &event;
	dwtControl->syncTap				 			= Tap_Disabled;
	dwtControl->cycTap 							= 0 ;
	dwtControl->postCnt 						= 0;
	dwtControl->postReset 						= 0;
	dwtControl->EnableDisable_CycleCountCounter = Disable;
}

void init_EventStatus(EventStatus *eventStatus)
{
	eventStatus->CycleCountEvent  			= Disable ;
	eventStatus->FoldedInstructionCount  	= Disable ;
	eventStatus->LSUCount  					= Disable ;
	eventStatus->SleepCount  				= Disable ;
	eventStatus->ExceptionOverheadCount  	= Disable ;
	eventStatus->CPICount  					= Disable ;
	eventStatus->InterruptTracing  			= Disable ;
	eventStatus->PCSampling  				= Disable ;
}

void init_DWT8bitCounter(DWT_8bitCounter *counter)
{
	counter->previousValue = 0 ;
	counter->currentValue  = 0 ;
}

void init_DWT32bitCounter(DWT_32bitCounter *counter)
{
	counter->previousValue = 0 ;
	counter->currentValue  = 0 ;
}

void init_DWTComparatorInfo(DWT_ComparatorInfo *dwtCompInfo,DWT_FunctionInfo *dwtFunction)
{	
	init_DWTFunctionInfo(dwtFunction);

	dwtCompInfo->address 		 = 0 ;
	dwtCompInfo->ignoreMask 	 = Ignore_Non ;
	dwtCompInfo->dwtFunctionInfo = dwtFunction;
}

void init_DWTFunctionInfo(DWT_FunctionInfo *dwtFunctionInfo)
{
	dwtFunctionInfo->MATCHED 	= 0 ;
	dwtFunctionInfo->DATAVADDR1	= 0 ;
	dwtFunctionInfo->DATAVADDR0	= 0 ;
	dwtFunctionInfo->dataSize	= Byte ;
	dwtFunctionInfo->LNK1ENA 	= 0 ;
	dwtFunctionInfo->DATAVMATCH	= 0	;
	dwtFunctionInfo->CYCMATCH 	= 0 ;
	dwtFunctionInfo->EMITRANGE	= 0 ;
	dwtFunctionInfo->dwtFunction= DWTFunction_Disabled ;
}