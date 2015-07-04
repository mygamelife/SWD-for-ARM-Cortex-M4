#ifndef CoreDebug_H
#define CoreDebug_H

#include "Clock.h"
#include "Reset.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"

typedef enum 
{
	CORE_DEBUG_MODE,
	CORE_DEBUG_HALT,
	CORE_SINGLE_STEP,
	CORE_MASK_INTERRUPT,
	CORE_SNAP_STALL
}CoreControl ;

typedef enum
{
	STATUS_RESET,
	STATUS_RETIRED_INSTRUCTION,
	STATUS_LOCKEDUP,
	STATUS_SLEEPING,
	STATUS_HALTED 
}CoreStatus ;



#define DHCSR_REG     0xE000EDF0
#define DFSR_REG      0xE000ED30

#define SET_CORE_DEBUG 			0xA05F0001
#define SET_CORE_DEBUG_HALT 	0xA05F0003
#define SET_CORE_STEP			0xA05F0007
#define SET_CORE_MASKINT		0xA05F000B
#define SET_CORE_SNAPSTALL		0xA05F0023

#define CORE_DEBUG_MASK	 		0x00000001
#define CORE_DEBUG_HALT_MASK 	0x00020003
#define CORE_STEP_MASK			0x00000007
#define CORE_MASKINT_MASK		0x0002000B
#define CORE_SNAPSTALL_MASK		0x00020023

#define CORE_DEBUG_bit_SET		0x00000001
#define CORE_DEBUG_HALT_bit_SET	0x00020003
#define CORE_STEP_bit_SET		0x00000007
#define CORE_MASKINT_bit_SET	0x0002000B
#define CORE_SNAPSTALL_bit_SET	0x00020023
	
#define FALSE 0
#define TRUE 1

int setCore(CoreControl corecontrol);
int isCore(CoreControl corecontrol,uint32_t dataRead);
uint32_t get_Core_WriteValue(CoreControl corecontrol);

#endif // CoreDebug_H
