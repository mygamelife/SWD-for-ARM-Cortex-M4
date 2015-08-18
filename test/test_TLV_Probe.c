#include "unity.h"
#include "TLV_Probe.h"
#include "TLV_State.h"
#include <stdio.h>
#include "Delay.h"
#include "Emulator.h"
#include "mock_UART.h"
#include "Clock.h"
#include "mock_swdStub.h"
#include "swd_Utilities.h"
#include "mock_configurePort.h"
#include "mock_IO_Operations.h"
#include "mock_stm32f4xx_hal_uart.h"
#include "mock_Register_ReadWrite.h"
#include "Bit_Operations.h"
#include "mock_stm32f429i_discovery.h"

uint32_t readDummy = 0xFFFFFFFF;

void setUp(void)  {}

void tearDown(void) {}

void test_load_SectorErase_Instruction_should_wait_untill_target_response_OK_before_load_instruction(void)
{
  uint32_t status = 0;
  memoryReadAndReturnWord_ExpectAndReturn(SWD_TARGET_STATUS, TARGET_OK);
  
  /* load flash start and end address to sram */
  memoryWriteWord_Expect(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_20);
  memoryWriteWord_Expect(SWD_FLASH_END_ADDRESS, ADDR_FLASH_SECTOR_22);
  
  // /* load instruction to sram */
  memoryWriteWord_Expect(SWD_INSTRUCTION, INSTRUCTION_ERASE_SECTOR);
  
  loadEraseSectorInstruction((uint32_t *)ADDR_FLASH_SECTOR_20, (uint32_t *)ADDR_FLASH_SECTOR_22);
}

void test_loadMassEraseInstruction_should_wait_untill_target_response_OK_before_load_instruction(void)
{
  memoryReadAndReturnWord_ExpectAndReturn(SWD_TARGET_STATUS, TARGET_BUSY);
  memoryReadAndReturnWord_ExpectAndReturn(SWD_TARGET_STATUS, TARGET_OK);
  
  /* load bank select to sram */
  memoryWriteWord_Expect(SWD_BANK_SELECT, FLASH_BANK_BOTH);
  
  /* load instruction to sram */
  memoryWriteWord_Expect(SWD_INSTRUCTION, INSTRUCTION_MASS_ERASE);  
  
  loadMassEraseInstruction(FLASH_BANK_BOTH);
}

void test_loadCopyInstruction_should_load_src_address_dest_address_and_length_into_SRAM_instruction_address(void)
{
  memoryReadAndReturnWord_ExpectAndReturn(SWD_TARGET_STATUS, TARGET_OK);
  
  /* load SRAM start address into sram */
  memoryWriteWord_Expect(SWD_SRAM_START_ADDRESS, 0x200001F0);
  
  /* load Flash start address into sram */
  memoryWriteWord_Expect(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_18);
  
  /* load length into sram */
  memoryWriteWord_Expect(SWD_DATA_SIZE, 2000);

	/* load copy instructoin into sram */
  memoryWriteWord_Expect(SWD_INSTRUCTION, INSTRUCTION_COPY);
  
  loadCopyFromSRAMToFlashInstruction((uint32_t *)0x200001F0, (uint32_t *)ADDR_FLASH_SECTOR_18, 2000);
}

void xtest_waitIncomingData_should_get_out_the_loop_when_data_is_arrive(void)
{
  UART_HandleTypeDef uartHandle;
  uint8_t buffer[1024];
  
  HAL_UART_Receive_ExpectAndReturn(&uartHandle, buffer, ONE_BYTE, 5000, HAL_OK);
  waitIncomingData(&uartHandle, buffer);
}

void xtest_waitIncomingData_should_wait_until_data_is_arrive(void)
{
  UART_HandleTypeDef uartHandle;
  uint8_t buffer[1024] = {0};
  
  HAL_UART_Receive_ExpectAndReturn(&uartHandle, buffer, ONE_BYTE, 5000, HAL_ERROR);
  HAL_UART_Receive_ExpectAndReturn(&uartHandle, buffer, ONE_BYTE, 5000, HAL_ERROR);
  HAL_UART_Receive_ExpectAndReturn(&uartHandle, buffer, ONE_BYTE, 5000, HAL_OK);
  waitIncomingData(&uartHandle, buffer);
}

void test_tlvDecodeAndWriteToRam_should_decode_when_chksum_is_correct(void)
{
	uint8_t buffer[] = {  TLV_WRITE,  //type
                        9, //length
                        0xAA, 0xBB, 0xCC, 0xDD, //Address
                        0x10, 0x20, 0x30, 0x40, //Data
                        0x60}; //CheckSum
                        
  memoryWriteWord_Expect(0xAABBCCDD, 0x10203040);
  tlvDecodeAndWriteToRam(buffer);
}

