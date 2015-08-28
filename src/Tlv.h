#ifndef Tlv_H
#define Tlv_H

#include <stdio.h>
#include <stdint.h>
#include "Serial.h"
#include "Tlv_ex.h"
#include "CException.h"

typedef struct
{
  uint8_t type;
  uint8_t length;
  uint8_t *value;
} Tlv;

typedef struct
{
  HANDLE hSerial;
  uint8_t txBuffer[0];
  uint8_t rxBuffer[0];
} Tlv_Session;

Tlv *tlvCreatePacket(uint8_t command, uint8_t size, uint8_t *data);
Tlv_Session *tlvCreateSession(void);

void tlvSend(Tlv_Session *session, Tlv *tlv);
Tlv *tlvReceive(Tlv_Session *session);
#endif // Tlv_H
