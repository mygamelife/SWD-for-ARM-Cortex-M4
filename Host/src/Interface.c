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

/** getRegisterAddress is a function to get user Input
  * in string and get the register address from the string
  *
  * Input   : name is the string enter by user
  *
  * return  : Register_Address in enum
  */
int getRegisterAddress(char *name)  {
  if(strcmp(name, "R0") == 0)           return R0;
  else if(strcmp(name, "R1") == 0)      return R1;
  else if(strcmp(name, "R2") == 0)      return R2;
  else if(strcmp(name, "R3") == 0)      return R3;
  else if(strcmp(name, "R4") == 0)      return R4;
  else if(strcmp(name, "R5") == 0)      return R5;
  else if(strcmp(name, "R6") == 0)      return R6;
  else if(strcmp(name, "R7") == 0)      return R7;
  else if(strcmp(name, "R8") == 0)      return R8;
  else if(strcmp(name, "R9") == 0)      return R9;
  else if(strcmp(name, "R10") == 0)     return R10;
  else if(strcmp(name, "R11") == 0)     return R11;
  else if(strcmp(name, "R12") == 0)     return R12;
  else if(strcmp(name, "SP") == 0)      return SP;
  else if(strcmp(name, "LR") == 0)      return LR;
  else if(strcmp(name, "PC") == 0)      return PC;
  else if(strcmp(name, "xPSR") == 0)    return xPSR;
  else if(strcmp(name, "MSP") == 0)     return MSP;
  else if(strcmp(name, "PSP") == 0)     return PSP;
  else if(strcmp(name, "SR") == 0)      return SR;
  
  else Throw(ERR_INVALID_REGISTER_ADDRESS);
}

/** userLoadProgram is a function to get load program instruction
  * and file path need to load into RAM/Flash from user
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userLoadProgram(String *userInput)  {
  static User_Session userSession;  int regAddress = 0;
  CEXCEPTION_T err;   
  Identifier *iden, *memory;
  
  Try {
    memory = (Identifier*)getToken(userInput);
    iden = (Identifier*)getToken(userInput);
  } Catch(err) {
    Throw(ERR_INCOMPLETE_COMMAND);
  }
  
  if(iden->type != FILE_TOKEN)                  Throw(ERR_EXPECT_FILE_PATH);
  else if(memory->type != IDENTIFIER_TOKEN)     Throw(ERR_EXPECT_MEMORY_SELECTION);
  else if(strcmp(memory->name, "ram") == 0)     userSession.tlvCommand = TLV_WRITE_RAM;
  else if(strcmp(memory->name, "flash") == 0)   userSession.tlvCommand = TLV_WRITE_FLASH;
  else Throw(ERR_INVALID_MEMORY_SELECTION);
  
  userSession.fileName = iden->name;
  
  return &userSession;
}

/** userReadMemory is a function to get read memory instruction
  * and memory address need to read from user
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userReadMemory(String *userInput) {
  static User_Session userSession;  int regAddress = 0;
  CEXCEPTION_T err;   Number *address, *size;
  
  Try {
    address = (Number*)getToken(userInput);
    size = (Number*)getToken(userInput);
  } Catch(err)  {
    Throw(ERR_INCOMPLETE_COMMAND);
  }

  if(address->type != NUMBER_TOKEN)     Throw(ERR_EXPECT_NUMBER);
  else if(size->type != NUMBER_TOKEN)   Throw(ERR_EXPECT_NUMBER);
    
  userSession.tlvCommand = TLV_READ_MEMORY;
  userSession.address = address->value;
  userSession.size = size->value * 4;
  
  return &userSession;
}

/** userWriteRegister is a function to get register address
  * and value need to write into register
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userWriteRegister(String *userInput)  {
  static User_Session userSession;  int regAddress = 0;
  CEXCEPTION_T err;
  Identifier* iden; Number* data;
  
  Try {
    iden = (Identifier*)getToken(userInput);
    data = (Number*)getToken(userInput);    
  } Catch(err)  {
    Throw(ERR_INCOMPLETE_COMMAND);
  }
  
  if(iden->type != IDENTIFIER_TOKEN)    Throw(ERR_EXPECT_REGISTER_ADDRESS);
  else if(data->type != NUMBER_TOKEN)   Throw(ERR_EXPECT_NUMBER);
  
  regAddress = getRegisterAddress(iden->name);
  userSession.tlvCommand = TLV_WRITE_REGISTER;
  userSession.data = &data->value;
  userSession.address = regAddress;

  return &userSession;
}

/** userReadRegister is a function to get register address
  * enter by user
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userReadRegister(String *userInput) {
  static User_Session userSession;  int regAddress = 0;
  CEXCEPTION_T err; Identifier* iden;
  
  Try {
    iden = (Identifier*)getToken(userInput);
  } Catch(err)  {
    Throw(ERR_INCOMPLETE_COMMAND);
  }
  
  if(iden->type != IDENTIFIER_TOKEN)  Throw(ERR_EXPECT_REGISTER_ADDRESS);
  
  regAddress = getRegisterAddress(iden->name);
  userSession.tlvCommand = TLV_READ_REGISTER;
  userSession.address = regAddress;

  return &userSession;
}

/** userHaltTarget is a function to get halt instruction from user
  *
  * Input   : NONE
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userHaltTarget(void) {
  static User_Session userSession;
  
  userSession.tlvCommand = TLV_HALT_TARGET;
  
  return &userSession;
}

/** userRunTarget is a function to get run instruction from user
  *
  * Input   : NONE
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userRunTarget(void) {
  static User_Session userSession;
  
  userSession.tlvCommand = TLV_RUN_TARGET;
  
  return &userSession;
}

/** userStepTarget is a function to get step instruction 
  * and number of step required from user
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userStepTarget(String *userInput) {
  static User_Session userSession;
  Number *data; CEXCEPTION_T err;
  
  Try {
    data = (Number*)getToken(userInput);
  } Catch(err) {
    Throw(ERR_INCOMPLETE_COMMAND);
  }

  if(data->type != NUMBER_TOKEN)  Throw(ERR_EXPECT_NUMBER);

  userSession.tlvCommand = TLV_STEP;
  userSession.data = &data->value;
  
  return &userSession;
}

/** userExit is a function to exit instruction
  *
  * Input   : userInput is the string enter by userExit
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userExit(void) {
  static User_Session userSession;
  
  userSession.tlvCommand = TLV_EXIT;
  
  return &userSession;
}

/** userInputInterpreter is a function to interpreter user input to 
  * a meaningful information to machine
  *
  * Input   : userInput is the string enter by userExit
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userInputInterpreter(String *userInput)  {
  
  Identifier *command = (Identifier*)getToken(userInput);
  
  if(strcmp(command->name, "load") == 0)          return userLoadProgram(userInput);
  if(strcmp(command->name, "rmem") == 0)          return userReadMemory(userInput);
  else if(strcmp(command->name, "wreg") == 0)     return userWriteRegister(userInput);
  else if(strcmp(command->name, "reg") == 0)      return userReadRegister(userInput);
  else if(strcmp(command->name, "halt") == 0)     return userHaltTarget();
  else if(strcmp(command->name, "run") == 0)      return userRunTarget();
  else if(strcmp(command->name, "step") == 0)     return userStepTarget(userInput);
  else if(strcmp(command->name, "exit") == 0)     return userExit();
  
  else Throw(ERR_INVALID_USER_COMMAND);
}

/** waitUserCommand is a function to aquire whatever information
  * enter by user
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
