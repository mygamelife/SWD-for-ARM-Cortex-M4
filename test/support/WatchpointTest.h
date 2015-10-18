#ifndef WatchpointTest_H
#define WatchpointTest_H

#include "DWT_Unit.h"
#include "CoreDebug.h"

void datawatchpoint_TestCase_ReadByte_LDRB(Tlv_Session *session);
void datawatchpoint_TestCase_ReadByte_LDRH(Tlv_Session *session);
void datawatchpoint_TestCase_ReadByte_LDR(Tlv_Session *session);

void datawatchpoint_TestCase_ReadHalfword_LDRB(Tlv_Session *session);
void datawatchpoint_TestCase_ReadHalfword_LDRH(Tlv_Session *session);
void datawatchpoint_TestCase_ReadHalfword_LDR(Tlv_Session *session);

void datawatchpoint_TestCase_ReadWord_LDRB(Tlv_Session *session);
void datawatchpoint_TestCase_ReadWord_LDRH(Tlv_Session *session);
void datawatchpoint_TestCase_ReadWord_LDR(Tlv_Session *session);

void datawatchpoint_TestCase_WriteByte_STRB(Tlv_Session *session);
void datawatchpoint_TestCase_WriteByte_STRH(Tlv_Session *session);
void datawatchpoint_TestCase_WriteByte_STR(Tlv_Session *session);

void datawatchpoint_TestCase_WriteHalfword_STRB(Tlv_Session *session);
void datawatchpoint_TestCase_WriteHalfword_STRH(Tlv_Session *session);
void datawatchpoint_TestCase_WriteHalfword_STR(Tlv_Session *session);

void datawatchpoint_TestCase_WriteWord_STRB(Tlv_Session *session);
void datawatchpoint_TestCase_WriteWord_STRH(Tlv_Session *session);
void datawatchpoint_TestCase_WriteWord_STR(Tlv_Session *session);



#endif // WatchpointTest_H
