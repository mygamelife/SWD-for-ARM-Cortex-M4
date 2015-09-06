#ifndef Tlv_H
#define Tlv_H

#ifdef TEST
extern int uartReady;
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "Tlv_ex.h"
#include "uart.h"
#include "CException.h"
#include "Tlv_ErrorCode.h"

typedef struct
{
  void *handler;
  uint8_t txBuffer[255];
  uint8_t rxBuffer[255];
  /* Send and Receive state */
  Tlv_State sendState;
  Tlv_State receiveState;
  /* Host and Probe state */
  Host_State hostState;
  Probe_State probeState;
  bool TIMEOUT_FLAG;
  bool DATA_RECEIVE_FLAG;
  bool DATA_SEND_FLAG;
  bool ONGOING_PROCESS_FLAG;
} Tlv_Session;

Tlv_Session *tlvCreateSession(void);

Tlv *tlvCreatePacket(uint8_t command, uint8_t size, uint8_t *data);
void tlvPackIntoBuffer(uint8_t *targetBuffer, uint8_t *currentBuffer, int length);

void tlvSend(Tlv_Session *session, Tlv *tlv);
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
void tlvReportError(Tlv_Session *session, uint8_t errorCode);

#endif // Tlv_H
