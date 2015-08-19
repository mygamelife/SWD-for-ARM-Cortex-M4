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
#include "mock_CoreDebug_Utilities.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvCreateNewPacket_given_type_length_and_value(void)
{
  int i = 0;
  uint8_t chksum = 0;
  
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE_RAM);
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
                      
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
  
  for(i; i < sizeof(buffer); i++)
    tlv->value[i] = buffer[i];
  
  tlvInsertCheckSum(tlv->value, sizeof(buffer));
  
  TEST_ASSERT_EQUAL_HEX8(0x4c, buffer[19]);
}

void test_tlvCalculateCheckSum_given_index_4_and_length_should_calculate_checksum_start_at_index_4(void)
{
  uint8_t chksum = 0;  
	uint8_t buffer[] = {0xAA, 0xBB, 0xCC, 0xDD,
                      0x12, 0x2C, 0x3A, 0x49, 0x54, 0x60, 0x7F, 0x86, 0x92, 0xA1,
                      0xA4, 0xBC, 0xC5, 0xD9, 0xE4, 0xF0, 0xF1, 0xF2, 0xFF, 0xF6};
  
  tlvInsertCheckSum(buffer, sizeof(buffer));
  
  TEST_ASSERT_EQUAL_HEX8(0x9B, buffer[sizeof(buffer) - 1]);
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
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
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
  
  tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, 249, 0, dataAddress, tlv);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvGetBytesData_call_multiple_times_should_data_should_match(void) {
  TLV *tlv;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  
  tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, 249, 0, dataAddress, tlv);

  dataAddress += TLV_DATA_SIZE;
  tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, 249, 0, dataAddress, tlv);
  
  dataAddress += TLV_DATA_SIZE;
  tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, 249, 0, dataAddress, tlv);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvGetBytesData_given_size_8_should_only_get_8_bytes_data(void) {
  TLV *tlv;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  
  tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
  tlv->length += ADDRESS_LENGTH;
  tlvGetBytesData(dataAddress, tlv, 8);
  
  // TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE_RAM);
  // TEST_ASSERT_EQUAL(tlv->length, 13);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[4], 0x80);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[5], 0xB4);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[6], 0x00);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[7], 0xAF);
  
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[8], 0x00);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[9], 0xaf);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[10], 0x00);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[11], 0x23);
  
  // /* chksum */
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[12], 0xC3);
  
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[13], 0x00);
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, tlv->length, 0, dataAddress, tlv);
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
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  
  tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
  tlvPack4ByteAddress(destAddress, tlv);
  tlvGetBytesData(dataAddress, tlv, TLV_DATA_SIZE);
  tlvPackPacketIntoBuffer(txBuffer, tlv);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, 253, destAddress, dataAddress, tlv);

  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvPackPacketIntoBuffer_should_stop_get_data_from_elf_file_when_size_is_zero(void)
{
  TLV *tlv;
  
  int size = 0;
  uint8_t txBuffer[1024] = {0};
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  int fileSize = getSectionSize(elfData, index);
  
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  
  while(fileSize > 0) {
    size = tlvCheckDataSize(fileSize);
    tlv = tlvCreateNewPacket(TLV_WRITE_RAM);
    tlvPack4ByteAddress(destAddress, tlv);
    tlvGetBytesData(dataAddress, tlv, size);
    tlvPackPacketIntoBuffer(txBuffer, tlv);
    
    TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, tlv->length, destAddress, dataAddress, tlv);
    
    destAddress += size;
    dataAddress += size;
    fileSize -= size;
  }
  
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
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  
  uartSendBytes_IgnoreAndReturn(1024);
  tlvWriteDataChunk(dataAddress, destAddress, TLV_DATA_SIZE, hSerial);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvWriteDataChunk_should_stop_send_chunk_of_data_when_size_is_zero(void)
{
  HANDLE hSerial;
  int i = 0, index = 0, size = 0;
	uint8_t buffer[TLV_DATA_SIZE];
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  index = getIndexOfSectionByName(elfData, ".text");
  int fileSize = getSectionSize(elfData, index);
  uint8_t *dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  
  while(fileSize > 0) {
    size = tlvCheckDataSize(fileSize);
    uartSendBytes_IgnoreAndReturn(1024);
    tlvWriteDataChunk(dataAddress, destAddress, TLV_DATA_SIZE, hSerial);
    fileSize -= size;
    dataAddress += size;
    destAddress += size;
  }
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvWaitReplyFromProbe_expect_probe_reply_PROBE_OK_in_first_try(void)
{
  TlvHost_TypeDef host;  HANDLE hSerial; 
  host.hSerial = hSerial;
  
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
  tlvWaitReplyFromProbe(&host);
}

void test_tlvWaitReplyFromProbe_should_catch_error_after_3_tries_when_TLV_DATA_CORRUPTED(void)
{
  CEXCEPTION_T err;
  TlvHost_TypeDef host;  HANDLE hSerial; 
  host.hSerial = hSerial;
  
  Try {
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    uartSendBytes_IgnoreAndReturn(1);
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    
    tlvWaitReplyFromProbe(&host);
    TEST_FAIL_MESSAGE("Should throw an error due to ERR_DATA_CORRUPTED");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL_MESSAGE(ERR_DATA_CORRUPTED, err, "ERR_DATA_CORRUPTED");
  }
}

  /********************************************
  *                 TLV-HOST                  *
  *                                           *
  *********************************************/
void test_tlvWriteRam_should_stop_transmit_data_when_probe_is_not_reply()
{
  TlvHost_TypeDef host;
  HANDLE hSerial;
  CEXCEPTION_T err;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  host.fileSize = 248;
  host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  host.hSerial = hSerial;
  
  /* Send Instruction and wait for reply */
  uartSendByte_ExpectAndReturn(hSerial, TLV_START_TRANSMISSION, 1);
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
  
  /* Send data to probe */
  uartSendBytes_IgnoreAndReturn(1);
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);

  /* Wait probe reply */
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
  uartSendByte_ExpectAndReturn(hSerial, TLV_END_TRANSMISSION, 1);  
  tlvWriteRam(&host);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvWriteRam_given_TLV_DATA_CORRUPTED_should_retry_3_times()
{
  TlvHost_TypeDef host;
  HANDLE hSerial;
  CEXCEPTION_T err;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  host.fileSize = getSectionSize(elfData, index);
  host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  host.hSerial = hSerial;
  
  Try {
    /* Send Instruction and wait for reply */
    uartSendByte_ExpectAndReturn(hSerial, TLV_START_TRANSMISSION, 1);
    uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
      
    /* Send data to probe */
    uartSendBytes_IgnoreAndReturn(1);
      
    /* Wait probe reply */
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    uartSendBytes_IgnoreAndReturn(1);
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
    uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
      
    tlvWriteRam(&host);
    TEST_FAIL_MESSAGE("Should throw an error due to ERR_DATA_CORRUPTED");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL_MESSAGE(ERR_DATA_CORRUPTED, err, "Expect ERR_DATA_CORRUPTED");
  }
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvWriteRam_should_stop_retry_if_probe_reply_ok()
{
  TlvHost_TypeDef host;
  HANDLE hSerial;
  CEXCEPTION_T err;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  host.fileSize = 248 + 248 + 248;
  host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  uint32_t destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  host.hSerial = hSerial;
  
  /* Send Instruction and wait for reply */
  uartSendByte_ExpectAndReturn(hSerial, TLV_START_TRANSMISSION, 1);
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
    
  /* Send data to probe */  
  uartSendBytes_IgnoreAndReturn(1);  
  uartGetByte_ExpectAndReturn(hSerial, TLV_DATA_CORRUPTED);
  
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK); 
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
  
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
  
  uartSendBytes_IgnoreAndReturn(1);  
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);

  uartSendByte_ExpectAndReturn(hSerial, TLV_END_TRANSMISSION, 1);
  
  tlvWriteRam(&host);

  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvWriteRam_should_send_end_transmission_when_file_size_is_zero(void)
{
  TlvHost_TypeDef host;
  HANDLE hSerial;
  CEXCEPTION_T err;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  host.fileSize = 0;
  host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);

  host.hSerial = hSerial;
  
  /* Send Instruction and wait for reply */
  uartSendByte_ExpectAndReturn(hSerial, TLV_START_TRANSMISSION, 1);
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
    
  /* Wait probe reply */
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
  uartSendByte_ExpectAndReturn(hSerial, TLV_END_TRANSMISSION, 1);
  
  tlvWriteRam(&host);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_verifyReadDataFromRam_should_send_stop_if_data_is_not_match()
{
  TlvHost_TypeDef host;
  HANDLE hSerial;
  CEXCEPTION_T err;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  host.fileSize = getSectionSize(elfData, index);
  host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  host.hSerial = hSerial;

  verifyReadDataFromRam(&host, 256);
  
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_tlvReadRam_should_send_tlv_Read_Command_contain_size_and_address()
{
  TlvHost_TypeDef host;
  HANDLE hSerial;
  CEXCEPTION_T err;
  
  ElfData *elfData = openElfFile("test/ELF_File/FlashProgrammer.elf");
  int index = getIndexOfSectionByName(elfData, ".text");
  host.fileSize = 248;
  host.dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  host.destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  host.hSerial = hSerial;
  
  /* Send Instruction and wait for reply */
  uartSendByte_ExpectAndReturn(hSerial, TLV_START_TRANSMISSION, 1);
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
  
  /* Send Request */
  uartSendBytes_ExpectAndReturn(host.hSerial, host.txBuffer, ONE_BYTE, 255);
  /* Receive data sent from probe */
  host.rxBuffer[0] = TLV_SEND;
  uartGetBytes_ExpectAndReturn(host.hSerial, host.rxBuffer, ONE_BYTE, 0);
  
  /* Wait probe reply */
  uartGetByte_ExpectAndReturn(hSerial, PROBE_OK);
  uartSendByte_ExpectAndReturn(hSerial, TLV_END_TRANSMISSION, 1);
  
  tlvReadRam(&host);
  // TEST_ASSERT_EQUAL(TLV_READ, host.txBuffer[0]);
  // TEST_ASSERT_EQUAL(5, host.txBuffer[1]);
  // TEST_ASSERT_EQUAL(TLV_READ, host.txBuffer[0]);
  closeFileInTxt(elfData->myFile);
  free(elfData);
}

void test_createTlvPacket_should_create_packet_given_TLV_WRITE_RAM_size_5_and_data()
{ 
  uint8_t buffer[] = {0xA, 0xB, 0xC, 0xD};
  
  TLV *tlv = createTlvPacket(TLV_WRITE_RAM, 4, buffer);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, tlv->type);
  TEST_ASSERT_EQUAL(5, tlv->length);
  TEST_ASSERT_EQUAL(0x0A0B0C0D, get4Byte(&buffer[0]));
}

void test_tlvSend_should_able_to_send_tlv_packet_using_UART()
{ 
  HANDLE hSerial;
  initSerialComm_IgnoreAndReturn(hSerial);
  TLV_Session *session = createTLVSession();
  uint8_t buffer[] = {0xA, 0xB, 0xC, 0xD};
  TLV *tlv = createTlvPacket(TLV_WRITE_RAM, 1, buffer);
  
  uartSendBytes_ExpectAndReturn(session->hSerial, (uint8_t *)tlv, ONE_BYTE, ONE_BYTE);
  tlvSend(session, tlv);
}

void test_tlvReceive_should_receive_tlv_packet_send_from_others()
{ 
  HANDLE hSerial;
  initSerialComm_IgnoreAndReturn(hSerial);
  TLV_Session *session = createTLVSession();
  
  session->rxBuffer[0] = TLV_READ; //type
  session->rxBuffer[1] = 0x5; //data
  session->rxBuffer[2] = 0x1; //data
  session->rxBuffer[3] = 0x2; //data
  session->rxBuffer[4] = 0x3; //data
  session->rxBuffer[5] = 0x4; //data
  session->rxBuffer[6] = 0xF5; //data
  
  uartGetBytes_ExpectAndReturn(session->hSerial, session->rxBuffer, 2, 2);
  uartGetBytes_ExpectAndReturn(session->hSerial, session->rxBuffer, 5, 5); 
  
  TLV *tlv = tlvReceive(session);

  TEST_ASSERT_EQUAL(TLV_READ, tlv->type);
  TEST_ASSERT_EQUAL(0x5, session->rxBuffer[1]);
  TEST_ASSERT_EQUAL(0x1, session->rxBuffer[2]);
  TEST_ASSERT_EQUAL(0x2, session->rxBuffer[3]);
  TEST_ASSERT_EQUAL(0x3, session->rxBuffer[4]);
  TEST_ASSERT_EQUAL(0x4, session->rxBuffer[5]);
  TEST_ASSERT_EQUAL(0xF5, session->rxBuffer[6]);
}

void test_tlvReadTargetRegister_should_send_register_address_by_using_tlv_packet()
{ 
  HANDLE hSerial;
  initSerialComm_IgnoreAndReturn(hSerial);
  TLV_Session *session = createTLVSession();

  uint8_t address[] = {CORE_REG_PC};
  TLV *tlv = createTlvPacket(TLV_READ_REGISTER, 4, address);
  uartSendBytes_ExpectAndReturn(session->hSerial, (uint8_t *)tlv, ONE_BYTE, 5);
  
  uartGetBytes_ExpectAndReturn(session->hSerial, session->rxBuffer, 2, 2);
  uartGetBytes_ExpectAndReturn(session->hSerial, session->rxBuffer, tlv->length, 4);
  
  tlvReadTargetRegister(session, CORE_REG_PC);
}

void test_tlvWriteTargetRegister_should_write_to_given_data_into_specified_register()
{ 
  HANDLE hSerial;
  initSerialComm_IgnoreAndReturn(hSerial);
  TLV_Session *session = createTLVSession();

  uint8_t data[] = {CORE_REG_SR, 0xBE, 0xEF, 0xCA, 0xFE}; //address and data
  
  TLV *tlv = createTlvPacket(TLV_WRITE_REGISTER, 8, data);
  
  uartSendBytes_ExpectAndReturn(session->hSerial, (uint8_t *)tlv, ONE_BYTE, ONE_BYTE);
  
  session->rxBuffer[1] = 0;
  uartGetBytes_ExpectAndReturn(session->hSerial, session->rxBuffer, 2, 2);
  
  tlvWriteTargetRegister(session, CORE_REG_SR, 0xBEEFCAFE);
}

void test_tlvHaltTarget_should_send_command_to_probe_to_halt_target(void)
{ 
  HANDLE hSerial;
  initSerialComm_IgnoreAndReturn(hSerial);
  TLV_Session *session = createTLVSession();

  uint8_t data[] = {0xA0, 0x5F, 0x00, 0x03}; //halt core
  
  TLV *tlv = createTlvPacket(TLV_HALT_TARGET, 4, data);
  
  uartSendBytes_ExpectAndReturn(session->hSerial, (uint8_t *)tlv, ONE_BYTE, ONE_BYTE);

  session->rxBuffer[1] = 0;
  uartGetBytes_ExpectAndReturn(session->hSerial, session->rxBuffer, 2, 2);
  
  tlvHaltTarget(session);
}

void test_tlvRunTarget_should_send_command_to_probe_to_run_target(void)
{ 
  HANDLE hSerial;
  initSerialComm_IgnoreAndReturn(hSerial);
  TLV_Session *session = createTLVSession();

  uint8_t data[] = {0xA0, 0x5F, 0x00, 0x00}; //run core
  
  TLV *tlv = createTlvPacket(TLV_HALT_TARGET, 4, data);
  
  uartSendBytes_ExpectAndReturn(session->hSerial, (uint8_t *)tlv, ONE_BYTE, ONE_BYTE);
  session->rxBuffer[1] = 0;
  uartGetBytes_ExpectAndReturn(session->hSerial, session->rxBuffer, 2, 2);
  
  tlvRunTarget(session);
}

void test_tlvStep_given_step_4_instruction_should_pass_to_probe(void)
{ 
  HANDLE hSerial;
  initSerialComm_IgnoreAndReturn(hSerial);
  TLV_Session *session = createTLVSession();

  uint8_t data[] = {4};
  
  TLV *tlv = createTlvPacket(TLV_HALT_TARGET, 4, data);
  
  uartSendBytes_ExpectAndReturn(session->hSerial, (uint8_t *)tlv, ONE_BYTE, ONE_BYTE);
  session->rxBuffer[1] = 0;
  uartGetBytes_ExpectAndReturn(session->hSerial, session->rxBuffer, 2, 2);  
  tlvStep(session, 4);
}
