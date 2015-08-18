#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Serial.h"
#include "TLV_Host.h"
#include "GetHeaders.h"
// #include "CException.h"

int main(void) {
  TlvHost_TypeDef host; ElfData *elfData;
  int index = 0;
  
  elfData = openElfFile("../FlashProgrammer/FlashProgrammer.elf");
  
  index = getIndexOfSectionByName(elfData, ".isr_vector");
  host.fileSize = getSectionSize(elfData, index);
  host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  host.hSerial = initSerialComm(UART_PORT, UART_BAUD_RATE);
  
  printf("Flash ISR_VECTOR \n");
  tlvWriteRam(&host);
  
  // elfData = openElfFile("../FlashProgrammer/FlashProgrammer.elf");
  // index = getIndexOfSectionByName(elfData, ".text");
  // host.fileSize = getSectionSize(elfData, index);
  // host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  // host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  // host.hSerial = initSerialComm(UART_PORT, UART_BAUD_RATE);
  
  // printf("Flash FLASH_PROGRAMMER \n");
  // tlvWriteRam(&host);
  
  closeSerialPort(host.hSerial);
  closeFileInTxt(elfData->myFile);
  free(elfData);
  return 0;
}