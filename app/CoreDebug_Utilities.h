#ifndef CoreDebug_Utilities_H
#define CoreDebug_Utilities_H

#include <stdint.h>
#include "Misc_Utilities.h"

#define SET_CORE_NORMAL			0xA05F0000
#define SET_CORE_DEBUG 			0xA05F0001
#define SET_CORE_DEBUG_HALT 	0xA05F0003
#define SET_CORE_STEP			0xA05F0005
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

#define DEMCR_VC_HARDERR_MASK	0x300
#define DEMCR_VC_INTERR_MASK	0x200
#define DEMCR_VC_BUSERR_MASK	0x100
#define DEMCR_VC_STATERR_MASK	0x80
#define DEMCR_VC_CHKERR_MASK	0x40
#define DEMCR_VC_NOCPERR_MASK	0x20
#define DEMCR_VC_MMERR_MASK		0x10
#define DEMCR_VC_CORERESET_MASK	0x1

#define FALSE 0
#define TRUE 1

#define ENABLE_DWT_ITM	1
#define DISABLE_DWT_ITM 0

#define CoreRegister_Read 0
#define CoreRegister_Write 1

typedef struct DebugEvent DebugEvent ;
typedef struct CoreStatus CoreStatus  ;
typedef struct DebugTrap DebugTrap ;
typedef struct DebugMonitorStatus DebugMonitorStatus ;

typedef enum 
{
	CORE_NORMAL_MODE,
	CORE_DEBUG_MODE,
	CORE_DEBUG_HALT,
	CORE_SINGLE_STEP,
	CORE_MASK_INTERRUPT,
	CORE_SNAPSTALL
}CoreControl ;

typedef enum 
{
	DebugMonitor_DISABLE,
	DebugMonitor_ENABLE,
	DebugMonitor_STEP
}DebugMonitorControl ;

typedef enum
{
	Halting_DebugMode,
	Monitor_DebugMode,
}DebugMode ;

typedef enum
{
	ARM_R0 = 0,
	ARM_R1,
	ARM_R2,
	ARM_R3,
	ARM_R4,
	ARM_R5,
	ARM_R6,
	ARM_R7,
	ARM_R8,
	ARM_R9,
	ARM_R10,
	ARM_R11,
	ARM_R12,
	ARM_SP,
	ARM_LR,
	DebugReturnAddress,
	ARM_xPSR,
	MainStackPointer,
	ProcessStackPointer,
	SpecialRegister,
	FPSCR = 33,
	FP_REGS0 = 64,
	FP_REGS1 ,
	FP_REGS2 ,
	FP_REGS3 ,
	FP_REGS4 ,
	FP_REGS5 ,
	FP_REGS6 ,
	FP_REGS7 ,
	FP_REGS8 ,
	FP_REGS9 ,
	FP_REGS10 ,
	FP_REGS11 ,
	FP_REGS12 ,
	FP_REGS13 ,
	FP_REGS14 ,
	FP_REGS15 ,
	FP_REGS16 ,
	FP_REGS17 ,
	FP_REGS18 ,
	FP_REGS19 ,
	FP_REGS20 ,
	FP_REGS21 ,
	FP_REGS22 ,
	FP_REGS23 ,
	FP_REGS24 ,
	FP_REGS25 ,
	FP_REGS26 ,
	FP_REGS27 ,
	FP_REGS28 ,
	FP_REGS29 ,
	FP_REGS30 ,
	FP_REGS31 
}Core_RegisterSelect;

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

struct DebugTrap
{
	int VC_HARDERR;
	int VC_INTERR;
	int VC_BUSERR;
	int VC_STATERR;
	int VC_CHKERR;
	int VC_NOCPERR;
	int VC_MMERR;
	int VC_CORERESET;
};

struct DebugMonitorStatus
{
	int MON_REQ;
	int MON_STEP;
	int MON_PEND;
	int MON_EN;
};

int isCore(CoreControl coreControl,CoreStatus *coreStatus);

void init_CoreStatus(CoreStatus *coreStatus);
void init_DebugEvent(DebugEvent *debugEvent);
void init_DebugTrap(DebugTrap *debugTrap);

void update_CoreStatus(CoreStatus *coreStatus, uint32_t dataRead);
void update_DebugEvent(DebugEvent *debugEvent, uint32_t dataRead);
void update_DebugTrapStatus(DebugTrap *debugTrap, uint32_t dataRead);




uint32_t get_Core_WriteValue(CoreControl coreControl);
uint32_t get_CoreRegisterAccess_WriteValue(Core_RegisterSelect coreRegister,int CoreRegister_ReadWrite);
uint32_t get_DebugExceptionMonitorControl_WriteValue(DebugMonitorControl debugMonitorControl,DebugTrap *debugTrap,int enable_DWT_ITM);
uint32_t get_ClearDebugEvent_WriteValue(DebugEvent *debugEvent);

#endif // CoreDebug_Utilities_H
