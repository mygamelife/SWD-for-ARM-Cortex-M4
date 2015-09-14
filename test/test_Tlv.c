#include "unity.h"
#include <stdbool.h>
#include "Tlv.h"
#include "Tlv_ex.h"
#include "mock_uart.h"
#include "ErrorCode.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvPackIntoBuffer_given_target_and_current_buffer_and_length(void)
{
  uint8_t target[0];
  uint8_t current[] = {0xDE, 0xAD, 0xBE, 0xEF};
  
  TEST_ASSERT_EQUAL_HEX8(0xC8, tlvPackIntoBuffer(target, current, 4));
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
	Tlv *tlv = tlvCreatePacket(TLV_READ_MEMORY, 0, 0);
  
  TEST_ASSERT_EQUAL(TLV_READ_MEMORY, tlv->type);
  TEST_ASSERT_EQUAL(1, tlv->length);
}

void test_tlvCreatePacket_given_32bit_address_should_convert_to_byte(void)
{ 
  uint32_t address = 0x12345678;
	Tlv *tlv = tlvCreatePacket(TLV_READ_MEMORY, 4, (uint8_t *)&address);
  
  TEST_ASSERT_EQUAL(TLV_READ_MEMORY, tlv->type);
  TEST_ASSERT_EQUAL(5, tlv->length);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&tlv->value[0]));
}

void test_tlvCreatePacket_given_32bit_data_should_convert_to_byte(void)
{ 
  uint32_t data[] = {0x12345678, 0xDEADBEEF, 0xBEEFCAFE};
	Tlv *tlv = tlvCreatePacket(TLV_READ_MEMORY, 12, (uint8_t *)data);
  
  TEST_ASSERT_EQUAL(TLV_READ_MEMORY, tlv->type);
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
  
	Tlv *tlv = tlvCreatePacket(TLV_OK, 0, NULL);
  
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
  session.sendState = TLV_SEND_BEGIN;
  uartReady = SET;
  tlvSend(&session, tlv);
  
  sendBytes_ExpectAndReturn(session.handler, session.txBuffer, tlv->length + 2, 0x00);
  tlvSendService(&session);
  
  TEST_ASSERT_EQUAL(false, session.dataSendFlag);
}

void test_tlvSendService_should_not_send_when_uartReady_is_not_SET(void)
{ 
  uartReady = RESET;
  Tlv_Session session;
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  session.sendState = TLV_SEND_BEGIN;
  
  tlvSend(&session, tlv);
  tlvSendService(&session);
  
  TEST_ASSERT_EQUAL(true, session.dataSendFlag);
}

void test_tlvReceive_should_return_NULL_if_no_data_is_arrive(void)  {
	Tlv_Session session;
  
  session.dataReceiveFlag = false;
  session.timeOutFlag = false;
  
  Tlv *tlv = tlvReceive(&session);
  
  TEST_ASSERT_NULL(tlv);
}

void test_tlvReceive_should_throw_error_when_time_out_flag_is_set(void)  {
  CEXCEPTION_T err;
  Tlv_Session session;
  
  Try {
    session.dataReceiveFlag = true;
    session.timeOutFlag = true;
  
    Tlv *tlv = tlvReceive(&session);
    printf("Should Throw TLV_TIME_OUT Exception!");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(TLV_TIME_OUT, err);
  }
}

void test_tlvReceive_should_receive_tlv_packet_send_by_others(void)  {
	Tlv_Session session;
  
  session.timeOutFlag = false;
  session.dataReceiveFlag = true;
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

void test_tlvReceiveService_should_receive_type_at_the_first_state(void)
{
  Tlv_Session session;
  
  session.receiveState = TLV_RECEIVE_TYPE;
  session.dataReceiveFlag = false;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  
  /* Received Type */
  getBytes_ExpectAndReturn(session.handler, session.rxBuffer, 1, 0x00); //data arrive
  
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_LENGTH, session.receiveState);
  TEST_ASSERT_EQUAL(false, session.dataReceiveFlag);
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, session.rxBuffer[0]);
}

void test_tlvReceiveService_should_receive_length_at_the_second_state(void)
{
  Tlv_Session session;
  
  session.receiveState = TLV_RECEIVE_LENGTH;
  session.dataReceiveFlag = false;
  
  session.rxBuffer[1] = 5;
  
  /* Received Length */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[1], 1, 0x00); //data arrive
  
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_VALUE, session.receiveState);
  TEST_ASSERT_EQUAL(false, session.dataReceiveFlag);
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, session.rxBuffer[0]);
}

