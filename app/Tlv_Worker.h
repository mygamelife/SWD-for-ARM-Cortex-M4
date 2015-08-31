#ifndef Tlv_Worker_H
#define Tlv_Worker_H

#include <stdio.h>
#include <stdint.h>
#include "Tlv_ex.h"
#include "UART.h"

Tlv_Session *tlvCreateWorkerSession(void);
Tlv *tlvCreatePacket(uint8_t command, uint8_t size, uint8_t *data);
void tlvPackIntoBuffer(uint8_t *targetBuffer, uint8_t *currentBuffer, int length);

void tlvSend(Tlv_Session *session, Tlv *tlv);
Tlv *tlvReceive(Tlv_Session *session);

int tlvVerifyData(Tlv *tlv);
#endif // Tlv_Worker_H
