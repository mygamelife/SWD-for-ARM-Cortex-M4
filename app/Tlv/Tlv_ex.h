#ifndef Tlv_ex_H
#define Tlv_ex_H

#define TLV_DATA_SIZE   248

#define get4Byte(x)               (*(uint32_t *)(x))
#define convertToBigEndian(x)     ((*(uint32_t *)(x)) >> 24 ) | (((*(uint32_t *)(x)) << 8) & 0x00ff0000) |  \
                                  (((*(uint32_t *)(x)) >> 8) & 0x0000ff00) | ((*(uint32_t *)(x)) << 24)

typedef struct
{
  uint8_t type;
  uint8_t length;
  uint8_t value[255];
} Tlv;

/* Tlv Command */
typedef enum
{
  TLV_WRITE_RAM = 10,
  TLV_WRITE_FLASH,
  TLV_READ_MEMORY,
  TLV_WRITE_REGISTER,
  TLV_READ_REGISTER,
  TLV_HALT_TARGET,
  TLV_RUN_TARGET,
  TLV_STEP,
  TLV_MULTI_STEP,
  TLV_BREAKPOINT,
  TLV_WATCHPOINT,
  TLV_EXIT,
  TLV_SVC,
} Tlv_Command;

typedef enum
{
  TLV_SEND_BEGIN,
  TLV_SEND_END,
  TLV_RECEIVE_TYPE,
  TLV_RECEIVE_LENGTH,
  TLV_RECEIVE_VALUE,
  TLV_RECEIVE_CHECKSUM,
  TLV_SEND_PACKET,
  TLV_RECEIVE_PACKET,
  TLV_WAIT_RESPONSE,
  TLV_OPEN_FILE,
  TLV_LOAD_ISR_VECTOR,
  TLV_LOAD_RO_DATA,
  TLV_LOAD_DATA,
  TLV_LOAD_TEXT,
  TLV_LOAD_FLASH_PROGRAMMER,
  TLV_LOAD_ACTUAL_PROGRAM
} Tlv_State;

typedef enum
{
  HOST_WAIT_USER_COMMAND,
  HOST_RECEIVE_COMMAND,
  HOST_INTERPRET_COMMAND,
  HOST_WAITING_RESPONSE,
  HOST_EXIT
} Host_State;

typedef enum
{
  PROBE_RECEIVE_PACKET,
  PROBE_INTERPRET_PACKET
} Probe_State;

/* Tlv acknowledge */
typedef enum
{
  TLV_OK = 1
} Tlv_Ack;

typedef enum
{
  TLV_NOT_OK = 2
} Tlv_Nack;

#endif // Tlv_ex_H
