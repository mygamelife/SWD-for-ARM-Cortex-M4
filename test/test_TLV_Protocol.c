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

void test_tlvCreateNewPacket_given_type_length_and_value()
{
  int i = 0;
  uint8_t chksum = 0;
  
  TLV_TypeDef *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE);
  TEST_ASSERT_EQUAL(tlv->length, 0);
  
  for(i; i < DATA_SIZE; i++)
    TEST_ASSERT_EQUAL_HEX8(tlv->value[i], 0x00);
}

void test_tlvCalculateCheckSum_given_data_and_length_should_calc_check_sum(void)
{
  int i = 0;
  uint8_t chksum = 0;
	uint8_t buffer[] = {0x10, 0x24, 0x62, 0x00, 0x46, 0x4C, 0x55, 0x49, 
                      0x44, 0x20, 0x50, 0x52, 0x4F, 0x46, 0x49, 0x4C,
                      0x45, 0x00, 0x46, 0x4C};
                      
  TLV_TypeDef *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  for(i; i < sizeof(buffer); i++)
    tlv->value[i] = buffer[i];
  
  chksum = tlvCalculateCheckSum(tlv->value, sizeof(buffer), 0);
  
  TEST_ASSERT_EQUAL_HEX8(chksum, 0x33);
}

void test_tlvCalculateCheckSum_given_index_4_and_length_should_calculate_checksum_start_at_index_4(void)
{
  uint8_t chksum = 0;  
	uint8_t buffer[] = {0xAA, 0xBB, 0xCC, 0xDD,
                      0x12, 0x2C, 0x3A, 0x49, 0x54, 0x60, 0x7F, 0x86, 0x92, 0xA1,
                      0xA4, 0xBC, 0xC5, 0xD9, 0xE4, 0xF0, 0xF1, 0xF2, 0xFF, 0xF6};
  
  chksum = tlvCalculateCheckSum(buffer, sizeof(buffer), 4);
  
  TEST_ASSERT_EQUAL_HEX8(chksum, 0xA9);
}

void test_tlvVerifyCheckSum_given_data_with_actual_chksum_should_verify_it_with_add_up_all_the_data(void)
{
  uint8_t chksum = 0, result = 0;
  
	uint8_t buffer[] = {0xAA, 0xBB, 0xCC, 0xDD, //Address
                      0x12, 0x2C, 0x3A, 0x49, //Data
                      0x54, 0x60, 0x7F, 0x86,
                      0x92, 0xA1, 0xA4, 0xBC,
                      0xC5, 0xD9, 0xE4, 0xF0,
                      0xF1, 0xF2, 0xFF, 0xF6,
                      0xA9}; //CheckSum
                      
  result = tlvVerifyCheckSum(buffer, sizeof(buffer), 4);
  
  TEST_ASSERT_EQUAL(result, 1);
}

void test_tlvGetByteData_should_read_byte_data_from_elf_file_and_updata_the_size_address_and_index(void)
{
  int i = 0;
	uint8_t buffer[1024];
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  buffer[0] = tlvGetByteDataFromElfFile(pElf);
  
  TEST_ASSERT_EQUAL(pElf->codeIndex, 1);
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200001f1);
  TEST_ASSERT_EQUAL_HEX32(pElf->size, 0x000048c7);
  TEST_ASSERT_EQUAL_HEX8(buffer[0], 0x80);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElf);
}

void test_tlvGetByteData_should_update_all_the_information_according_to_how_many_times_it_read(void)
{
  int i = 0;
	uint8_t buffer[1024];
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  buffer[0] = tlvGetByteDataFromElfFile(pElf);
  buffer[1] = tlvGetByteDataFromElfFile(pElf);
  buffer[2] = tlvGetByteDataFromElfFile(pElf);
  buffer[3] = tlvGetByteDataFromElfFile(pElf);
  
  TEST_ASSERT_EQUAL(pElf->codeIndex, 4);
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200001f4);
  TEST_ASSERT_EQUAL_HEX32(pElf->size, 0x000048c4);
  
  TEST_ASSERT_EQUAL_HEX8(buffer[0], 0x80);
  TEST_ASSERT_EQUAL_HEX8(buffer[1], 0xb5);
  TEST_ASSERT_EQUAL_HEX8(buffer[2], 0x94);
  TEST_ASSERT_EQUAL_HEX8(buffer[3], 0xb0);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElf);
}

void test_tlvPackBytesAddress_given_index_0_and_address_0x12345678_buffer_should_contain_the_address_in_bytes(void ) {
  uint8_t buffer[10];
  
  tlvPackBytesAddress(0x12345678, buffer, 0);
  
  TEST_ASSERT_EQUAL_HEX8(buffer[0], 0x12);
  TEST_ASSERT_EQUAL_HEX8(buffer[1], 0x34);
  TEST_ASSERT_EQUAL_HEX8(buffer[2], 0x56);
  TEST_ASSERT_EQUAL_HEX8(buffer[3], 0x78);
}

void test_tlvPackBytesAddress_given_index_4_and_address_0xDEADBEEF_buffer_should_contain_the_address_start_at_position_4(void ) {
  uint8_t buffer[10] = {0};
  
  tlvPackBytesAddress(0xDEADBEEF, buffer, 4);
  
  TEST_ASSERT_EQUAL_HEX8(buffer[0], 0x00);
  TEST_ASSERT_EQUAL_HEX8(buffer[1], 0x00);
  TEST_ASSERT_EQUAL_HEX8(buffer[2], 0x00);
  TEST_ASSERT_EQUAL_HEX8(buffer[3], 0x00);
  
  TEST_ASSERT_EQUAL_HEX8(buffer[4], 0xDE);
  TEST_ASSERT_EQUAL_HEX8(buffer[5], 0xAD);
  TEST_ASSERT_EQUAL_HEX8(buffer[6], 0xBE);
  TEST_ASSERT_EQUAL_HEX8(buffer[7], 0xEF);
  
  TEST_ASSERT_EQUAL_HEX8(buffer[8], 0x00);
  TEST_ASSERT_EQUAL_HEX8(buffer[9], 0x00);
}

void test_tlvGetDataFromElf_data_should_contain_address_data_and_checkSum(void)
{ 
  int result = 0;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  TLV_TypeDef *tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvGetDataFromElf(tlv, pElf);
  
  TEST_ASSERT_EQUAL(result, 1);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  /* Elf section Address and Size*/
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200002E8);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0x47D0);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElf);
}

void test_tlvGetDataFromElf_calling_tlvGetDataFromElf_3_times_should_get_data_and_update_information_correctly(void)
{
  int result = 0;
  TLV_TypeDef *tlv;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvGetDataFromElf(tlv, pElf);
  TEST_ASSERT_EQUAL(result, 1);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvGetDataFromElf(tlv, pElf);
  TEST_ASSERT_EQUAL(result, 1);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  result = tlvGetDataFromElf(tlv, pElf);
  TEST_ASSERT_EQUAL(result, 1);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  /* Elf section Address and Size*/
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200004D8);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0x45E0);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElf);
}