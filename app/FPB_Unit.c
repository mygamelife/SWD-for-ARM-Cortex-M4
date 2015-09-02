#include "FPB_Unit.h"


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
 *            return -1 if invalid comparator is chosen
 */
int manualSetInstructionBreakpoint(int instructionCOMPno,uint32_t instructionAddress,int matchingMode)
{
  uint32_t configData = 0 ;
  
  if(checkForValidInstructionComparator(instructionCOMPno) == -1)
    return -1 ;
  
  configData = (instructionAddress & FP_COMP_ADDRESS_MASK) +matchingMode + FP_COMP_ENABLE ;
  
  disableFPBUnit();
  
  memoryWriteWord((uint32_t)&(INSTRUCTION_COMP[instructionCOMPno]),configData);
  
  setCoreMode(CORE_DEBUG_MODE);
  enableFPBUnit();

  instructionComparatorReady[instructionCOMPno] = COMP_BUSY;
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
int manualSetInstructionRemapping(int instructionCOMPno,uint32_t instructionAddress, uint32_t remapAddress)
{
  uint32_t configData = 0 ;
  
  if(checkForValidInstructionComparator(instructionCOMPno) == -1)
    return -1 ;
  
  configData = (instructionAddress & FP_COMP_ADDRESS_MASK) + FP_COMP_ENABLE ;
  
  disableFPBUnit();
  
  memoryWriteWord((uint32_t)&(FPB->FP_REMAP),(remapAddress & FP_REMAP_ADDRESS_MASK));
  memoryWriteWord((uint32_t)&(INSTRUCTION_COMP[instructionCOMPno]),configData);
  
  setCoreMode(CORE_DEBUG_MODE);
  enableFPBUnit();
  
  instructionComparatorReady[instructionCOMPno] = COMP_BUSY;
  
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
int manualSetLiteralRemapping(int literalCOMPno,uint32_t literalAddress, uint32_t remapAddress)
{
  uint32_t configData = 0 ;
  
  if(checkForValidLiteralComparator(literalCOMPno) == -1)
    return -1 ;
  
  configData = (literalAddress & FP_COMP_ADDRESS_MASK) + FP_COMP_ENABLE ;
  
  disableFPBUnit();
  
  memoryWriteWord((uint32_t)&(FPB->FP_REMAP),(remapAddress & FP_REMAP_ADDRESS_MASK));
  memoryWriteWord((uint32_t)&(LITERAL_COMP[literalCOMPno]),configData);
  
  setCoreMode(CORE_DEBUG_MODE);
  enableFPBUnit();
  
  literalComparatorReady[literalCOMPno] = COMP_BUSY ;
  
  return 0 ;
}

/**
 *  Use to set for instruction address breakpoint ()
 *
 *  Input : instructionAddress is the address that will be breakpointed
 *          matchingMode defines the behaviour when the comparator is matched
 *          Possible value :
 *					  MATCH_LOWERHALFWORD	    Set breakpoint on lower halfword (Bits[1:0] are 0b00)			
 *					  MATCH_UPPERHALFWORD	    Set breakpoint on upper halfword (Bits[1:0] are 0b10)			
 *					  MATCH_WORD		          Set breakpoint on both upper and lower halfword						
 *
 *  Output :  return 0 if instruction breakpoint is set
 *            return -1 if invalid comparator is chosen
 */
int autoSetInstructionBreakpoint(uint32_t instructionAddress,int matchingMode)
{
  return manualSetInstructionBreakpoint(selectNextFreeComparator(INSTRUCTION_TYPE),instructionAddress,matchingMode);
}

/**
 *  Disable the selected Instruction Comparator
 * 
 *  Input : instructionCOMPno is the comparator number going to be disabled		
 *				  Possible values : 
 *					  INSTRUCTION_COMP0			Instruction comparator number 0	
 *					  INSTRUCTION_COMP1			Instruction comparator number 1	
 *					  INSTRUCTION_COMP2     Instruction comparator number 2
 *					  INSTRUCTION_COMP3     Instruction comparator number 3
 *					  INSTRUCTION_COMP4     Instruction comparator number 4
 *					  INSTRUCTION_COMP5     Instruction comparator number 5
 *
 *  Output :  return 0 if comparator is disabled
 *            return -1 if invalid comparator is chosen
 */
int disableInstructionComparator(int instructionCOMPno)
{
  if(checkForValidInstructionComparator(instructionCOMPno) == -1)
    return -1 ;
  
  memoryWriteWord((uint32_t)&(INSTRUCTION_COMP[instructionCOMPno]),FP_COMP_DISABLE);
  
  instructionComparatorReady[instructionCOMPno] = COMP_READY ;
  
  return 0 ;
}

/**
 *  Disable the selected Literal Comparator
 * 
 *  Input : literalCOMPno is the comparator number going to be disabled		
 *				  Possible values : 
 *					  LITERAL_COMP0			    Literal comparator number 0	
 *					  LITERAL_COMP1			    Litreal comparator number 1	
 *
 *  Output :  return 0 if comparator is disabled
 *            return -1 if invalid comparator is chosen
 */
int disableLiteralComparator(int literalCOMPno)
{
  if(checkForValidLiteralComparator(literalCOMPno) == -1)
    return -1 ;
  
  memoryWriteWord((uint32_t)&(LITERAL_COMP[literalCOMPno]),FP_COMP_DISABLE);
  
  literalComparatorReady[literalCOMPno] = COMP_READY ;
  
  return 0;
}

/**
 *  Read all instruction/literal comparator and update their respective ready flag
 *   
 *  Input : comparatorType is the to differentiate between instruction and literal comparator
 *				  Possible values : 
 *					  INSTRUCTION_TYPE			Instruction Comparator
 *					  LITERAL_TYPE			    Literal Comparator   
 */
void readAndUpdateComparatorReadyFlag(int comparatorType)
{
  uint32_t data = 0 ;
  uint32_t *compPtr ;
  int *compFlagPtr  ;
  int i = 0,max = 0 ;
  
  if(comparatorType == INSTRUCTION_TYPE)
  {
    max = INSTRUCTION_COMP_NUM;
    compPtr = (uint32_t *)&(INSTRUCTION_COMP[0]);
    compFlagPtr = &(instructionComparatorReady[0]);
  }
  else
  {
    max = LITERAL_COMP_NUM;
    compPtr = (uint32_t *)&(LITERAL_COMP[0]);
    compFlagPtr = &(literalComparatorReady[0]);
  }
 
  for(i = 0 ; i < max ; i ++)
  { 
      memoryReadWord((uint32_t)&(compPtr[i]),&data);
      compFlagPtr[i] = (int)(data & FPB_ENABLED_MASK);
  }
}


