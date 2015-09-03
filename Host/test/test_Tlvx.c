#include "unity.h"
#include <stdbool.h>
#include "Tlv.h"
#include "Tlv_ex.h"
#include "mock_uart.h"
#include "Tlv_ErrorCode.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvPackIntoBuffer_given_target_and_current_buffer_and_length(void)
{
  uint8_t target[0];
  uint8_t current[] = {0xDE, 0xAD, 0xBE, 0xEF};
  
	tlvPackIntoBuffer(target, current, 4);
  
  TEST_ASSERT_EQUAL_HEX8(0xDE, target[0]);
  TEST_ASSERT_EQUAL_HEX8(0xAD, target[1]);
  TEST_ASSERT_EQUAL_HEX8(0xBE, target[2]);
  TEST_ASSERT_EQUAL_HEX8(0xEF, target[3]);
  TEST_ASSERT_EQUAL_HEX8(0xC8, target[4]);
}

void test_tlvCreatePacket_given_command_size_and_data(void)
{
  uint8_t buffer[] = {0xDE, 0xAD, 0xBE, 0xEF};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, 4, buffer);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, tlv->type);
  TEST_ASSERT_EQUAL(5, tlv->length);
  TEST_ASSERT_EQUAL_HEX8(0xDE, tlv->value[0]);
  TEST_ASSERT_EQUAL_HEX8(0xAD, tlv->value[1]);
  TEST_ASSERT_EQUAL_HEX8(0xBE, tlv->value[2]);
  TEST_ASSERT_EQUAL_HEX8(0xEF, tlv->value[3]);
  TEST_ASSERT_EQUAL_HEX8(0xC8, tlv->value[4]);
}

void test_tlvCreatePacket_given_command_but_0_size_and_0_data(void)
{ 
	Tlv *tlv = tlvCreatePacket(TLV_READ_RAM, 0, 0);
  
  TEST_ASSERT_EQUAL(TLV_READ_RAM, tlv->type);
  TEST_ASSERT_EQUAL(1, tlv->length);
}

void test_tlvCreatePacket_given_32bit_address_should_convert_to_byte(void)
{ 
  uint32_t address = 0x12345678;
	Tlv *tlv = tlvCreatePacket(TLV_READ_RAM, 4, (uint8_t *)&address);
  
  TEST_ASSERT_EQUAL(TLV_READ_RAM, tlv->type);
  TEST_ASSERT_EQUAL(5, tlv->length);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&tlv->value[0]));
}

void test_tlvCreatePacket_given_32bit_data_should_convert_to_byte(void)
{ 
  uint32_t data[] = {0x12345678, 0xDEADBEEF, 0xBEEFCAFE};
	Tlv *tlv = tlvCreatePacket(TLV_READ_RAM, 12, (uint8_t *)data);
  
  TEST_ASSERT_EQUAL(TLV_READ_RAM, tlv->type);
  TEST_ASSERT_EQUAL(13, tlv->length);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&tlv->value[0]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&tlv->value[4]));
  TEST_ASSERT_EQUAL_HEX32(0xBEEFCAFE, get4Byte(&tlv->value[8]));
}

