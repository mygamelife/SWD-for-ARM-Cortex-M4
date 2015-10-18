#ifndef BreakpointTest_H
#define BreakpointTest_H

#include "FPB_Unit.h"
#include "CoreDebug.h"

void instructionBreakPointTestCase_2bytes_LowerHalfWord(Tlv_Session *session);
void instructionBreakPointTestCase_2bytes_UpperHalfWord(Tlv_Session *session);
void instructionBreakPointTestCase_2bytes_Word(Tlv_Session *session);

void instructionBreakPointTestCase_4bytes_UpperHalfWord(Tlv_Session *session);
void instructionBreakPointTestCase_4bytes_Word(Tlv_Session *session);

void instructionBreakPointTestCase_2bytes_4bytes_4bytes_LowerHalfword(Tlv_Session *session);
void instructionBreakPointTestCase_2bytes_4bytes_4bytes_Word(Tlv_Session *session);

void instructionBreakPointTestCase_2bytes_4bytes_2bytes_Word(Tlv_Session *session);
#endif // BreakpointTest_H
