#ifndef ProgramLoader_H
#define ProgramLoader_H

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "Tlv.h"
#include "Yield.h"
#include "ErrorCode.h"
#include "CException.h"
#include "Interface.h"
#include "SystemTime.h"
#include "ProgramVerifier.h"

typedef enum {
  PROCESS_BUSY = 0,
  PROCESS_DONE
} Process_Status;

#if defined(TEST)
/* Flash Programmer File Path */
#define FP_PATH    "test/ElfFiles/FlashProgrammer.elf"
#else
#define FP_PATH    "target/FlashProgrammer.elf"
#endif

/* ##### Host Interpreter Macros ##### */
#define isProbeAlive(timeout, x)      do {  if(timeout)                     \
                                            {                               \
                                              resetTask(x);                 \
                                              resetSystemTime();            \
                                              Throw(PROBE_NOT_RESPONDING);  \
                                            }                               \
                                         } while(0)                         \
                                           
#define systemExit(__SESSION__)       ((__SESSION__)->exit = 1)
#define isExit(__SESSION__)           (((__SESSION__)->exit == 1) ? 1 : 0  )

#define writeRam(session, data, address, size)     writeMemory(session, data, address, size, TLV_WRITE_RAM)
#define writeFlash(session, data, address, size)   writeMemory(session, data, address, size, TLV_WRITE_FLASH)

/* Read/Write target register */
Process_Status writeRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t data);
uint32_t readRegister(Tlv_Session *session, uint32_t registerAddress);

/* Halt target */
Process_Status halt(Tlv_Session *session);

/* Run target */
Process_Status run(Tlv_Session *session);

/* Step target */
uint32_t multipleStep(Tlv_Session *session, int nInstructions);

/* Reset */
Process_Status softReset(Tlv_Session *session);
Process_Status hardReset(Tlv_Session *session);
Process_Status vectorReset(Tlv_Session *session);

/* Read/Write target memory */
Process_Status writeMemory(Tlv_Session *session, uint8_t *data, uint32_t address, int size, Tlv_Command memory);
uint8_t *readMemory(Tlv_Session *session, uint32_t address, int size);

/* Write/Load Flash and RAM */
int loadProgram(Tlv_Session *session, Program *p, Tlv_Command memorySelect);
int loadRam(Tlv_Session *session, Program *p);
int loadFlash(Tlv_Session *session, Program *p);
Process_Status reactiveProgram(Tlv_Session *session, Program *p);
Process_Status _writeFlash(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size);
/* Flash Erase */
Process_Status eraseSection(Tlv_Session *session, uint32_t address, int size);
Process_Status eraseAll(Tlv_Session *session, uint32_t banks);

/* Set Breakpoint */
Process_Status setBreakpoint(Tlv_Session *session, uint32_t address);

/* Remove Breakpoint */

/* Stop Flash Patch Remap */

/* Wait Debug Events */
EventType tlvWaitDebugEvents(Tlv_Session *session, EventType event);

int selectCommand(Tlv_Session *session, User_Session *us);
int hostInterpreter(Tlv_Session *session);

uint8_t *createDataBlock(int size);
void delDataBlock(uint8_t *s);

#endif // ProgramLoader_H