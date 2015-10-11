#include "unity.h"
#include <stdbool.h>
#include "Tlv.h"
#include "Tlv_ex.h"
#include "uart.h"
#include "ErrorCode.h"
#include "GetTime.h"

void setUp(void) {}

void tearDown(void) {}

// void test_tlvService_(void)
// {
  // uint32_t data[] = { 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222, 
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222, 
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222, 0x11111111, 0x22222222,
                      // 0x11111111, 0x22222222};
                      
  // int i = 0, j = 0; 
  // Tlv_Session *session = tlvCreateSession();
  // Tlv *reply, *tlv;
  
  // tlv = tlvCreatePacket(TLV_WRITE_RAM, sizeof(data), (uint8_t *)data);
  
  // for(j = 0; j < 100; j++) {
    // tlvSend(session, tlv);
    // do {
      // tlvService(session);
      // reply = tlvReceive(session);
    // } while(reply == NULL);
    
    // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataReceiveFlag);
    // TEST_ASSERT_EQUAL(FLAG_CLEAR, session->dataSendFlag);
    // TEST_ASSERT_EQUAL(TLV_WRITE_RAM, reply->type);
    // TEST_ASSERT_EQUAL(sizeof(data) + 1, reply->length);
    
    // for(i = 0; i < sizeof(data); i += 8) {
      // printf("index %d\n", i + 0);
      // TEST_ASSERT_EQUAL(0x11111111, get4Byte(&reply->value[i + 0]));
      // printf("index %d\n", i + 4);
      // TEST_ASSERT_EQUAL(0x22222222, get4Byte(&reply->value[i + 4]));
    // }    
  // }
  // closeSerialPort((HANDLE *)session->handler);
// }

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