#include "FPB_Utilities.h"
int instructionComparatorReady[INSTRUCTION_COMP_NUM] = {};
int literalComparatorReady[LITERAL_COMP_NUM] = {};

int checkForValidInstructionComparator(int instructionCOMPno)
{
  int result = - 1;
  
  if(instructionCOMPno >= 0 && instructionCOMPno <= 5)
    result = 1;
  
  return result ;
}

int checkForValidLiteralComparator(int literalCOMPno)
{
  int result = - 1;
  
  if(literalCOMPno == 0 || literalCOMPno == 1)
    result = 1 ;
  
  return result ;
}

/**
 *  Use to select the next free/available instruction comparator to use
 *
 *  Output : return INSTRUCTION_COMP0 - INSTRUCTION_COMP5 if any one of them is free and INSTRUCTION_TYPE is chosen
 *           return LITERAL_COMP0     - LITERAL_COMP1 if any one of them is free and LITERAL_TYPE is chosen
 *           return -1 if all the comparators are busy
 *
 */
uint32_t selectNextFreeComparator(int comparatorType)
{
  int *compFlagPtr ;
  int nextComparator = 0 ;
  int max = 0 ;
  
  readAndUpdateComparatorReadyFlag(comparatorType);
  
  if(comparatorType == INSTRUCTION_TYPE)
  {
    max = INSTRUCTION_COMP_NUM;
    compFlagPtr = &(instructionComparatorReady[0]);
  }
  else
  {
    max = LITERAL_COMP_NUM;
    compFlagPtr = &(literalComparatorReady[0]);
  }
  
  for(nextComparator = 0 ; nextComparator < max ; nextComparator++)
  {
    if(compFlagPtr[nextComparator] == COMP_READY)
      return nextComparator;
  }

  return -1 ;
}
