#include "unity.h"
#include <stdbool.h>
#include "Tlv.h"
#include "TlvEx.h"
#include "mock_Uart.h"
#include "ErrorCode.h"
#include "mock_SystemTime.h"

void setUp(void) {}

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
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  
  tlvSend(session, tlv);
  
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(5, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0xBEEFCAFE, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX8(0x8B, session->txBuffer[6]);
}

void test_tlvSend_should_throw_err_when_tlvSend_is_call_but_flag_is_set(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  CEXCEPTION_T err;
  
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  
  Try {
    SET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG);
    tlvSend(session, tlv);
  } Catch(err) {
    TEST_ASSERT_EQUAL(TLV_TRANSMISSION_BUSY, err);
  }
}

void test_tlvSend_given_tlv_with_command_and_length_1_and_data_is_null(void)
{ 
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
	Tlv *tlv = tlvCreatePacket(TLV_OK, 0, NULL);
  
  tlvSend(session, tlv);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(TLV_OK, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(1, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX8(0x0, session->txBuffer[2]);
}

void test_tlvSendService_should_change_state_after_tlvSend_and_do_nothing_after_that(void)
{ 
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  tlvSend(session, tlv);
  
  sendBytes_ExpectAndReturn(session->handler, session->txBuffer, tlv->length + 2, 0x00);
  tlvSendService(session);
  
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
}

void test_tlvSendService_should_not_send_when_uartReady_is_not_SET(void)
{ 
  uartTxReady = 0;
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  
  tlvSend(session, tlv);
  tlvSendService(session);
  
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
}

void test_tlvReceive_should_return_NULL_if_no_data_is_arrive(void)  {
	uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  Tlv *tlv = tlvReceive(session);
  
  TEST_ASSERT_NULL(tlv);
}

void test_tlvReceive_should_throw_error_when_time_out_flag_is_set(void)  {
  CEXCEPTION_T err;
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  Try {
    SET_FLAG_STATUS(session, TLV_TIMEOUT_FLAG);
  
    Tlv *tlv = tlvReceive(session);
    printf("Should Throw TLV_TIME_OUT Exception!");
  }
  Catch(err)  {
    TEST_ASSERT_EQUAL(TLV_TIME_OUT, err);
  }
}

void test_tlvReceive_should_receive_tlv_packet_send_by_others(void)  {
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  SET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG);

  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 5;
  session->rxBuffer[2] = 0x44;
  session->rxBuffer[3] = 0x33;
  session->rxBuffer[4] = 0x22;
  session->rxBuffer[5] = 0x11;
  session->rxBuffer[6] = 0x56;
  
  Tlv *tlv = tlvReceive(session);
  
  TEST_ASSERT_NOT_NULL(tlv);
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, tlv->type);
  TEST_ASSERT_EQUAL(5, tlv->length);
  TEST_ASSERT_EQUAL_HEX32(0x11223344, get4Byte(&tlv->value[0]));
  TEST_ASSERT_EQUAL_HEX8(0x56, tlv->value[4]); //chksum
}

void test_tlvReceiveService_should_receive_type_at_the_first_state(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  session->rxBuffer[0] = TLV_WRITE_RAM;
  
  /* Received Type */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[0], 0x00); //data arrive
  
  tlvReceiveService(session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_LENGTH, session->receiveState);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, session->rxBuffer[0]);
}

void test_tlvReceiveService_should_receive_value_after_length(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  session->receiveState = TLV_RECEIVE_LENGTH;
  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 5;
  session->rxBuffer[2] = 0xA;
  session->rxBuffer[3] = 0xB;
  session->rxBuffer[4] = 0xC;
  session->rxBuffer[5] = 0xD;
  session->rxBuffer[6] = 0xF; //dummy chksum
  
  /* Received Type */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[1], 0x00); //data arrive
  
  /* Received Length */
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 5, 0x00); //receive 5 data
  
  tlvReceiveService(session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_VALUE, session->receiveState);
}

void test_tlvReceiveService_should_get_type_length_and_value(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 5;
  session->rxBuffer[2] = 0xEF;
  session->rxBuffer[3] = 0xBE;
  session->rxBuffer[4] = 0xAD;
  session->rxBuffer[5] = 0xDE;
  session->rxBuffer[6] = 0x8B;//chksum
  
  /* Received Type */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[0], 0x00); //received type
  tlvReceiveService(session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_LENGTH, session->receiveState);
  
  /* Received Length */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[1], 0x00); //received length
  /* Received value */
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 5, 0x00); //received value (interrupt)
  
  resetSystemTime_Expect();
  tlvReceiveService(session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_VALUE, session->receiveState);

  tlvReceiveService(session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_TYPE, session->receiveState);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
}

void test_tlvReceiveService_should_set_time_out_flag_if_no_data_arrive_after_first_2_byte(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  session->receiveState = TLV_RECEIVE_VALUE;
  
  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 5;
  session->rxBuffer[2] = 0xEF;
  session->rxBuffer[3] = 0xBE;
  session->rxBuffer[4] = 0xAD;
  session->rxBuffer[5] = 0xDE;
  session->rxBuffer[6] = 0x8B;//chksum
  
  resetSystemTime_Expect();
  /* Received Value */
  tlvReceiveService(session);
  
  TEST_ASSERT_EQUAL(TLV_RECEIVE_TYPE, session->receiveState);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
}

