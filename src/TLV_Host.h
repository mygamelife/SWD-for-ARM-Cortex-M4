#ifndef TLV_Host_H
#define TLV_Host_H

#include <stdio.h>
#include <stdint.h>
#include "GetHeaders.h"
#include "TLV_State.h"
#include "ErrorCode.h"
// #include "CException.h"
#include "Serial.h"

typedef struct  {
  int fileSize;
  HANDLE hSerial;
  uint8_t *dataAddress;
  uint32_t destAddress;
} TlvHost_TypeDef;

uint8_t tlvCalculateCheckSum(uint8_t *buffer, int length, int index);
uint8_t tlvVerifyCheckSum(uint8_t *buffer, int length, int index);
TLV_State tlvCheckAcknowledge(uint8_t acknowledge);

TLV *tlvCreateNewPacket(uint8_t type);
void tlvPackPacketIntoBuffer(uint8_t *buffer, TLV *tlvPacket);

void tlvGetBytesData(uint8_t *dataAddress, TLV *tlv, int size);
void tlvPack4ByteAddress(uint32_t address, TLV *tlv);

void tlvWriteRam(TlvHost_TypeDef *host);

void tlvWriteDataChunk(uint8_t *dataAddress, uint32_t destAddress, int size, HANDLE hSerial);

/* <!For internal use only!> */
int tlvVerifyResponse(int response);
void tlvWaitReplyFromProbe(HANDLE hSerial, TlvHost_TypeDef *host);

#endif // TLV_Host_H
