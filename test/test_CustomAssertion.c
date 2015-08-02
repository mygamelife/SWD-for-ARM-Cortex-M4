#include "unity.h"
#include "CustomAssertion.h"
#include "TLV_Protocol.h"
#include "GetHeaders.h"
#include "Read_File.h"
#include "CException.h"
#include "ErrorCode.h"
#include "ProgramElf.h"
#include <stdlib.h>

void setUp(void)
{
  initElfData();
}

void tearDown(void)
{
}

void test_assertTLV_call_put_data_1_times(void)
{ 
  int result = 0;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  TLV_TypeDef *tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvPutDataIntoBuffer(tlv, pElf);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, tlv->length, pElf, tlv);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
}

void test_assertTLV_call_put_data_2_times(void)
{ 
  int result = 0;
  TLV_TypeDef *tlv;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvPutDataIntoBuffer(tlv, pElf);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvPutDataIntoBuffer(tlv, pElf);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, tlv->length, pElf, tlv);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
}

/* This test code is use to test the feedback message
void test_assertTLV_given_Wrong_value_should_give_some_feedback(void)
{
  uint8_t value[] = {0x1, 0x2, 0x3, 0x4, 0x5};
  uint8_t actualValue[] = {0x1, 0x2, 0x3, 0x4, 0x6};
  
  TLV_TypeDef *tlv = tlvCreatePacket(TLV_WRITE, sizeof(actualValue), actualValue);
  
	TEST_ASSERT_EQUAL_TLV(TLV_WRITE, sizeof(value), value, tlv);
}*/