#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "TLV_Protocol.h"
#include "GetHeaders.h"
#include "Read_File.h"
#include "CException.h"
#include "ErrorCode.h"
#include "ProgramElf.h"

void setUp(void)
{
  initElfData();
}

void tearDown(void)
{
}

void test_tlvCalculateCheckSum_given_data_and_length_should_calc_check_sum(void)
{
  uint8_t chksum = 0;
  
	uint8_t buffer[] = {0x10, 0x24, 0x62, 0x00, 0x46, 0x4C, 0x55, 0x49, 
                      0x44, 0x20, 0x50, 0x52, 0x4F, 0x46, 0x49, 0x4C,
                      0x45, 0x00, 0x46, 0x4C};
  
  chksum = tlvCalculateCheckSum(buffer, sizeof(buffer));
  
  TEST_ASSERT_EQUAL(chksum, 0x33);
}

void test_tlvCreatePacket_given_type_length_and_value()
{
  int i = 0;
  uint8_t chksum = 0;
  uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
  
  TLV_TypeDef *tlv = tlvCreatePacket(TLV_WRITE, sizeof(data), data);
  
  TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE);
  TEST_ASSERT_EQUAL(tlv->length, 5);
  TEST_ASSERT_EQUAL(tlv->value[0], 0x01);
  TEST_ASSERT_EQUAL(tlv->value[1], 0x02);
  TEST_ASSERT_EQUAL(tlv->value[2], 0x03);
  TEST_ASSERT_EQUAL(tlv->value[3], 0x04);
  TEST_ASSERT_EQUAL(tlv->value[4], 0x05);
}

void test_tlvPackPacketIntoTxBuffer_should_store_packet_info_into_buffer()
{
  int size = 0;
  uint8_t txBuffer[1024];
  uint8_t data[10] = {0x22, 0x33, 0x44, 0x55, 0x66,
                      0x77, 0x88, 0x99, 0xAA, 0xBB};
                      
  TLV_TypeDef *tlv = tlvCreatePacket(TLV_WRITE, sizeof(data), data);

  tlvPackPacketIntoTxBuffer(txBuffer, tlv);
  
  TEST_ASSERT_EQUAL(txBuffer[0], TLV_WRITE);  //type
  TEST_ASSERT_EQUAL(txBuffer[1], 13); //length
  TEST_ASSERT_EQUAL(txBuffer[2], 0x22); 
  TEST_ASSERT_EQUAL(txBuffer[3], 0x33);
  TEST_ASSERT_EQUAL(txBuffer[4], 0x44);
  TEST_ASSERT_EQUAL(txBuffer[5], 0x55);
  TEST_ASSERT_EQUAL(txBuffer[6], 0x66); //data
  TEST_ASSERT_EQUAL(txBuffer[7], 0x77);
  TEST_ASSERT_EQUAL(txBuffer[8], 0x88);
  TEST_ASSERT_EQUAL(txBuffer[9], 0x99);
  TEST_ASSERT_EQUAL(txBuffer[10], 0xAA);
  TEST_ASSERT_EQUAL(txBuffer[11], 0xBB);
  TEST_ASSERT_EQUAL(txBuffer[12], 0xAF); //chksum
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
}

