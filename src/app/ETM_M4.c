#include "ETM_M4.h"

/**
 *  Check whether is ETM locked
 *
 *  Output :  return Non-zero(2) if ETM is locked, writes are ignored
 *			      return Zero if writes to ETM are permitted
 */
int isETMLocked()
{
  int result = -1 ;
  uint32_t dataRead = 0 ;
  
  memoryReadWord((uint32_t)&(ETM->ETMLSR),&dataRead);
  
  return (dataRead & ETM_ETMLSR_ETMLOCKEDUNLOCKED_Msk);
}

/**
 *  Read and return ETM ID which holds the ETM architecture variant
 *
 *  Output : return ETM ID of the device
 *           return 0 if ETM is not present
 */
uint32_t getETMID()
{
  uint32_t etmid = 0;
  
  memoryReadWord((uint32_t)&(ETM->ETMIDR),&etmid);
  
  return etmid ;
}