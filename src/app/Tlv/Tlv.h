#ifndef Tlv_H
#define Tlv_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Uart.h"
#include "TlvEx.h"
#include "ErrorCode.h"
#include "SystemTime.h"
#include "Yield.h"

#if defined(HOST) || defined(TEST)
extern volatile int uartTxReady;
extern volatile int uartRxReady;
#endif

typedef struct
{
  void *handler;
  uint8_t txBuffer[255];                  /* Tlv transmit buffer used to transfer tlv packet to target                */
  uint8_t rxBuffer[255];                  /* Tlv receive buffer used to receive tlv packet from target                */
  Tlv_State receiveState;                 /* Send and Receive state */
  uint32_t flags;                         /* Flags */
  bool exit;
} Tlv_Session;

typedef enum {
  FLAG_CLEAR = false,
  FLAG_SET = true,
} Flag_Status;

/* ##### Defined Tlv Flags In Numeric ##### */
#define TLV_TIMEOUT_FLAG                                ((uint32_t)0x00000001)
#define TLV_DATA_RECEIVE_FLAG                           ((uint32_t)0x00000002)
#define TLV_DATA_TRANSMIT_FLAG                          ((uint32_t)0x00000004)
#define TLV_ONGOING_PROCESS_FLAG                        ((uint32_t)0x00000008)
#define TLV_SET_BREAKPOINT_FLAG                         ((uint32_t)0x00000010)
#define TLV_SET_WATCHPOINT_FLAG                         ((uint32_t)0x00000020)
#define TLV_BREAKPOINT_WAS_SET_FLAG                     ((uint32_t)0x00000040)
 
/* ##### Tlv Flags Status Macros ##### */
#define GET_FLAG_STATUS(__SESSION__, __FLAG__)          (((__SESSION__)->flags & (__FLAG__)) == (__FLAG__))
#define SET_FLAG_STATUS(__SESSION__, __FLAG__)          ((__SESSION__)->flags |= (__FLAG__))
#define CLEAR_FLAG_STATUS(__SESSION__, __FLAG__)        ((__SESSION__)->flags &= ~(__FLAG__))

#define tlvReply(session, command, size, data)          tlvSendRequest(session, command, size, data)

Tlv_Session *tlvCreateSession(void);

Tlv *tlvCreatePacket(uint8_t command, uint8_t size, uint8_t *data);
uint8_t tlvPackIntoBuffer(uint8_t *targetBuffer, uint8_t *currentBuffer, int length);
uint8_t tlvUpdateChecksum(uint8_t oldChecksum, uint8_t newChecksum);

void tlvSend(Tlv_Session *session, Tlv *tlv);
void tlvSendRequest(Tlv_Session *session, Tlv_Command command, int size, uint8_t *data);
Tlv *tlvReceive(Tlv_Session *session);

/* Tlv service state machine */
void tlvSendService(Tlv_Session *session);
void tlvReceiveService(Tlv_Session *session);
void tlvService(Tlv_Session *session);

/* Tlv helper function */
int verifyTlvData(Tlv *tlv);
int isTlvCommand(uint8_t command);
int isTlvAck(Tlv *tlv);
int verifyTlvPacket(Tlv *tlv);
void tlvErrorReporter(Tlv_Session *session, uint8_t errorCode);
void tlvReadDataChunk(Tlv_Session *session, uint32_t *destAddress, int *size);
void tlvWriteDataChunk(Tlv_Session *session, uint8_t **dataAddress, uint32_t *destAddress, int *size, Tlv_Command memorySelect);

#endif // Tlv_H
