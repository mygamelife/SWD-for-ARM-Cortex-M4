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
  *
  * input   : NONE
  *
  * return  : NONE
  */
void tpiuInit(void) {

  /* Configure low level TPIU trace port (SWO) */
  tpiuConfigTracePort();

  /* The TPIU APB registers can be read and written only if the bit TRCENA of the Debug
     Exception and Monitor Control Register (DEMCR) is set.
     This bit enable trace and debug blocks :
      - Data Watchpoint and Trace       (DWT)
      - Instrumentation Trace Macrocell (ITM)
      - Embedded Trace Macrocell        (ETM)
      - Trace Port Interface Unit       (TPIU). */
  enableTraceDebug();
  /* Trace port supported size */
  tpiuSupportedPortSize(TPIU_PORT_SIZE_1);
  /* Current trace port size is 1 (SWO) */
  tpiuPortSize(TPIU_PORT_SIZE_1);

  /* Bit [1:0]  : the formatter is enabled
     Bit [8]    : set to 1 to indicate that triggers are indicated */
  tpiuFormatter(0x100);
  /* Configure the asynchronous clock pre-scale to slow down the clock frequency */
  tpiuAsynPrescaler(7);
  /* Select Asynchronous mode protocol */
  tpiuProtocol(NRZ_PROTOCOL);
  /* Enable trace I/O and select I/O Trace Mode */
  enableTraceIOandMode(ASYNC_MODE);
}