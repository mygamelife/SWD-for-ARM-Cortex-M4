#include "unity.h"
#include "TLV_Probe.h"
#include <stdio.h>
#include "mock_UART.h"
#include "Delay.h"
#include "mock_configurePort.h"
#include "mock_stm32f4xx_hal_uart.h"
#include "mock_Register_ReadWrite.h"
#include "mock_stm32f429i_discovery.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvWaitInstructionFromHost_should_wait_until_instruction_is_arrive(void)
{
  UART_HandleTypeDef uartHandle;
  stm32UartGetByte_ExpectAndReturn(&uartHandle, TLV_START);
  stm32UartGetByte_ExpectAndReturn(&uartHandle, TLV_START_TRANSMISSION);
  
  tlvWaitInstructionFromHost(&uartHandle);
}

void test_tlvWriteToTargetRam_given_TLV_INITIATE_state_should_wait_for_instruction(void)
{
  UART_HandleTypeDef uartHandle;
  TLVProbe_TypeDef tlvProbe;
  
  tlvProbe.state = TLV_INITIATE;
  tlvProbe.uartHandle = &uartHandle;
  
  stm32UartGetByte_ExpectAndReturn(&uartHandle, TLV_START_TRANSMISSION);
  tlvWriteToTargetRam(&tlvProbe);
}

void test_tlvWriteToTargetRam_given_TLV_RECEIVE_PACKET_state_should_receive_packet_send_from_host(void)
{
  UART_HandleTypeDef uartHandle;
  TLVProbe_TypeDef tlvProbe;
  
  tlvProbe.state = TLV_RECEIVE_PACKET;
  tlvProbe.uartHandle = &uartHandle;
  
  // stm32UartGetByte_ExpectAndReturn(&uartHandle, TLV_WRITE);
  HAL_UART_Receive_ExpectAndReturn(&uartHandle, tlvProbe.rxBuffer, sizeof(tlvProbe.rxBuffer), 5000, HAL_OK);
  stm32UartSendByte_Expect(&uartHandle, PROBE_OK);
  
  tlvProbe.rxBuffer[0] = TLV_END_TRANSMISSION;

  // tlvProbe.rxBuffer[0] = TLV_END_TRANSMISSION;
  
  tlvWriteToTargetRam(&tlvProbe);
  TEST_ASSERT_EQUAL(TLV_END, tlvProbe.state);
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

// void test_tlvVerifyValue_given_tlv_structure_should_sum_up_data_value_with_chksum_and_return_1(void ) {
  // int i = 0;
  // TLV tlv;
  
  // tlv.type = TLV_WRITE;
  // tlv.length = 13;
  
  // uint8_t buffer[] = {  0xCA, 0xCA, 0xCA, 0xCA,  //address
                        // 0x01, 0x02, 0x03, 0x04,  //data
                        // 0x05, 0x06, 0x07, 0x08,
                        // 0xDC};  //chksum
                
  // for(i; i < tlv.length; i++) {
    // tlv.value[i] = buffer[i + 4];
  // } 
  
  // TEST_ASSERT_EQUAL(tlvVerifyValue(&tlv), 1);
// }

// void test_tlvVerifyValue_given_wrong_chksum_should_return_0(void ) {
  // int i = 0;
  // TLV tlv;
  
  // tlv.type = TLV_WRITE;
  // tlv.length = 13;
  
  // uint8_t buffer[] = {  0xCA, 0xCA, 0xCA, 0xCA,  //address
                        // 0x01, 0x02, 0x03, 0x04,  //data
                        // 0x05, 0x06, 0x07, 0x08,
                        // 0xAB};  //chksum
                
  // for(i; i < tlv.length; i++) {
    // tlv.value[i] = buffer[i + 4];
  // } 
  
  // TEST_ASSERT_EQUAL(tlvVerifyValue(&tlv), 0);
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