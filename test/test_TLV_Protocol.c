#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <malloc.h>
#include "TLV_Protocol.h"
#include "GetHeaders.h"
#include "Read_File.h"
#include "CException.h"
#include "ErrorCode.h"
#include "ProgramElf.h"
#include "CustomAssertion.h"

void setUp(void)
{
  initElfData();
}

void tearDown(void) {}

void test_tlvCalculateCheckSum_given_data_and_length_should_calc_check_sum(void)
{
  uint8_t chksum = 0;
  
	uint8_t buffer[] = {0x10, 0x24, 0x62, 0x00, 0x46, 0x4C, 0x55, 0x49, 
                      0x44, 0x20, 0x50, 0x52, 0x4F, 0x46, 0x49, 0x4C,
                      0x45, 0x00, 0x46, 0x4C};
  
  chksum = tlvCalculateCheckSum(buffer, sizeof(buffer), 0);
  
  TEST_ASSERT_EQUAL(chksum, 0x33);
}

void test_tlvCreateNewPacket_given_type_length_and_value()
{
  int i = 0;
  uint8_t chksum = 0;
  
  TLV_TypeDef *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE);
  TEST_ASSERT_EQUAL(tlv->length, 0);
  TEST_ASSERT_EQUAL(tlv->value[0], 0x00);
  TEST_ASSERT_EQUAL(tlv->value[1], 0x00);
  TEST_ASSERT_EQUAL(tlv->value[2], 0x00);
  TEST_ASSERT_EQUAL(tlv->value[3], 0x00);
  TEST_ASSERT_EQUAL(tlv->value[4], 0x00);
  TEST_ASSERT_EQUAL(tlv->value[5], 0x00);
}

void test_tlvPutDataIntoBuffer_given_address_0x200001f0_and_data_0x80b594b0_should_get_exactly_the_same(void)
{
  int result = 0;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  TLV_TypeDef *tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvPutDataIntoBuffer(tlv, pElf);
  
  TEST_ASSERT_EQUAL(result, 1);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);

  /* Checksum */
  TEST_ASSERT_EQUAL_HEX8(tlv->value[tlv->length - 1], 0x02);
  
  TEST_ASSERT_EQUAL_HEX32(pElf->codeIndex, 0x000000F8);
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200002E8);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0x47D0);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
}

void test_tlvPutDataIntoBuffer_PutDataIntoBuffer_2_times_codeIndex_address_and_size_should_update_accordingly(void)
{
  int result = 0;
  TLV_TypeDef *tlv;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvPutDataIntoBuffer(tlv, pElf);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvPutDataIntoBuffer(tlv, pElf);
  
  TEST_ASSERT_EQUAL(result, 1);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);

  /* Checksum */
  TEST_ASSERT_EQUAL_HEX8(tlv->value[tlv->length - 1], 0x45);
  
  TEST_ASSERT_EQUAL_HEX32(pElf->codeIndex, 0x000001F0);
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200003E0);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0x46D8);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
}

void test_tlvGetByteData_should_read_byte_data_from_elf_file(void)
{
  int i = 0;
	uint8_t txBuffer[1024];
  
  dataFromElf->myFile = openFile("test/ELF_File/FlashProgrammer.elf", "rb+");
  dataFromElf->eh = getElfHeader(dataFromElf);
  dataFromElf->sh = getSectionHeaders(dataFromElf);
  dataFromElf->programElf = getAllSectionInfo(dataFromElf);
  
  txBuffer[0] = tlvGetByte(dataFromElf->programElf[4].section, 0);
  txBuffer[1] = tlvGetByte(dataFromElf->programElf[4].section, 1);
  txBuffer[2] = tlvGetByte(dataFromElf->programElf[4].section, 2);
  txBuffer[3] = tlvGetByte(dataFromElf->programElf[4].section, 3);
  
  TEST_ASSERT_EQUAL(txBuffer[0], 0x80);
  TEST_ASSERT_EQUAL(txBuffer[1], 0xb5);
  TEST_ASSERT_EQUAL(txBuffer[2], 0x94);
  TEST_ASSERT_EQUAL(txBuffer[3], 0xb0);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
}

