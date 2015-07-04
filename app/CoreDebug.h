#ifndef CoreDebug_H
#define CoreDebug_H

#include "Clock.h"
#include "Reset.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"

typedef struct DebugEvent DebugEvent;
typedef struct CoreStatus CoreStatus ;

typedef enum 
{
	CORE_DEBUG_MODE,
	CORE_DEBUG_HALT,
	CORE_SINGLE_STEP,
	CORE_MASK_INTERRUPT,
	CORE_SNAP_STALL
}CoreControl ;

struct CoreStatus
{
	int S_RESET ;
	int S_RETIRE ;
	int S_LOCKUP ;
	int S_SLEEP ;
	int S_HALTED ;
	int S_REGRDY ;
};

struct DebugEvent
{
	int EXTERNAL ;
	int VCATCH ;
	int DWTTRAP ;
	int BKPT ;
	int HALTED ;
};


#define DHCSR_REG     0xE000EDF0
#define DFSR_REG      0xE000ED30

#define SET_CORE_DEBUG 			0xA05F0001
#define SET_CORE_DEBUG_HALT 	0xA05F0003
#define SET_CORE_STEP			0xA05F0007
#define SET_CORE_MASKINT		0xA05F000B
#define SET_CORE_SNAPSTALL		0xA05F0023

#define DHCSR_DEBUG_MASK	 	0x00000001
#define DHCSR_DEBUG_HALT_MASK 	0x00020003
#define DHCSR_STEP_MASK			0x00000007
#define DHCSR_MASKINT_MASK		0x0002000B
#define DHCSR_SNAPSTALL_MASK	0x00020023

#define DHCSR_DEBUG_bit_SET			0x00000001
#define DHCSR_DEBUG_HALT_bit_SET	0x00020003
#define DHCSR_STEP_bit_SET			0x00000007
#define DHCSR_MASKINT_bit_SET		0x0002000B
#define DHCSR_SNAPSTALL_bit_SET		0x00020023

#define DFSR_EXTERNAL_MASK	0x00000010
#define DFSR_VCATCH_MASK	0x00000008
#define DFSR_DWTTRAP_MASK	0x00000004
#define DFSR_BKPT_MASK		0x00000002
#define DFSR_HALTED_MASK	0x00000001

#define DFSR_EXTERNAL_bit_SET	0x00000010
#define DFSR_VCATCH_bit_SET		0x00000008
#define DFSR_DWTTRAP_bit_SET	0x00000004
#define DFSR_BKPT_bit_SET		0x00000002
#define DFSR_HALTED_bit_SET		0x00000001
	
#define FALSE 0
#define TRUE 1

int setCore(CoreControl corecontrol);
int isCore(CoreControl corecontrol,uint32_t dataRead);
uint32_t get_Core_WriteValue(CoreControl corecontrol);

void init_DebugEvent(DebugEvent *debugEvent);
void check_DebugEvent(DebugEvent *debugEvent);
void update_DebugEvent(DebugEvent *debugEvent, uint32_t dataRead);


#endif // CoreDebug_H
