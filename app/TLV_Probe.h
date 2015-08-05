#ifndef TLV_Probe_H
#define TLV_Probe_H

#include <stdint.h>
#include "TLV_State.h"

int tlvGetLength(uint8_t *buffer);
uint8_t tlvGetType(uint8_t *buffer);
uint32_t tlvGetWordAddress(uint8_t *buffer, int index, int length);
// TLV *tlvDecodePacket(uint8_t *buffer);

#endif // TLV_Probe_H
