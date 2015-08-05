#include "unity.h"
#include "TLV_Probe.h"

void setUp(void)
{
}

void tearDown(void) {}

void test_tlvGetWordAddress_given_address_DE_AD_BE_EF_should_get_0xDEADBEEF_address(void ) {
  uint8_t buffer[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xAB, 0xAC};
  
  uint32_t address32 = tlvGetWordAddress(buffer, 0, 4);
  
  TEST_ASSERT_EQUAL_HEX32(address32, 0xDEADBEEF);
}

// void test_tlvDecodePacket_given_require_info_inside_rxBuffer(void)
// {
  // int result = 0;
  // uint8_t chksum = 0;
  // uint8_t rxBuffer[1024] = {TLV_WRITE,  //type
                            // 13,       //length
                            // 0xAA, 0xBB, 0xCC, 0xDD, //address
                            // 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF1, 0xC1, 0xB7, //data
                            // 0xC8}; //chekcsum
  
  // TLV *tlv = tlvDecodePacket(rxBuffer);
  
  // /* Type */
  // TEST_ASSERT_EQUAL(tlv->type, TLV_WRITE);
  
  // /* Length */
  // TEST_ASSERT_EQUAL(tlv->length, 13);
  
  // /* Address */
  // TEST_ASSERT_EQUAL_HEX32(tlv->decodeAddress, 0xAABBCCDD);
  
  // /* Data */
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[0], 0xA1);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[1], 0xB2);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[2], 0xC3);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[3], 0xD4);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[4], 0xE5);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[5], 0xF1);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[6], 0xC1);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[7], 0xB7);
  // TEST_ASSERT_EQUAL_HEX8(tlv->value[8], 0xC8); //chksum

  // chksum = tlvVerifyCheckSum(tlv->value, tlv->length - ADDRESS_LENGTH, 0);
  // TEST_ASSERT_EQUAL(chksum, 1);
// }

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