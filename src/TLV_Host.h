#ifndef TLV_Host_H
#define TLV_Host_H

#include <stdio.h>
#include <stdint.h>
#include "GetHeaders.h"
#include "TLV_State.h"
#include "ErrorCode.h"
#include "CoreDebug_Utilities.h"
// #include "CException.h"
#include "Serial.h"

typedef struct  {
  int fileSize;
  HANDLE hSerial;
  uint8_t *dataAddress;
  uint32_t destAddress;
  uint8_t rxBuffer[255];
  uint8_t txBuffer[255];
} TlvHost_TypeDef;

typedef struct {
  HANDLE hSerial;
  uint8_t txBuffer[0];
  uint8_t rxBuffer[0];
} TLV_Session;

void tlvInsertCheckSum(uint8_t *buffer, int length);
uint8_t tlvVerifyCheckSum(uint8_t *buffer, int length, int index);
TLV_State tlvCheckAcknowledge(uint8_t acknowledge);

TLV *tlvCreateNewPacket(uint8_t type);
void tlvPackPacketIntoBuffer(uint8_t *buffer, TLV *tlvPacket);

void tlvGetBytesData(uint8_t *dataAddress, TLV *tlv, int size);
void tlvPack4ByteAddress(uint32_t address, TLV *tlv);

void tlvWriteRam(TlvHost_TypeDef *host);
void tlvReadRam(TlvHost_TypeDef *host);

void verifyReadDataFromRam(TlvHost_TypeDef *host, int sizeToRead);

void tlvWriteDataChunk(uint8_t *dataAddress, uint32_t destAddress, int size, HANDLE hSerial);

/* <!For internal use only!> */
int tlvVerifyResponse(int response);
int tlvWaitReplyFromProbe(TlvHost_TypeDef *host);

void tlvReadTargetRegister(TLV_Session *session, uint32_t registerAddress);
void tlvWriteTargetRegister(TLV_Session *session, uint32_t registerAddress, uint32_t data);

void tlvHaltTarget(TLV_Session *session);
void tlvRunTarget(TLV_Session *session);

void tlvStep(TLV_Session *session, int nInstructions);
/** TLV Module
  *
  */
void tlvSend(TLV_Session *session, TLV *tlv);
TLV *tlvReceive(TLV_Session *session);
TLV_Session *createTLVSession(void);
TLV *createTlvPacket(uint8_t command, uint8_t size, uint8_t *data);
uint8_t *convertWordToByte(uint32_t wordData);
void tlvPack4ByteData(TLV *tlv, uint32_t data, int index);

void userCommandPrompt();
#endif // TLV_Host_H