void test_tlvGetByteData_should_read_last_byte_data_from_elf_file(void)
{
  int i = 0, index = 0;
	uint8_t txBuffer[1024];
  uint16_t fileSize = 0;
  
  dataFromElf->myFile = openFile("test/ELF_File/FlashProgrammer.elf", "rb+");
  dataFromElf->eh = getElfHeader(dataFromElf);
  dataFromElf->sh = getSectionHeaders(dataFromElf);
  dataFromElf->programElf = getAllSectionInfo(dataFromElf);
  
  index = getIndexOfSectionByName(dataFromElf, ".text");
  
  fileSize = dataFromElf->sh[index].sh_size;

  txBuffer[0] = tlvGetByte(dataFromElf->programElf[index].section, fileSize - 4);
  txBuffer[1] = tlvGetByte(dataFromElf->programElf[index].section, fileSize - 3);
  txBuffer[2] = tlvGetByte(dataFromElf->programElf[index].section, fileSize - 2);
  txBuffer[3] = tlvGetByte(dataFromElf->programElf[index].section, fileSize - 1);
  
  TEST_ASSERT_EQUAL(txBuffer[0], 0x9e);
  TEST_ASSERT_EQUAL(txBuffer[1], 0x46);
  TEST_ASSERT_EQUAL(txBuffer[2], 0x70);
  TEST_ASSERT_EQUAL(txBuffer[3], 0x47);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
}

void test_tlvGetAddressInByte_given_address_0x12345678_should_split_into_four_bytes_12_34_56_78(void ) {
  uint8_t buffer[10];
  
  tlvGetBytesAddress(0x12345678, buffer);
  
  TEST_ASSERT_EQUAL(buffer[0], 0x12);
  TEST_ASSERT_EQUAL(buffer[1], 0x34);
  TEST_ASSERT_EQUAL(buffer[2], 0x56);
  TEST_ASSERT_EQUAL(buffer[3], 0x78);
}

void test_tlvGetWordAddress_given_address_DE_AD_BE_EF_should_get_0xDEADBEEF_address(void ) {
  uint8_t buffer[4] = {0xDE, 0xAD, 0xBE, 0xEF};
  
  uint32_t address32 = tlvGetWordAddress(buffer, 0);
  
  TEST_ASSERT_EQUAL_HEX32(address32, 0xDEADBEEF);
}

void test_tlvPutBytesIntoBuffer_should_update_section_address_and_size_after_put_bytes(void)
{
  int result = 0;
  uint16_t fileSize = 0;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  TLV_TypeDef *tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvPutDataIntoBuffer(tlv, pElf);
  
  TEST_ASSERT_EQUAL(result, 1);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  
  /* Elf section Address and Size*/
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200002e8);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0x47D0);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
}

void test_tlvPutBytesIntoBuffer_should_return_0_when_all_data_is_transfer(void)
{
  int result = 1, chksumResult = 0;
  uint16_t fileSize = 0;
  TLV_TypeDef *tlv;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  while(result != 0) {
    tlv = tlvCreateNewPacket(TLV_WRITE);
    result = tlvPutDataIntoBuffer(tlv, pElf);
  }
  
  TEST_ASSERT_EQUAL(result, 0);
  TEST_ASSERT_EQUAL(tlv->length, 37);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 37, pElf, tlv);

  /** Verify check sum 
    */
  chksumResult = tlvVerifyCheckSum(tlv->value, tlv->length, 4);
  TEST_ASSERT_EQUAL(chksumResult, 1);
  
  /** Elf section CodeIndex, Address and Size
    */
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x20004ab8);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0);
  TEST_ASSERT_EQUAL_HEX16(pElf->codeIndex, 0x48C8);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
}