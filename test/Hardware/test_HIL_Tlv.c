#include "unity.h"
#include <stdbool.h>
#include "Tlv.h"
#include "Uart.h"
#include "TlvEx.h"
#include "SystemTime.h"
#include "ErrorCode.h"

void setUp(void) {}

void tearDown(void) {}

void test_tlvLoopBack_(void) {
  Tlv_Session *session = tlvCreateSession();
  Tlv *reply, *tlv;
  int i, j, k; CEXCEPTION_T err;
  uint8_t data[252] = {0};
  
  for(j = 0; j < 50; j++) {
    for(i = 0, k = j; i < 252; i++, k++) data[i] = k;

    tlv = tlvCreatePacket(TLV_LOOP_BACK, 252, data);
    tlvSend(session, tlv);
    do {
      tlvService(session);
      reply = tlvReceive(session);
    } while(reply == NULL);
    
    TEST_ASSERT_EQUAL(TLV_OK, reply->type);
    TEST_ASSERT_EQUAL(252 + 1, reply->length);
    
    printf("packet number %d\n", j);
    
    for(i = 0, k = j; i < 252; i++, k++) {
      TEST_ASSERT_EQUAL_HEX8(k + 2, reply->value[i]);
    }
  }
  closePort(session);
}