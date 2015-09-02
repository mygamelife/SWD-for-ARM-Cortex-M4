#include "Tlv.h"
#include "unity.h"
#include "Tlv_ex.h"
#include <malloc.h>
#include "Read_File.h"
#include "GetHeaders.h"
#include "ProgramElf.h"
#include "mock_uart.h"
#include "ProgramLoader.h"
#include "Tlv_ErrorCode.h"

void setUp(void)  {}

void tearDown(void) {}

void test_tlvWriteTargetRegister_should_send_register_address_and_data_to_probe_using_UART(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x12345678;
  uint32_t data = 0xDEADBEEF;
  
  uint32_t buffer[] = {0x12345678, 0xDEADBEEF};
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_REGISTER, 8, (uint8_t *)buffer);
  
  // uartSendBytes_ExpectAndReturn(hSerial, (uint8_t *)tlv, 2, 2);
  // uartSendBytes_ExpectAndReturn(hSerial, tlv->value, tlv->length, tlv->length);
  
  tlvWriteTargetRegister(session, &address, &data);
}

void test_tlvReadTargetRegister_should_send_register_address_to_probe_using_UART_to_perfom_read_operation(void)
{
  HANDLE hSerial;
  uartInit_IgnoreAndReturn(hSerial);
	Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0x12345678;
  
  uint32_t buffer[] = {0x12345678};
  Tlv *tlv = tlvCreatePacket(TLV_READ_REGISTER, 4, (uint8_t *)buffer);
  
  // uartSendBytes_ExpectAndReturn(hSerial, (uint8_t *)tlv, 2, 2);
  // uartSendBytes_ExpectAndReturn(hSerial, tlv->value, tlv->length, tlv->length);
  
  tlvReadTargetRegister(session, &address);
}