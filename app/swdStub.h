#ifndef swdStub_H
#define swdStub_H

#include <stdint.h>
#include <malloc.h>
#include "Flash.h"

#if !defined(TEST)
#include "SystemConfigure.h"
#endif

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

/** Stub instruction number definition
  */
typedef enum {
  STUB_CLEAR = 0,
  STUB_COPY,
  STUB_ERASE,
  STUB_MASSERASE
} Stub_Instruction;

/** Stub status number definition
  */
typedef enum {
  STUB_OK = 0,
  STUB_BUSY,
} Stub_Status;

/** Structure type to access the SwdStub.
  */
typedef struct Stub_Type {
  volatile int instruction;                     /*!< Offset: 0x00 (R/W)  Swd Stub instruction       */
  volatile int status;                          /*!< Offset: 0x04 (R/W)  Swd Stub status            */
  volatile int dataSize;                        /*!< Offset: 0x08 (R/W)  Swd Stub data size         */
  volatile uint32_t flashAddress;               /*!< Offset: 0x0C (R/W)  Swd Stub flash address     */
  volatile uint32_t sramAddress;                /*!< Offset: 0x10 (R/W)  Swd Stub sram address      */
  volatile uint32_t banks;                      /*!< Offset: 0x14 (R/W)  Swd Stub bank selection    */
} Stub_Type;

#if defined (TEST)
  Stub_Type *STUB;
  extern Stub_Type *STUB;
#else
  #define STUB_BASE                         (0x20004E00UL)               /*!< Swd Stub Base Address       */
  #define STUB                              ((Stub_Type *)  STUB_BASE)   /*!< STUB configuration struct   */
#endif

void stubInit(void);
void swdStub(int stubInstruction);
void stubCopy(void);
void stubErase(void);
void stubMassErase(void);

/* SVC service */ 
int IsSvcActive(void);
void svcServiceCall(void);
void svcServiceHandler(unsigned int *svc_args);
#endif // swdStub_H
