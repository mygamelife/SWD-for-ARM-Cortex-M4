#ifndef ProgramLoader_H
#define ProgramLoader_H

#include <malloc.h>
#include "Tlv.h"
#include "GetHeaders.h"
#include "Tlv_ErrorCode.h"
#include "CException.h"
#include "Interface.h"

#define PROGRAM_COUNTER    15

/* Read/Write target register */
void tlvReadTargetRegister(Tlv_Session *session, uint32_t registerAddress);
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t *data);

/* Read/Write target RAM */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size, Tlv_Command memorySelect);
void tlvWriteTargetMemory(Tlv_Session *session, uint8_t **dataAddress, uint32_t *destAddress, int *size, Tlv_Command memorySelect);
void tlvLoadProgram(Tlv_Session *session, char *file, Tlv_Command memorySelect);

void tlvLoadToRam(Tlv_Session *session, char *file);
void tlvLoadToFlash(Tlv_Session *session, char *file);

void tlvReadDataChunk(Tlv_Session *session, uint32_t destAddress, int size);
void tlvReadTargetMemory(Tlv_Session *session, uint32_t *destAddress, int *size);

void tlvHaltTarget(Tlv_Session *session);
void tlvRunTarget(Tlv_Session *session);
void tlvMultipleStepTarget(Tlv_Session *session, int nInstructions);

void selectCommand(Tlv_Session *session, User_Session *userSession);
void hostInterpreter(Tlv_Session *session);

#define tlvLoadToRam(session, file)   tlvLoadProgram(session, file, TLV_WRITE_RAM)

#endif // ProgramLoader_H