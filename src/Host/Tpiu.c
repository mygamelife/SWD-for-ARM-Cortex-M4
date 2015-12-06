#include "Tpiu.h"

/* TPIU configuration function to configure Trace Port I/O 
  ITM_BASE            (0xE0000000UL)
  DWT_BASE            (0xE0001000UL)
  TPI_BASE            (0xE0040000UL) 
 */
void configTpiu(void) {
  /* Current trace port size is 2 */
  tpiuPortSize(TPIU_PORT_SIZE_2);
  
  /* Bit [1:0]  : the formatter is enabled 
     Bit [8]    : set to 1 to indicate that triggers are indicated */
  tpiuFormatter(0x102);
  
  /* TPIU select Synchronous mode protocol by default*/
  tpiuProtocol(SYNC_MODE);
  
  /* Enable trace and debug blocks :
      - Data Watchpoint and Trace       (DWT)
      - Instrumentation Trace Macrocell (ITM)
      - Embedded Trace Macrocell        (ETM)
      - Trace Port Interface Unit       (TPIU). */
  enableDWTandITM();
  
  /* Enable trace I/O and select Synchronous Mode with a
     TRACEDATA size of 1 */
  enableAndSelectTraceMode(DBGMCU_CR_TRACE_MODE_0);
}