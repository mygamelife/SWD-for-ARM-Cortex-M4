#include "Itm.h"

int isItmUnlocked(void) {
  uint32_t itmStatus = 0;

  memoryReadWord((uint32_t)&ITM->LSR, &itmStatus);

  if(itmStatus == 1)
    return ITM_UNLOCK_SUCCESS;

  return ITM_UNLOCK_FAILED;
}

/* Initialize Instrumentation Trace Macrocell (ITM)
 */
void itmInit(void) {
	uint32_t readData = 0;
  /* Configure Trace Port Interface Unit TPIU
     before using debug blocks */
  tpiuInit();

  /* Unlock write access to ITM register */
  unlockItm();

  if(isItmUnlocked() == ITM_UNLOCK_FAILED)
    Throw(ERR_ITM_UNLOCK_FAILED);

  /* Bit 2 = SYNCENA: this bit must be to 1 to enable the DWT to
     generate synchronization triggers so that the TPIU can then
     emit the synchronization packets.
     ATB ID is set to 0000001 (7bit) */
  itmControlReg(0x00010015);
  // memoryReadWord((uint32_t)&ITM->TCR, &readData);
  // printf("ITM control reg %x\n", readData);

  /* Enable ITM Stimulus Port 0 */
  itmStimulusPort(0x1);
  // memoryReadWord((uint32_t)&ITM->TER, &readData);
  // printf("ITM Trace Enable Register %x\n", readData);

  /* Unmask the selected stimulus port before user can use */
  itmUnmaskStimulusPort(0x1);
  // memoryReadWord((uint32_t)&ITM->TPR, &readData);
  // printf("ITM Trace Privilege Register %x\n", readData);
}