void test_tlvDecodeAndWriteToRam_shouldnt_decode_when_chksum_is_diff_from_sum_up_value(void)
{
	uint8_t buffer[] = {  TLV_WRITE,  //type
                        17, //length
                        0xAA, 0xBB, 0xCC, 0xDD, //Address
                        0x10, 0x20, 0x30, 0x40, //Data
                        0x50, 0x60, 0x70, 0x80,
                        0x90, 0xA0, 0xB0, 0xC0,
                        0xA9}; //CheckSum

  tlvDecodeAndWriteToRam(buffer);
}

void test_verifyValue_given_tlv_structure_should_sum_up_data_value_with_chksum_and_return_1(void ) {
  int i = 0, result = 0;
  
  uint8_t buffer[] = {  0xCA, 0xCA, 0xCA, 0xCA,  //address
                        0x01, 0x02, 0x03, 0x04,  //data
                        0x05, 0x06, 0x07, 0x08,
                        0xDC};  //chksum
  
  result = verifyValue(&buffer[4], 8);
  TEST_ASSERT_EQUAL(result, 1);
}

void test_verifyValue_given_wrong_chksum_should_return_0(void ) {
  int i = 0, result = 0;
  
  uint8_t buffer[] = {  0xCA, 0xCA, 0xCA, 0xCA,  //address
                        0x01, 0x02, 0x03, 0x04,  //data
                        0x05, 0x06, 0x07, 0x08,
                        0x09, 0x0A, 0x0B, 0x0C,
                        0xAB};  //chksum
                
  result = verifyValue(&buffer[4], 12);
  TEST_ASSERT_EQUAL(result, 0);
}

void xtest_probeProgrammer_given_PROBE_WAIT_state_should_wait_for_instruction(void)
{
  UART_HandleTypeDef uartHandle;
  Probe_TypeDef probe;
  
  probe.state = PROBE_WAIT;
  probe.uartHandle = &uartHandle;
  
  HAL_UART_Receive_ExpectAndReturn(&uartHandle, probe.rxBuffer, ONE_BYTE, 5000, HAL_ERROR);
  HAL_UART_Receive_ExpectAndReturn(&uartHandle, probe.rxBuffer, ONE_BYTE, 5000, HAL_OK);
  
  probeProgrammer(&probe);
  
  TEST_ASSERT_EQUAL(PROBE_INTERPRET_INSTRUCTION, probe.state);
}

void test_probeProgrammer_given_TLV_START_TRANSMISSION_instruction_probe_should_reply_OK(void)
{
  UART_HandleTypeDef uartHandle;
  Probe_TypeDef probe;
  
  probe.state = PROBE_INTERPRET_INSTRUCTION;
  probe.uartHandle = &uartHandle;
  probe.rxBuffer[0] = TLV_START_TRANSMISSION;
  
  stm32UartSendByte_Expect(&uartHandle, PROBE_OK);
  
  probeProgrammer(&probe);
  
  TEST_ASSERT_EQUAL(PROBE_WAIT, probe.state);
}

void test_probeProgrammer_given_TLV_WRITE_instruction_probe_write_data_to_ram_and_reply_OK_when_done_writing(void)
{
  UART_HandleTypeDef uartHandle;
  Probe_TypeDef probe;
  
  probe.state = PROBE_INTERPRET_INSTRUCTION;
  probe.uartHandle = &uartHandle;
  
  /* Type */
  probe.rxBuffer[0] = TLV_WRITE;
  /* Length */
  probe.rxBuffer[1] = 13;
  /* Address */
  probe.rxBuffer[2] = 0x20;
  probe.rxBuffer[3] = 0x00;
  probe.rxBuffer[4] = 0x1F;
  probe.rxBuffer[5] = 0xFC;
  /* Value */
  probe.rxBuffer[6] = 0xDE;
  probe.rxBuffer[7] = 0xAD;
  probe.rxBuffer[8] = 0xBE;
  probe.rxBuffer[9] = 0xEF;
  
  probe.rxBuffer[10] = 0xBE;
  probe.rxBuffer[11] = 0xEF;
  probe.rxBuffer[12] = 0xCA;
  probe.rxBuffer[13] = 0xFE;
  /* Checksum */
  probe.rxBuffer[14] = 0x53;
  
  memoryWriteWord_Expect(0x20001FFC, 0xDEADBEEF);
  memoryWriteWord_Expect(0x20002000, 0xBEEFCAFE);
  
  stm32UartSendByte_Expect(&uartHandle, PROBE_OK);
  probeProgrammer(&probe);
  
  TEST_ASSERT_EQUAL(PROBE_WAIT, probe.state);
}

