#ifndef FPB_Utilities_H
#define FPB_Utilities_H

#include <stdint.h>

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

#define INSTRUCTION_COMP0	FP_COMP0
#define INSTRUCTION_COMP1	FP_COMP1
#define INSTRUCTION_COMP2	FP_COMP2
#define INSTRUCTION_COMP3	FP_COMP3
#define INSTRUCTION_COMP4	FP_COMP4
#define INSTRUCTION_COMP5	FP_COMP5

#define LITERAL_COMP0	FP_COMP6
#define LITERAL_COMP1	FP_COMP7

#define MATCH_REMAP					  0x00000000
#define MATCH_LOWERHALFWORD	 	0x40000000
#define MATCH_UPPERHALFWORD		0x80000000
#define MATCH_WORD				    0xC0000000

#define FP_COMP_ENABLE	        0x1
#define FP_COMP_DISABLE         0
      
#define FP_COMP_ADDRESS_MASK		0x1FFFFFFC
#define FP_COMP_MATCH_MASK			0xC0000000
#define FP_REMAP_ADDRESS_MASK		0x1FFFFFE0


#define FPB_ENABLED_MASK 			0x1

#define DISABLE_FPB 0x2
#define ENABLE_FPB 	0x3

int checkForValidInstructionComparator(uint32_t instructionCOMPno);
int checkForValidLiteralComparator(uint32_t literalCOMPno);
int checkForValidFPComparator(uint32_t compNo);

#endif // FPB_Utilities_H
