#ifndef TLV_State_H
#define TLV_State_H

#define TLV_DATA_SIZE   248

#define getByte(x)      (*(uint8_t *)(x))
#define get4Byte(x)     ((*(uint32_t *)(x)) >> 24 ) | (((*(uint32_t *)(x)) << 8) & 0x00ff0000) |  \
                        (((*(uint32_t *)(x)) >> 8) & 0x0000ff00) | ((*(uint32_t *)(x)) << 24)

typedef struct
{
  uint8_t type;
  uint8_t length;
  uint8_t value[255];
} TLV;

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
  TLV_END,
  TLV_COMPLETE
} TLV_State;

typedef enum
{
  PROBE_START = 0,
  PROBE_WAIT,
  PROBE_INTERPRET_INSTRUCTION,
  PROBE_END
} Probe_Programmer_State;

/* TLV Instruction */
typedef enum
{
  TLV_START_TRANSMISSION  = 10,
  TLV_END_TRANSMISSION    = 20,
  TLV_WRITE               = 30,
  TLV_READ                = 40,
  TLV_DATA_CORRUPTED      = 50
} TLV_Instruction;

typedef enum  {
  NO_RESPONSE = 0,
  PROBE_FAULT,
  PROBE_OK,
  PROBE_COMPLETE,
  WRITE_SUCCESS,
  WRITE_FAIL
} ProbeStatus;

#define ONE_BYTE                  255
#define ADDRESS_LENGTH            4
#define CHECKSUM_LENGTH           1

#define TLV_TYPE_INDEX            0
#define TLV_ADDRESS_INDEX         0
#define TLV_LENGTH_INDEX          1

/* TLV Instruction */
#define TLV_BUFFER_EMPTY          0
#define TLV_CLEAR                 (uint8_t)0x11
#define TLV_READ                  (uint8_t)0x13
#define TLV_BUSY                  (uint8_t)0x14
#define TLV_ABORT                 (uint8_t)0x15

#endif // TLV_State_H
