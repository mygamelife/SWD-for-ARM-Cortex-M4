#include "unity.h"
#include <stdlib.h>
#include "CustomAssertion.h"
#include "Tlv.h"
#include "TlvEx.h"
#include "mock_Uart.h"
#include "CException.h"
#include "ErrorCode.h"
#include "mock_SystemTime.h"

void setUp(void)  {}

void tearDown(void) {}

void test_assertTLV_given_type_length_address_dataAddress_and_index(void)
{ 
  uint32_t buffer[3] = {0xDEADBEEF, 0xABCDABCD, 0xAABBCCDD};
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, 12, (uint8_t *)buffer);
  
  TEST_ASSERT_EQUAL_TLV(TLV_WRITE_RAM, 13, 0xDEADBEEF, (uint8_t *)&buffer[1], tlv);
}