void test_tlvReceiveService_should_receive_value_at_the_third_state(void)
{
  Tlv_Session session;
  
  session.receiveState = TLV_RECEIVE_VALUE;
  session.dataReceiveFlag = false;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 5;
  session.rxBuffer[2] = 0xEF;
  session.rxBuffer[3] = 0xBE;
  session.rxBuffer[4] = 0xAD;
  session.rxBuffer[5] = 0xDE;
  session.rxBuffer[6] = 0x8B;//chksum
  
  /* Received Value */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[2], 1, 0x00); //data arrive
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_VALUE, session.receiveState);
  
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[3], 1, 0x00); //data arrive
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_VALUE, session.receiveState);
  
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[4], 1, 0x00); //data arrive
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_VALUE, session.receiveState);
  
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[5], 1, 0x00); //data arrive
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_VALUE, session.receiveState);
  
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[6], 1, 0x00); //data arrive
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_TYPE, session.receiveState);
  TEST_ASSERT_EQUAL(true, session.dataReceiveFlag);
}

void test_tlvReceiveService_should_set_time_out_flag_if_no_data_arrive_after_first_2_byte(void)
{
  Tlv_Session session;
  
  session.receiveState = TLV_RECEIVE_VALUE;
  session.dataReceiveFlag = false;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 5;
  session.rxBuffer[2] = 0xEF;
  session.rxBuffer[3] = 0xBE;
  session.rxBuffer[4] = 0xAD;
  session.rxBuffer[5] = 0xDE;
  session.rxBuffer[6] = 0x8B;//chksum
  
  /* Received Value */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[2], 1, 0x01); //data arrive
  tlvReceiveService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_TYPE, session.receiveState);
  TEST_ASSERT_EQUAL(false, session.dataReceiveFlag);
  TEST_ASSERT_EQUAL(true, session.timeOutFlag);
}

void test_tlvService_should_able_to_receive_while_sending(void)
{
  Tlv_Session session;
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  tlvSend(&session, tlv);
  session.sendState = TLV_SEND_BEGIN;
  session.receiveState = TLV_RECEIVE_TYPE;
  session.timeOutFlag = false;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 5;
  session.rxBuffer[2] = 0xDD;
  session.rxBuffer[3] = 0xCC;
  session.rxBuffer[4] = 0xBB;
  session.rxBuffer[5] = 0xAA;
  session.rxBuffer[6] = 0xFF; //dummy chksum
  
  uartReady = SET;
  sendBytes_ExpectAndReturn(session.handler, session.txBuffer, tlv->length + 2, 0x00);
  /* Received type */
  getBytes_ExpectAndReturn(session.handler, session.rxBuffer, 1, 0x00);
  tlvService(&session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_LENGTH, session.receiveState);
  
  uartReady = RESET;
  /* Received length */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[1], 1, 0x00);
  tlvService(&session);
  
  /* Received value */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[2], 1, 0x00);
  tlvService(&session);
  
  /* Received value */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[3], 1, 0x00);
  tlvService(&session);
  
  /* Received value */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[4], 1, 0x00);
  tlvService(&session);
  
  /* Received value */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[5], 1, 0x00);
  tlvService(&session);

  /* Received checksum */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[6], 1, 0x00);
  tlvService(&session);
  
  TEST_ASSERT_EQUAL(TLV_RECEIVE_TYPE, session.receiveState);
  TEST_ASSERT_EQUAL(true, session.dataReceiveFlag);
  TEST_ASSERT_EQUAL(false, session.timeOutFlag);
}

void test_tlvService_should_receive_while_wating_uart_to_ready_send(void)
{
  Tlv_Session session;
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  tlvSend(&session, tlv);
  session.receiveState = TLV_RECEIVE_TYPE;
  session.sendState = TLV_SEND_BEGIN;
  session.dataReceiveFlag = false;
  session.timeOutFlag = false;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 5;
  session.rxBuffer[2] = 0xDD;
  session.rxBuffer[3] = 0xCC;
  session.rxBuffer[4] = 0xBB;
  session.rxBuffer[5] = 0xAA;
  session.rxBuffer[6] = 0xFF; //dummy chksum
  
  uartReady = RESET;
  /* Received type */
  getBytes_ExpectAndReturn(session.handler, session.rxBuffer, 1, 0x00);
  tlvService(&session);
  
  uartReady = SET;
  sendBytes_ExpectAndReturn(session.handler, session.txBuffer, tlv->length + 2, 0x00);
  /* Received length */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[1], 1, 0x00);
  tlvService(&session);
  
  TEST_ASSERT_EQUAL(false, session.dataSendFlag);
  TEST_ASSERT_EQUAL(false, session.dataReceiveFlag);
  TEST_ASSERT_EQUAL(false, session.timeOutFlag);
}

