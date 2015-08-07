#ifndef DWT_Utilities_H
#define DWT_Utilities_H

#include <stdint.h>
#include "Misc_Utilities.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"
#include "swd_Utilities.h"
#include "Register_ReadWrite.h"

#define DWT_CTRL		0xE0001000
#define DWT_CYCCNT		0xE0001004
#define DWT_CPICNT		0xE0001008
#define DWT_EXCCNT		0xE000100C
#define DWT_SLEEPCNT	0xE0001010
#define DWT_LSUCNT		0xE0001014
#define DWT_FOLDCNT		0xE0001018
#define DWT_PCSR		0xE000101C

#define DWT_COMP0 	0xE0001020
#define DWT_MASK0 	0xE0001024
#define DWT_FUNC0 	0xE0001028

#define DWT_COMP1 	0xE0001030
#define DWT_MASK1 	0xE0001034
#define DWT_FUNC1	0xE0001038

#define DWT_COMP2 	0xE0001040
#define DWT_MASK2 	0xE0001044
#define DWT_FUNC2	0xE0001048

#define DWT_COMP3 	0xE0001050
#define DWT_MASK3 	0xE0001054
#define DWT_FUNC3	0xE0001058

#define NUMCOMP_MASK		0xF0000000
#define NOTRCPKT_MASK		0x8000000
#define NOEXTTRIG_MASK		0x4000000
#define NOCYCCNT_MASK		0x2000000
#define NOPRFCNT_MASK		0x1000000
#define CYCEVTENA_MASK		0x400000
#define FOLDEVTENA_MASK 	0x200000
#define LSUEVTENA_MASK		0x100000
#define SLEEPEVTENA_MASK 	0x80000
#define EXCEVTENA_MASK		0x40000
#define CPIEVTENA_MASK		0x20000
#define EXCTRCENA_MASK		0x10000
#define PCSAMPLENA_MASK 	0x1000
#define SYNCTAP_MASK		0xC00
#define CYCTAP_MASK			0x200
#define POSTINIT_MASK		0x1E0
#define POSTRESET_MASK		0x1E
#define CYCCNTENA_MASK		0x1

#define _8bitCounter_MASK	0XFF

#define MATCHED_MASK		0x1000000
#define DATAVADDR1_MASK		0xF0000
#define DATAVADDR0_MASK		0xF000
#define DATAVSIZE_MASK		0xC00
#define LNK1ENA_MASK		0x200
#define DATAVMATCH_MASK		0x100
#define CYCMATCH_MASK		0x80
#define EMITRANGE_MASK		0x20
#define FUNCTION_MASK		0xF

#define DWTMASK_MASK		0xF
#define DWT_ENABLED_MASK	0x1

#define SUPPORTED 		0
#define NOT_SUPPORTED 	1

#define DWT_NOTMATCH	0
#define DWT_MATCH		1

#define DWT_DISABLE 0
#define DWT_ENABLE 1

typedef struct DWTInfo DWTInfo ;
typedef struct DWT_ComparatorInfo DWT_ComparatorInfo ;
typedef struct DWT_FunctionInfo DWT_FunctionInfo ;
typedef struct DWT_Control DWT_Control;
typedef struct DWT_32bitCounter DWT_32bitCounter ;
typedef struct DWT_8bitCounter DWT_8bitCounter ;
typedef struct DWT_EventStatus DWT_EventStatus;

typedef enum
{
	WATCHPOINT_MASK_NOTHING = 0 ,
	WATCHPOINT_MASK_BIT0 ,
	WATCHPOINT_MASK_BIT1_BIT0,
	WATCHPOINT_MASK_BIT2_BIT0,
	WATCHPOINT_MASK_BIT3_BIT0,
	WATCHPOINT_MASK_BIT4_BIT0,
	WATCHPOINT_MASK_BIT5_BIT0,
	WATCHPOINT_MASK_BIT6_BIT0,
	WATCHPOINT_MASK_BIT7_BIT0,
	WATCHPOINT_MASK_BIT8_BIT0,
	WATCHPOINT_MASK_BIT9_BIT0,
	WATCHPOINT_MASK_BIT10_BIT0,
	WATCHPOINT_MASK_BIT11_BIT0,
	WATCHPOINT_MASK_BIT12_BIT0,
	WATCHPOINT_MASK_BIT13_BIT0,
	WATCHPOINT_MASK_BIT14_BIT0,
}Watchpoint_AddressMask;

