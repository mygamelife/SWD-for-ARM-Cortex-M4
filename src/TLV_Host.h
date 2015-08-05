#ifndef TLV_Host_H
#define TLV_Host_H

#include "TLV_Protocol.h"
#include "GetHeaders.h"
#include "Serial.h"

typedef struct {
  TLV_State state;
  HANDLE hSerial;
  ElfSection *pElf;
} TLVSession;

#define TLV_BUFFER_EMPTY        0

void tlvHost(TLVSession *tlvSession);

#endif // TLV_Host_H
