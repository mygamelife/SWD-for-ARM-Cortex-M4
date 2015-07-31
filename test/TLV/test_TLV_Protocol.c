#include "unity.h"
#include <stdio.h>
#include <windows.h>
#include "TLV_Protocol.h"
#include "mock_Serial.h"
#include "mock_Yield.h"

void setUp(void)
{
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

void test_tlvPackPacketIntoBuffer_should_store_packet_info_into_buffer()
{
  uint8_t txBuffer[1024];
  uint8_t data[10] = {0x22, 0x33, 0x44, 0x55, 0x66,
                      0x77, 0x88, 0x99, 0xAA, 0xBB};
                      
  TLV_TypeDef *tlv = tlvCreatePacket(TLV_WRITE, sizeof(data), data);

  tlvPackPacketIntoBuffer(txBuffer, tlv);
  
  TEST_ASSERT_EQUAL(txBuffer[0], TLV_WRITE);  //type
  TEST_ASSERT_EQUAL(txBuffer[1], 10); //length
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

/*
void test_tlvHost_given_TLV_START_should_create_packet_and_change_state_to_TLV_SEND_TYPE()
{
  HANDLE hSerial;
  
  TlvState state = TLV_START;
  
  initSerialComm_IgnoreAndReturn(hSerial);
  
  tlvHost(&state);
  TEST_ASSERT_EQUAL(state, TLV_SEND_TYPE);
}*/