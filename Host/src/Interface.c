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
  printf("9. Set breakpoint\n");
  printf("10. Exit\n");
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
  
  if(strcmp(name, "R0") == 0)                 return R0;
  else if(strcmp(name, "R1") == 0)            return R1;
  else if(strcmp(name, "R2") == 0)            return R2;
  else if(strcmp(name, "R3") == 0)            return R3;
  else if(strcmp(name, "R4") == 0)            return R4;
  else if(strcmp(name, "R5") == 0)            return R5;
  else if(strcmp(name, "R6") == 0)            return R6;
  else if(strcmp(name, "R7") == 0)            return R7;
  else if(strcmp(name, "R8") == 0)            return R8;
  else if(strcmp(name, "R9") == 0)            return R9;
  else if(strcmp(name, "R10") == 0)           return R10;
  else if(strcmp(name, "R11") == 0)           return R11;
  else if(strcmp(name, "R12") == 0)           return R12;
  else if(strcmp(name, "SP") == 0)            return SP;
  else if(strcmp(name, "LR") == 0)            return LR;
  else if(strcmp(name, "PC") == 0)            return PC;
  else if(strcmp(name, "xPSR") == 0)          return xPSR;
  else if(strcmp(name, "MSP") == 0)           return MSP;
  else if(strcmp(name, "PSP") == 0)           return PSP;
  else if(strcmp(name, "SR") == 0)            return SR;
  else if(strcmp(name, "FPSCR") == 0)         return FPSCR;
  else if(strcmp(name, "FPREGS0") == 0)       return FPREGS0;
  else if(strcmp(name, "FPREGS1") == 0)       return FPREGS1;
  else if(strcmp(name, "FPREGS2") == 0)       return FPREGS2;
  else if(strcmp(name, "FPREGS3") == 0)       return FPREGS3;
  else if(strcmp(name, "FPREGS4") == 0)       return FPREGS4;
  else if(strcmp(name, "FPREGS5") == 0)       return FPREGS5;
  else if(strcmp(name, "FPREGS6") == 0)       return FPREGS6;
  else if(strcmp(name, "FPREGS7") == 0)       return FPREGS7;
  else if(strcmp(name, "FPREGS8") == 0)       return FPREGS8;
  else if(strcmp(name, "FPREGS9") == 0)       return FPREGS9;
  else if(strcmp(name, "FPREGS10") == 0)      return FPREGS10;
  else if(strcmp(name, "FPREGS11") == 0)      return FPREGS11;
  else if(strcmp(name, "FPREGS12") == 0)      return FPREGS12;
  else if(strcmp(name, "FPREGS13") == 0)      return FPREGS13;
  else if(strcmp(name, "FPREGS14") == 0)      return FPREGS14;
  else if(strcmp(name, "FPREGS15") == 0)      return FPREGS15;
  else if(strcmp(name, "FPREGS16") == 0)      return FPREGS16;
  else if(strcmp(name, "FPREGS17") == 0)      return FPREGS17;
  else if(strcmp(name, "FPREGS18") == 0)      return FPREGS18;
  else if(strcmp(name, "FPREGS19") == 0)      return FPREGS19;
  else if(strcmp(name, "FPREGS20") == 0)      return FPREGS20;
  else if(strcmp(name, "FPREGS21") == 0)      return FPREGS21;
  else if(strcmp(name, "FPREGS22") == 0)      return FPREGS22;
  else if(strcmp(name, "FPREGS23") == 0)      return FPREGS23;
  else if(strcmp(name, "FPREGS24") == 0)      return FPREGS24;
  else if(strcmp(name, "FPREGS25") == 0)      return FPREGS25;
  else if(strcmp(name, "FPREGS26") == 0)      return FPREGS26;
  else if(strcmp(name, "FPREGS27") == 0)      return FPREGS27;
  else if(strcmp(name, "FPREGS28") == 0)      return FPREGS28;
  else if(strcmp(name, "FPREGS29") == 0)      return FPREGS29;
  else if(strcmp(name, "FPREGS30") == 0)      return FPREGS30;
  else if(strcmp(name, "FPREGS31") == 0)      return FPREGS31;
  
  else Throw(ERR_INVALID_REGISTER_ADDRESS);
}

