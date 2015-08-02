#include "CustomAssertion.h"

void assertTLV(uint8_t type, uint8_t length, uint8_t *value, TLV_TypeDef *actual, int line) {
  int index = 0;  char msg[30];
  
  TEST_ASSERT_EQUAL(type, actual->type);
  TEST_ASSERT_EQUAL(length, actual->length);

  for(index; index < length; index++) {
    if(value[index] != actual->value[index])
      sprintf(msg, "Value not match at index %d", index);
    
    UNITY_TEST_ASSERT_EQUAL_INT(value[index], actual->value[index], line, msg);
	}
}

// void assertTxBuffer(uint8_t type, uint8_t length, uint8_t *address, uint8_t *value, uint8_t chksum, uint8_t *txBuffer, int line) {
  // int index = 0;  char msg[30];
  
  // TEST_ASSERT_EQUAL(type, txBuffer[0]);
  
  // TEST_ASSERT_EQUAL(length, txBuffer[1]);
  
  // TEST_ASSERT_EQUAL(address[0], txBuffer[2]);
  // TEST_ASSERT_EQUAL(address[1], txBuffer[3]);
  // TEST_ASSERT_EQUAL(address[2], txBuffer[4]);
  // TEST_ASSERT_EQUAL(address[3], txBuffer[5]);
  
  // for(index; index < length; index++) {
    // if(value[index] != txBuffer[index + 6])
      // sprintf(msg, "Value not match at index %d", index);
    
    // UNITY_TEST_ASSERT_EQUAL_INT(value[index], txBuffer[index + 6], line, msg);
	// }
  
  // TEST_ASSERT_EQUAL(txBuffer[index + 6], chksum);
// }