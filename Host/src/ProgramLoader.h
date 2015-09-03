#ifndef ProgramLoader_H
#define ProgramLoader_H

#include "Tlv.h"
#include "GetHeaders.h"
#include "Tlv_ErrorCode.h"
#include "CException.h"

/* Read/Write target register */
void tlvReadTargetRegister(Tlv_Session *session, uint32_t *registerAddress);
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t *registerAddress, uint32_t *data);

/* Read/Write target RAM */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size);
void tlvWriteTargetRam(Tlv_Session *session, uint8_t *dataAddress, uint32_t *destAddress, int *size);
#endif // ProgramLoader_H
