#ifndef Interface_H
#define Interface_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "Token.h"
#include "Tlv.h"

typedef struct
{
  int size;
  uint32_t *data;
  uint32_t address;
  char *fileName;
  Tlv_Command tlvCommand;
} User_Session;

void displayOptionMenu(void);
User_Session *waitUserCommand(void);
User_Session *userInputInterpreter(int option, String *str);
int getRegisterAddress(char *name);
void displayTlvData(Tlv *tlv);

/* User Instruction */
User_Session *userLoadRam(String *userInput);
User_Session *userLoadFlash(String *userInput);
User_Session *userReadMemory(String *userInput);
User_Session *userWriteRegister(String *userInput);
User_Session *userReadRegister(String *userInput);
User_Session *userHaltTarget(String *userInput);
User_Session *userRunTarget(String *userInput);
User_Session *userStepTarget(String *userInput);
User_Session *userExit(String *userInput);

#endif // Interface_H