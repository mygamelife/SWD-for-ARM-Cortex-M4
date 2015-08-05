#ifndef TLV_Protocol_H
#define TLV_Protocol_H

#include <stdio.h>
#include <stdint.h>

#define HOST_TRANSMITTER
#define DATA_SIZE     253

typedef enum
{
  TLV_EMPTY_BUFFER = 0,
  TLV_RECEIVE_TIMEOUT
} TLV_ErrorCode;

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

typedef struct
{
  uint8_t type;
  int length;
  uint32_t decodeAddress;
  uint32_t decodeValue[DATA_SIZE];
  uint8_t value[DATA_SIZE];
  TLV_ErrorCode errorCode;
} TLV;

typedef struct	{
	uint32_t state;
} TaskBlock;

#define ONE_BYTE                  256
#define ADDRESS_LENGTH            4
#define CHECKSUM_LENGTH           1

#define BAUD_RATE                 9600
#define TLV_BUFFER_EMPTY          0
#define TLV_CLEAR                 (uint8_t)0x10
#define TLV_WRITE                 (uint8_t)0x20
#define TLV_READ                  (uint8_t)0x30
#define TLV_BUSY                  (uint8_t)0x40
#define TLV_OK                    (uint8_t)0x04

#if defined (HOST_TRANSMITTER)
  #include "GetHeaders.h"
  #include "Serial.h"

  typedef struct {
    TLV_State state;
    HANDLE hSerial;
    ElfSection *pElf;
  } TLVSession;

  void tlvHost(TLVSession *tlvSession);
  uint8_t tlvGetByteDataFromElfFile(ElfSection *pElf);
  void tlvGetDataFromElf(TLV *tlv, ElfSection *pElf);
#endif

TLV *tlvCreateNewPacket(uint8_t type);
uint8_t tlvCalculateCheckSum(uint8_t *buffer, int length, int index);
uint8_t tlvVerifyCheckSum(uint8_t *buffer, int length, int index);
void tlvPackBytesAddress(uint32_t address, uint8_t *buffer, int index);
uint32_t tlvGetWordAddress(uint8_t *buffer, int index);
void tlvPackPacketIntoTxBuffer(uint8_t *buffer, TLV *tlvPacket);
TLV *tlvDecodePacket(uint8_t *buffer);

#endif // TLV_Protocol_H
