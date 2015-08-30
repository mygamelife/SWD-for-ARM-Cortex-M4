#ifndef Tlv_ex_H
#define Tlv_ex_H

#define TLV_DATA_SIZE   248

#define getByte(x)      (*(uint8_t *)(x))
#define get4Byte(x)     (*(uint32_t *)(x))

typedef struct
{
  uint8_t type;
  uint8_t length;
  uint8_t value[256];
} Tlv;

typedef struct
{
  void *handler;
  uint8_t txBuffer[256];
  uint8_t rxBuffer[256];
} Tlv_Session;

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
  TLV_NOT_OK = -1
} Tlv_Nack;

typedef enum
{
  ERR_TIME_OUT = -10,
} Tlv_ErrorCode;

#endif // Tlv_ex_H