void test_tlvService_should_set_time_out_flag_when_timeout_occur(void)
{
  Tlv_Session session;
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  tlvSend(&session, tlv);
  session.sendState = TLV_SEND_BEGIN;
  session.receiveState = TLV_RECEIVE_TYPE;
  session.dataReceiveFlag = false;
  session.timeOutFlag = false;
  
  session.rxBuffer[0] = TLV_WRITE_RAM;
  session.rxBuffer[1] = 5;
  
  uartReady = SET;
  sendBytes_ExpectAndReturn(session.handler, session.txBuffer, tlv->length + 2, 0x00);
  /* Received Type */
  getBytes_ExpectAndReturn(session.handler, session.rxBuffer, 1, 0x00);
  tlvService(&session);
  
  uartReady = RESET;
  /* Received length */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[1], 1, 0x00);
  tlvService(&session);
  
  /* Received length */
  getBytes_ExpectAndReturn(session.handler, &session.rxBuffer[2], 1, 0x01);
  tlvService(&session);
  
  TEST_ASSERT_EQUAL(false, session.dataSendFlag);
  TEST_ASSERT_EQUAL(false, session.dataReceiveFlag);
  TEST_ASSERT_EQUAL(true, session.timeOutFlag);
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

void test_isTlvCommand_should_return_1_if_command_is_valid(void)
{ 
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_WRITE_RAM));
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_READ_MEMORY));
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_WRITE_REGISTER));
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_READ_REGISTER));
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_HALT_TARGET));
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_RUN_TARGET));
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_STEP));
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_MULTI_STEP));
  TEST_ASSERT_EQUAL(1, isTlvCommand(TLV_BREAKPOINT));
}

void test_isTlvCommand_should_return_0_if_command_is_invalid(void)
{ 
  TEST_ASSERT_EQUAL(0, isTlvCommand(0xFF));
}

void test_isTlvAck_if_the_type_is_acknowledgement_should_return_1(void)
{ 
  Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  TEST_ASSERT_EQUAL(1, isTlvAck(tlv));
}

void test_isTlvAck_if_the_type_is_negative_acknowledgement_should_throw_the_error_code_inside_packet(void)
{ 
  uint8_t errorCode = TLV_TIME_OUT;
  Tlv *tlv = tlvCreatePacket(TLV_NOT_OK, 1, &errorCode);
  CEXCEPTION_T err;
  
  Try {
    isTlvAck(tlv);
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(TLV_TIME_OUT, err);  
  }
}

void test_verifyTlvPacket_should_return_1_if_response_is_tlv_acknowledge_OK(void)
{
	Tlv *tlv = tlvCreatePacket(TLV_OK, 0, NULL);
  TEST_ASSERT_EQUAL(1, verifyTlvPacket(tlv));
}

void test_verifyTlvPacket_should_throw_error_if_nack(void)
{
  CEXCEPTION_T err;
  
  Try {
    uint8_t errorCode = TLV_TIME_OUT;
    Tlv *tlv = tlvCreatePacket(TLV_NOT_OK, 1, &errorCode);
    verifyTlvPacket(tlv);
    printf("Should Throw TLV_TIME_OUT Exception!");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(TLV_TIME_OUT, err);
  }
}

void test_verifyTlvPacket_should_throw_error_if_invalid_command(void)
{
  CEXCEPTION_T err;
  
  Try {
    uint8_t buffer[] = {0xAA, 0xBB, 0xCC, 0xDD};
    Tlv *tlv = tlvCreatePacket(0xFF, 12, buffer);
    verifyTlvPacket(tlv);
    printf("Should Throw TLV_INVALID_COMMAND Exception!");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(TLV_INVALID_COMMAND, err);
  }
}

void test_tlvReportError_is_to_create_a_packet_contain_errorCode_to_report_the_error(void)
{
  Tlv_Session session;
  probeErrorReporter(&session, PROBE_TLV_CHECKSUM_ERROR);
}