typedef enum
{
	DWTComparator_Disabled = 0 ,
	ITM_SampleEmit_PC = 1 ,
	ITM_Emit_AddressOffset = 100,
	ITM_Emit_DataRW = 2,
	ITM_Emit_Data_AddressOffset_RW = 200 ,
	ITM_Sample_PC_DataValue_RW = 3,
	ITM_Emit_AddressOffset_DataValue_RW = 300,
	Watchpoint_PCMatch  = 4,
	Watchpoint_Read = 5,
	Watchpoint_Write = 6,
	Watchpoint_ReadWrite = 7,
	ETM_Trigger_PCMatch = 8,
	ETM_Trigger_Read = 9,
	ETM_Trigger_Write = 10,
	ETM_Trigger_ReadWrite = 11,
	Sample_DataRead =12,
	Sample_DataAddressOffset_Read=1200,
	Sample_DataWrite =13,
	Sample_DataAddressOffset_Write=1300,
	Sample_PC_Data_Read =14,
	Sample_DataAddressOffset_Data_Read=1400,
	Sample_PC_Data_Write =15,
	Sample_DataAddressOffset_Data_Write=1500,
}DWTFunction;

typedef enum
{
	ADDRESS_COMPARISON = 0 ,
	DATA_COMPARISON ,
	CYCLECOUNT_COMPARISON
}Watchpoint_ComparisonMode;

typedef enum
{
	Tap_Disabled = 0,
	Tap_CYCCNTbit24 ,
	Tap_CYCCNTbit26 ,
	Tap_CYCCNTbit28 ,
}SyncTap;

typedef enum
{
	WATCHPOINT_BYTE = 0,
	WATCHPOINT_HALFWORD,
	WATCHPOINT_WORD
}Watchpoint_DataSize;

typedef enum
{
	CycleCount = 0 ,
	PCSample
}Enum32bitCounter;

typedef enum
{
	CPICount = 0,
	ExceptionOverheadCount,
	SleepCount,
	LSUCount,
	FoldedInstructionCount
}Enum8bitCounter;

struct DWTInfo
{
	int enableDWT;
	
	DWT_Control *dwtControl ;

	DWT_32bitCounter *CycleCount ;
	DWT_8bitCounter *CPICount ;
	DWT_8bitCounter *ExceptionOverheadCount ;
	DWT_8bitCounter *SleepCount ;
	DWT_8bitCounter *LSUCount ;
	DWT_8bitCounter *FoldedInstructionCount ;
	DWT_32bitCounter *PCSample ;

	DWT_ComparatorInfo *dwtCompInfo[4] ;
};

struct DWT_EventStatus
{
	int CycleCountEvent ;
	int FoldedInstructionCountEvent  ;
	int LSUCountEvent ;
	int SleepCountEvent  ;
	int ExceptionOverheadCountEvent  ;
	int CPICountEvent  ;
	int InterruptTracingEvent  ;
	int PCSamplingEvent  ;
};

struct DWT_8bitCounter
{
	int previousValue ;
	int currentValue ;
};


struct DWT_32bitCounter
{
	uint32_t previousValue ;
	uint32_t currentValue ;
};

struct DWT_Control
{
	int numberComparator;
	int support_Tracing;
	int support_ExternalTrigger;
	int support_CycleCount ;
	int support_ProfilingCount ;
	DWT_EventStatus *dwtEventStatus ;
	SyncTap syncTap ;
	int cycTap ;
	int postCnt;
	int postReset;
	int enableCycleCount;
};

struct DWT_FunctionInfo
{
	int comparatorMatched ;
	int secondLinkCompNo;
	int firstLinkCompNo ;
	Watchpoint_DataSize dataSize ;
	int secondLinkCompEnable ;
	int dataMatchingEnable ;
	int cycleMatchingEnable ;
	int enableTracePackets ;
	DWTFunction dwtFunction ;
};

