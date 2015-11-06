#ifndef Tpiu_H
#define Tpiu_H

#include "stm32f429xx.h"
#include "core_cm4.h"

/* TPIU trace port size selection
 */
#define TPIU_PORT_SIZE_1            ((uint32_t) 0x00000001)
#define TPIU_PORT_SIZE_2            ((uint32_t) 0x00000002)
#define TPIU_PORT_SIZE_3            ((uint32_t) 0x00000003)
#define TPIU_PORT_SIZE_4            ((uint32_t) 0x00000004)

/* TPIU trace port protocol selection
   - The standard UART (NRZ) capture mechanism, 5% accuracy is needed.
   - The Manchester encoded version is tolerant up to 10%.
 */
#define SYNC_TRACE_PORT_MODE        ((uint32_t) 0x00000000)
#define SWO_MANCHESTER_MODE         ((uint32_t) 0x00000001)
#define SWO_NRZ_MODE                ((uint32_t) 0x00000002)

/* Enable trace and debug features to allow
   stm32f429xx to use the debug blocks such as ITM/ETM/DWT/TPIU
 */
#define enableTrace()               (CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk)  /* Enable use of the trace and debug blocks */
#define enableTraceIo()             (DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN)  /* Enable Trace I/O features  */
#define selectTraceAsynMode()       (DBGMCU->CR |= (uint32_t)0x00000000)  /* Select Trace Asynchronous Mode */

void tpiuConfig(void);

#endif // Tpiu_H
