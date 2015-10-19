#include "Interface.h"

#define BUFFER_SIZE 2000
char InputBuffer[BUFFER_SIZE];

void displayOptionMenu(void)  {
  printf("\n");
  printf("=============================================================\n");
  printf("##        ARM CORTEX Serial Wire Debugger Interface        ##\n");
  printf("=============================================================\n");
  printf("\n");
  printf("See 'help' or 'help <command>' to read about a specific user command\n\n");
}

void displayFourByteInRow(uint8_t *data) {
  int i;
  
  for(i = 0; i < 16; i += 4)
    printf(" 0x%08x", get4Byte(&data[i]));
}

void displayMemoryMap(uint8_t *data, int length) {
  int row, col, rowLength, index = 4;
  uint32_t address = 0;
  
  rowLength = ((length / 4) - 1)/ 5 + 1;
  address = get4Byte(&data[0]);
  
  for(row = 0; row < rowLength; row ++)  {
    printf("> 0x%08x", address); address += 4;
    if(length > 4) {
      displayFourByteInRow(&data[index]);
      index += 16;      
    }
    printf("\n");
  }
  printf("\n");
}

void displayTlvData(Tlv *tlv)  {
  int i, length = 0, counter = 0;
  length = tlv->length;
  
  if(length == 1) 
    printf("> OK\n\n");
  else 
    displayMemoryMap(tlv->value, length - 1);
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
  else if(strcmp(memory->name, "ram") == 0)     userSession.tlvCommand = TLV_LOAD_RAM;
  else if(strcmp(memory->name, "flash") == 0)   userSession.tlvCommand = TLV_LOAD_FLASH;
  else Throw(ERR_INVALID_MEMORY_SELECTION);
  
  userSession.fileName = iden->name;
  
  return &userSession;
}

/** userWriteMemory is a function to get write data into
  * memory map RAM/Flash
  *
  * Input   : userInput is the string enter by user
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *userWriteMemory(String *userInput) {
  static User_Session userSession; CEXCEPTION_T err;
  Identifier *memory; Number* data, *address;
  int i = 0; uint32_t dataBlock[1024];
  
  Try {
    memory = (Identifier *)getToken(userInput);
    address = (Number *)getToken(userInput);
    if(address->type != NUMBER_TOKEN)   Throw(ERR_EXPECT_NUMBER);
    while(userInput->length > 1) {
      data = (Number *)getToken(userInput);
      if(data->type != NUMBER_TOKEN)   Throw(ERR_EXPECT_NUMBER);
      dataBlock[i++] = data->value;
    }
  } Catch(err) {
    Throw(ERR_INCOMPLETE_COMMAND);
  }
  
  if(memory->type != IDENTIFIER_TOKEN)          Throw(ERR_EXPECT_MEMORY_SELECTION);
  else if(strcmp(memory->name, "ram") == 0)     userSession.tlvCommand = TLV_WRITE_RAM;
  else if(strcmp(memory->name, "flash") == 0)   userSession.tlvCommand = TLV_WRITE_FLASH;
  else Throw(ERR_INVALID_MEMORY_SELECTION);
  
  userSession.size = i * 4;
  userSession.address = address->value;
  userSession.data = dataBlock;
  
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
  uint32_t data32;
  
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
  data32 = (uint32_t)data->value;
  userSession.data = &data32;
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
  uint32_t data32;
  
  Try {
    data = (Number*)getToken(userInput);
  } Catch(err) {
    Throw(ERR_INCOMPLETE_COMMAND);
  }

  if(data->type != NUMBER_TOKEN)  Throw(ERR_EXPECT_NUMBER);

  userSession.tlvCommand = TLV_STEP;
  data32 = (uint32_t)data->value;
  userSession.data = &data32;
  
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
  userSession.size = size->value;
  
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
      userSession.address = (uint32_t)getFlashBank(iden->name);
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

Command_Code getCommandCode(char *commandName) {

  if(strcmp(commandName, "help") == 0)                return HELP;
  else if(strcmp(commandName, "load") == 0)           return LOAD;
  else if(strcmp(commandName, "write") == 0)          return WRITE;
  else if(strcmp(commandName, "rmem") == 0)           return READ_MEMORY;
  else if(strcmp(commandName, "wreg") == 0)           return WRITE_REGISTER;
  else if(strcmp(commandName, "reg") == 0)            return READ_REGISTER;
  else if(strcmp(commandName, "halt") == 0)           return HALT;
  else if(strcmp(commandName, "run") == 0)            return RUN;
  else if(strcmp(commandName, "step") == 0)           return STEP;
  else if(strcmp(commandName, "brkpt") == 0)          return BREAKPOINT;
  else if(strcmp(commandName, "erase") == 0)          return ERASE;
  else if(strcmp(commandName, "reset") == 0)          return RESET_COMMAND;
  else if(strcmp(commandName, "exit") == 0)           return EXIT;
  
  else Throw(ERR_INVALID_USER_COMMAND);
}

/** helpMenu is a function to display useful information
  * about the user command
  *
  * Input   : NONE
  *
  * return  : NONE
  */
