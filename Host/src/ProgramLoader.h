#ifndef ProgramLoader_H
#define ProgramLoader_H

#include <malloc.h>
#include "Tlv.h"
#include "GetHeaders.h"
#include "ErrorCode.h"
#include "CException.h"
#include "Interface.h"
#include "GetTime.h"

typedef enum {
  FILE_CLOSED = 0,
  FILE_OPENED
} File_Status;

extern int fileStatus;

/* Read/Write target register */
void tlvReadTargetRegister(Tlv_Session *session, uint32_t registerAddress);
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t *data);

/* Read/Write target RAM */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size, Tlv_Command memorySelect);
void tlvWriteTargetMemory(Tlv_Session *session, uint8_t **dataAddress, uint32_t *destAddress, int *size, Tlv_Command memorySelect);

/* Load Flash/RAM */
void tlvLoadProgram(Tlv_Session *session, char *file, Tlv_Command memorySelect);
void tlvLoadAndRunFlashProgrammer(Tlv_Session *session, char *file);
void tlvLoadToRam(Tlv_Session *session, char *file);
void tlvLoadToFlash(Tlv_Session *session, char *file);

/* Flash Erase */
void tlvFlashErase(Tlv_Session *session, uint32_t address, int size);
void tlvFlashMassErase(Tlv_Session *session, uint32_t banks);

/* Read Memory */
void tlvReadDataChunk(Tlv_Session *session, uint32_t destAddress, int size);
void tlvReadTargetMemory(Tlv_Session *session, uint32_t *destAddress, int *size);

/* Halt target */
void tlvHaltTarget(Tlv_Session *session);

/* Run target */
void tlvRunTarget(Tlv_Session *session);

/* Step target */
void tlvMultipleStepTarget(Tlv_Session *session, int nInstructions);

/* Set Breakpoint */
void tlvSetBreakpoint(Tlv_Session *session, uint32_t address);

/* Reset */
void tlvSoftReset(Tlv_Session *session);
void tlvHardReset(Tlv_Session *session);

void selectCommand(Tlv_Session *session, User_Session *userSession);
void hostInterpreter(Tlv_Session *session);
void isLastOperationDone(Tlv_Session *session);

// #define tlvLoadToRam(session, file)   tlvLoadProgram(session, file, TLV_WRITE_RAM)

#endif // ProgramLoader_H