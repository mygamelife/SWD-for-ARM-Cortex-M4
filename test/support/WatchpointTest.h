#ifndef WatchpointTest_H
#define WatchpointTest_H

#include "DWT_Unit.h"
#include "CoreDebug.h"

void datawatchpoint_TestCase_ReadByte_LDRB();
void datawatchpoint_TestCase_ReadByte_LDRH();
void datawatchpoint_TestCase_ReadByte_LDR();

void datawatchpoint_TestCase_ReadHalfword_LDRB();
void datawatchpoint_TestCase_ReadHalfword_LDRH();
void datawatchpoint_TestCase_ReadHalfword_LDR();

void datawatchpoint_TestCase_ReadWord_LDRB();
void datawatchpoint_TestCase_ReadWord_LDRH();
void datawatchpoint_TestCase_ReadWord_LDR();

void datawatchpoint_TestCase_WriteByte_STRB();
void datawatchpoint_TestCase_WriteByte_STRH();
void datawatchpoint_TestCase_WriteByte_STR();

void datawatchpoint_TestCase_WriteHalfword_STRB();
void datawatchpoint_TestCase_WriteHalfword_STRH();
void datawatchpoint_TestCase_WriteHalfword_STR();

void datawatchpoint_TestCase_WriteWord_STRB();
void datawatchpoint_TestCase_WriteWord_STRH();
void datawatchpoint_TestCase_WriteWord_STR();



#endif // WatchpointTest_H
