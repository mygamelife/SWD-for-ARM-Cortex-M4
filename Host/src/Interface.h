#ifndef Interface_H
#define Interface_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "Token.h"
#include "Tlv.h"

typedef struct
{
  int size;
  uint32_t *data;
  uint32_t address;
  char fileName[1024];
  Tlv_Command tlvCommand;
} User_Session;

void displayOptionMenu(void);
User_Session *waitUserCommand(void);
User_Session *userInputInterpreter(int option, String *str);
int getRegisterAddress(char *name);
void displayTlvData(Tlv *tlv);

#endif // Interface_H