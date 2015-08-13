#ifndef TLV_State_H
#define TLV_State_H

#define DATA_SIZE     253

typedef enum
{
  TLV_CLEAR_ERROR = 0,
  TLV_EMPTY_BUFFER,
  TLV_RECEIVE_TIMEOUT,
  TLV_INVALID_TYPE,
  TLV_INVALID_LENGTH,
  TLV_INVALID_VALUE
} TLV_ErrorCode;

typedef enum
{
  TLV_START = 0,
  TLV_TRANSMIT_DATA,
  TLV_WAIT_REPLY,
  TLV_END,
  TLV_COMPLETE
} TLV_State;

typedef enum  {
  PROBE_BUSY = 0,
  PROBE_FAULT,
  PROBE_OK,
  PROBE_COMPLETE
} ProbeStatus;

typedef struct
{
  uint8_t type;
  int length;
  uint32_t sectionAddress;
  uint8_t value[DATA_SIZE];
  uint32_t value32[DATA_SIZE];
  uint8_t chksum;
  TLV_ErrorCode errorCode;
} TLV;

#define ONE_BYTE                  256
#define ADDRESS_LENGTH            4
#define CHECKSUM_LENGTH           1
#define BAUD_RATE                 9600

#define TLV_TYPE_INDEX            0
#define TLV_ADDRESS_INDEX         0
#define TLV_LENGTH_INDEX          1

/* TLV Instruction */
#define TLV_BUFFER_EMPTY          0
#define TLV_CLEAR                 (uint8_t)0x10
#define TLV_WRITE                 (uint8_t)0x20
#define TLV_READ                  (uint8_t)0x30
#define TLV_BUSY                  (uint8_t)0x40
#define TLV_TRANSFER_COMPLETE     (uint8_t)0x50
#define TLV_ABORT                 (uint8_t)0x60
#define TLV_DATA_CORRUPTED        (uint8_t)0x70

#endif // TLV_State_H
