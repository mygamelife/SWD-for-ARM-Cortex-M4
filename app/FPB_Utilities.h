#ifndef FPB_Utilities_H
#define FPB_Utilities_H

#include <stdint.h>
#include <string.h>
#include "Misc_Utilities.h"

typedef struct FPBInfo FPBInfo ;
typedef struct ComparatorInfo ComparatorInfo ;

struct FPBInfo
{
	int numberLiteralCOMP ;
	int numberInstructionCOMP ;
	int inUse_InstructionCOMP ;
	int inUse_LiteralCOMP ;
	
	int EnableDisable ;
	
	uint32_t SRAM_REMAP_address ;
	
	ComparatorInfo *compInfo[8] ;
};

struct ComparatorInfo
{
	int comparatorNumber ;
	char comparatorType[20];
	uint32_t address ;
	int EnableDisable ;
	uint32_t matchingMode ;
};

#define FP_CTRL 	0xE0002000
#define FP_REMAP	0xE0002004
#define FP_COMP0	0xE0002008
#define FP_COMP1	0xE000200C
#define FP_COMP2	0xE0002010
#define FP_COMP3	0xE0002014
#define FP_COMP4	0xE0002018
#define FP_COMP5	0xE000201C
#define FP_COMP6	0xE0002020
#define FP_COMP7	0xE0002024

#define Match_REMAP					0x00000000
#define Match_Lower16bits	 		0x40000000
#define Match_Upper16bits			0x80000000
#define Match_32bits				0xC0000000

#define FP_COMP_ADDRESS_MASK		0x1FFFFFFC
#define FP_COMP_MATCH_MASK			0xC0000000
#define FP_COMP_ENABLE_MASK			1
#define FP_COMP_ENABLE				1
#define FP_REMAP_Address_MASK		0x1FFFFFE0
#define FP_REMAP_ReadAddress_MASK	0x3FFFFFE0

#define FPB_ENABLED_MASK 			0x1
#define FPB_NUMLIT_MASK				0xF00
#define FPB_NUMINSTRUCTION_MASK		0xF0

#define DISABLE_FPB 0x2
#define ENABLE_FPB 	0x3

	
#define Enable 1 
#define Disable 0

void init_FPBInfo(FPBInfo *fpbInfo);
void init_ComparatorInfo(ComparatorInfo *compInfo,int compNo,char *compType,uint32_t address,int EnableDisable,int matchingMode);
void update_ComparatorInfo(ComparatorInfo *compInfo,uint32_t address,int EnableDisable,uint32_t matchingMode);

void process_FPControlData(FPBInfo *fpbInfo,uint32_t dataRead);
void process_FPComparatorData(ComparatorInfo *compInfo,uint32_t dataRead);
void process_FPRemapData(FPBInfo *fpbInfo,uint32_t dataRead);

int isFPB_EnabledDisabled(FPBInfo *fpbInfo,int EnableDisable);

int isFP_COMP_Updated(uint32_t dataWrote,uint32_t dataRead);
int isFP_REMAP_Updated(uint32_t dataWrote,uint32_t dataRead);
int isFP_COMP_Updated(uint32_t dataWrote,uint32_t dataRead);

uint32_t get_FP_CTRL_WriteValue(int EnableDisable);
uint32_t get_FP_COMP_WriteValue(uint32_t address,int matchingMode);
uint32_t get_FP_REMAP_WriteValue(uint32_t address);
int get_ComparatorInfoNumber(uint32_t COMP_no);





#endif // FPB_Utilities_H
