#ifndef Itm_H
#define Itm_H

#include <stdint.h>
#include "Tpiu.h"
#include "CException.h"
#include "ErrorCode.h"

enum {
  ITM_UNLOCK_FAILED   = 0,
  ITM_UNLOCK_SUCCESS  = 1,
} Itm_Status;

#define unlockItm()               memoryWriteWord((uint32_t)&ITM->LAR, 0xC5ACCE55)
#define itmControlReg(value)      memoryWriteWord((uint32_t)&ITM->TCR, value)

#define itmStimulusPort(__PORT__)           memoryWriteWord((uint32_t)&ITM->TER, __PORT__)
/* Unmask the selected stimulus port before user can use */
#define itmUnmaskStimulusPort(__UNMASK__)   memoryWriteWord((uint32_t)&ITM->TPR, __UNMASK__)

void itmInit(void);
int isItmUnlocked(void);

#endif // Itm_H
