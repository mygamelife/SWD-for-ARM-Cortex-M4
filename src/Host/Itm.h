#ifndef Itm_H
#define Itm_H

#include <stdint.h>

#define enableTrace()               (CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk)  /* Enable use of the trace and debug blocks */
#define enableTraceIo()             (DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN)  /* Enable Trace I/O features  */
#define selectTraceAsynMode()       (DBGMCU->CR |= (uint32_t)0x00000000)  /* Select Trace Asynchronous Mode */

#define TPIU_PORT_SIZE_1            ((uint32_t) 0x00000001)
#define TPIU_PORT_SIZE_2            ((uint32_t) 0x00000002)
#define TPIU_PORT_SIZE_3            ((uint32_t) 0x00000003)
#define TPIU_PORT_SIZE_4            ((uint32_t) 0x00000004)

// #SerialWire Output (Manchester). This is the reset value. 10 - SerialWire Output (NRZ)
void tpiuConfig();
#endif // Itm_H
