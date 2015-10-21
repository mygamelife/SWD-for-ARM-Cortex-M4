#include "CodeStepping.h"

/**
 *	Check whether the Stack Pointer is pointing to default location to be used to determine whether is it currently inside a  subroutine
 *	
 *  Output  : return 1 if TRUE
 *            return 0 if false
 */
int isStackPointerPointingToDefaultLocation()
{
  uint32_t sp = 0; 
  
  sp = readCoreRegister(CORE_REG_SP);
  if(sp == INITIALMAINCODE_SP)
    return 1 ;
  else
    return 0 ;
}

/**
 *	Check whether the selected address contains 32 bits instruction 
 *	
 *  Input   : address is the address that will be checked for the instruction
 *            machineCode is the machine code read in that address and will be used later
 *
 *  Output  : return 1 if TRUE
 *            return 0 if false
 */
int isSelectedAddressContains32bitsInstructionExtended(uint32_t address,uint32_t *machineCode)
{
  uint32_t dataRead = 0 ;
  
  memoryReadHalfword(address,&dataRead);
  *machineCode = dataRead ;

  dataRead = dataRead & ENCODING_32BIT_MASK ;
  
  if(dataRead == ENCODING_32BIT_CASE1 || dataRead == ENCODING_32BIT_CASE2 || dataRead == ENCODING_32BIT_CASE3)
    return 1;
  else 
    return 0;
}

/**
 *	Check whether the selected address contains 32 bits instruction 
 *	
 *  Input   : address is the address that will be checked for the instruction
 *
 *  Output  : return 1 if TRUE
 *            return 0 if false
 */
int isSelectedAddressContains32bitsInstruction(uint32_t address)
{
  uint32_t dummy = 0 ;
  
  return isSelectedAddressContains32bitsInstructionExtended(address,&dummy);
}

/**
 *	Check whether the selected address contains calling subroutine instruction
 *	
 *  Input   : address is the address that will be checked for the instruction
 *  Output  : return 1 if TRUE
 *            return 0 if false
 */
int isSelectedAddressContainsCallingSubroutineInstruction(uint32_t address)
{
  uint32_t machineCode = 0 ;
  
  memoryReadWord(address,&machineCode);
  machineCode = machineCode & BL_INSTRUCTION_MASK ;
  
  if(machineCode == BL_ENCODING)
    return 1 ;
  
  return 0 ;
}

/**
 *	Check whether the selected machine code mateches return from subroutine instruction
 *	
 *  Input   : machineCode is the machine code that will be checked
 *  Output  : return 1 if TRUE
 *            return 0 if false
 */
int isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(uint32_t machineCode)
{
  if(machineCode == POP_R7andLR_MACHINECODE || machineCode == BX_LR_MACHINECODE || machineCode == MOV_LRtoPC_MACHINECODE)
    return 1 ;

  return 0 ;
}

/**
 *  Perform step into function once
 *
 *  Output : return PC after stepped into
 */
uint32_t stepIntoOnce()
{
  stepOnly(1);
  
  return(readCoreRegister(CORE_REG_PC));
}

/**
 *  Perform step over an instruction
 *
 *  Output : return PC after stepped over if successful
 *           return 0 if failed to step over due to insufficient amount of HW comparator 
 *
 */
uint32_t stepOver()
{
  uint32_t pc = 0 ,unusedData = 0 ;
  int comparatorUsed = 0 ;
  
  pc = readCoreRegister(CORE_REG_PC);

  if(isSelectedAddressContains32bitsInstructionExtended(pc,&unusedData))
  {
    if(isSelectedAddressContainsCallingSubroutineInstruction(pc))
    {
      comparatorUsed = autoSetInstructionBreakpoint(pc + 4, MATCH_WORD);
      if(comparatorUsed == -1)
        return 0 ;
    
      while(!(hasBreakpointDebugEventOccured()));

      disableInstructionComparator(comparatorUsed);
      clearBreakpointDebugEvent();
    
      return pc + 4 ;
    }
  }
  
  return(stepIntoOnce());
}

/**
 *  Perform step out of an subroutine
 *
 *  Output : return PC after stepped over if successful
 *           return 0 if failed to step over due to insufficient amount of HW comparator or possibly not inside a subroutine
 *
 */
uint32_t stepOut()
{
  uint32_t pc = 0 ,machineCode = 0 ;
  int found = 0 ;
  int comparatorUsed = 0 ;
  
  if(isStackPointerPointingToDefaultLocation())
    return 0 ;

  pc = readCoreRegister(CORE_REG_PC);
  while(!found)
  { 
    if(isSelectedAddressContains32bitsInstructionExtended(pc,&machineCode))
      pc +=4 ;
    else //16bit instruction
    {
      if(isSelectedMachineCodeMatchesReturnFromSubroutineInstruction(machineCode))
        found = 1;   
      else
        pc += 2 ;
    }
  }
  
  comparatorUsed = autoSetInstructionBreakpoint(pc,MATCH_WORD);
  if(comparatorUsed == -1)
    return 0 ;
    
  while(!(hasBreakpointDebugEventOccured()));
  disableInstructionComparator(comparatorUsed);
  clearBreakpointDebugEvent();
 
  return(stepIntoOnce());
}