void test_tlvService_should_able_to_receive_while_sending(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  tlvSend(session, tlv);
  
  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 5;
  session->rxBuffer[2] = 0xDD;
  session->rxBuffer[3] = 0xCC;
  session->rxBuffer[4] = 0xBB;
  session->rxBuffer[5] = 0xAA;
  session->rxBuffer[6] = 0xFF; //dummy chksum
  
  /* Send Packet */
  uartTxReady = 1;
  sendBytes_ExpectAndReturn(session->handler, session->txBuffer, tlv->length + 2, 0x00);
  
  /* Received type */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[0], 0x00);
  tlvService(session);
  uartTxReady = 0;
  
  TEST_ASSERT_EQUAL(TLV_RECEIVE_LENGTH, session->receiveState);
  
  /* Received length */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[1], 0x00);
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 5, 0x00);
  resetSystemTime_Expect();

  tlvService(session);
  TEST_ASSERT_EQUAL(TLV_RECEIVE_VALUE, session->receiveState);
  
  uartRxReady = 1;
  /* Received value */
  tlvService(session);
  
  TEST_ASSERT_EQUAL(TLV_RECEIVE_TYPE, session->receiveState);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
}

void test_tlvService_should_receive_while_wating_uart_to_ready_send(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  tlvSend(session, tlv);
  
  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 5;
  session->rxBuffer[2] = 0xDD;
  session->rxBuffer[3] = 0xCC;
  session->rxBuffer[4] = 0xBB;
  session->rxBuffer[5] = 0xAA;
  session->rxBuffer[6] = 0xFF; //dummy chksum
  
  uartTxReady = 0;
  /* Received type */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[0], 0x00);
  tlvService(session);
  
  uartTxReady = 1;
  sendBytes_ExpectAndReturn(session->handler, session->txBuffer, tlv->length + 2, 0x00);
  /* Received length */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[1], 0x00);
  /* Received Value */
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 5, 0x00);
  
  tlvService(session);
  
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_TIME_OUT));
}

void test_tlvService_should_set_time_out_flag_when_timeout_occur(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  uint8_t buffer[] = {0xFE, 0xCA, 0xEF, 0xBE};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  
  tlvSend(session, tlv);
  
  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 5;
  
  uartTxReady = 1;
  
  sendBytes_ExpectAndReturn(session->handler, session->txBuffer, tlv->length + 2, 0x00);
  /* Received Type */
  getByte_ExpectAndReturn(session->handler, session->rxBuffer, 0x00);
  
  tlvService(session);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  
  uartTxReady = 0;
  
  /* Received length */
  getByte_ExpectAndReturn(session->handler, &session->rxBuffer[1], 0x00);
  /* Received Value */
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 5, 0x01);
  
  resetSystemTime_Expect();

  tlvService(session);
  
  tlvService(session);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
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
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  tlvErrorReporter(session, PROBE_TLV_CHECKSUM_ERROR);
}

void test_tlvSendRequest_should_receive_write_target_register_size_8_address_0x12345678_and_data_0xDEADBEEF(void)
{
  uartInit_Ignore();
  Tlv_Session *session = tlvCreateSession();
  
  uint32_t data[] = {0x12345678, 0xDEADBEEF};
  tlvSendRequest(session, TLV_WRITE_REGISTER, 8, (uint8_t *)data);
  
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(TLV_WRITE_REGISTER, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX8(0xB4, session->txBuffer[10]); //chksum
}

void test_tlvReadDataChunk_should_send_request_read_data_in_chunk(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  int size = 255;
  uint32_t address = 0x20001000;
  
  tlvReadDataChunk(session, &address, &size);
  
  TEST_ASSERT_EQUAL_HEX32(0x200010F8, address);
  TEST_ASSERT_EQUAL(0x7, size);
  TEST_ASSERT_EQUAL(FLAG_SET, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(TLV_READ_MEMORY, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(6, session->txBuffer[1]);
  TEST_ASSERT_EQUAL(0x20001000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL(248, session->txBuffer[6]);
  TEST_ASSERT_EQUAL_HEX8(0xD8, session->txBuffer[7]); //chksum
}

void test_tlvWriteDataChunk_should_send_data_in_chunk_to_ram_and_update_data_address_and_size(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  int size = 4;
  uint32_t address = 0x10000000, data[] = {0xDEADBEEF, 0xAAAAAAAA};
  uint8_t *dataPtr = (uint8_t *)data;
  
  tlvWriteDataChunk(session, &dataPtr, &address, &size, TLV_WRITE_RAM);
  
  TEST_ASSERT_EQUAL_HEX8(TLV_WRITE_RAM, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(9, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x10000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX8(0xB8, session->txBuffer[10]); //chksum
  
  TEST_ASSERT_EQUAL(0, size);
  TEST_ASSERT_EQUAL_HEX32(0x10000004, address);
  TEST_ASSERT_EQUAL_HEX32(0xAAAAAAAA, get4Byte(dataPtr));
}

void test_tlvWriteDataChunk_should_send_data_in_chunk_to_flash_and_update_data_address_and_size(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  int size = 255;
  uint32_t address = 0x10000000, data[] = {0xDEADBEEF, 0xAAAAAAAA};
  uint8_t *dataPtr = (uint8_t *)data;
  
  tlvWriteDataChunk(session, &dataPtr, &address, &size, TLV_WRITE_FLASH);
  
  TEST_ASSERT_EQUAL_HEX8(TLV_WRITE_FLASH, session->txBuffer[0]);
  TEST_ASSERT_EQUAL(253, session->txBuffer[1]);
  TEST_ASSERT_EQUAL_HEX32(0x10000000, get4Byte(&session->txBuffer[2]));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&session->txBuffer[6]));
  TEST_ASSERT_EQUAL_HEX32(0xAAAAAAAA, get4Byte(&session->txBuffer[10]));
  
  TEST_ASSERT_EQUAL(7, size);
  TEST_ASSERT_EQUAL_HEX32(0x100000F8, address);
}