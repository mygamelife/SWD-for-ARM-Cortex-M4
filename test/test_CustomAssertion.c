#include "unity.h"
#include "CustomAssertion.h"
#include "TLV_Host.h"
#include "TLV_State.h"
#include "mock_Serial.h"
#include "GetHeaders.h"
#include "Read_File.h"
#include "CException.h"
#include "ErrorCode.h"
#include "ProgramElf.h"
#include <stdlib.h>

void setUp(void)  {}

void tearDown(void) {}

void test_assertTLV_given_type_length_address_dataAddress_and_index(void)
{ 
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  
  //Address
  tlv->value[0] = 0xDE;
  tlv->value[1] = 0xAD;
  tlv->value[2] = 0xBE;
  tlv->value[3] = 0xEF;
  
  //Data
  tlv->value[4] = 0x80;
  tlv->value[5] = 0xb5;
  tlv->value[6] = 0x86;
  tlv->value[7] = 0xb0;
  
  tlv->length += 8;
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 9, 0xDEADBEEF, dataAddress, tlv);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}