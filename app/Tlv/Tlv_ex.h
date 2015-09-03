#ifndef Tlv_ex_H
#define Tlv_ex_H

#define TLV_DATA_SIZE   248

#define getByte(x)      (*(uint8_t *)(x))
#define get4Byte(x)     (*(uint32_t *)(x))

typedef struct
{
  uint8_t type;
  uint8_t length;
  uint8_t value[255];
} Tlv;

typedef enum
{
  TLV_SEND_BEGIN,
  TLV_SEND_END,
  TLV_RECEIVE_BEGIN,
  TLV_RECEIVE_END,
  TLV_SEND_PACKET,
  TLV_RECEIVE_PACKET,
  TLV_WAIT_RESPONSE
} Tlv_State;

typedef enum
{
  HOST_RECEIVE_COMMAND,
  HOST_INTERPRET_COMMAND,
  HOST_WAITING_RESPONSE
} Host_State;

typedef enum
{
  PROBE_RECEIVE_PACKET,
  PROBE_INTERPRET_PACKET
} Probe_State;

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
  Tlv *userCommand;
  Tlv_State sendState;
  Tlv_State receiveState;
  Tlv_State writeRegisterState;
  Tlv_State readRegisterState;
  Tlv_State writeRAMState;
  Tlv_State readRAMState;
  Host_State programmerState;
  Probe_State taskManagerState;
  bool TIMEOUT_FLAG;
  bool DATA_ARRIVE_FLAG;
  bool DATA_SEND_FLAG;
  bool ONGOING_PROCESS_FLAG;
  uint8_t dataAddress;
  uint32_t destAddress;
  uint8_t size;
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

#endif // Tlv_ex_H
