#ifndef TLV_Host_H
#define TLV_Host_H

#include <stdio.h>
#include <stdint.h>
#include "GetHeaders.h"
#include "TLV_State.h"
#include "Serial.h"

typedef struct {
  TLV_State state;
  HANDLE hSerial;
  //ElfSection *pElf;
} TLVSession;

void tlvHost(TLVSession *tlvSession);
//uint8_t tlvGetByteDataFromElfFile(ElfSection *pElf);
//void tlvGetDataFromElf(TLV *tlv, ElfSection *pElf);

TLV *tlvCreateNewPacket(uint8_t type);
uint8_t tlvCalculateCheckSum(uint8_t *buffer, int length, int index);
uint8_t tlvVerifyCheckSum(uint8_t *buffer, int length, int index);
void tlvPackPacketIntoBuffer(uint8_t *buffer, TLV *tlvPacket);
TLV_State tlvCheckAcknowledge(uint8_t acknowledge);

void tlvGetBytesData(uint8_t *dataAddress, TLV *tlv, int size);
void tlvPack4ByteAddress(uint32_t address, TLV *tlv);

void tlvWriteRam(uint8_t *dataAddress, uint32_t *destAddress, int size);
void tlvWriteDataChunk(uint8_t *dataAddress, uint32_t *destAddress, int size, HANDLE hSerial);
#endif // TLV_Host_H
