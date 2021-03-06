#ifndef CoreDebug_Utilities_H
#define CoreDebug_Utilities_H

#include <stdint.h>
#include "MemoryReadWrite.h"
#include "stm32f429xx.h"
#include "core_cm4.h"

#define DHCSR_REG                 (uint32_t)&(CoreDebug->DHCSR)
#define DFSR_REG	                (uint32_t)&(SCB->DFSR)
#define DEMCR_REG	                (uint32_t)&(CoreDebug->DEMCR)
#define DCRSR_REG	                (uint32_t)&(CoreDebug->DCRSR)  
#define DCRDR_REG 	              (uint32_t)&(CoreDebug->DCRDR)

#define AIRCR_REG                 (uint32_t)&(SCB->AIRCR)

#define REQUEST_SYSTEM_RESET      0xFA050004
#define REQUEST_VECTOR_RESET      0xFA050001

#define CORE_REG_READ             0
#define CORE_REG_WRITE            0x10000

#define DFSR_EXTERNAL_MASK	      0x00000010
#define DFSR_VCATCH_MASK	        0x00000008
#define DFSR_DWTTRAP_MASK	        0x00000004
#define DFSR_BKPT_MASK		        0x00000002
#define DFSR_HALTED_MASK	        0x00000001

#define EXTERNAL_DEBUGEVENT       (1UL << SCB_DFSR_EXTERNAL_Pos)  
#define VCATCH_DEBUGEVENT         (1UL << SCB_DFSR_VCATCH_Pos) 
#define DWTTRAP_DEBUGEVENT        (1UL << SCB_DFSR_DWTTRAP_Pos) 
#define BKPT_DEBUGEVENT           (1UL << SCB_DFSR_BKPT_Pos) 
#define HALTED_DEBUGEVENT         (1UL << SCB_DFSR_HALTED_Pos)   

#define DEMCR_TRCENA_MASK		      ((uint32_t)0x01000000)
#define DEMCR_MON_REQ_MASK		    0x80000
#define DEMCR_MON_STEP_MASK		    0x40000
#define DEMCR_MON_PEND_MASK		    0x20000
#define DEMCR_MON_EN_MASK		      0x10000
#define DEMCR_VC_HARDERR_MASK	    0x400
#define DEMCR_VC_INTERR_MASK	    0x200
#define DEMCR_VC_BUSERR_MASK	    0x100
#define DEMCR_VC_STATERR_MASK	    0x80
#define DEMCR_VC_CHKERR_MASK	    0x40
#define DEMCR_VC_NOCPERR_MASK	    0x20
#define DEMCR_VC_MMERR_MASK		    0x10
#define DEMCR_VC_CORERESET_MASK	  0x1

#define VC_DISABLEALL             0
#define VC_CORERESET              (1UL << CoreDebug_DEMCR_VC_CORERESET_Pos)  
#define VC_MMERR                  (1UL << CoreDebug_DEMCR_VC_MMERR_Pos)
#define VC_NOCPERR                (1UL << CoreDebug_DEMCR_VC_NOCPERR_Pos)
#define VC_CHKERR                 (1UL << CoreDebug_DEMCR_VC_CHKERR_Pos) 
#define VC_STATERR                (1UL << CoreDebug_DEMCR_VC_STATERR_Pos) 
#define VC_BUSERR                 (1UL << CoreDebug_DEMCR_VC_BUSERR_Pos)
#define VC_INTERR                 (1UL << CoreDebug_DEMCR_VC_INTERR_Pos) 
#define VC_HARDERR                (1UL << CoreDebug_DEMCR_VC_HARDERR_Pos)

#define ENABLE_DWT_ITM	          1
#define DISABLE_DWT_ITM           0

typedef enum 
{
	CORE_NORMAL_MODE = 0xA05F0000,
	CORE_NORMAL_MASKINT = 0xA05F0008,
	CORE_DEBUG_MODE = 0xA05F0001,
	CORE_DEBUG_HALT = 0xA05F0003,
	CORE_SINGLE_STEP = 0xA05F0005,
	CORE_SINGLE_STEP_MASKINT = 0xA05F000D,
	CORE_SNAPSTALL = 0xA05F0023
} CoreMode;

typedef enum
{
	CORE_REG_R0 = 0,
	CORE_REG_R1,
	CORE_REG_R2,
	CORE_REG_R3,
	CORE_REG_R4,
	CORE_REG_R5,
	CORE_REG_R6,
	CORE_REG_R7,
	CORE_REG_R8,
	CORE_REG_R9,
	CORE_REG_R10,
	CORE_REG_R11,
	CORE_REG_R12,
	CORE_REG_SP,
	CORE_REG_LR,
	CORE_REG_PC,
	CORE_REG_xPSR,
	CORE_REG_MSP,
	CORE_REG_PSP,
	CORE_REG_SR, //19
	CORE_REG_FPSCR = 33,
	CORE_REG_FPREGS0 = 64,
	CORE_REG_FPREGS1 ,
	CORE_REG_FPREGS2 ,
	CORE_REG_FPREGS3 ,
	CORE_REG_FPREGS4 ,
	CORE_REG_FPREGS5 ,
	CORE_REG_FPREGS6 ,
	CORE_REG_FPREGS7 ,
	CORE_REG_FPREGS8 ,
	CORE_REG_FPREGS9 ,
	CORE_REG_FPREGS10 ,
	CORE_REG_FPREGS11 ,
	CORE_REG_FPREGS12 ,
	CORE_REG_FPREGS13 ,
	CORE_REG_FPREGS14 ,
	CORE_REG_FPREGS15 ,
	CORE_REG_FPREGS16 ,
	CORE_REG_FPREGS17 ,
	CORE_REG_FPREGS18 ,
	CORE_REG_FPREGS19 ,
	CORE_REG_FPREGS20 ,
	CORE_REG_FPREGS21 ,
	CORE_REG_FPREGS22 ,
	CORE_REG_FPREGS23 ,
	CORE_REG_FPREGS24 ,
	CORE_REG_FPREGS25 ,
	CORE_REG_FPREGS26 ,
	CORE_REG_FPREGS27 ,
	CORE_REG_FPREGS28 ,
	CORE_REG_FPREGS29 ,
	CORE_REG_FPREGS30 ,
	CORE_REG_FPREGS31 = 95
}CoreRegister;

#endif // CoreDebug_Utilities_H
