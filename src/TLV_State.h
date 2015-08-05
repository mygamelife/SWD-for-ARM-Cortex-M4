#ifndef TLV_State_H
#define TLV_State_H

typedef enum
{
  TLV_CLEAR_ERROR = 0,
  TLV_EMPTY_BUFFER,
  TLV_RECEIVE_TIMEOUT
} TLV_ErrorCode;

typedef enum
{
  TLV_START = 0,
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
#define TLV_BUFFER_EMPTY          0
#define TLV_CLEAR                 (uint8_t)0x10
#define TLV_WRITE                 (uint8_t)0x20
#define TLV_READ                  (uint8_t)0x30
#define TLV_BUSY                  (uint8_t)0x40
#define TLV_OK                    (uint8_t)0x04

#endif // TLV_State_H
