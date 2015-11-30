#ifndef Interface_H
#define Interface_H

#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <malloc.h>
#include "Token.h"
#include "Tlv.h"
#include "ErrorCode.h"

#define INPUT_BUFFER_SIZE 2048

typedef struct
{
  int size;
  uint32_t data[INPUT_BUFFER_SIZE];
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

typedef enum {
  BANK_1 = 1,
  BANK_2,
  BOTH_BANK,
} Flash_Bank;

typedef enum {
  HELP = 1,
  LOAD,
  WRITE_COMMAND,
  READ_MEMORY,
  WRITE_REGISTER,
  READ_REGISTER,
  HALT,
  RUN,
  STEP,
  BREAKPOINT,
  WATCHPOINT,
  ERASE,
  RESET_COMMAND,
  EXIT
} Command_Code;

void displayOptionMenu(void);
User_Session *createNewUserSession(void);
User_Session *waitUserCommand(void);
void InterpreteCommand(User_Session *us, String *userInput);
int getRegisterAddress(char *name);
int getFlashBank(char *name);
void displayTlvData(Tlv *tlv);
void displayMemoryMap(uint8_t *data, int length);
void displayFourByteInRow(uint8_t *data);
void helpMenu(String *userInput);
void helpCommand(Command_Code ccode);
Command_Code getCommandCode(char *commandName);

/* User Instruction */
void userLoadProgram(User_Session *us, String *userInput);
void userWriteMemory(User_Session *us, String *userInput);
void userReadMemory(User_Session *us, String *userInput);
void userWriteRegister(User_Session *us, String *userInput);
void userReadRegister(User_Session *us, String *userInput);
void userStepTarget(User_Session *us, String *userInput);
void userHaltTarget(User_Session *us);
void userRunTarget(User_Session *us);
void userSetBreakpoint(User_Session *us, String *userInput);
void userErase(User_Session *us, String *userInput);
void userSectionErase(User_Session *us, String *userInput);
void userReset(User_Session *us, String *userInput);
void userExit(User_Session *us);

void deleteUserSession(User_Session *us);
#endif // Interface_H