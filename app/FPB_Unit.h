#ifndef FPB_Unit_H
#define FPB_Unit_H

#include "swd_Utilities.h"
#include "Register_ReadWrite.h"
#include "FPB_Utilities.h"
#include "CoreDebug.h"
#include "CoreDebug_Utilities.h"

#define InstructionCOMP_0	FP_COMP0
#define InstructionCOMP_1	FP_COMP1
#define InstructionCOMP_2	FP_COMP2
#define InstructionCOMP_3	FP_COMP3
#define InstructionCOMP_4	FP_COMP4
#define InstructionCOMP_5	FP_COMP5

#define LiteralCOMP_0	FP_COMP6
#define LiteralCOMP_1	FP_COMP7

int control_FPB(FPBInfo *fpbInfo,int EnableDisable);
int read_FPBControl(FPBInfo *fpbInfo);

int configure_FP_COMP(FPBInfo *fpbInfo,uint32_t COMP_no,uint32_t address,int matchingMode,int EnableDisable);
int configure_FP_REMAP(FPBInfo *fpbInfo,uint32_t SRAM_REMAP_address);

int set_InstructionBKPT(FPBInfo *fpbInfo,uint32_t InstructionCOMP_no,uint32_t address,int matchingMode);
int set_InstructionREMAP(FPBInfo *fpbInfo,uint32_t InstructionCOMP_no,uint32_t address);
int set_LiteralREMAP(FPBInfo *fpbInfo,uint32_t LiteralCOMP_no,uint32_t address);

int disable_FPComp(FPBInfo *fpbInfo,uint32_t COMP_no);
int reenable_FPComp(FPBInfo *fpbInfo,uint32_t COMP_no);

int prepare_FPBOperations(FPBInfo *fpbInfo,CoreStatus *coreStatus,DebugExceptionMonitor *debugExceptionMonitor,DebugTrap *debugTrap,DebugMonitorControl debugMonitorControl);

#endif // FPB_Unit_H