void test_tlvCreateDataBuffer_given_buffer_and_size(void ) {
  uint8_t buffer[200] = {0x10, 0x20};
  TLV_DataBuffer *tlvDataBuffer;
  
  tlvDataBuffer = tlvCreateDataBuffer(buffer, sizeof(buffer));
  
  TEST_ASSERT_EQUAL(tlvDataBuffer->data[0], 0x10);
  TEST_ASSERT_EQUAL(tlvDataBuffer->data[1], 0x20);
  TEST_ASSERT_EQUAL(tlvDataBuffer->length, 200);
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

void test_tlvPutBytesIntoBuffer_given_address_0x200001f0_and_data_0x80b594b0_should_get_exactly_the_same(void)
{
  int result = 0;
  uint8_t buffer[8], buffer2[8] = {0x20, 0x00, 0x01, 0xf0, 0x80, 0xb5, 0x94, 0xb0};
  uint16_t fileSize = 0;
  
  TLV_DataBuffer *tlvDataBuffer = tlvCreateDataBuffer(buffer, sizeof(buffer));
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  result = tlvPutDataIntoBuffer(tlvDataBuffer, pElf);
  
  TEST_ASSERT_EQUAL(result, 1);
  
  /* Address */
  TEST_ASSERT_EQUAL_HEX8(tlvDataBuffer->data[0], buffer2[0]);
  TEST_ASSERT_EQUAL_HEX8(tlvDataBuffer->data[1], buffer2[1]);
  TEST_ASSERT_EQUAL_HEX8(tlvDataBuffer->data[2], buffer2[2]);
  TEST_ASSERT_EQUAL_HEX8(tlvDataBuffer->data[3], buffer2[3]);
  
  /* Data */
  TEST_ASSERT_EQUAL_HEX8(tlvDataBuffer->data[4], buffer2[4]);
  TEST_ASSERT_EQUAL_HEX8(tlvDataBuffer->data[5], buffer2[5]);
  TEST_ASSERT_EQUAL_HEX8(tlvDataBuffer->data[6], buffer2[6]);
  TEST_ASSERT_EQUAL_HEX8(tlvDataBuffer->data[7], buffer2[7]);
  
  closeFileInTxt(dataFromElf->myFile);
}

void test_tlvPutBytesIntoBuffer_should_update_address_and_size_every_time_putIntoBuffer(void)
{
  int result = 0;
  uint8_t buffer[8];
  uint16_t fileSize = 0;
  
  TLV_DataBuffer *tlvDataBuffer = tlvCreateDataBuffer(buffer, sizeof(buffer));
  pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  result = tlvPutDataIntoBuffer(tlvDataBuffer, pElfSection);
  
  TEST_ASSERT_EQUAL(result, 1);

  TEST_ASSERT_EQUAL_HEX32(pElfSection->address, 0x200001f4);
  TEST_ASSERT_EQUAL_HEX16(pElfSection->size, 0x48C4);
  
  TLV_DataBuffer *tlvDataBuffer2 = tlvCreateDataBuffer(buffer, sizeof(buffer));
  tlvPutDataIntoBuffer(tlvDataBuffer2, pElfSection);
  
  TEST_ASSERT_EQUAL_HEX32(pElfSection->address, 0x200001f8);
  TEST_ASSERT_EQUAL_HEX16(pElfSection->size, 0x48C0);
  
  closeFileInTxt(dataFromElf->myFile);
}

void test_tlvPutBytesIntoBuffer_should_return_0_when_all_data_is_transfer(void)
{
  int result = 0, i = 0;
  uint8_t buffer[204];
  uint16_t fileSize = 0;
  TLV_DataBuffer *tlvDataBuffer;
  
  pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  for(i; i < 95; i ++)  {
    tlvDataBuffer = tlvCreateDataBuffer(buffer, sizeof(buffer));
    result = tlvPutDataIntoBuffer(tlvDataBuffer, pElfSection);
  }
  
  TEST_ASSERT_EQUAL(result, 0);
  TEST_ASSERT_EQUAL_HEX16(pElfSection->size, 0);
  TEST_ASSERT_EQUAL_HEX32(pElfSection->address, 0x20004ab8);
  
  closeFileInTxt(dataFromElf->myFile);
}

void test_tlvPackPacketIntoTxBuffer_should_get_data_from_elf_file_then_pack_into_txBuffer(void)
{
  int result = 0;
  uint8_t txBuffer[1024], data[204];
  
  pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  TLV_DataBuffer *tlvDataBuffer = tlvCreateDataBuffer(data, sizeof(data));
  TEST_ASSERT_EQUAL(tlvDataBuffer->length, 204);
  
  result = tlvPutDataIntoBuffer(tlvDataBuffer, pElfSection);
  TEST_ASSERT_EQUAL(result, 1);
  
  TLV_TypeDef *tlv = tlvCreatePacket(TLV_WRITE, sizeof(data), data);
  tlvPackPacketIntoTxBuffer(txBuffer, tlv);
  
  closeFileInTxt(dataFromElf->myFile);
}