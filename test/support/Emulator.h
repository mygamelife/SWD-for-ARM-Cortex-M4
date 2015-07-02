#ifndef Emulator_H
#define Emulator_H

#include "unity.h"
#include "swd_Utilities.h"
#include "mock_IO_Operations.h"
#include "mock_configurePort.h"

#define OK      0x4
#define WAIT    0x2
#define FAULT   0x1

uint32_t MSB_LSB_Conversion(uint32_t input);

void emulateRead(int data, int numOfBits);
void emulateWrite(int data, int numOfBits);
void emulateTurnAroundRead();
void emulateTurnAroundWrite();
void emulateSwdOutput();
void emulateSwdInput();
void emulateIdleClock(int numOfClocks);
void emulateLineReset(int numOfClocks);
void emulateResetTarget();
void emulateSWDRegister_Write(int Address,int APnDP,int emulateACK, uint32_t data);
void emulateSWDRegister_Read(int Address,int APnDP,int emulateACK,int emulateParity, uint32_t emulateData);
#endif // Emulator_H
