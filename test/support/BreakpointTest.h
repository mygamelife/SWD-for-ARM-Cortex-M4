#ifndef BreakpointTest_H
#define BreakpointTest_H

#include "FPB_Unit.h"
#include "CoreDebug.h"

void instructionBreakPointTestCase_2bytes_LowerHalfWord();
void instructionBreakPointTestCase_2bytes_UpperHalfWord();
void instructionBreakPointTestCase_2bytes_Word();

void instructionBreakPointTestCase_4bytes_UpperHalfWord();
void instructionBreakPointTestCase_4bytes_Word();

void instructionBreakPointTestCase_2bytes_4bytes_4bytes_LowerHalfword();
void instructionBreakPointTestCase_2bytes_4bytes_4bytes_Word();

void instructionBreakPointTestCase_2bytes_4bytes_2bytes_Word();
#endif // BreakpointTest_H
