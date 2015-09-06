#include "Interface.h"

#define BUFFER_SIZE 100

void displayOptionMenu(void)  {
  printf("1. Write RAM\n");
  printf("2. Read RAM\n");
  printf("3. Write Register\n");
  printf("4. Read Register\n");
  printf("5. Exit\n");
}

void displayTlvData(Tlv *tlv)  {
  int i, length = 0, counter = 0;
  length = tlv->length;
  
  printf("length %d\n", length);
  if(length > 1)  {
    printf("%x\n", get4Byte(&tlv->value[0]));
    for(i = 4; i < length - 1; i += 4)  {

    if(counter == 4) {
        counter = 0;
        printf("\n");
      }
      printf("%x ", get4Byte(&tlv->value[i]));
      counter++;
    }
    printf("\n");
  }
  else printf("OK\n");
}

int getRegisterAddress(char *name)  {
  if(strcmp(name, "R0") == 0)         return 0;
  else if(strcmp(name, "R1") == 0)    return 1;
  else if(strcmp(name, "R2") == 0)    return 2;
  else if(strcmp(name, "R3") == 0)    return 3;
  else if(strcmp(name, "PC") == 0)    return 15;
}

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
      printf("%x\n", data->value);
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
    userSession.data = (uint32_t *)&data->value;
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
