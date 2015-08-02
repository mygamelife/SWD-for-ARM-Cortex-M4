#include "unity.h"
#include "CustomAssertion.h"
#include "TLV_Protocol.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_assertTLV_given_type_length_and_value(void)
{
  uint8_t value[] = {0x1, 0x2, 0x3, 0x4, 0x5};
  uint8_t actualValue[] = {0x1, 0x2, 0x3, 0x4, 0x5};
  
  TLV_TypeDef *tlv = tlvCreatePacket(TLV_WRITE, sizeof(actualValue), actualValue);
  
	TEST_ASSERT_EQUAL_TLV(TLV_WRITE, sizeof(value), value, tlv);
}

/* This test code is use to test the feedback message
void test_assertTLV_given_Wrong_value_should_give_some_feedback(void)
{
  uint8_t value[] = {0x1, 0x2, 0x3, 0x4, 0x5};
  uint8_t actualValue[] = {0x1, 0x2, 0x3, 0x4, 0x6};
  
  TLV_TypeDef *tlv = tlvCreatePacket(TLV_WRITE, sizeof(actualValue), actualValue);
  
	TEST_ASSERT_EQUAL_TLV(TLV_WRITE, sizeof(value), value, tlv);
}*/

/*
void test_assertTxBuffer_given_type_length_address_value_chksum_and_txBuffer(void)
{
  uint8_t value[] = {0x1, 0x2, 0x3, 0x4, 0x5};
  uint8_t actualValue[] = {0x1, 0x2, 0x3, 0x4, 0x5};
  
  TLV_TypeDef *tlv = tlvCreatePacket(TLV_WRITE, sizeof(actualValue), actualValue);
  
	TEST_ASSERT_EQUAL_TXBUFFER(TLV_WRITE, uint8_t length, uint8_t *address, uint8_t *value, uint8_t chksum, uint8_t *txBuffer, int line);
}*/