#ifndef TLV_Protocol_H
#define TLV_Protocol_H

#include <stdio.h>
#include <stdint.h>
#include "Yield.h"
#include "UART.h"

#if defined TEST
#include "Serial.h"
#endif

/* Type-Length-Value prototype */
typedef struct
{
  uint8_t type;
  uint8_t length;
  uint8_t value[0];
} TLV_TypeDef;

typedef struct	{
	uint32_t state;
} TaskBlock;

typedef enum
{
  TLV_START = 0,
  TLV_SEND_TYPE,
  TLV_SEND_LENGTH,
  TLV_SEND_VALUE,
  TLV_WAIT_REPLY,
  TLV_RECEIVE_TYPE,
  TLV_RECEIVE_LENGTH,
  TLV_RECEIVE_VALUE,
  TLV_SEND_REPLY
} TlvState;

typedef enum  {
  PROBE_OK = 0,
  PROBE_BUSY
} ProbeStatus;

#define ONE_BYTE                  8
#define BAUD_RATE                 9600
#define TLV_CLEAR_INSTRUCTION     (uint8_t)0x00
#define TLV_WRITE                 (uint8_t)0x01
#define TLV_READ_INSTRUCTION      (uint8_t)0x02
#define TLV_BUSY_INSTRUCTION      (uint8_t)0x03
#define TLV_OK                    (uint8_t)0x04

TLV_TypeDef *tlvCreatePacket(uint8_t type, uint8_t length, uint8_t *value);
#if defined TEST
int tlvCheckProbeStatus(HANDLE hSerial, uint8_t size, uint8_t *rxBuffer);
void tlvHost(TlvState *state);
void tlvReadFromBinary(FILE *pFile);
//void tlvPackByteDataIntoBuffer(uint8_t *buffer, uint8_t *data);
#endif

void tlvProbe(TlvState *state);

#endif // TLV_Protocol_H