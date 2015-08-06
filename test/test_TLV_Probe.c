#include "unity.h"
#include "TLV_Probe.h"

void setUp(void)
{}

void tearDown(void) {}


void test_tlvGetWordAddress_given_address_DE_AD_BE_EF_should_get_0xDEADBEEF_address(void ) {
  uint8_t buffer[6] = {0xDE, 0xAD, 0xBE, 0xEF};
  
  uint32_t address32 = tlvGetWordAddress(buffer, 0);
  
  TEST_ASSERT_EQUAL_HEX32(address32, 0xDEADBEEF);
}

void test_tlvGetWordAddress_given_address_ABCFACECAFEEFDEF_should_get_0xFACECAFE_address(void ) {
  uint8_t buffer[10] = {0xAB, 0xC0, 0xFA, 0xCE, 0xCA, 0xFE, 0xEF, 0xDE, 0xF};
  
  uint32_t address32 = tlvGetWordAddress(buffer, 2);
  
  TEST_ASSERT_EQUAL_HEX32(address32, 0xFACECAFE);
}

void test_tlvGetValue_tlv_buffer_should_contain_address_data_and_chksum(void ) {
  uint8_t buffer[50] = {TLV_CLEAR, //type
                        9,  //length    
                        0xAD, 0xBE, 0xEF, 0xAB, //address
                        0xA0, 0xB0, 0xC0, 0xD0, // data
                        0xFF}; //checksum
  
  TLV tlv;
  tlvGetValue(buffer, tlv.value, 2, buffer[1]);
  
  TEST_ASSERT_EQUAL_HEX8(tlv.value[0], 0xAD);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[1], 0xBE);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[2], 0xEF);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[3], 0xAB);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[4], 0xA0);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[5], 0xB0);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[6], 0xC0);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[7], 0xD0);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[8], 0xFF);
}

void test_tlvGetValue_tlv_buffer_should_only_contain_data(void ) {
  uint8_t buffer[50] = {TLV_CLEAR, //type
                        17,  //length    
                        0xAA, 0xBB, 0xCC, 0xDD, //address
                        0xBE, 0xEF, 0xCA, 0xFE, // data
                        0x0B, 0xAD, 0xBE, 0xEF, // data
                        0x01, 0x02, 0x03, 0x04, // data
                        0xFF}; //checksum
  
  TLV tlv;
  tlvGetValue(buffer, tlv.value, 6, 17 - ADDRESS_LENGTH - CHECKSUM_LENGTH);
  
  TEST_ASSERT_EQUAL_HEX8(tlv.value[0], 0xBE);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[1], 0xEF);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[2], 0xCA);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[3], 0xFE);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[4], 0x0B);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[5], 0xAD);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[6], 0xBE);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[7], 0xEF);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[8], 0x01);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[9], 0x02);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[10], 0x03);
  TEST_ASSERT_EQUAL_HEX8(tlv.value[11], 0x04);
}

void test_tlvDecodePacket_given_require_info_inside_rxBuffer(void)
{
  int result = 0;
  uint8_t chksum = 0;
  uint8_t buffer[1024] = {TLV_WRITE,  //type
                            9,       //length
                            0x44, 0x55, 0x66, 0x77, //address
                            0xA1, 0xB2, 0xC3, 0xD4, //data
                            0xC8}; //chekcsum
  
  TLV *tlv = tlvDecodePacket(buffer);
  
  /* Type */
  TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE);
  
  /* Length */
  TEST_ASSERT_EQUAL(tlv->length, 9);
  
  /* Address */
  TEST_ASSERT_EQUAL_HEX32(tlv->sectionAddress, 0x44556677);
  
  /* Data */
  TEST_ASSERT_EQUAL_HEX8(tlv->value[0], 0xA1);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[1], 0xB2);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[2], 0xC3);
  TEST_ASSERT_EQUAL_HEX8(tlv->value[3], 0xD4);

  /* Check Sum */
  TEST_ASSERT_EQUAL_HEX8(tlv->chksum, 0xC8);
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
  
  // /* Type */
  // TEST_ASSERT_EQUAL(tlvDecode->type, TLV_WRITE);
  
  // /* Length */
  // TEST_ASSERT_EQUAL(tlvDecode->length, 253);
  
  // /* Address */
  // TEST_ASSERT_EQUAL_HEX32(tlvDecode->decodeAddress, 0x200001F0);
  
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
  
  // TEST_ASSERT_EQUAL_HEX8(tlvDecode->value[248], 0x02); //checksum
  
  // /* Checksum */
  // chksum = tlvVerifyCheckSum(tlvDecode->value, tlvDecode->length - ADDRESS_LENGTH, 0);
  // TEST_ASSERT_EQUAL(chksum, 1);
  
  // closeFileInTxt(dataFromElf->myFile);
  // free(pElfSection);
  // free(dataFromElf);
// }