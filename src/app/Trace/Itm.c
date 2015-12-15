#include "Itm.h"

/* Initialize Instrumentation Trace Macrocell (ITM)
 */
void itmInit(void) {
	uint32_t readData = 0;
  /* Configure Trace Port Interface Unit TPIU 
     before using debug blocks */
  tpiuInit();
  
  // setCoreMode(CORE_NORMAL_MODE);
  
  memoryReadWord((uint32_t)&ITM->LSR, &readData);
  // printf("Before unlock %x\n", readData);
  
  /* Unlock write access to ITM register */
  memoryWriteWord((uint32_t)&ITM->LAR, 0xC5ACCE55);
  memoryReadWord((uint32_t)&ITM->LSR, &readData);
  // printf("After unlock %x\n", readData);
  
  /* Bit 2 = SYNCENA: this bit must be to 1 to enable the DWT to
     generate synchronization triggers so that the TPIU can then
     emit the synchronization packets.
     ATB ID is set to 0000001 (7bit) */
  // ITM->TCR = 0x00010005;
  memoryWriteWord((uint32_t)&ITM->TCR, 0x00010015);
  memoryReadWord((uint32_t)&ITM->TCR, &readData);
  // printf("ITM control reg %x\n", readData);
  
  /* Enable ITM Stimulus Port 0 */
  // ITM->TER = 0x1;
  memoryWriteWord((uint32_t)&ITM->TER, 0x1);
  memoryReadWord((uint32_t)&ITM->TER, &readData);
  // printf("ITM Trace Enable Register %x\n", readData);
  
  /* Unmask ITM stimulus ports 7:0 */
  // ITM->TPR = 0x1;
  memoryWriteWord((uint32_t)&ITM->TPR, 0x1);
  memoryReadWord((uint32_t)&ITM->TPR, &readData);
  // printf("ITM Trace Privilege Register %x\n", readData);
}
