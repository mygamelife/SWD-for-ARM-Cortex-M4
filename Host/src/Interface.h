#ifndef Interface_H
#define Interface_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "Token.h"
#include "Tlv.h"
#include "ErrorCode.h"

typedef struct
{
  int size;
  uint32_t *data;
  uint32_t address;
  char *fileName;
  Tlv_Command tlvCommand;
} User_Session;

typedef enum {
  R0 = 0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	R9,
	R10,
	R11,
	R12,
	SP,
	LR,
	PC,
	xPSR,
	MSP,
	PSP,
	SR,
	FPSCR = 33,
	FPREGS0 = 64,
	FPREGS1 ,
	FPREGS2 ,
	FPREGS3 ,
	FPREGS4 ,
	FPREGS5 ,
	FPREGS6 ,
	FPREGS7 ,
	FPREGS8 ,
	FPREGS9 ,
	FPREGS10 ,
	FPREGS11 ,
	FPREGS12 ,
	FPREGS13 ,
	FPREGS14 ,
	FPREGS15 ,
	FPREGS16 ,
	FPREGS17 ,
	FPREGS18 ,
	FPREGS19 ,
	FPREGS20 ,
	FPREGS21 ,
	FPREGS22 ,
	FPREGS23 ,
	FPREGS24 ,
	FPREGS25 ,
	FPREGS26 ,
	FPREGS27 ,
	FPREGS28 ,
	FPREGS29 ,
	FPREGS30 ,
	FPREGS31 = 95
} Register_Address;

void displayOptionMenu(void);
User_Session *waitUserCommand(void);
User_Session *userInputInterpreter(String *str);
int getRegisterAddress(char *name);
void displayTlvData(Tlv *tlv);

/* User Instruction */
User_Session *userLoadProgram(String *userInput);
User_Session *userReadMemory(String *userInput);
User_Session *userWriteRegister(String *userInput);
User_Session *userReadRegister(String *userInput);
User_Session *userStepTarget(String *userInput);
User_Session *userHaltTarget(void);
User_Session *userRunTarget(void);
User_Session *userExit(void);

#endif // Interface_H