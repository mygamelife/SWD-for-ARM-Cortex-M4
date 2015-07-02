#ifndef FPB_Unit_H
#define FPB_Unit_H

#include "Reset.h"
#include "swd_Utilities.h"
#include "configurePort.h"
#include "Bit_Operations.h"
#include "Register_ReadWrite.h"

#define FPB_REG 	0xE0002000
#define FP_COMP0	0xE0002008
#define FP_COMP1	0xE000200C
#define FP_COMP2	0xE0002010
#define FP_COMP3	0xE0002014
#define FP_COMP4	0xE0002018
#define FP_COMP5	0xE000201C
#define FP_COMP6	0xE0002020
#define FP_COMP7	0xE0002024

#define InstructionCOMP_0	FP_COMP0
#define InstructionCOMP_1	FP_COMP1
#define InstructionCOMP_2	FP_COMP2
#define InstructionCOMP_3	FP_COMP3
#define InstructionCOMP_4	FP_COMP4
#define InstructionCOMP_5	FP_COMP5

#define LiteralCOMP_0	FP_COMP6
#define LiteralCOMP_1	FP_COMP7

#define Match_REMAP					0x00000000
#define Match_Lower16bits	 		0x40000000
#define Match_Upper16bits			0x80000000
#define Match_32bits				0xC0000000

#define FP_COMP_Address_MASK		0x1FFFFFFC
#define FP_COMP_Enable				1
#define FP_REMAP_Address_MASK		0x1FFFFFC0

#define ENABLE_FPB 0x00000003
#define FPB_MASK 1
#define TRUE 1
#define FALSE 0

int FPB_Enable();
int configure_FP_COMP(uint32_t COMP_no,uint32_t address,int Matching_mode);
int configure_FP_REMAP(uint32_t SRAM_REMAP_address);
uint32_t get_FP_COMP_WriteValue(uint32_t address,int Matching_mode);

int isFPB_Enabled(uint32_t data);
int isFP_COMP_Updated(uint32_t dataWrote,uint32_t dataRead);
#endif // FPB_Unit_H
