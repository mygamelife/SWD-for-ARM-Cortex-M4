#ifndef DWT_Utilities_H
#define DWT_Utilities_H

#include <stdint.h>

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

#define NOT_SUPPORTED 	0
#define SUPPORTED 		1

typedef struct DWTInfo DWTInfo ;
typedef struct DWT_ComparatorInfo DWTComparatorInfo ;
typedef struct DWT_FunctionInfo DWTFunctionInfo ;
typedef struct DWTControl DWTControl
typedef struct DWT_32bitCounter DWT_32bitCounter ;
typedef struct DWT_8bitCounter DWT_8bitCounter ;

typedef enum
{
	Ignore_Non = 0 ,
	Ignore_Bit0 ,
	Ignore_Bit1_Bit0,
	Ignore_Bit2_Bit0,
	Ignore_Bit3_Bit0,
	Ignore_Bit4_Bit0,
	Ignore_Bit5_Bit0,
	Ignore_Bit6_Bit0,
	Ignore_Bit7_Bit0,
	Ignore_Bit8_Bit0,
	Ignore_Bit9_Bit0,
	Ignore_Bit10_Bit0,
	Ignore_Bit11_Bit0,
	Ignore_Bit12_Bit0,
	Ignore_Bit13_Bit0,
	Ignore_Bit14_Bit0,
}IgnoreMask;

typedef enum
{
	Disabled = 0 ,
	ITM_SampleEmit_PC = 1 ,
	ITM_Emit_AddressOffset = 100,
	ITM_Emit_DataRW = 2,
	ITM_Emit_Data_AddressOffset_RW = 200 ,
	ITM_Sample_PC_DataValue_RW = 3,
	ITM_Emit_AddressOffset_DataValue_RW = 300,
	Watchpoint_PCMatch  = 4,
	Watchpoint_DataRead = 5,
	Watchpoint_DataWrite = 6,
	Watchpoint_DataReadWrite = 7,
	ETM_Trigger_PCMatch = 8,
	ETM_Trigger_DataRead = 9,
	ETM_Trigger_DataWrite = 10,
	ETM_TriggerDataReadWrite = 11,
	Sample_DataRead =12,
	Sample_DataAddressOffset_Read=1200,
	Sample_DataWrite =13,
	Sample_DataAddressOffset_Write=1300,
	Sample_PC_Data_Read =14,
	Sample_DataAddressOffset_Data_Read=1400,
	Sample_PC_Data_Write =15,
	Sample_DataAddressOffset_Data_Write=1500,
}DWTFunction;

struct DWTInfo
{
	DWTControl dwtControl ;
	
	DWT_32bitCounter CycleCount
	DWT_8bitCounter CPICount
	DWT_8bitCounter ExceptionOverheadCount
	DWT_8bitCounter SleepCount
	DWT_8bitCounter LSUCount
	DWT_8bitCounter FoldedInstructionCount
	DWT_32bitCounter PCSample
	
	DWTComparatorInfo *dwtCompInfo[4] ;	
}


struct DWTControl 
{
	int numberComparator;
	int support_Tracing;
	int support_ExternalTrigger;
	int support_CycleCount ;
	int support_ProfilingCount ;
	int EnableDisable_CycleCountEvent ;
	int EnableDisable_FoldedInstructionCount ;
	int EnableDisable_LSUCount ;
	int EnableDisable_SleepCount ;
	int EnableDisable_ExceptionOverheadCount ;
	int EnableDisable_CPICount ;
	int EnableDisable_InterruptTracing ;
	int EnableDisable_PCSampling ;
	int syncTap ;
	int cycTap ;
	int postCnt
	int postReset
	int EnableDisable_CycleCountCounter;
}

struct DWT_8bitCounter
{
	int previousValue ;
	int currentValue ;
}


struct DWT_32bitCounter
{
	uint32_t previousValue ;
	uint32_t currentValue ;
}

struct DWT_ComparatorInfo
{
	uint32_t address ;
	IgnoreMask ignoreMask ;
	DWTFunctionInfo functionInfo ;
}

struct DWT_FunctionInfo
{
	int MATCHED ;
	int DATAVADDR1;
	int DATAVADDR0 ;
	int DATAVSIZE ;
	int LNK1ENA ;
	int DATAVMATCH ;
	int CYCMATCH ;
	int EMITRANGE
	DWTFunction dwtFunction ;
}



#endif // DWT_Utilities_H
