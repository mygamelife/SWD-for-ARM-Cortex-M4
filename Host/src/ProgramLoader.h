#ifndef ProgramLoader_H
#define ProgramLoader_H

#include "Tlv.h"
#include "GetHeaders.h"
#include "Tlv_ErrorCode.h"
#include "CException.h"
#include "Interface.h"
#include "Message.h"

/* Read/Write target register */
void tlvReadTargetRegister(Tlv_Session *session, uint32_t *registerAddress);
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t *registerAddress, uint32_t *data);

/* Read/Write target RAM */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size);
void tlvWriteTargetRam(Tlv_Session *session, uint8_t *dataAddress, uint32_t *destAddress, int *size);

void commandInterpreter(Tlv_Session *session);

void selectCommand(Tlv_Session *session, Tlv *tlv);
void hostProgrammer(Tlv_Session *session);
#endif // ProgramLoader_H
