#ifndef Stub_H
#define Stub_H

#include <stdint.h>
#include <malloc.h>
#include "Flash.h"
#include "SystemConfigure.h"

/** SWD Instruction address 
  */
#define TWO_KBYTES                    2048 //2k byte
#define FOUR_KBYTES                   4096 //4k byte

/* Inline assembler helper directive: call SVC with the given immediate */
#if !defined (TEST)
#define svc(code)                     __asm volatile ("svc %[immediate]"::[immediate] "I" (code))
#endif

/* SVC instruction code */
#define SVC_SERVICE_CALL              0
#define SVC_REQUEST_SRAM_ADDRESS      1
#define SVC_REQUEST_COPY              2
#define SVC_REQUEST_ERASE             3
#define SVC_REQUEST_MASS_ERASE        4
#define SVC_REQUEST_CONFIGURE         5

#define STUB_ID                       (uint32_t)(0x0ABCDEF0)
/** Stub instruction number definition
  */
typedef enum {
  STUB_CLEAR = 0,
  STUB_COPY,
  STUB_ERASE,
  STUB_MASSERASE,
  STUB_PRESCALE_SYSCLK,
  STUB_GET_SYSCLK,
} Stub_Instruction;

/** Stub status number definition
  */
typedef enum {
  STUB_OK = 0,
  STUB_BUSY,
} Stub_Status;

/** Structure type to access the SwdStub.
  */
typedef struct
{
  volatile int instruction;                 /*!< Offset: 0x00 (R/W)  Swd Stub instruction           */
  volatile int status;                      /*!< Offset: 0x04 (R/W)  Swd Stub status                */
  volatile int dataSize;                    /*!< Offset: 0x08 (R/W)  Swd Stub data size             */
  volatile uint32_t flashAddress;           /*!< Offset: 0x0C (R/W)  Swd Stub flash address         */
  volatile uint32_t sramAddress;            /*!< Offset: 0x10 (R/W)  Swd Stub sram address          */
  volatile uint32_t banks;                  /*!< Offset: 0x14 (R/W)  Swd Stub bank selection        */
  volatile uint32_t sysClockPrescale;       /*!< Offset: 0x18 (R/W)  Prescale value used to prescale system clock   */
  volatile uint32_t sysClock;               /*!< Offset: 0x1C (R/W)  Current system clock frequency */
  volatile uint32_t id;                     /*!< Offset: 0x20 (R/W)  Stub ID */
} Stub_Type;

#if defined (TEST)
  Stub_Type *Stub;
  extern Stub_Type *Stub;
#else
  #define STUB_BASE                         (0x20004E00UL)               /*!< Swd Stub Base Address       */
  #define Stub                              ((Stub_Type *)  STUB_BASE)   /*!< Stub configuration struct   */
#endif

void stubInit(void);
void stub(int stubInstruction);
void stubCopy(void);
void stubErase(void);
void stubMassErase(void);
void stubPrescaleSystemClock(void);
void stubGetSystemClock(void);
void stubSetID(void);

/* SVC service */ 
int IsSvcActive(void);
void svcServiceCall(void);
void svcServiceHandler(unsigned int *svc_args);
#endif // SwdStub_H
