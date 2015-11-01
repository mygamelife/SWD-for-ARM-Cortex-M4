#ifndef TlvEx_H
#define TlvEx_H

#define TLV_DATA_SIZE   248

#define get1Byte(__DATA__)         (* (uint8_t  *)(__DATA__))
#define get2Byte(__DATA__)        (* (uint16_t *)(__DATA__))
#define get4Byte(__DATA__)        (* (uint32_t *)(__DATA__))

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
  TLV_LOAD_RAM,
  TLV_LOAD_FLASH,
  TLV_READ_MEMORY,
  TLV_WRITE_REGISTER,
  TLV_READ_REGISTER,
  TLV_READALL_REGISTER,
  TLV_HALT_TARGET,
  TLV_RUN_TARGET,
  TLV_STEP,
  TLV_STEPOVER,
  TLV_STEPOUT,
  TLV_MULTI_STEP,
  TLV_BREAKPOINT,
  TLV_REMOVE_BREAKPOINT,
  TLV_REMOVE_ALL_BREAKPOINT,
  TLV_STOP_REMAP,
  TLV_STOP_ALL_REMAP,
  TLV_WATCHPOINT,
  TLV_EXIT,
  TLV_SVC,
  TLV_FLASH_ERASE,
  TLV_FLASH_MASS_ERASE,
  TLV_SOFT_RESET,
  TLV_HARD_RESET,
  TLV_VECT_RESET,
  TLV_LOOP_BACK,
  TLV_WRITE_WORD,                     /* Write Data Into Target In Word           */
  TLV_WRITE_HALFWORD,                 /* Write Data Into Target In HalfWord       */
  TLV_WRITE_BYTE,                     /* Write Data Into Target In Byte           */
  TLV_READ_WORD,                      /* Read Data Into Target In Word            */
  TLV_READ_HALFWORD,                  /* Request Read Halfword Data from target                           */
  TLV_DEBUG_EVENTS,                   /* Request Debug Event to check for specific event to happen        */
} Tlv_Command;

typedef enum
{
  TLV_RECEIVE_TYPE,
  TLV_RECEIVE_LENGTH,
  TLV_RECEIVE_VALUE,
  TLV_RECEIVE_CHECKSUM,
  TLV_SEND_PACKET,
  TLV_RECEIVE_PACKET,
  TLV_WAIT_RESPONSE,
  TLV_OPEN_FILE,
  /*### Load Elf State ###*/
  TLV_LOAD_ISR_VECTOR,
  TLV_LOAD_TEXT,
  TLV_LOAD_RO_DATA,
  TLV_LOAD_INIT_ARRAY,
  TLV_LOAD_FINI_ARRAY,
  TLV_LOAD_DATA,
  /*### Load Elf State ###*/
  TLV_LOAD_PROGRAM,
  TLV_UPDATE_PC,
  TLV_RUN_PROGRAM,
  TLV_LOAD_FLASH_PROGRAMMER,
  TLV_LOAD_ACTUAL_PROGRAM,
  TLV_REQUEST_ERASE,
  /* State used by Probe */
  WRITE_TO_RAM,
  COPY_TO_FLASH,
  REQUEST_ERASE,
  WAIT_OPERATION_COMPLETE,
} Tlv_State;

typedef enum
{
  HOST_WAIT_USER_COMMAND,
  HOST_RECEIVE_COMMAND,
  HOST_INTERPRET_COMMAND,
  HOST_WAITING_RESPONSE,
  HOST_EXIT,
} Host_State;

typedef enum
{
  PROBE_RECEIVE_PACKET,
  PROBE_INTERPRET_PACKET,
} Probe_State;

/* Tlv acknowledge */
typedef enum
{
  TLV_OK = 1,
} Tlv_Ack;

typedef enum
{
  TLV_NOT_OK = 2,
} Tlv_Nack;

typedef enum {
  BREAKPOINT_EVENT = 1,
  WATCHPOINT_EVENT,
} EventType;

#endif // TlvEx_H