void test_probeProgrammer_probe_reply_TLV_DATA_CORRUPTED_when_fail_to_write_into_target_ram(void)
{
  UART_HandleTypeDef uartHandle;
  Probe_TypeDef probe;
  
  probe.state = PROBE_INTERPRET_INSTRUCTION;
  probe.uartHandle = &uartHandle;
  
  /* Type */
  probe.rxBuffer[0] = TLV_WRITE;
  /* Length */
  probe.rxBuffer[1] = 17;
  /* Address */
  probe.rxBuffer[2] = 0x20;
  probe.rxBuffer[3] = 0x00;
  probe.rxBuffer[4] = 0x1F;
  probe.rxBuffer[5] = 0xFC;
  /* Value */
  probe.rxBuffer[6] = 0xDE;
  probe.rxBuffer[7] = 0xAD;
  probe.rxBuffer[8] = 0xBE;
  probe.rxBuffer[9] = 0xEF;
  
  probe.rxBuffer[10] = 0xBE;
  probe.rxBuffer[11] = 0xEF;
  probe.rxBuffer[12] = 0xCA;
  probe.rxBuffer[13] = 0xFE;
  
  probe.rxBuffer[14] = 0xAA;
  probe.rxBuffer[15] = 0xBB;
  probe.rxBuffer[16] = 0xCC;
  probe.rxBuffer[17] = 0xDD;
  /* Checksum */
  probe.rxBuffer[18] = 0x23;
  
  stm32UartSendByte_Expect(&uartHandle, TLV_DATA_CORRUPTED);
  probeProgrammer(&probe);
  
  TEST_ASSERT_EQUAL(PROBE_WAIT, probe.state);
}

void test_readFromTargetRam_should_read_the_given_address_and_send_back_to_host(void)
{ 
  UART_HandleTypeDef uartHandle;
  uint32_t data = 0;
  
  uint8_t buffer[] = {TLV_READ,  //type
                      0x8,
                      0xAB, 0xCD, 0xEF, 0x00}; //length
  
  memoryReadWord_ExpectAndReturn(0xABCDEF00, &data, 0);
  stm32UartSendByte_Expect(&uartHandle, data);
  memoryReadWord_ExpectAndReturn(0xABCDEF04, &data, 0);
  stm32UartSendByte_Expect(&uartHandle, data);
  
  readFromTargetRam(&uartHandle, buffer);
}
// void test_tlvDecodePacket_should_decode_after_receive_TLV_Packet(void)
// {
  // uint8_t txBuffer[1024], chksum = 0;
  
  // pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  // /* Create Packet */
  // TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  // /* Put Data into Buffer */
  // tlvGetDataFromElf(tlv, pElfSection);
  
  // TEST_ASSERT_EQUAL_TLV(TLV_WRITE, 253, pElfSection, tlv);
  
  // /* Pack into TXBUFFER */
  // tlvPackPacketIntoTxBuffer(txBuffer, tlv);
  
  // /* Decode txBuffer */
  // TLV *tlvDecode = tlvDecodePacket(txBuffer);
  
  // TEST_ASSERT_EQUAL(tlvDecode->errorCode, TLV_CLEAR_ERROR); 
  
  // /* Type */
  // TEST_ASSERT_EQUAL(tlvDecode->type, TLV_WRITE);  
  // /* Length */
  // TEST_ASSERT_EQUAL(tlvDecode->length, 253);
  // /* Address */
  // TEST_ASSERT_EQUAL_HEX32(tlvDecode->sectionAddress, 0x200001F0);
  // /* Fist Data */
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[0], 0x80);
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[1], 0xb5);
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[2], 0x94);
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[3], 0xb0);
  // /* Last Data */
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[244], 0x80);
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[245], 0xBD);
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[246], 0x00);
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[247], 0xBF);
  // /* CHECK_SUM */
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[248], 0x02); 
  
  // closeFileInTxt(dataFromElf->myFile);
  // free(pElfSection);
  // free(dataFromElf);
// }

// void test_tlvGetWordAddress_given_address_DE_AD_BE_EF_should_get_0xDEADBEEF_address(void ) {
  // uint8_t buffer[6] = {0xDE, 0xAD, 0xBE, 0xEF};
  
  // uint32_t address32 = tlvConvertFromByteToWord(buffer, 0);
  
  // TEST_ASSERT_EQUAL_HEX32(address32, 0xDEADBEEF);
// }

