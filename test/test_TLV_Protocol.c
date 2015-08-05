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
#include "mock_Serial.h"
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
  
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  
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
                      
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  
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
  free(dataFromElf);
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
  free(dataFromElf);
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
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetDataFromElf(tlv, pElf);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  /* Elf section Address and Size*/
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200002E8);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0x47D0);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElf);
  free(dataFromElf);
}

void test_tlvGetDataFromElf_calling_tlvGetDataFromElf_3_times_should_get_data_and_update_information_correctly(void)
{
  TLV *tlv;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetDataFromElf(tlv, pElf);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetDataFromElf(tlv, pElf);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetDataFromElf(tlv, pElf);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  /* Elf section Address and Size*/
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x200004D8);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0x45E0);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElf);
  free(dataFromElf);
}

void test_tlvGetDataFromElf_get_all_the_data_from_elf_file_until_size_is_0(void)
{
  TLV *tlv;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");

  while(pElf->size != 0) {
    tlv = tlvCreateNewPacket(TLV_WRITE);
    tlvGetDataFromElf(tlv, pElf);
    TEST_ASSERT_EQUAL_TLV(TLV_WRITE, tlv->length, pElf, tlv);
    TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  }

  /* Elf section Address and Size*/
  TEST_ASSERT_EQUAL_HEX32(pElf->address, 0x20004ab8);
  TEST_ASSERT_EQUAL_HEX16(pElf->size, 0x00);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElf);
  free(dataFromElf);
}

void test_tlvPackPacketIntoTxBuffer_should_get_data_from_elf_file_then_pack_into_txBuffer(void)
{
  int i = 0;
  uint8_t txBuffer[1024], chksum = 0;
  
  ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  tlvGetDataFromElf(tlv, pElfSection);  
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, DATA_SIZE, pElf, tlv);
  TEST_ASSERT_EQUAL(tlvVerifyCheckSum(tlv->value, tlv->length, 4), 1);
  
  tlvPackPacketIntoTxBuffer(txBuffer, tlv);
  
  /* Type */
  TEST_ASSERT_EQUAL(txBuffer[0], TLV_WRITE);
  
  /* Length */
  TEST_ASSERT_EQUAL(txBuffer[1], tlv->length);
  
  /* Address */
  TEST_ASSERT_EQUAL(txBuffer[2], 0x20);
  TEST_ASSERT_EQUAL(txBuffer[3], 0x00);
  TEST_ASSERT_EQUAL(txBuffer[4], 0x01);
  TEST_ASSERT_EQUAL(txBuffer[5], 0xf0);
  
  /* Data */
  for(i += 4; i < tlv->length - 1; i++)
    TEST_ASSERT_EQUAL(txBuffer[i + 2], tlv->value[i]);
  
  TEST_ASSERT_EQUAL_HEX8(txBuffer[254], tlv->value[DATA_SIZE - 1]);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
  free(dataFromElf);
}

void test_tlvGetWordAddress_given_address_DE_AD_BE_EF_should_get_0xDEADBEEF_address(void ) {
  uint8_t buffer[4] = {0xDE, 0xAD, 0xBE, 0xEF};
  
  uint32_t address32 = tlvGetWordAddress(buffer, 0);
  
  TEST_ASSERT_EQUAL_HEX32(address32, 0xDEADBEEF);
}

void test_tlvDecodePacket_given_require_info_inside_rxBuffer(void)
{
  int result = 0;
  uint8_t chksum = 0;
  uint8_t rxBuffer[1024] = {TLV_WRITE,  //type
                            13,       //length
                            0xAA, 0xBB, 0xCC, 0xDD, //address
                            0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF1, 0xC1, 0xB7, //data
                            0xC8}; //chekcsum
  
  TLV *tlv = tlvDecodePacket(rxBuffer);
  
  /* Type */
  TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE);
  
  /* Length */
  TEST_ASSERT_EQUAL(tlv->length, 13);
  
  /* Address */
  TEST_ASSERT_EQUAL_HEX32(tlv->decodeAddress, 0xAABBCCDD);
  
  /* Data */
  TEST_ASSERT_EQUAL_HEX8(tlv->value[0], 0xA1);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[1], 0xB2);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[2], 0xC3);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[3], 0xD4);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[4], 0xE5);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[5], 0xF1);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[6], 0xC1);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[7], 0xB7);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[8], 0xC8); //chksum

  chksum = tlvVerifyCheckSum(tlv->value, tlv->length - ADDRESS_LENGTH, 0);
  TEST_ASSERT_EQUAL(chksum, 1);
}

