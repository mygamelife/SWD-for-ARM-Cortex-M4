#include "Interface.h"

#define BUFFER_SIZE 1024

void displayOptionMenu(void)  {
  printf("1. Write RAM\n");
  printf("2. Read RAM\n");
  printf("3. Write Register\n");
  printf("4. Read Register\n");
  printf("5. Load Program To RAM\n");
  printf("6. Load Program To Flash\n");
  printf("7. Halt target\n");
  printf("8. run target\n");
  printf("9. Step target\n");
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

int getRegisterAddress(char *name)  {
  if(strcmp(name, "R0") == 0)         return 0;
  else if(strcmp(name, "R1") == 0)    return 1;
  else if(strcmp(name, "R2") == 0)    return 2;
  else if(strcmp(name, "R3") == 0)    return 3;
  else if(strcmp(name, "SP") == 0)    return 13;
  else if(strcmp(name, "LR") == 0)    return 14;
  else if(strcmp(name, "PC") == 0)    return 15;
  else if(strcmp(name, "xPSR") == 0)  return 16;
  else if(strcmp(name, "MSP") == 0)   return 17;
  else if(strcmp(name, "PSP") == 0)   return 18;
  else if(strcmp(name, "SR") == 0)    return 19;
}

/** userInputInterpreter
  *
  * Input   : NONE
  *
  * Return  : NONE
  */
User_Session *userInputInterpreter(int option, String *str)  {
  static User_Session userSession;
  int regAddress = 0, i = 0;
  Number *data, *size, *address;
  Identifier *iden;
  uint32_t buffer[255] = {0};
  
  if(option == 1) {
    address = (Number*)getToken(str);
    size = (Number*)getToken(str);
    
    while(str->length != 1) {
      data = (Number*)getToken(str);
      buffer[i] = data->value;
      i++;
    }
    
    userSession.tlvCommand = TLV_WRITE_RAM;
    userSession.data = buffer;
    userSession.address = address->value;
    userSession.size = size->value * 4;
    return &userSession;
  }
  else if(option == 2) {
    address = (Number*)getToken(str);
    size = (Number*)getToken(str);
    
    userSession.tlvCommand = TLV_READ_RAM;
    userSession.address = address->value;
    userSession.size = size->value * 4;
    return &userSession;
  }
  else if(option == 3) {
    iden = (Identifier*)getToken(str);
    data = (Number*)getToken(str);
    regAddress = getRegisterAddress(iden->name);
    
    userSession.tlvCommand = TLV_WRITE_REGISTER;
    userSession.data = &data->value;
    userSession.address = regAddress;
    return &userSession;
  }
  else if(option == 4) {
    iden = (Identifier*)getToken(str);
    regAddress = getRegisterAddress(iden->name);
    
    userSession.tlvCommand = TLV_READ_REGISTER;
    userSession.address = regAddress;
    return &userSession;
  }
  else if(option == 5) {
    iden = (Identifier*)getToken(str);
    userSession.tlvCommand = TLV_LOAD_PROGRAM_RAM;
    userSession.fileName = iden->name;
    return &userSession;
  }
  else if(option == 6) {
    //do something
  }
  else if(option == 7) {
    userSession.tlvCommand = TLV_HALT_TARGET;
    return &userSession;
  }
  else if(option == 8) {
    userSession.tlvCommand = TLV_RUN_TARGET;
    return &userSession;
  }
  else if(option == 9) {
    data = (Number*)getToken(str);
    
    userSession.tlvCommand = TLV_STEP;
    userSession.data = &data->value;
    return &userSession;
  }
  else if(option == 10) {
    userSession.tlvCommand = TLV_EXIT;
    return &userSession;
  }
}

User_Session *waitUserCommand(void) {
  Number *num;  String *str;
  
  char InputBuffer[BUFFER_SIZE] = {0};

  while(1) {
    fgets(InputBuffer, BUFFER_SIZE, stdin);
    if(InputBuffer[strlen(InputBuffer) - 1] == '\n') break;
  }
  
  str = stringNew(InputBuffer);
  num = (Number*)getToken(str);
  
  return userInputInterpreter(num->value, str);
}