// void test_tlvGetWordAddress_given_address_ABCFACECAFEEFDEF_should_get_0xFACECAFE_address(void ) {
  // uint8_t buffer[10] = {0xAB, 0xC0, 0xFA, 0xCE, 0xCA, 0xFE, 0xEF, 0xDE, 0xF};
  
  // uint32_t address32 = tlvConvertFromByteToWord(buffer, 2);
  
  // TEST_ASSERT_EQUAL_HEX32(address32, 0xFACECAFE);
// }

// void test_tlvGetValue_tlv_buffer_should_contain_address_data_and_chksum(void ) {
  // uint8_t buffer[50] = {TLV_CLEAR, //type
                        // 9,  //length    
                        // 0xAD, 0xBE, 0xEF, 0xAB, //address
                        // 0xA0, 0xB0, 0xC0, 0xD0, // data
                        // 0xFF}; //checksum
  
  // TLV tlv;
  // tlvGetValue(buffer, tlv.value, 2, buffer[1]);
  
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[0], 0xAD);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[1], 0xBE);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[2], 0xEF);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[3], 0xAB);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[4], 0xA0);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[5], 0xB0);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[6], 0xC0);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[7], 0xD0);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[8], 0xFF);
// }

// void test_tlvGetValue_tlv_buffer_should_only_contain_data(void ) {
  // uint8_t buffer[50] = {TLV_CLEAR, //type
                        // 17,  //length    
                        // 0xAA, 0xBB, 0xCC, 0xDD, //address
                        // 0xBE, 0xEF, 0xCA, 0xFE, // data
                        // 0x0B, 0xAD, 0xBE, 0xEF, // data
                        // 0x01, 0x02, 0x03, 0x04, // data
                        // 0xFF}; //checksum
  
  // TLV tlv;
  // tlvGetValue(buffer, tlv.value, 6, 17 - ADDRESS_LENGTH);
  
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[0], 0xBE);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[1], 0xEF);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[2], 0xCA);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[3], 0xFE);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[4], 0x0B);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[5], 0xAD);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[6], 0xBE);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[7], 0xEF);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[8], 0x01);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[9], 0x02);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[10], 0x03);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[11], 0x04);
  // TEST_ASSERT_EQUAL_HEX8(tlv.value[12], 0xFF);
// }

// void test_tlvVerifyType_given_TLV_WRITE_should_return_1(void ) {
  
  // TEST_ASSERT_EQUAL(tlvVerifyType(TLV_WRITE), 1);
// }

// void test_tlvVerifyType_given_INVALID_TYPE_should_return_0(void ) {
  
  // TEST_ASSERT_EQUAL(tlvVerifyType(PROBE_OK), 0);
// }

// void test_tlvVerifyLength_given_length_520_should_return_1(void ) {
  
  // TEST_ASSERT_EQUAL(tlvVerifyLength(520), 1);
// }

// void test_tlvVerifyLength_given_length_0_should_return_0(void ) {
  
  // TEST_ASSERT_EQUAL(tlvVerifyLength(0), 0);
// }



// void test_tlvConvertDataFromByteToWord_given_data_in_byte_inside_buffer_should_convert_to_word(void ) {
  // int i = 0;
  // uint8_t buffer[] = {  0xCA, 0xCA, 0xCA, 0xCA,  //address
                        // 0xFA, 0xCE, 0xDE, 0xAF,  //data
                        // 0xDE, 0xAD, 0xCA, 0xFE,
                        // 0xAB};  //chksum
  // uint32_t data32 = 0;
  
  // data32 = tlvConvertFromByteToWord(buffer, 4);
  
  // TEST_ASSERT_EQUAL(data32, 0xFACEDEAF);
  
  // data32 = tlvConvertFromByteToWord(buffer, 8);
  
  // TEST_ASSERT_EQUAL(data32, 0xDEADCAFE);
// }

// void test_tlvCheckOperation_given_TLV_WRITE_should_write_into_RAM(void) {
  // int i = 0;
  // uint8_t buffer[] = {  TLV_WRITE,  //type
                        // 13, //length
                        // 0xCA, 0xCA, 0xCA, 0xCA,  //address
                        // 0xFA, 0xCE, 0xDE, 0xAF,  //data
                        // 0xDE, 0xAD, 0xCA, 0xFE,
                        // 0xAD};  //chksum
  // TLV *tlv = tlvDecodePacket(buffer);
  // TEST_ASSERT_EQUAL_HEX8(tlv->type, TLV_WRITE);
  // TEST_ASSERT_EQUAL(tlv->length, 13);
  
  // uint32_t address = tlv->sectionAddress;
  // memoryAccessWrite_ExpectAndReturn(address, tlvConvertFromByteToWord(tlv->value, 0), 0);
  // tlvCheckOperation(tlv);
// }