struct DWT_ComparatorInfo
{
	uint32_t data ;
	Watchpoint_AddressMask addressMask ;
	DWT_FunctionInfo *dwtFunctionInfo ;
};

void init_DWTInfo(DWTInfo *dwtInfo);
void init_DWTControl(DWT_Control *dwtControl);
void init_DWTEventStatus(DWT_EventStatus *dwtEventStatus);
void init_DWT8bitCounter(DWT_8bitCounter *counter);
void init_DWT32bitCounter(DWT_32bitCounter *counter);
void init_DWTComparatorInfo(DWT_ComparatorInfo *dwtCompInfo,DWT_FunctionInfo *dwtFunction);
void init_DWTFunctionInfo(DWT_FunctionInfo *dwtFunctionInfo);

void configure_DWTEventGeneration(DWT_EventStatus *dwtEventStatus,int cycleCountEnable,int folderInstructionEnable,int LSUEnable,int sleepEnable,int exceptionOverheadEnable,int CPIEnable,int interruptTracingEnable,int PCSamplingEnable);

void process_DWTControlData(DWT_Control *dwtControl,uint32_t dataRead);
void process_DWTEventStatusData(DWT_EventStatus *dwtEventStatus,uint32_t dataRead);
void process_8bitCounterData(DWT_8bitCounter *counter,uint32_t dataRead,Enum8bitCounter counterEnum);
void process_32bitCounterData(DWT_32bitCounter *counter,uint32_t dataRead,Enum32bitCounter counterEnum);
void process_DWTComparatorData(DWT_ComparatorInfo *dwtCompInfo,uint32_t dataRead);
void process_DWTMaskData(DWT_ComparatorInfo *dwtCompInfo,uint32_t dataRead);
void process_DWTFunctionData(DWT_FunctionInfo *dwtFunctionInfo,uint32_t dataRead);

uint32_t get_DWTFunction_WriteValue(int firstLinkCompNo,int secondLinkCompNo,Watchpoint_ComparisonMode mode,Watchpoint_DataSize size,DWTFunction function);
uint32_t get_DWTControl_WriteValue(DWT_EventStatus *eventStatus,SyncTap syncTap,int cycTap,int posCnt,int postReset,int enableCycleCount);

int isDWT_EnabledDisabled(DWTInfo *dwtInfo,int enable);

int get_DWTComparatorInfoNumber(uint32_t address);

void equaliseLinkedComparator(uint32_t first_COMPno,uint32_t firstAddress,Watchpoint_AddressMask firstMask,int numberCOMP1,uint32_t *second_COMPno,uint32_t *secondAddress,Watchpoint_AddressMask *secondMask,int *numberCOMP2);


int control_DWT(DWTInfo *dwtInfo,DebugExceptionMonitor *debugExceptionMonitor,DebugTrap *debugTrap,DebugMonitorControl debugMonitorControl,int enable);

int read_DWTControl(DWTInfo *dwtInfo);
int read_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno);
int read_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno);
int read_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno);

int configure_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno,uint32_t data);
int configure_DWTMask(DWTInfo *dwtInfo,uint32_t DWT_MASKno,Watchpoint_AddressMask addressMask);
int configure_DWTFunction(DWTInfo *dwtInfo,uint32_t DWT_FUNCno,int firstLinkCompNo,int secondLinkCompNo,Watchpoint_ComparisonMode mode,Watchpoint_DataSize size,DWTFunction function);

int disable_DWTComparator(DWTInfo *dwtInfo,uint32_t DWT_COMPno);

int setup_DWTOperations(DWTInfo *dwtInfo,CoreStatus *coreStatus,DebugExceptionMonitor *debugExceptionMonitor,DebugMonitorControl debugMonitorControl);
int cleanup_DWTOperations(DWTInfo *dwtInfo,CoreStatus *coreStatus,DebugExceptionMonitor *debugExceptionMonitor);
#endif // DWT_Utilities_H