#ifndef TLV_Probe_H
#define TLV_Probe_H

#include <stdio.h>
#include <stdint.h>
#include "UART.h"
#include "swdStub.h"
#include "TLV_State.h"
#include "Register_ReadWrite.h"

typedef struct {
  TLV_State state;
  UART_HandleTypeDef *uartHandle;
  uint8_t rxBuffer[255];
  uint8_t txBuffer[1024];
} Probe_TypeDef;

void loadEraseSectorInstruction(uint32_t *startSector, uint32_t *endSector);
void loadMassEraseInstruction(uint32_t bankSelect);
void loadCopyFromSRAMToFlashInstruction(uint32_t *dataAddress, uint32_t *destAddress, int size);

// void tlvGetValue(uint8_t *buffer, uint8_t *tlvBuffer, int index, int length);
// uint32_t tlvConvertFromByteToWord(uint8_t *buffer, int index);
void getTlvInstruction(UART_HandleTypeDef *uartHandle);
void waitIncomingData(UART_HandleTypeDef *uartHandle, uint8_t *buffer);
void probeProgrammer(Probe_TypeDef *tlvProbe);

void readFromTargetRam(UART_HandleTypeDef *uartHandle, uint8_t *buffer);

//void readFromTargetFlash();

int tlvDecodeAndWriteToRam(uint8_t *buffer);
int verifyValue(uint8_t *data, uint8_t length);

// TLV *tlvDecodePacket(uint8_t *buffer);
// int tlvVerifyType(uint8_t type);
// int tlvVerifyLength(int length);
// void tlvCheckOperation(TLV *tlv);
#endif // TLV_Probe_H