/** getRegisterAddress is a function to get user Input
  * in string and get the register address from the string
  *
  * Input   : name is the string enter by user
  *
  * return  : Register_Address in enum
  */
int getFlashBank(char *name) {
  
  if(strcmp(name, "bank1") == 0)              return BANK_1;
  else if(strcmp(name, "bank2") == 0)         return BANK_2;
  else if(strcmp(name, "full") == 0)          return BOTH_BANK;
  
  else Throw(ERR_INVALID_BANK_SELECTION);
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

/** userSetBreakpoint is a function to get address need
  * to set breakpoint from user
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userSetBreakpoint(String *userInput) {
  static User_Session userSession;
  CEXCEPTION_T err; Number *address;
  
  Try {
    address = (Number*)getToken(userInput);
  } Catch(err)  {
    Throw(ERR_INCOMPLETE_COMMAND);
  }

  if(address->type != NUMBER_TOKEN)     Throw(ERR_EXPECT_NUMBER);
  
  userSession.tlvCommand = TLV_BREAKPOINT;
  userSession.address = address->value;

  return &userSession;
}

User_Session *userSectionErase(String *userInput) {
  static User_Session userSession;
  CEXCEPTION_T err;   Number *address, *size;
  
  Try {
    address = (Number*)getToken(userInput);
    size = (Number*)getToken(userInput);
  } Catch(err)  {
    Throw(ERR_INCOMPLETE_COMMAND);
  }

  if(address->type != NUMBER_TOKEN)     Throw(ERR_EXPECT_NUMBER);
  else if(size->type != NUMBER_TOKEN)   Throw(ERR_EXPECT_NUMBER);
    
  userSession.tlvCommand = TLV_FLASH_ERASE;
  userSession.address = address->value;
  userSession.size = size->value * 4;
  
  return &userSession;
}

/** userErase is a function to retrieve erase option 
  * enter by user
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userErase(String *userInput) {
  static User_Session userSession;
  CEXCEPTION_T err; Identifier* iden;
  int eraseSection = 0;
  
  Try {
    iden = (Identifier*)getToken(userInput);
  } Catch(err)  {
    Throw(ERR_INCOMPLETE_COMMAND);
  }
  
  if(iden->type != IDENTIFIER_TOKEN)  Throw(ERR_EXPECT_ERASE_SECTION);
  
  else {
    if(strcmp(iden->name, "section") == 0) {
      return userSectionErase(userInput);
    }
    else {
      userSession.tlvCommand = TLV_FLASH_MASS_ERASE;
      eraseSection = getFlashBank(iden->name);
      userSession.data = &eraseSection;
    }
  }

  return &userSession;
}

/** userReset is a function to retrieve reset option 
  * enter by user
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userReset(String *userInput) {
  static User_Session userSession;
  CEXCEPTION_T err; Identifier *option;
  
  Try {
    option = (Identifier*)getToken(userInput);
  } Catch(err) {
    Throw(ERR_INCOMPLETE_COMMAND);
  }
  
  if(option->type != IDENTIFIER_TOKEN)          Throw(ERR_OPTION_NOT_FOUND);
  else if(strcmp(option->name, "soft") == 0)    userSession.tlvCommand = TLV_SOFT_RESET;
  else if(strcmp(option->name, "hard") == 0)    userSession.tlvCommand = TLV_HARD_RESET;
  else Throw(ERR_INVALID_COMMAND);
  
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
  
  if(strcmp(command->name, "load") == 0)                return userLoadProgram(userInput);
  if(strcmp(command->name, "rmem") == 0)                return userReadMemory(userInput);
  else if(strcmp(command->name, "wreg") == 0)           return userWriteRegister(userInput);
  else if(strcmp(command->name, "reg") == 0)            return userReadRegister(userInput);
  else if(strcmp(command->name, "halt") == 0)           return userHaltTarget();
  else if(strcmp(command->name, "run") == 0)            return userRunTarget();
  else if(strcmp(command->name, "step") == 0)           return userStepTarget(userInput);
  else if(strcmp(command->name, "brkpt") == 0)          return userSetBreakpoint(userInput);
  else if(strcmp(command->name, "erase") == 0)          return userErase(userInput);
  else if(strcmp(command->name, "reset") == 0)          return userReset(userInput);
  else if(strcmp(command->name, "exit") == 0)           return userExit();
  
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
