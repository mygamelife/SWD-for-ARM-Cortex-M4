#ifndef Tlv_ex_H
#define Tlv_ex_H

#define TLV_DATA_SIZE   248

#define getByte(x)      (*(uint8_t *)(x))
#define get4Byte(x)     ((*(uint32_t *)(x)) >> 24 ) | (((*(uint32_t *)(x)) << 8) & 0x00ff0000) |  \
                        (((*(uint32_t *)(x)) >> 8) & 0x0000ff00) | ((*(uint32_t *)(x)) << 24)

/* Tlv Command */
typedef enum
{
  TLV_WRITE_RAM = 10,
  TLV_READ_RAM,
  TLV_WRITE_REGISTER,
  TLV_READ_REGISTER,
  TLV_HALT_TARGET,
  TLV_RUN_TARGET,
  TLV_STEP
} Tlv_Command;

/* Tlv acknowledge */
typedef enum
{
  TLV_OK = 1,
} Tlv_Ack;

typedef enum
{
  TLV_DATA_CORRUPTED = -1,
} Tlv_Nack;

typedef enum
{
  ERR_TIME_OUT = 1,
} Tlv_ErrorCode;

#endif // Tlv_ex_H
