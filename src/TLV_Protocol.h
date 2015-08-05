#ifndef TLV_Protocol_H
#define TLV_Protocol_H

#include <stdio.h>
#include <stdint.h>
#include "Serial.h"
#include "GetHeaders.h"

#define DATA_SIZE     253

/* Type-Length-Value prototype */
typedef enum
{
  TLV_EMPTY_BUFFER = 0,
  TLV_RECEIVE_TIMEOUT
} TLV_ErrorCode;

typedef struct
{
  uint8_t type;
  int length;
  uint32_t decodeAddress;
  uint32_t decodeValue[DATA_SIZE];
  uint8_t value[DATA_SIZE];
  TLV_ErrorCode errorCode;
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
  TLV_TRANSMIT_DATA,
  TLV_WAIT_REPLY,
} TLV_State;

typedef enum  {
  PROBE_BUSY = 0,
  PROBE_OK
} ProbeStatus;

#define ONE_BYTE                  256
#define ADDRESS_LENGTH            4
#define CHECKSUM_LENGTH           1

#define BAUD_RATE                 9600
#define TLV_CLEAR_INSTRUCTION     (uint8_t)0x00
#define TLV_WRITE                 (uint8_t)0x01
#define TLV_READ_INSTRUCTION      (uint8_t)0x02
#define TLV_BUSY_INSTRUCTION      (uint8_t)0x03
#define TLV_OK                    (uint8_t)0x04

TLV_TypeDef *tlvCreateNewPacket(uint8_t type);
uint8_t tlvCalculateCheckSum(uint8_t *buffer, int length, int index);
uint8_t tlvVerifyCheckSum(uint8_t *buffer, int length, int index);
uint8_t tlvGetByteDataFromElfFile(ElfSection *pElf);
void tlvPackBytesAddress(uint32_t address, uint8_t *buffer, int index);
void tlvGetDataFromElf(TLV_TypeDef *tlv, ElfSection *pElf);

uint32_t tlvGetWordAddress(uint8_t *buffer, int index);
void tlvPackPacketIntoTxBuffer(uint8_t *buffer, TLV_TypeDef *tlvPacket);
TLV_TypeDef *tlvDecodePacket(uint8_t *buffer);

#endif // TLV_Protocol_H
