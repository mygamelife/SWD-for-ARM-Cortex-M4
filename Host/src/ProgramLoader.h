#ifndef ProgramLoader_H
#define ProgramLoader_H

#include <malloc.h>
#include "Tlv.h"
#include "Yield.h"
#include "GetHeaders.h"
#include "ErrorCode.h"
#include "CException.h"
#include "Interface.h"
#include "GetTime.h"

// #define FLASH_PROGRAMMER_FILE_PATH    \
        // "C:/Users/susan_000/Projects/SWD-for-ARM-Cortex-M4/FlashProgrammer/FlashProgrammer/Debug/bin/FlashProgrammer.elf"

extern int programSize;

/* Read/Write target register */
uint32_t tlvReadTargetRegister(Tlv_Session *session, uint32_t registerAddress);
int tlvWriteTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t *data);

/* Halt target */
int tlvHaltTarget(Tlv_Session *session);

/* Run target */
int tlvRunTarget(Tlv_Session *session);

/* Step target */
uint32_t tlvMultipleStepTarget(Tlv_Session *session, int nInstructions);

/* Reset */
int tlvSoftReset(Tlv_Session *session);
int tlvHardReset(Tlv_Session *session);

/* Read/Write target RAM */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size, Tlv_Command memorySelect);
void tlvWriteTargetMemory(Tlv_Session *session, uint8_t **dataAddress, uint32_t *destAddress, int *size, Tlv_Command memorySelect);

/* Load Flash/RAM */
void tlvLoadProgram(Tlv_Session *session, char *file, Tlv_Command memorySelect);
void tlvLoadToRam(Tlv_Session *session, char *file);
void tlvLoadToFlash(Tlv_Session *session, char *file);

/* Flash Erase */
void tlvEraseTargetFlash(Tlv_Session *session, uint32_t address, int size);
void tlvRequestFlashErase(Tlv_Session *session, uint32_t address, int size);
void tlvRequestFlashMassErase(Tlv_Session *session, uint32_t banks);
void tlvMassEraseTargetFlash(Tlv_Session *session, uint32_t banks);

/* Read Memory */
void tlvReadDataChunk(Tlv_Session *session, uint32_t destAddress, int size);
void tlvReadTargetMemory(Tlv_Session *session, uint32_t *destAddress, int *size);

/* Set Breakpoint */
void tlvSetBreakpoint(Tlv_Session *session, uint32_t address);

void selectCommand(Tlv_Session *session, User_Session *userSession);
void hostInterpreter(Tlv_Session *session);
int isLastOperationDone(Tlv_Session *session);

#endif // ProgramLoader_H