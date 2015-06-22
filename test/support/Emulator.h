#ifndef Emulator_H
#define Emulator_H

#include "unity.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"

void emulateRead(int data, int numOfBits);
void emulateWrite(int data, int numOfBits);
void emulateTurnAroundRead();
void emulateTurnAroundWrite();
void emulateSwdOutput();
void emulateSwdInput();
void emulateIdleClock(int numOfClocks);
#endif // Emulator_H
