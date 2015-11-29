#ifndef ProgramLoader_H
#define ProgramLoader_H

#include <malloc.h>
#include <string.h>
#include "Tlv.h"
#include "Yield.h"
#include "GetHeaders.h"
#include "ErrorCode.h"
#include "CException.h"
#include "Interface.h"
#include "SystemTime.h"

typedef enum {
  PROCESS_BUSY = 0,
  PROCESS_DONE
} Process_Status;

#if defined(TEST)
#define FLASH_PROGRAMMER_FILE_PATH            "build/release/target/FlashProgrammer.elf"
#else
#define FLASH_PROGRAMMER_FILE_PATH            "target/FlashProgrammer.elf"
#endif

/* ##### Host Interpreter Macros ##### */
#define systemExit(__SESSION__)                         ((__SESSION__)->exit = 1)
#define isExit(__SESSION__)                             (((__SESSION__)->exit == 1) ? 1 : 0  )
#define IS_TLV_EXIT(__SESSION__)                        (((__SESSION__)->tlvCommand == TLV_EXIT) ? 1 : 0  )
#define HOST_CHANGE_STATE(__SESSION__, __STATE__)       ((__SESSION__)->hostState = __STATE__             )

#define tlvWriteToRam(session, dataAddress, destAddress, size)                    \
        writeMemory(session, dataAddress, destAddress, size, TLV_WRITE_RAM)

#define tlvWriteToFlash(session, dataAddress, destAddress, size)                  \
        writeMemory(session, dataAddress, destAddress, size, TLV_WRITE_FLASH)

#define isProbeAlive(timeout, x)      do {  if(timeout)                     \
                                            {                               \
                                              resetTask(x);                 \
                                              Throw(PROBE_NOT_RESPONDING);  \
                                            }                               \
                                         } while(0)
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

/* Write target memory */
Process_Status writeMemory(Tlv_Session *session, uint8_t *data, uint32_t *address, int *size, Tlv_Command memory);

/* Write/Load Flash and RAM */
void tlvLoadProgram(Tlv_Session *session, char *file, Tlv_Command memorySelect);
void tlvLoadToRam(Tlv_Session *session, char *file);
void tlvLoadToFlash(Tlv_Session *session, char *file);
Process_Status _tlvWriteToFlash(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size);

/* Flash Erase */
Process_Status tlvRequestFlashErase(Tlv_Session *session, uint32_t address, int size);
Process_Status tlvEraseTargetFlash(Tlv_Session *session, uint32_t address, int size);
Process_Status tlvRequestFlashMassErase(Tlv_Session *session, uint32_t banks);
void tlvMassEraseTargetFlash(Tlv_Session *session, uint32_t banks);

/* Read Memory */
uint8_t *readMemory(Tlv_Session *session, uint32_t *destAddress, int *size);

/* Set Breakpoint */
void tlvSetBreakpoint(Tlv_Session *session, uint32_t address);

/* Remove Breakpoint */


/* Stop Flash Patch Remap */

/* Wait Debug Events */
EventType tlvWaitDebugEvents(Tlv_Session *session, EventType event);

int selectCommand(Tlv_Session *session, User_Session *userSession);
int hostInterpreter(Tlv_Session *session);

#endif // ProgramLoader_H