#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <malloc.h>
#include "TLV_Host.h"
#include "GetHeaders.h"
#include "Read_File.h"
#include "CException.h"
#include "ErrorCode.h"
#include "ProgramElf.h"
#include "mock_Serial.h"
#include "CustomAssertion.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvCreateNewPacket_given_type_length_and_value(void)
{
  int i = 0;
  uint8_t chksum = 0;
  
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE);
  TEST_ASSERT_EQUAL(tlv->length, 1);
  
  for(i; i < TLV_DATA_SIZE; i++)
    TEST_ASSERT_EQUAL_HEX8(0x00, tlv->value[i]);
}

void test_tlvCalculateCheckSum_given_data_and_length_should_calc_check_sum(void)
{
  int i = 0;
  uint8_t chksum = 0;
	uint8_t buffer[] = {0x10, 0x24, 0x62, 0x00, 0x46, 0x4C, 0x55, 0x49, 
                      0x44, 0x20, 0x50, 0x52, 0x4F, 0x46, 0x49, 0x4C,
                      0x45, 0x00, 0x46, 0x4C};
                      
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  for(i; i < sizeof(buffer); i++)
    tlv->value[i] = buffer[i];
  
  chksum = tlvCalculateCheckSum(tlv->value, sizeof(buffer), 0);
  
  TEST_ASSERT_EQUAL_HEX8(0x33, chksum);
}

void test_tlvCalculateCheckSum_given_index_4_and_length_should_calculate_checksum_start_at_index_4(void)
{
  uint8_t chksum = 0;  
	uint8_t buffer[] = {0xAA, 0xBB, 0xCC, 0xDD,
                      0x12, 0x2C, 0x3A, 0x49, 0x54, 0x60, 0x7F, 0x86, 0x92, 0xA1,
                      0xA4, 0xBC, 0xC5, 0xD9, 0xE4, 0xF0, 0xF1, 0xF2, 0xFF, 0xF6};
  
  chksum = tlvCalculateCheckSum(buffer, sizeof(buffer), 4);
  
  TEST_ASSERT_EQUAL_HEX8(0xA9, chksum);
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
  
  TEST_ASSERT_EQUAL(1, result);
}

void test_tlvPack4ByteAddress_given_index_0_and_address_0x12345678_buffer_should_contain_the_address_in_bytes(void) {
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvPack4ByteAddress(0x12345678, tlv);
  
  TEST_ASSERT_EQUAL_HEX8(0x12, tlv->value[0]);
  TEST_ASSERT_EQUAL_HEX8(0x34, tlv->value[1]);
  TEST_ASSERT_EQUAL_HEX8(0x56, tlv->value[2]);
  TEST_ASSERT_EQUAL_HEX8(0x78, tlv->value[3]);
  
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&tlv->value[0]));
}

void test_tlvGetBytesData_should_read_248_bytes_from_dataAddress(void) {
  TLV *tlv;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 249, 0, dataAddress, tlv);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvGetBytesData_call_multiple_times_should_data_should_match(void) {
  TLV *tlv;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 249, 0, dataAddress, tlv);
  
  dataAddress += TLV_DATA_SIZE;
  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 249, 0, dataAddress, tlv);
  
  dataAddress += TLV_DATA_SIZE;
  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 249, 0, dataAddress, tlv);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvGetBytesData_given_size_8_should_only_get_8_bytes_data(void) {
  TLV *tlv;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t *destAddress = (uint32_t *)getSectionVirtualAddress(elfData, 0);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlv->length += ADDRESS_LENGTH;
  tlvGetBytesData(dataAddress, tlv, 8);
  
  TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE);
  TEST_ASSERT_EQUAL(tlv->length, 13);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[4], 0x80);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[5], 0xB5);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[6], 0x86);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[7], 0xB0);
  
  TEST_ASSERT_EQUAL_HEX8(tlv->value[8], 0x00);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[9], 0xaf);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[10], 0x00);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[11], 0x23);
  
  //chksum
  TEST_ASSERT_EQUAL_HEX8(tlv->value[12], 0xC3);
  
  TEST_ASSERT_EQUAL_HEX8(tlv->value[13], 0x00);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvPackPacketIntoBuffer_should_get_data_from_elf_file_then_pack_into_txBuffer(void)
{
  uint8_t txBuffer[1024] = {0};
  uint32_t index32 = 0;
  TLV *tlv;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t *destAddress = (uint32_t *)getSectionVirtualAddress(elfData, 0);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvPack4ByteAddress((uint32_t)destAddress, tlv);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  tlvPackPacketIntoBuffer(txBuffer, tlv);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 253, get4Byte(&txBuffer[2]), &txBuffer[6], tlv);

  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvWriteDataChunk_should_send_chunk_of_data_using_UART(void)
{
  HANDLE hSerial;
  int i = 0, index = 0, size = 0;
	uint8_t buffer[TLV_DATA_SIZE];
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  
  index = getIndexOfSectionByName(elfData, ".text");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t *destAddress = (uint32_t *)getSectionVirtualAddress(elfData, 0);
  
  uartSendBytes_IgnoreAndReturn(1024);
  tlvWriteDataChunk(dataAddress, destAddress, TLV_DATA_SIZE, hSerial);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

  /********************************************
  *                 TLV-HOST                  *
  *                                           *
  *********************************************/
void test_tlvWriteRam_should_transmit_chunk_data_in_size_of_248_bytes()
{
  HANDLE hSerial;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  
  int index = getIndexOfSectionByName(elfData, ".text");
  int size = getSectionSize(elfData, index);
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t *destAddress = (uint32_t *)getSectionVirtualAddress(elfData, 0);
  
  /* Init uart */
  initSerialComm_IgnoreAndReturn(hSerial);
  uartSendBytes_IgnoreAndReturn(1024);
  uartGetByte_IgnoreAndReturn(PROBE_OK);
  closeSerialPort_Ignore();
  
  tlvWriteRam(dataAddress, destAddress, size);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

// void test_tlvCheckAcknowledge_given_PROBE_OK_should_change_state_to_TLV_START()
// {
  // uint8_t state;
  
  // state = tlvCheckAcknowledge(PROBE_OK);
  
  // TEST_ASSERT_EQUAL(state, TLV_START);
// }

// void test_tlvCheckAcknowledge_received_3_times_PROBE_FAULT_should_change_state_to_TLV_ABORT()
// {
  // uint8_t state;
  
  // state = tlvCheckAcknowledge(PROBE_FAULT);
  // TEST_ASSERT_EQUAL(state, TLV_TRANSMIT_DATA);
  // state = tlvCheckAcknowledge(PROBE_FAULT);
  // TEST_ASSERT_EQUAL(state, TLV_TRANSMIT_DATA);
  // state = tlvCheckAcknowledge(PROBE_FAULT);
  // TEST_ASSERT_EQUAL(state, TLV_TRANSMIT_DATA);
  // state = tlvCheckAcknowledge(PROBE_FAULT);
  // TEST_ASSERT_EQUAL(state, TLV_ABORT);
// }

// void test_tlvCheckAcknowledge_when_received_PROBE_COMPLETE_should_change_state_to_TLV_COMPLETE()
// {
  // uint8_t state;
  
  // state = tlvCheckAcknowledge(PROBE_COMPLETE);
  // TEST_ASSERT_EQUAL(state, TLV_COMPLETE);
// }