#ifndef TLV_Host_H
#define TLV_Host_H

#include "TLV_Protocol.h"
#include "GetHeaders.h"

typedef struct {
  TLV_State *state;
  HANDLE hSerial;
  ElfData *pElfData;
} TLVSession;

void tlvHost(TLVSession *tlvSession);

#endif // TLV_Host_H
