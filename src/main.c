#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Serial.h"
#include "Tlv.h"
#include "GetHeaders.h"

int main(void) {
  //TlvHost_TypeDef host; ElfData *elfData;
  //int index = 0;
  
  // elfData = openElfFile("../FlashProgrammer/FlashProgrammer.elf");
  
  // index = getIndexOfSectionByName(elfData, ".isr_vector");
  // host.fileSize = getSectionSize(elfData, index);
  // host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  // host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  // host.hSerial = initSerialComm(UART_PORT, UART_BAUD_RATE);
  
  // printf("Flash ISR_VECTOR............\n");
  // tlvWriteRam(&host);
  
  // while(uartGetBytes(host.hSerial, host.rxBuffer, ONE_BYTE) == 0);
  // printf("Probe is ready for next task!\n");
  
  // index = getIndexOfSectionByName(elfData, ".data");
  // host.fileSize = getSectionSize(elfData, index);
  // host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  // host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  
  // printf("Flash DATA............\n");
  // tlvWriteRam(&host);
  
  // while(uartGetBytes(host.hSerial, host.rxBuffer, ONE_BYTE) == 0);
  // printf("Probe is ready for next task!\n");
  
  // index = getIndexOfSectionByName(elfData, ".text");
  // host.fileSize = getSectionSize(elfData, index);
  // host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  // host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  
  // printf("LOADING FLASH_PROGRAMMER............\n");
  // tlvWriteRam(&host);
  
  uint8_t buffer[] = { 0xBE, 0xEF, 0xCA, 0xFE, 0xAA, 0xBB, 0xCC, 0xDD,
                       0xBE, 0xEF, 0xCA, 0xFE, 0xAA, 0xBB, 0xCC, 0xDD,
                       0xBE, 0xEF, 0xCA, 0xFE, 0xAA, 0xBB, 0xCC, 0xDD};
  Tlv_Session *session = tlvCreateSession();
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
	// Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  printf("Opening port\n");
  
  tlvSend(session, tlv);
  
  printf("Closing port\n");
  closeSerialPort(session->hSerial);
  // closeFileInTxt(elfData->myFile);
  // free(elfData);
  return 0;
}