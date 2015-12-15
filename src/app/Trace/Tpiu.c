#include "Tpiu.h"

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

  /* The TPIU APB registers can be read and written only if the bit TRCENA of the Debug
     Exception and Monitor Control Register (DEMCR) is set.
     This bit enable trace and debug blocks :
      - Data Watchpoint and Trace       (DWT)
      - Instrumentation Trace Macrocell (ITM)
      - Embedded Trace Macrocell        (ETM)
      - Trace Port Interface Unit       (TPIU). */
  enableTraceDebug();
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
  tpiuFormatter(0x100);
  memoryReadWord((uint32_t)&TPI->FFCR, &readData);
  // printf("TPIU formatter %x\n", readData);

  /* TPIU select Synchronous mode protocol by default*/
  tpiuProtocol(NRZ_PROTOCOL);
  memoryReadWord((uint32_t)&TPI->SPPR, &readData);
  // printf("TPIU protocol %x\n", readData);

  /* Enable trace I/O and select I/O Trace Mode */
  enableTraceIOandMode(ASYNC_MODE);
  memoryReadWord((uint32_t)&DBGMCU->CR, &readData);
  // printf("DBGMCU->CR %x\n", readData);

  memoryReadWord((uint32_t)&TPI->DEVID, &readData);
  // printf("TPIU device ID %x\n", readData);
}