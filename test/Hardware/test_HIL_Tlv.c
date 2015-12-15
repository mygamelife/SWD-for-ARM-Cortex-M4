#include "unity.h"
#include <stdbool.h>
#include "Tlv.h"
#include "Uart.h"
#include "TlvEx.h"
#include "Yield.h"
#include "SystemTime.h"
#include "ErrorCode.h"

static Tlv_Session *session = NULL;

void setUp(void) {
  if(session == NULL) {
    session = tlvCreateSession();
  }
}

void tearDown(void) {}

void test_tlvLoopBack_should_throw_when_data_size_is_1_but_length_is_3(void) {
  CEXCEPTION_T err;
  Tlv *reply, *tlv;
  uint8_t data[] = {TLV_LOOP_BACK, 3, 0xaa};

  sendBytes(session->handler, data, 3);

  Try {
    do {
      tlvService(session);
      reply = tlvReceive(session);
    } while(reply == NULL);    
  } Catch(err) {
    TEST_ASSERT_EQUAL(PROBE_TLV_TIMEOUT, err);
  }
}

void test_tlvLoopBack_(void) {
  Tlv *reply, *tlv;
  int i, j, k;
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