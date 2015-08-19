#ifndef TLV_Probe_H
#define TLV_Probe_H

#include <stdio.h>
#include <stdint.h>
#include "UART.h"
#include "swdStub.h"
#include "TLV_State.h"
#include "Register_ReadWrite.h"
#include "CoreDebug.h"

typedef struct {
  TLV_State state;
  UART_HandleTypeDef *uartHandle;
  uint8_t rxBuffer[255];
  uint8_t txBuffer[255];
} Probe_TypeDef;

typedef struct {
  UART_HandleTypeDef *uartHandle;
  uint8_t receive[10];
  uint8_t transmit[10];
} TLV_Session;

void loadEraseSectorInstruction(uint32_t *startSector, uint32_t *endSector);
void loadMassEraseInstruction(uint32_t bankSelect);
void loadCopyFromSRAMToFlashInstruction(uint32_t *dataAddress, uint32_t *destAddress, int size);

// void tlvGetValue(uint8_t *buffer, uint8_t *tlvBuffer, int index, int length);
// uint32_t tlvConvertFromByteToWord(uint8_t *buffer, int index);
void getTlvInstruction(UART_HandleTypeDef *uartHandle);
void waitIncomingData(UART_HandleTypeDef *uartHandle, uint8_t *buffer);
void probeProgrammer(Probe_TypeDef *tlvProbe, TLV_Session *session);

//void readFromTargetRam(UART_HandleTypeDef *uartHandle, uint8_t *buffer);
void readFromTargetRam(Probe_TypeDef *probe);

int tlvDecodeAndWriteToRam(uint8_t *buffer);
int verifyValue(uint8_t *data, uint8_t length);

void writeTargetRegister(TLV_Session *session, uint32_t registerAddress, uint32_t data);
void readTargetRegister(TLV_Session *session, uint32_t registerAddress);

void haltTarget(TLV_Session *session, uint32_t setCoreCommand);
void runTarget(TLV_Session *session, uint32_t setCoreCommand);

void step(TLV_Session *session, int nInstructions);

TLV_Session *createTlvSession(void);
uint8_t *convertWordToByte(uint32_t wordData);
TLV *createTlvPacket(uint8_t command, uint8_t size, uint8_t *data);


void tlvSend(TLV_Session *session, TLV *tlv);
TLV *tlvReceive(TLV_Session *session);

// TLV *tlvDecodePacket(uint8_t *buffer);
// int tlvVerifyType(uint8_t type);
// int tlvVerifyLength(int length);
// void tlvCheckOperation(TLV *tlv);
#endif // TLV_Probe_H
