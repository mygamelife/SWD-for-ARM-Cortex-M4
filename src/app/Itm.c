#include "Itm.h"

/* Initialize Instrumentation Trace Macrocell (ITM)
 */
void itmInit(void) {
	uint32_t readData = 0;
  /* Configure Trace Port Interface Unit TPIU 
     before using debug blocks */
  tpiuInit();
  
  /* Unlock write access to ITM register */
  memoryWriteWord((uint32_t)&ITM->LAR, 0xC5ACCE55);
  memoryReadWord((uint32_t)&ITM->LSR, &readData);
  
  /* Bit 2 = SYNCENA: this bit must be to 1 to enable the DWT to
     generate synchronization triggers so that the TPIU can then
     emit the synchronization packets.
     ATB ID is set to 0000001 (7bit) */
  // ITM->TCR = 0x00010005;
  memoryWriteWord((uint32_t)&ITM->TCR, 0x00010005);
  
  /* Enable ITM Stimulus Port 0 */
  // ITM->TER = 0x1;
  memoryWriteWord((uint32_t)&ITM->TER, 0x1);
  
  /* Unmask ITM stimulus ports 7:0 */
  // ITM->TPR = 0x1;
  memoryWriteWord((uint32_t)&ITM->TPR, 0x1);
}

void itmInitx(void) {
	uint32_t readData = 0;
  /* Configure Trace Port Interface Unit TPIU 
     before using debug blocks */
  
  memoryReadWord((uint32_t)&ITM->LSR, &readData);
  printf("before unlock %x\n", readData);
  /* Unlock write access to ITM register */
  memoryWriteWord((uint32_t)&ITM->LAR, 0xC5ACCE55);
  memoryReadWord((uint32_t)&ITM->LSR, &readData);
  printf("after unlock %x\n", readData);
  
  tpiuInitx();
  /* Bit 2 = SYNCENA: this bit must be to 1 to enable the DWT to
     generate synchronization triggers so that the TPIU can then
     emit the synchronization packets.
     ATB ID is set to 0000001 (7bit) */
  /* Enable SWOENA and disable SYNCENA */
  memoryWriteWord((uint32_t)&ITM->TCR, 0x00010011);
  
  /* Enable ITM Stimulus Port 0 */
  // ITM->TER = 0x1;
  memoryWriteWord((uint32_t)&ITM->TER, 0x1);
  
  /* Unmask ITM stimulus ports 7:0 */
  memoryWriteWord((uint32_t)&ITM->TPR, 0x1);
}