void test_tlvSend_given_session_and_tlv_structure_pointer_should_transmit_the_tlv_over_using_UART(void)
{ 
	Tlv_Session session;
  
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  
  tlvSend(&session, tlv);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, session.txBuffer[0]);
  TEST_ASSERT_EQUAL(5, session.txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0xBEEFCAFE, get4Byte(&session.txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX8(0x8B, session.txBuffer[6]);
}

void test_tlvSend_given_tlv_with_command_and_length_1_and_data_is_null(void)
{ 
  Tlv_Session session;
  
	Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  tlvSend(&session, tlv);
  
  TEST_ASSERT_EQUAL(TLV_OK, session.txBuffer[0]);
  TEST_ASSERT_EQUAL(1, session.txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX8(0x0, session.txBuffer[2]);
}

void test_tlvSendService_should_change_state_after_tlvSend_and_do_nothing_after_that(void)
{ 
  Tlv_Session session;
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  tlvSend(&session, tlv);
  
  sendBytes_ExpectAndReturn(session.handler, session.txBuffer, tlv->length + 2, 0x00);
  tlvSendService(&session);
  TEST_ASSERT_EQUAL(false, session.DATA_SEND_FLAG);
}

void test_tlvReceive_should_return_NULL_if_no_data_is_arrive(void)  {
	Tlv_Session session;
  
  session.DATA_ARRIVE_FLAG = false;
  
  Tlv *tlv = tlvReceive(&session);
  
  TEST_ASSERT_NULL(tlv);
}

void test_tlvReceive_should_receive_tlv_packet_send_by_others(void)  {
	Tlv_Session session;
  
  session.DATA_ARRIVE_FLAG = true;
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 6;
  session.rxBuffer[2] = 0x44;
  session.rxBuffer[3] = 0x33;
  session.rxBuffer[4] = 0x22;
  session.rxBuffer[5] = 0x11;
  
  Tlv *tlv = tlvReceive(&session);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, tlv->type);
  TEST_ASSERT_EQUAL(6, tlv->length);
  TEST_ASSERT_EQUAL_HEX32(0x11223344, get4Byte(&tlv->value[0]));
}

void test_tlvReceiveService_rxBuffer_should_stored_tlv_packet(void)
{
  Tlv_Session session;
  
  session.receiveState = RECEIVE_BEGIN;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 5;
  session.rxBuffer[2] = 0xEF;
  session.rxBuffer[3] = 0xBE;
  session.rxBuffer[4] = 0xAD;
  session.rxBuffer[5] = 0xDE;
  session.rxBuffer[6] = 0x8B;//chksum
  
  getBytes_ExpectAndReturn(session.handler, session.rxBuffer, 2, 0x00); //data arrive
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[2], 5, 0x00);
  
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(true, session.DATA_ARRIVE_FLAG);
}

void test_tlvReceiveService_rxBuffer_should_stored_null_if_no_data_arrive(void)
{
  Tlv_Session session;
  
  session.receiveState = RECEIVE_BEGIN;
  session.DATA_ARRIVE_FLAG = false;
  
  getBytes_ExpectAndReturn(session.handler, session.rxBuffer, 2, 0x01); //data arrive
  
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(false, session.DATA_ARRIVE_FLAG);
}

void test_tlvReceiveService_should_set_time_out_flag_when_data_didnt_arrive_after_first_2_byte(void)
{
  Tlv_Session session;
  
  session.receiveState = RECEIVE_BEGIN;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 10;
  
  getBytes_ExpectAndReturn(session.handler, session.rxBuffer, 2, 0x00); //data arrive
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[2], 10, 0x01); //timeout occur
  
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(true, session.DATA_ARRIVE_FLAG);
  TEST_ASSERT_EQUAL(true, session.TIMEOUT_FLAG);
}

void test_tlvService_should_able_to_receive_while_sending(void)
{
  Tlv_Session session;
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  tlvSend(&session, tlv);
  session.receiveState = RECEIVE_BEGIN;
  session.DATA_ARRIVE_FLAG = false;
  session.TIMEOUT_FLAG = false;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 5;
  session.rxBuffer[2] = 0xDD;
  session.rxBuffer[3] = 0xCC;
  session.rxBuffer[4] = 0xBB;
  session.rxBuffer[5] = 0xAA;
  session.rxBuffer[6] = 0xFF; //dummy chksum
  
  sendBytes_ExpectAndReturn(session.handler, session.txBuffer, tlv->length + 2, 0x00);
  getBytes_ExpectAndReturn(session.handler, session.rxBuffer, 2, 0x00);
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[2], 5, 0x00);
  
  tlvService(&session);
  
  TEST_ASSERT_EQUAL(true, session.DATA_ARRIVE_FLAG);
  TEST_ASSERT_EQUAL(false, session.DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session.TIMEOUT_FLAG);
}

void test_verifyTlvData_should_verify_the_data_in_the_given_tlv_packet(void)
{
  uint8_t buffer[] = {0xEF, 0xBE, 0xAD, 0xDE};
  
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
	
  TEST_ASSERT_EQUAL(1, verifyTlvData(tlv));
}

void test_verifyTlvData_given_wrong_length_should_return_data_invalid(void)
{
  uint8_t buffer[] = {0xEF, 0xBE, 0xAD, 0xDE};
  
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
	tlv->length = 2;
  
  TEST_ASSERT_EQUAL(0, verifyTlvData(tlv));
}