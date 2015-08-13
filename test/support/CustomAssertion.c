#include "CustomAssertion.h"

void assertTLV(uint8_t type, uint8_t length, uint32_t address, uint8_t *dataAddress, TLV *actual, int line) {
  int i;  char msg[30];
  
  TEST_ASSERT_EQUAL(type, actual->type);
  TEST_ASSERT_EQUAL(length, actual->length);
  TEST_ASSERT_EQUAL_HEX32(address, get4Byte(&actual->value[0]));

  for(i = 0; i < actual->length - ADDRESS_LENGTH - 1; i++) {
    if(dataAddress[i] != actual->value[i + ADDRESS_LENGTH]) {
      sprintf(msg, "Value not match at index %d", i);
      printf("data %x actual %x\n", dataAddress[i], actual->value[i + ADDRESS_LENGTH]);
    }
    
    UNITY_TEST_ASSERT_EQUAL_INT(dataAddress[i], actual->value[i + ADDRESS_LENGTH], line, msg);
	}
}