void helpMenu(String *userInput) {
  Identifier *command;
  Command_Code ccode;
  
  if(userInput->length <= 1) {
    printf("Available commands :\n\n");
    printf(" load     load program into target memory can either ram/flash\n");
    printf(" rmem     read data from target memory\n");
    printf(" wreg     write data into target register\n");
    printf(" reg      read data from target register\n");
    printf(" halt     halt target\n");
    printf(" run      run target\n");
    printf(" step     executes code one statement at a time\n");
    printf(" brkpt    stop or pause program at specific address\n");
    printf(" erase    erase target flash memory\n");
    printf(" reset    reset target process\n");
    printf(" exit     exit current program\n");
  }
  else {
    printf("\n");
    command = (Identifier*)getToken(userInput);
    ccode = getCommandCode(command->name);
    helpCommand(ccode);
  }
}

void helpCommand(Command_Code ccode) {
  
  switch(ccode) {
    case LOAD                         : printf(" load <memory> <file_path>                                                \n\n");
                                        printf(" <memory>     Target memory location that program will be                   \n");
                                        printf("              load into (ram/flash)                                         \n");
                                        printf(" <file_path>  Program elf file location                                     \n");
                                        break;
                                        
    case READ_MEMORY                  : printf(" rmem <address> <size>                                                      \n\n");
                                        printf(" <address>    Any valid memory address                                      \n");
                                        printf(" <size>       Size of the memory want to read                               \n");
                                        break;
    
    case WRITE_REGISTER               : printf(" wreg <reg_address> <value>                                               \n\n");                                       
                                        printf(" <reg_address>    Register address can be one of the following value :      \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Core Register 0 - 12 |   R0 - R12   |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Stack Pointer        |      SP      |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Link Register        |      LR      |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Program Counter      |      PC      |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Special-purpose      |              |               \n");
                                        printf("                      | program status       |     xPSR     |               \n");
                                        printf("                      | registers            |              |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Main Stack Pointer   |      MSP     |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Process Stack Pointer|      PSP     |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Special Register     |      SR      |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Floating Point       |              |               \n");
                                        printf("                      | Staus and            |     FPSCR    |               \n");
                                        printf("                      | Control Register     |              |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Floating Point       |              |               \n");
                                        printf("                      | Register 0 - 31      | FPSCR 0 - 31 |               \n");
                                        printf("                      =======================================               \n");
                                        printf(" <value>          Data write into register in byte format                   \n");
                                        break;
                                        
    case READ_REGISTER                : printf(" reg <reg_address>                                                        \n\n");    
                                        printf(" <reg_address>    Register address can be one of the following value :      \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Core Register 0 - 12 |   R0 - R12   |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Stack Pointer        |      SP      |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Link Register        |      LR      |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Program Counter      |      PC      |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Special-purpose      |              |               \n");
                                        printf("                      | program status       |     xPSR     |               \n");
                                        printf("                      | registers            |              |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Main Stack Pointer   |      MSP     |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Process Stack Pointer|      PSP     |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Special Register     |      SR      |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Floating Point       |              |               \n");
                                        printf("                      | Staus and            |     FPSCR    |               \n");
                                        printf("                      | Control Register     |              |               \n");
                                        printf("                      =======================================               \n");
                                        printf("                      | Floating Point       |              |               \n");
                                        printf("                      | Register 0 - 31      | FPSCR 0 - 31 |               \n");
                                        printf("                      =======================================               \n");
                                        break;
                                        
    case HALT                         : printf(" halt <empty>                                                             \n\n");
                                        break;
    
    case RUN                          : printf(" run <empty>                                                              \n\n");
                                        break;
    
    case STEP                         : printf(" step <number_of_step>                                                    \n\n");
                                        printf(" <number_of_step>   Number of statement can be executes at a time           \n");
                                        break;
                                        
    case BREAKPOINT                   : printf(" brkpt <address>                                                          \n\n");
                                        printf(" <address>   Any valid code region address 0x00000000 - 0x1FFFFFFF          \n");
                                        break;
                                        
    case ERASE                        : printf(" Section Erase                                                              \n");
                                        printf(" erase <section> <address> <size>                                         \n\n");
                                        printf(" <address>        Any valid flash address 0x08000000 - 0x081FFFFF           \n");
                                        printf(" <size>           Size of flash to erase can be any value                 \n\n");
                                        printf(" Mass Erase                                                                 \n");
                                        printf(" erase <bank_option>                                                      \n\n");
                                        printf(" <bank_option>    Flash bank option can be one of the following value :     \n");
                                        printf("                    bank1   :: Flash Bank 1 0x08000000 - 0x080E0000         \n");
                                        printf("                    bank2   :: Flash Bank 2 0x08100000 - 0x081FFFFF         \n");
                                        printf("                    full    :: Both Flash Bank 1 and 2                      \n");
                                        break;
    
    case RESET_COMMAND                        : printf(" reset <reset_option>                                                     \n\n");
                                        printf(" <reset_option>  Type of reset can be one of the following value :          \n");
                                        printf("                    hard    :: hardware reset                               \n");
                                        printf("                    soft    :: software reset                               \n");
                                        printf("                    vector  :: vector reset                                 \n");
                                        break;
                                        
    case EXIT                         : printf(" exit <empty>                                                             \n\n");
                                        break;
  }
}

