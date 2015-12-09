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

void tpiuConfigTracePort(void) {
  GPIO_Init init;

  init.mode = ALTFUNC_MODE;
  init.pin = SWO_TRACE_PIN;
  init.af = AF0;
  
  /* Configure Trace Port Clock */
  gpioConfigPort(SWO_TRACE_PORT, &init);
}

/** configTpiu is a function to configure TPIU
  */
void tpiuInit(void) {
  uint32_t readData = 0;
  
  /* Configure SWO */
  tpiuConfigTracePort();
  
  setCoreMode(CORE_DEBUG_MODE);
  
  /* The TPIU APB registers can be read and written only if the bit TRCENA of the Debug
     Exception and Monitor Control Register (DEMCR) is set.
     This bit enable trace and debug blocks :
      - Data Watchpoint and Trace       (DWT)
      - Instrumentation Trace Macrocell (ITM)
      - Embedded Trace Macrocell        (ETM)
      - Trace Port Interface Unit       (TPIU). */
  enableDWTandITM();
  memoryReadWord((uint32_t)&CoreDebug->DEMCR, &readData);
  // printf("CoreDebug Control Register %x\n", readData);
  
  memoryWriteWord((uint32_t)&TPI->SSPSR, TPIU_PORT_SIZE_1);
  memoryReadWord((uint32_t)&TPI->SSPSR, &readData);
  // printf("Supported port size %x\n", readData);

  /* Current trace port size is 1 (SWO) */
  tpiuPortSize(TPIU_PORT_SIZE_1);
  memoryReadWord((uint32_t)&TPI->CSPSR, &readData);
  // printf("TPIU trace port size %x\n", readData);
  
  /* Bit [1:0]  : the formatter is enabled 
     Bit [8]    : set to 1 to indicate that triggers are indicated */
  tpiuFormatter(0x102);
  memoryReadWord((uint32_t)&TPI->FFCR, &readData);
  // printf("TPIU formatter %x\n", readData);
  
  /* TPIU select Synchronous mode protocol by default*/
  tpiuProtocol(NRZ_MODE);
  memoryReadWord((uint32_t)&TPI->SPPR, &readData);
  // printf("TPIU protocol %x\n", readData);

  /* Enable trace I/O and select Asynchronous Mode */
  memoryWriteWord((uint32_t)&DBGMCU->CR, DBGMCU_CR_TRACE_IOEN);
  memoryReadWord((uint32_t)&DBGMCU->CR, &readData);
  // printf("DBGMCU->CR %x\n", readData);

  memoryReadWord((uint32_t)&TPI->DEVID, &readData);
  // printf("TPIU device ID %x\n", readData);
}