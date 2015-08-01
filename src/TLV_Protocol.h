#ifndef TLV_Protocol_H
#define TLV_Protocol_H

#include <stdio.h>
#include <stdint.h>

#if defined (TEST)
#include "Serial.h"
#include "GetHeaders.h"
#endif

/* TLV protocol type defination */
typedef uint8_t TLV_Byte;

/* Type-Length-Value prototype */
typedef struct
{
  uint8_t type;
  uint8_t length;
  uint8_t *value;
} TLV_TypeDef;

typedef struct
{
  uint8_t *data;
  uint8_t length;
} TLV_DataBuffer;

typedef struct	{
	uint32_t state;
} TaskBlock;

typedef enum
{
  TLV_START = 0,
  TLV_SEND_TYPE,
  TLV_SEND_LENGTH,
  TLV_SEND_VALUE,
  TLV_TRANSMIT_DATA,
  TLV_WAIT_REPLY
} TlvState;

typedef enum  {
  PROBE_OK = 0,
  PROBE_BUSY
} ProbeStatus;

#define ONE_BYTE                  256
#define BAUD_RATE                 9600
#define TLV_CLEAR_INSTRUCTION     (uint8_t)0x00
#define TLV_WRITE                 (uint8_t)0x01
#define TLV_READ_INSTRUCTION      (uint8_t)0x02
#define TLV_BUSY_INSTRUCTION      (uint8_t)0x03
#define TLV_OK                    (uint8_t)0x04

int tlvCalculateCheckSum(uint8_t *buffer, int length);
TLV_TypeDef *tlvCreatePacket(uint8_t type, uint8_t length, uint8_t *value);
TLV_Byte tlvGetByte(uint8_t *data, int index);
TLV_DataBuffer *tlvCreateDataBuffer(uint8_t *buffer, int size);
void tlvGetBytesAddress(uint32_t address, uint8_t *buffer);
uint32_t tlvGetWordAddress(uint8_t *buffer, int index);
void tlvPutDataIntoBuffer(TLV_DataBuffer *dataBuffer, ElfSection *pElf);
void tlvPackPacketIntoTxBuffer(uint8_t *buffer, TLV_TypeDef *tlvPacket);
#endif // TLV_Protocol_H