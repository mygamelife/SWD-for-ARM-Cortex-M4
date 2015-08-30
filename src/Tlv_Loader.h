#ifndef Tlv_Loader_H
#define Tlv_Loader_H

#include <stdio.h>
#include <stdint.h>
#include "Tlv_ex.h"
#include "Serial.h"
#include "CException.h"

Tlv_Session *tlvCreateLoaderSession(void);
Tlv *tlvCreatePacket(uint8_t command, uint8_t size, uint8_t *data);
void tlvPackIntoBuffer(uint8_t *targetBuffer, uint8_t *currentBuffer, int length);

void tlvSend(Tlv_Session *session, Tlv *tlv);
Tlv *tlvReceive(Tlv_Session *session);

#endif // Tlv_Loader_H
