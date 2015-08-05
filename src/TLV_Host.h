#ifndef TLV_Host_H
#define TLV_Host_H

#include <stdio.h>
#include <stdint.h>
#include "GetHeaders.h"
#include "TLV_State.h"
#include "Serial.h"

#define DATA_SIZE     253

typedef struct
{
  uint8_t type;
  int length;
  uint8_t value[DATA_SIZE];
} TLV;

typedef struct {
  TLV_State state;
  HANDLE hSerial;
  ElfSection *pElf;
} TLVSession;

void tlvHost(TLVSession *tlvSession);
uint8_t tlvGetByteDataFromElfFile(ElfSection *pElf);
void tlvGetDataFromElf(TLV *tlv, ElfSection *pElf);

TLV *tlvCreateNewPacket(uint8_t type);
uint8_t tlvCalculateCheckSum(uint8_t *buffer, int length, int index);
uint8_t tlvVerifyCheckSum(uint8_t *buffer, int length, int index);
void tlvPackBytesAddress(uint32_t address, uint8_t *buffer, int index);
void tlvPackPacketIntoTxBuffer(uint8_t *buffer, TLV *tlvPacket);
#endif // TLV_Host_H
