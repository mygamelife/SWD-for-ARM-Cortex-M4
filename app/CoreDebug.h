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
	CORE_SNAPSTALL
}CoreControl ;

struct CoreStatus
{
	int S_RESET ;
	int S_RETIRE ;
	int S_LOCKUP ;
	int S_SLEEP ;
	int S_HALT ;
	int S_REGRDY ;
	int C_SNAPSTALL ;
	int C_MASKINTS ;
	int C_STEP ;
	int C_HALT ;
	int C_DEBUGEN ;
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

#define DHCSR_S_RESET_MASK		0x200000
#define DHCSR_S_RETIRE_MASK		0x100000
#define DHCSR_S_LOCKUP_MASK		0x70000
#define DHCSR_S_SLEEP_MASK		0x40000
#define DHCSR_S_HALT_MASK		0x20000
#define DHCSR_S_REGRDY_MASK		0x10000
#define DHCSR_C_SNAPSTALL_MASK	0x10
#define DHCSR_C_MASKINTS_MASK	0x7
#define DHCSR_C_STEP_MASK		0x4
#define DHCSR_C_HALT_MASK		0x2
#define DHCSR_C_DEBUGEN_MASK	0x1

#define DFSR_EXTERNAL_MASK	0x00000010
#define DFSR_VCATCH_MASK	0x00000008
#define DFSR_DWTTRAP_MASK	0x00000004
#define DFSR_BKPT_MASK		0x00000002
#define DFSR_HALTED_MASK	0x00000001
	
#define FALSE 0
#define TRUE 1

int checkIs_BitSet(uint32_t data,uint32_t dataMask);

int setCore(CoreControl corecontrol,CoreStatus *coreStatus);
int isCore(CoreControl corecontrol,CoreStatus *coreStatus);
uint32_t get_Core_WriteValue(CoreControl corecontrol);

void init_CoreStatus(CoreStatus *coreStatus);
void update_CoreStatus(CoreStatus *coreStatus, uint32_t dataRead);

void init_DebugEvent(DebugEvent *debugEvent);
void check_DebugEvent(DebugEvent *debugEvent);
void update_DebugEvent(DebugEvent *debugEvent, uint32_t dataRead);


#endif // CoreDebug_H
