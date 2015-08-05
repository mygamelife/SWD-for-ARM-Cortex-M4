#ifndef CustomAssertion_H
#define CustomAssertion_H

#include <stdio.h>
#include "unity.h"
#include "TLV_Protocol.h"
#include "GetHeaders.h"

#define TEST_ASSERT_EQUAL_TLV(type, length, elf, actual)		assertTLV( type,    \
                                                                       length,  \
                                                                       elf,     \
                                                                       actual,  \
                                                                       __LINE__);                                                                            
                                                                  
void assertTLV(uint8_t type, uint8_t length, ElfSection *elf, TLV *actual, int line);

#endif // CustomAssertion_H