/** InterpreteCommand is a function to interpreter user input to 
  * a meaningful information to machine
  *
  * Input   : ccode is a Command code can be one of the following value
  *
  * return  : userSession contain all the information from the user input
  */
User_Session *InterpreteCommand(String *userInput) {
  Identifier *command; Command_Code ccode;
  
  command = (Identifier*)getToken(userInput);
  ccode = getCommandCode(command->name);
  
  if(ccode == HELP) {                         helpMenu(userInput); 
                                              return NULL;}
  else if(ccode == LOAD)                      return userLoadProgram(userInput);
  else if(ccode == WRITE)                     return userWriteMemory(userInput);
  else if(ccode == READ_MEMORY)               return userReadMemory(userInput);
  else if(ccode == WRITE_REGISTER)            return userWriteRegister(userInput);
  else if(ccode == READ_REGISTER)             return userReadRegister(userInput);
  else if(ccode == HALT)                      return userHaltTarget();
  else if(ccode == RUN)                       return userRunTarget();
  else if(ccode == STEP)                      return userStepTarget(userInput);
  else if(ccode == BREAKPOINT)                return userSetBreakpoint(userInput);
  else if(ccode == ERASE)                     return userErase(userInput);
  else if(ccode == RESET_COMMAND)                     return userReset(userInput);
  else if(ccode == EXIT)                      return userExit();
  
  else Throw(ERR_INVALID_USER_COMMAND);
}

/** waitUserCommand is a function to aquire whatever information
  * enter by user
  */
User_Session *waitUserCommand(void) {
  Number *num; String *str; 

  while(1) {
    printf("> ");
    fgets(InputBuffer, BUFFER_SIZE, stdin);
    if(InputBuffer[strlen(InputBuffer) - 1] == '\n') break;
  }
  
  str = stringNew(InputBuffer);
  
  return InterpreteCommand(str);
}
