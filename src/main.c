#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Serial.h"
#include "TLV_Host.h"
#include "GetHeaders.h"

int main(void) {
  ElfData *elfData = openElfFile("../test/ELF_File/FlashProgrammer.elf");

  int index = getIndexOfSectionByName(elfData, ".text");
  int size = getSectionSize(elfData, index);
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t *destAddress = (uint32_t *)getSectionVirtualAddress(elfData, 0);
  
  tlvWriteRam(dataAddress, destAddress, size); 
  return 0;
}