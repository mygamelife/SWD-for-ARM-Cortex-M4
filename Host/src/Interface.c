#include "Interface.h"

#define BUFFER_SIZE 1024

void displayOptionMenu(void)  {
  printf("1. Load Program To RAM\n");
  printf("2. Load Program To Flash\n");
  printf("3. Read Memory\n");
  printf("4. Write Register\n");
  printf("5. Read Register\n");
  printf("6. Halt target\n");
  printf("7. Run target\n");
  printf("8. Step target\n");
  printf("9. Exit\n");
}

void displayTlvData(Tlv *tlv)  {
  int i, length = 0, counter = 0;
  length = tlv->length;
  
  if(length == 5) {
    printf("Value : %x\n\n", get4Byte(&tlv->value[0]));
  } else if(length > 5)  {
    printf("Address %x\n", get4Byte(&tlv->value[0]));
    for(i = 4; i < length - 1; i += 4)  {

      if(counter == 4) {
        counter = 0;
        printf("\n");
      }
      printf("%x ", get4Byte(&tlv->value[i]));
      counter++;
    }
    printf("\n\n");
  }
  else printf("OK\n\n");
}

int getRegisterAddress(char *name)  {
  if(strcmp(name, "R0") == 0)         return R0;
  else if(strcmp(name, "R1") == 0)    return R1;
  else if(strcmp(name, "R2") == 0)    return R2;
  else if(strcmp(name, "R3") == 0)    return R3;
  else if(strcmp(name, "SP") == 0)    return SP;
  else if(strcmp(name, "LR") == 0)    return LR;
  else if(strcmp(name, "PC") == 0)    return PC;
  else if(strcmp(name, "xPSR") == 0)  return xPSR;
  else if(strcmp(name, "MSP") == 0)   return MSP;
  else if(strcmp(name, "PSP") == 0)   return PSP;
  else if(strcmp(name, "SR") == 0)    return SR;
}

// User_Session *userLoadRam(String *userInput)  {
  
// }
// User_Session *userLoadFlash(String *userInput);
// User_Session *userReadMemory(String *userInput);

/** userWriteRegister is a function to get register address
  * and value need to write into register
  *
  * Input   : userInput is the string enter by userExit
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userWriteRegister(String *userInput)  {
  static User_Session userSession;
  int regAddress = 0;
  
  Identifier* iden = (Identifier*)getToken(userInput);
  Number* data = (Number*)getToken(userInput);
  
  regAddress = getRegisterAddress(iden->name);

  userSession.tlvCommand = TLV_WRITE_REGISTER;
  userSession.data = &data->value;
  userSession.address = regAddress;
  
  return &userSession;
}

User_Session *userReadRegister(String *userInput) {
  
}

// User_Session *userHaltTarget(String *userInput);
// User_Session *userRunTarget(String *userInput);
// User_Session *userStepTarget(String *userInput);

/** userExit is a function to exit instruction
  *
  * Input   : userInput is the string enter by userExit
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userExit(String *userInput) {
  static User_Session userSession;
  
  userSession.tlvCommand = TLV_EXIT;
  
  return &userSession;
}

/** userInputInterpreter
  *
  * Input   : NONE
  *
  * Return  : NONE
  */
User_Session *userInputInterpreter(String *userInput)  {
  Identifier *command = (Identifier*)getToken(userInput);
  
  printf("Command %s\n", command->name);
  
  if(strcmp(command->name, "wreg") == 0)        return userWriteRegister(userInput);
  else if(strcmp(command->name, "reg") == 0)    return userReadRegister(userInput);
  else if(strcmp(command->name, "exit") == 0)   return userExit(userInput);
  // static User_Session userSession;
  // int regAddress = 0, i = 0;
  // Number *data, *size, *address;
  // Identifier *iden;
  // uint32_t buffer[255] = {0};
  
  // if(option == 1) {
    // iden = (Identifier*)getToken(str);
    // userSession.tlvCommand = TLV_WRITE_RAM;
    // userSession.fileName = iden->name;
    // return &userSession;
  // }
  // else if(option == 2) {
      // something here
  // }
  // else if(option == 3) {
    // address = (Number*)getToken(str);
    // size = (Number*)getToken(str);
    
    // userSession.tlvCommand = TLV_READ_MEMORY;
    // userSession.address = address->value;
    // userSession.size = size->value * 4;
    // return &userSession;
  // }
  // else if(option == 4) {
    // iden = (Identifier*)getToken(str);
    // data = (Number*)getToken(str);
    // regAddress = getRegisterAddress(iden->name);
    
    // userSession.tlvCommand = TLV_WRITE_REGISTER;
    // userSession.data = &data->value;
    // userSession.address = regAddress;
    // return &userSession;
  // }
  // else if(option == 5) {
    // iden = (Identifier*)getToken(str);
    // regAddress = getRegisterAddress(iden->name);
    
    // userSession.tlvCommand = TLV_READ_REGISTER;
    // userSession.address = regAddress;
    // return &userSession;
  // }
  // else if(option == 6) {
    // userSession.tlvCommand = TLV_HALT_TARGET;
    // return &userSession;
  // }
  // else if(option == 7) {
    // userSession.tlvCommand = TLV_RUN_TARGET;
    // return &userSession;
  // }
  // else if(option == 8) {
    // data = (Number*)getToken(str);
    
    // userSession.tlvCommand = TLV_STEP;
    // userSession.data = &data->value;
    // return &userSession;
  // }
  // else if(option == 9) {
    // userSession.tlvCommand = TLV_EXIT;
    // return &userSession;
  // }
}

/** waitUserCommand
  *
  */
User_Session *waitUserCommand(void) {
  Number *num;  String *str;
  char InputBuffer[BUFFER_SIZE] = {0};

  while(1) {
    fgets(InputBuffer, BUFFER_SIZE, stdin);
    if(InputBuffer[strlen(InputBuffer) - 1] == '\n') break;
  }
  
  str = stringNew(InputBuffer);
  
  return userInputInterpreter(str);
}
