#include "CustomAssertion.h"

void assertTLV(uint8_t type, uint8_t length, ElfSection *elf, TLV *actual, int line) {
  int index = 0, elfStart = 0;
  char msg[30];
  
  TEST_ASSERT_EQUAL(type, actual->type);
  TEST_ASSERT_EQUAL(length, actual->length);

  elfStart = elf->codeIndex - (actual->length - 5);

  for(index += ADDRESS_LENGTH; index < length - 1; index++) {
    if(elf->code[elfStart] != actual->value[index])
      sprintf(msg, "Value not match at index %d", index);
    
    // printf("%x    %x\n", elf->code[elfStart], actual->value[index]);
    UNITY_TEST_ASSERT_EQUAL_INT(elf->code[elfStart], actual->value[index], line, msg);
    elfStart++;
	}
}