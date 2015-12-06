#include "ETM.h"

int initETM()
{
  unlockETM();
}


/**
 *  Check whether is ETM locked
 *
 *  Output :  return 1 if ETM is locked, writes are ignored
 *			      return 0 if writes to ETM are permitted
 */
int isETMLocked()
{
  uint32_t dataRead = 0 ;
  
  memoryReadWord((uint32_t)&(ETM->ETMLSR),&dataRead);
  
  return (dataRead & ETM_ETMLSR_ETMLOCKEDUNLOCKED_Msk)
}