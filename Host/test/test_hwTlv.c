#include "unity.h"
#include <stdbool.h>
#include "Tlv.h"
#include "Tlv_ex.h"
#include "uart.h"
#include "ErrorCode.h"
#include "GetTime.h"

void setUp(void) {}

void tearDown(void) {}

void test_tlvLoopBack_(void) {
  Tlv_Session *session = tlvCreateSession();
  Tlv *reply, *tlv;
  int i, j; CEXCEPTION_T err;
  uint8_t data[252] = {0};
  
  
  for(j = 0; j < 500; j++) {
    for(i = 0; i < 252; i++) data[i] = i;

    tlv = tlvCreatePacket(TLV_LOOP_BACK, 252, data);
    tlvSend(session, tlv);
    do {
      tlvService(session);
      reply = tlvReceive(session);
    } while(reply == NULL);
    
    TEST_ASSERT_EQUAL(TLV_OK, reply->type);
    TEST_ASSERT_EQUAL(253, reply->length);
    
    for(i = 0; i < 252; i++) {
      TEST_ASSERT_EQUAL(i + 2, reply->value[i]);
    }    
  }
  closeSerialPort((HANDLE *)session->handler);
}