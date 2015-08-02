#ifndef CustomAssertion_H
#define CustomAssertion_H

#include <stdio.h>
#include "TLV_Protocol.h"

#define TEST_ASSERT_EQUAL_TLV(type, length, value, actual)		assertTLV( type,  \
                                                                         length, \
                                                                         value,  \
                                                                         actual, \
                                                                         __LINE__);                                                                            
                                                                  
void assertTLV(uint8_t type, uint8_t length, uint8_t *value, TLV_TypeDef *actual, int line);
//void assertTxBuffer(uint8_t type, uint8_t length, uint8_t *address, uint8_t *value, uint8_t chksum, uint8_t *txBuffer, int line);

#endif // CustomAssertion_H
