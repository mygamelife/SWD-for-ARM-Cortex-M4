#include "unity.h"
#include "Tlv_Worker.h"
#include "Tlv_ex.h"
#include "mock_UART.h"
#include "mock_stm32f4xx_hal_uart.h"

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

void test_tlvCreateWorkerSession_create_a_session_for_program_worker(void)
{ 
  UART_HandleTypeDef uartHandler;
  initUart_IgnoreAndReturn(&uartHandler);
	Tlv_Session *session = tlvCreateWorkerSession();
}

void test_tlvSend_given_session_and_tlv_structure_pointer_should_transmit_the_tlv_over_using_UART(void)
{ 
  UART_HandleTypeDef uartHandler;
  initUart_IgnoreAndReturn(&uartHandler);
	Tlv_Session *session = tlvCreateWorkerSession();
  uint8_t buffer[] = {0xBE, 0xEF, 0xCA, 0xFE, 0xBB};
  
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  
  /** send type and length */
  HAL_UART_Transmit_ExpectAndReturn(session->handler, (uint8_t *)tlv, 2, FIVE_SEC, HAL_OK);
  HAL_UART_Transmit_ExpectAndReturn(session->handler, tlv->value, tlv->length, FIVE_SEC, HAL_OK);
  
  tlvSend(session, tlv);
}

void test_tlvSend_given_tlv_with_command_and_length_1_and_data_is_null(void)
{ 
  UART_HandleTypeDef uartHandler;
  initUart_IgnoreAndReturn(&uartHandler);
	Tlv_Session *session = tlvCreateWorkerSession();
  
	Tlv *tlv = tlvCreatePacket(TLV_OK, 0, 0);
  
  /** send type and length */
  HAL_UART_Transmit_ExpectAndReturn(session->handler, (uint8_t *)tlv, 2, FIVE_SEC, HAL_OK);
  HAL_UART_Transmit_ExpectAndReturn(session->handler, tlv->value, tlv->length, FIVE_SEC, HAL_OK);
  
  tlvSend(session, tlv);
}

void test_tlvSend_testing_with_large_number_data(void)
{ 
  UART_HandleTypeDef uartHandler;
  initUart_IgnoreAndReturn(&uartHandler);
	Tlv_Session *session = tlvCreateWorkerSession();
  
  uint8_t buffer[] = {  0xBE, 0xEF, 0xCA, 0xFE,
                        0xAA, 0xBB, 0xCC, 0xDD,
                        0xAA, 0xBB, 0xCC, 0xDD,
                        0xBE, 0xEF, 0xCA, 0xFE,
                        0xBE, 0xEF, 0xCA, 0xFE,
                        0xAA, 0xBB, 0xCC, 0xDD,
                        0xAA, 0xBB, 0xCC, 0xDD,
                        0xBE, 0xEF, 0xCA, 0xFE,
                        0xBE, 0xEF, 0xCA, 0xFE,
                        0xAA, 0xBB, 0xCC, 0xDD};
                       
	Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
  
  HAL_UART_Transmit_ExpectAndReturn(session->handler, (uint8_t *)tlv, 2, FIVE_SEC, HAL_OK);
  HAL_UART_Transmit_ExpectAndReturn(session->handler, tlv->value, tlv->length, FIVE_SEC, HAL_OK);
  
  tlvSend(session, tlv);
}

void test_tlvReceive_should_return_NULL_if_no_data_is_arrive(void)  {
  UART_HandleTypeDef uartHandler;
  initUart_IgnoreAndReturn(&uartHandler);
	Tlv_Session *session = tlvCreateWorkerSession();
  
  HAL_UART_Receive_ExpectAndReturn(session->handler, session->rxBuffer, 2, FIVE_SEC, HAL_ERROR);

  Tlv *tlv = tlvReceive(session);
  
  TEST_ASSERT_NULL(tlv);
}

void test_tlvReceive_should_receive_tlv_packet_send_by_others(void)  {
  UART_HandleTypeDef uartHandler;
  initUart_IgnoreAndReturn(&uartHandler);
	Tlv_Session *session = tlvCreateWorkerSession();
  
  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 6;
  session->rxBuffer[2] = 0x44;
  session->rxBuffer[3] = 0x33;
  session->rxBuffer[4] = 0x22;
  session->rxBuffer[5] = 0x11;
  
  HAL_UART_Receive_ExpectAndReturn(session->handler, session->rxBuffer, 2, FIVE_SEC, HAL_OK);
  HAL_UART_Receive_ExpectAndReturn(session->handler, session->rxBuffer, 6, FIVE_SEC, HAL_OK);
  Tlv *tlv = tlvReceive(session);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, tlv->type);
  TEST_ASSERT_EQUAL(6, tlv->length);
  TEST_ASSERT_EQUAL_HEX32(0x11223344, get4Byte(&tlv->value[2]));
}

void test_tlvReceive_should_receive_tlv_packet_contain_8_data(void)  {
  UART_HandleTypeDef uartHandler;
  initUart_IgnoreAndReturn(&uartHandler);
	Tlv_Session *session = tlvCreateWorkerSession();
  
  session->rxBuffer[0] = TLV_WRITE_RAM;
  session->rxBuffer[1] = 10;
  session->rxBuffer[2] = 0xEF;
  session->rxBuffer[3] = 0xBE;
  session->rxBuffer[4] = 0xAD;
  session->rxBuffer[5] = 0xDE;
  session->rxBuffer[6] = 0x40;
  session->rxBuffer[7] = 0x30;
  session->rxBuffer[8] = 0x20;
  session->rxBuffer[9] = 0x10;
  
  HAL_UART_Receive_ExpectAndReturn(session->handler, session->rxBuffer, 2, FIVE_SEC, HAL_OK);
  HAL_UART_Receive_ExpectAndReturn(session->handler, session->rxBuffer, 10, FIVE_SEC, HAL_OK);
  Tlv *tlv = tlvReceive(session);
  
  TEST_ASSERT_EQUAL(TLV_WRITE_RAM, tlv->type);
  TEST_ASSERT_EQUAL(10, tlv->length);
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, get4Byte(&tlv->value[2]));
  TEST_ASSERT_EQUAL_HEX32(0x10203040, get4Byte(&tlv->value[6]));
}

void test_tlvVerifyData_should_verify_the_data_in_the_given_tlv_packet(void)
{
  uint8_t buffer[] = {0xEF, 0xBE, 0xAD, 0xDE};
  
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
	
  TEST_ASSERT_EQUAL(DATA_VALID, tlvVerifyData(tlv));
}

void test_tlvVerifyData_given_wrong_length_should_return_data_invalid(void)
{
  uint8_t buffer[] = {0xEF, 0xBE, 0xAD, 0xDE};
  
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(buffer), buffer);
	tlv->length = 2;
  
  TEST_ASSERT_EQUAL(DATA_INVALID, tlvVerifyData(tlv));
}