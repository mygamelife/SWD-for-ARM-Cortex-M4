#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Serial.h"
#include "TLV_Host.h"
#include "GetHeaders.h"
#include "CoreDebug_Utilities.h"
// #include "CException.h"

int main(void) {
  TlvHost_TypeDef host; ElfData *elfData;
  int index = 0;
  
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
  
  // while(uartGetBytes(host.hSerial, host.rxBuffer, ONE_BYTE))  {
    // if(host.rxBuffer[0] == TLV_SEND) 
    // {
      // printf("Probe is ready for next task!\n");
      // break;
    // }
  // }
  // host.fileSize = getSectionSize(elfData, index);
  // host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  // host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  
  // printf("READING Target RAM..........\n");
  // tlvReadRam(&host);
  TLV_Session *session = createTLVSession();
  printf("READ CORE DEBUG REGISTER..........\n");
  tlvReadTargetRegister(session, CORE_REG_PC);
  
  closeSerialPort(host.hSerial);
  // closeFileInTxt(elfData->myFile);
  // free(elfData);
  return 0;
}