#include "Itm.h"

/* Initialize Instrumentation Trace Macrocell (ITM)
 */
void itmConfig(void) {
  /* Configure Trace Port Interface Unit TPIU 
     before using debug blocks */
  tpiuConfig();
  
  /* Unlock write access to ITM register */
  ITM->LAR = 0xC5ACCE55;
  
  /* Bit 2 = SYNCENA: this bit must be to 1 to enable the DWT to
     generate synchronization triggers so that the TPIU can then
     emit the synchronization packets.
     ATB ID is set to 0000001 (7bit) */
  ITM->TCR = 0x00010005;
  
  /* Enable ITM Stimulus Port 0 */
  ITM->TER = 0x1;
  
  /* Unmask ITM stimulus ports 7:0 */
  ITM->TPR = 0x1;
}
