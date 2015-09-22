#ifndef swdStub_H
#define swdStub_H

#include <stdint.h>
#include "Flash.h"
#include "CoreDebug.h"

#if !defined(TEST)
#include "SystemConfigure.h"
#endif

/** SWD Instruction address 
  */
#define TWO_KBYTES                    2048 //2k byte
#define FOUR_KBYTES                   4096 //4k byte

/* Inline assembler helper directive: call SVC with the given immediate */
#if !defined (TEST)
#define svc(code)                     asm volatile ("svc %[immediate]"::[immediate] "I" (code))
#endif

/* SVC instruction code */
#define SVC_SERVICE_CALL              0
#define SVC_REQUEST_SRAM_ADDRESS      1
#define SVC_REQUEST_COPY              2
#define SVC_REQUEST_ERASE             3
#define SVC_REQUEST_MASS_ERASE        4
#define SVC_REQUEST_CONFIGURE         5

/* SVC service */ 
void svcServiceCall(void);
void svcServiceHandler(unsigned int *svc_args);
#endif // swdStub_H
