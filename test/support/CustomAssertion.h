#ifndef CustomAssertion_H
#define CustomAssertion_H

#include <stdio.h>
#include "unity.h"
#include "Tlv_Loader.h"
#include "GetHeaders.h"

#define TEST_ASSERT_EQUAL_TLV(  type,       \
                                length,     \
                                address,    \
                                dataAddress,\
                                actual  );		  assertTLV(  type,       \
                                                            length,     \
                                                            address,    \
                                                            dataAddress,\
                                                            actual,     \
                                                            __LINE__);                                                                            
                                                                  
void assertTLV(uint8_t type, uint8_t length, uint32_t address, uint8_t *dataAddress, Tlv *actual, int line);

#endif // CustomAssertion_H