void test_tlvDecodePacket_should_decode_after_receive_TLV_Packet(void)
{
  uint8_t txBuffer[1024], chksum = 0;
  
  pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  /* Create Packet */
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  /* Put Data into Buffer */
  tlvGetDataFromElf(tlv, pElfSection);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 253, pElfSection, tlv);
  
  /* Pack into TXBUFFER */
  tlvPackPacketIntoTxBuffer(txBuffer, tlv);
  
  /* Decode txBuffer */
  TLV *tlvDecode = tlvDecodePacket(txBuffer);
  
  /* Type */
  TEST_ASSERT_EQUAL(tlvDecode->type, TLV_WRITE);
  
  /* Length */
  TEST_ASSERT_EQUAL(tlvDecode->length, 253);
  
  /* Address */
  TEST_ASSERT_EQUAL_HEX32(tlvDecode->decodeAddress, 0x200001F0);
  
  /* Fist Data */
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[0], 0x80);
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[1], 0xb5);
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[2], 0x94);
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[3], 0xb0);
  
  /* Last Data */
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[244], 0x80);
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[245], 0xBD);
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[246], 0x00);
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[247], 0xBF);
  
  TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[248], 0x02); //checksum
  
  /* Checksum */
  chksum = tlvVerifyCheckSum(tlvDecode->value, tlvDecode->length - ADDRESS_LENGTH, 0);
  TEST_ASSERT_EQUAL(chksum, 1);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
  free(dataFromElf);
}

/**********************************************
  *             TLV-HOST                      *
  *********************************************
  */
void test_tlvHost_given_TLV_START_should_create_packet_and_pack_into_Tx_Buffer_and_change_state_to_TLV_TRANSMIT_DATA()
{
  HANDLE hSerial;
  TLVSession tlvSession;
  
  /* Open elf file */
  pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  /* Initialize TlvSeesion structure */
  tlvSession.state = TLV_START;
  tlvSession.pElf = pElfSection;
  tlvSession.hSerial = hSerial;
  
  /* Function call */
  tlvHost(&tlvSession);
  
  TEST_ASSERT_EQUAL(tlvSession.state, TLV_TRANSMIT_DATA);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
  free(dataFromElf);
}

// void test_tlvHost_when_state_changed_to_TLV_TRANSMIT_DATA_should_transmit_all_the_data_inside_txBUFFER_to_probe()
// {
  // HANDLE hSerial;
  // TLVSession tlvSession;
  
  // /* Open elf file */
  // pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  // /* Initialize TlvSeesion structure */
  // tlvSession.state = TLV_START;
  // tlvSession.pElf = pElfSection;
  // tlvSession.hSerial = hSerial;
  
  // /* Mock */
  // serialWriteByte_IgnoreAndReturn(255);
  
  // /* Function call */
  // tlvHost(&tlvSession);
  // tlvHost(&tlvSession);
  
  // /* Test */
  // TEST_ASSERT_EQUAL(tlvSession.state, TLV_WAIT_REPLY);
  
  // closeFileInTxt(dataFromElf->myFile);
  // free(pElfSection);
  // free(dataFromElf);
// }

// void test_tlvHost_when_state_changed_to_TLV_WAIT_REPLY_should_transmit_all_txbuffer_data_and_wait_probe_reply()
// {
  // HANDLE hSerial;
  // TLVSession tlvSession;
  
  // /* Open elf file */
  // pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  // /* Initialize TlvSeesion structure */
  // tlvSession.state = TLV_START;
  // tlvSession.pElf = pElfSection;
  // tlvSession.hSerial = hSerial;
  
  // /* Mock */
  // serialWriteByte_IgnoreAndReturn(255);
  // serialGetByte_ExpectAndReturn(tlvSession.hSerial, PROBE_OK);
  
  // /* Function call */
  // tlvHost(&tlvSession);
  // tlvHost(&tlvSession);
  // tlvHost(&tlvSession);
  
  // /* Test */
  // TEST_ASSERT_EQUAL(tlvSession.state, TLV_START);
  
  // closeFileInTxt(dataFromElf->myFile);
  // free(pElfSection);
  // free(dataFromElf);
// }