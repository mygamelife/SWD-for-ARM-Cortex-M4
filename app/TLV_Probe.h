#ifndef TLV_Probe_H
#define TLV_Probe_H

#include <stdio.h>
#include <stdint.h>
#include "TLV_State.h"
#include "UART.h"
#include "Register_ReadWrite.h"

typedef struct {
  TLV_State state;
  UART_HandleTypeDef *uartHandle;
  uint8_t rxBuffer[1024];
} TLVProbe_TypeDef;

// void tlvGetValue(uint8_t *buffer, uint8_t *tlvBuffer, int index, int length);
// uint32_t tlvConvertFromByteToWord(uint8_t *buffer, int index);
void tlvWaitInstructionFromHost(UART_HandleTypeDef *uartHandle);
void tlvWriteToTargetRam(TLVProbe_TypeDef *tlvProbe);
// TLV *tlvDecodePacket(uint8_t *buffer);
// int tlvVerifyType(uint8_t type);
// int tlvVerifyLength(int length);
// int tlvVerifyValue(TLV *data);
// void tlvCheckOperation(TLV *tlv);
#endif // TLV_Probe_H
