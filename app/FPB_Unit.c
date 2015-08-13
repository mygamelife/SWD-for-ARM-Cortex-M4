#include "FPB_Unit.h"


#define MATCH_REMAP					  0x00000000
#define MATCH_LOWERHALFWORD	 	0x40000000
#define MATCH_UPPERHALFWORD		0x80000000
#define MATCH_WORD				    0xC0000000

/**
 *  Use to set for instruction address breakpoint
 *
 *  Input : instructionCOMPno is the instruction comparator going to be used
 *				  Possible values : 
 *					  INSTRUCTION_COMP0			Instruction comparator number 0	
 *					  INSTRUCTION_COMP1			Instruction comparator number 1	
 *					  INSTRUCTION_COMP2     Instruction comparator number 2
 *					  INSTRUCTION_COMP3     Instruction comparator number 3
 *					  INSTRUCTION_COMP4     Instruction comparator number 4
 *					  INSTRUCTION_COMP5     Instruction comparator number 5
 *
 *          instructionAddress is the address that will be breakpointed
 *          matchingMode defines the behaviour when the comparator is matched
 *          Possible value :
 *					  MATCH_LOWERHALFWORD	    Set breakpoint on lower halfword (Bits[1:0] are 0b00)			
 *					  MATCH_UPPERHALFWORD	    Set breakpoint on upper halfword (Bits[1:0] are 0b10)			
 *					  MATCH_WORD		          Set breakpoint on both upper and lower halfword						
 *
 *  Output :  return 0 if instruction breakpoint is set
 *            retunr -1 if invalid comparator is chosen
 */
int setInstructionBreakpoint(uint32_t instructionCOMPno,uint32_t instructionAddress,int matchingMode)
{
  uint32_t configData = 0 ;
  
  if(checkForValidInstructionComparator(instructionCOMPno) == -1)
    return -1 ;
  
  configData = (instructionAddress & FP_COMP_ADDRESS_MASK) +matchingMode + FP_COMP_ENABLE ;
  
  disableFPBUnit();
  
  memoryWriteWord(instructionCOMPno,configData);
  
  setCoreMode(CORE_DEBUG_MODE);
  enableFPBUnit();
  
  return 0 ;
}

/**
 *  Use to set for instruction address remapping
 *
 *  Input : instructionCOMPno is the instruction comparator going to be used
 *				  Possible values : 
 *					  INSTRUCTION_COMP0			Instruction comparator number 0	
 *					  INSTRUCTION_COMP1			Instruction comparator number 1	
 *					  INSTRUCTION_COMP2     Instruction comparator number 2
 *					  INSTRUCTION_COMP3     Instruction comparator number 3
 *					  INSTRUCTION_COMP4     Instruction comparator number 4
 *					  INSTRUCTION_COMP5     Instruction comparator number 5
 *
 *          instructionAddress is the address that will be remapped to the remap addresss + 4n (where n = 0,1,2,3,4,5 of INSTRUCTION_COMPn)
 *          remap address is the base address for remapping
 *
 *  Output :  return 0 if instruction remapping is set
 *            retunr -1 if invalid comparator is chosen
 */
int setInstructionRemapping(uint32_t instructionCOMPno,uint32_t instructionAddress, uint32_t remapAddress)
{
  uint32_t configData = 0 ;
  
  if(checkForValidInstructionComparator(instructionCOMPno) == -1)
    return -1 ;
  
  configData = (instructionAddress & FP_COMP_ADDRESS_MASK) + FP_COMP_ENABLE ;
  
  disableFPBUnit();
  
  memoryWriteWord(FP_REMAP,(remapAddress & FP_REMAP_ADDRESS_MASK));
  memoryWriteWord(instructionCOMPno,configData);
  
  setCoreMode(CORE_DEBUG_MODE);
  enableFPBUnit();
  
  return 0 ;
}

/**
 *  Use to set for literal address remapping
 *
 *  Input : literalCOMPno is the literal comparator going to be used
 *				  Possible values : 
 *					  LITERAL_COMP0			Literal comparator number 0	
 *					  LITERAL_COMP1			Litreal comparator number 1	
 *
 *          literalAddress is the address that will be remapped to the remap addresss + 0x24 + 4n (where n = 0,1 of LITERAL_COMPn)
 *          remap address is the base address for remapping
 *
 *  Output :  return 0 if literal remapping is set
 *            retunr -1 if invalid comparator is chosen
 */
int setLiteralRemapping(uint32_t literalCOMPno,uint32_t literalAddress, uint32_t remapAddress)
{
  uint32_t configData = 0 ;
  
  if(checkForValidLiteralComparator(literalCOMPno) == -1)
    return -1 ;
  
  configData = (literalAddress & FP_COMP_ADDRESS_MASK) + FP_COMP_ENABLE ;
  
  disableFPBUnit();
  
  memoryWriteWord(FP_REMAP,(remapAddress & FP_REMAP_ADDRESS_MASK));
  memoryWriteWord(literalCOMPno,configData);
  
  setCoreMode(CORE_DEBUG_MODE);
  enableFPBUnit();
  
  return 0 ;
}


/**
 *  Disable the selected FP Comparator 
 * 
 *  Input : fpCOMPno is the comparator number going to be disabled		
 *				  Possible values : 
 *					  INSTRUCTION_COMP0			Instruction comparator number 0	
 *					  INSTRUCTION_COMP1			Instruction comparator number 1	
 *					  INSTRUCTION_COMP2     Instruction comparator number 2
 *					  INSTRUCTION_COMP3     Instruction comparator number 3
 *					  INSTRUCTION_COMP4     Instruction comparator number 4
 *					  INSTRUCTION_COMP5     Instruction comparator number 5
 *
 *					  LITERAL_COMP0			Literal comparator number 0	
 *					  LITERAL_COMP1			Litreal comparator number 1	
 *
 *  Output :  return 0 if comparator is disabled
 *            retunr -1 if invalid comparator is chosen
 */
int disableFPComparator(uint32_t compNo)
{
  if(checkForValidFPComparator(compNo) == -1)
    return -1 ;
  
  memoryWriteWord(compNo,FP_COMP_DISABLE);
  
  return 0 ;
}

