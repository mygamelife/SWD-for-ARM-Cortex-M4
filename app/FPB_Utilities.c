#include "FPB_Utilities.h"


int checkForValidInstructionComparator(uint32_t instructionCOMPno)
{
  int result = - 1;
  
  switch(instructionCOMPno)
  {
    case INSTRUCTION_COMP0  :
                              result = 1;
                              break ;
    case INSTRUCTION_COMP1  :
                              result = 1;
                              break ;
    case INSTRUCTION_COMP2  :
                              result = 1;
                              break ;
    case INSTRUCTION_COMP3  :
                              result = 1;
                              break ;
    case INSTRUCTION_COMP4  :
                              result = 1;
                              break ;
    case INSTRUCTION_COMP5  :
                              result = 1;
                              break ;
    default                 :
                              break ;
  }
  
  return result ;
}

int checkForValidLiteralComparator(uint32_t literalCOMPno)
{
  int result = - 1;
  
  if(literalCOMPno == LITERAL_COMP0 || literalCOMPno == LITERAL_COMP1)
    result = 1 ;
  
  return result ;
}

int checkForValidFPComparator(uint32_t compNo)
{
   int result = - 1;
  
  switch(compNo)
  {
    case FP_COMP0  :
                     result = 1;
                     break ;
    case FP_COMP1  :
                     result = 1;
                     break ;
    case FP_COMP2  :
                     result = 1;
                     break ;
    case FP_COMP3  :
                     result = 1;
                     break ;
    case FP_COMP4  :
                     result = 1;
                     break ;                 
    case FP_COMP5  :
                     result = 1;
                     break ;   
    case FP_COMP6  :
                     result = 1;
                     break ;
    case FP_COMP7  :
                     result = 1;
                     break ;                  
    default        :
                     break ;
  }
  
  return result ;
}

