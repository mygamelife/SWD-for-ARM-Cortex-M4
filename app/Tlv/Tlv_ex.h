#ifndef Tlv_ex_H
#define Tlv_ex_H

#define TLV_DATA_SIZE   248

#define getByte(x)      (*(uint8_t *)(x))
#define get4Byte(x)     (*(uint32_t *)(x))

typedef struct
{
  uint8_t type;
  int length;
  uint8_t value[255];
} Tlv;

typedef enum
{
  SEND_BEGIN,
  SEND_END,
  RECEIVE_BEGIN,
  RECEIVE_END
} Tlv_State;

typedef enum
{
  TRANSMISSION_BUSY,
  TRANSMISSION_FREE
} Tlv_Transmission_State;

typedef struct
{
  void *handler;
  uint8_t txBuffer[255];
  uint8_t rxBuffer[255];
  Tlv_State sendState;
  Tlv_State receiveState;
  bool TIMEOUT_FLAG;
  bool DATA_ARRIVE_FLAG;
  bool DATA_SEND_FLAG;
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
  TLV_STEP,
  TLV_MULTI_STEP,
  TLV_BREAKPOINT
} Tlv_Command;

/* Tlv acknowledge */
typedef enum
{
  TLV_OK = 1
} Tlv_Ack;

typedef enum
{
  TLV_NOT_OK = 2
} Tlv_Nack;

typedef enum  {
  DATA_VALID = 0,
  DATA_INVALID
} Tlv_Validation;

#endif // Tlv_ex_H
