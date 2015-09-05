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
  int i, length = 0;
  
  length = tlv->length;
  
  if(length > 1)  {
    for(i = 0; i < length; i += 4)  {
      if(i % 16 == 0) {
        printf("\n");
      } 
      printf("%x ", get4Byte(&tlv->value[i]));
    }
  }
}

Tlv *waitUserCommand(void) {
  int address = 0;  char InputBuffer[BUFFER_SIZE] = {0}, ch;
  Tlv *tlv;
  Identifier *iden;
  Number *num, *data;
  String *str;

  while(1) {
    fgets(InputBuffer, BUFFER_SIZE, stdin);
    if(InputBuffer[strlen(InputBuffer) - 1] == '\n') break;
  }
  
  str = stringNew(InputBuffer);
  
  num = (Number*)getToken(str);
  
  if(num->value == 1) {
    
  }
  else if(num->value == 2) {
    
  }
  else if(num->value == 3) {
    iden = (Identifier*)getToken(str);
    data = (Number*)getToken(str);
    
    if(iden->name == "R0")  address = 0;
    else if(iden->name == "R1")  address = 1;
    
    uint32_t writeRegInfo[] = {address, data->value};
    tlv = tlvCreatePacket(TLV_WRITE_REGISTER, 8, (uint8_t *)writeRegInfo);
    return tlv;
  }
  else if(num->value == 4) {
    iden = (Identifier*)getToken(str);
    
    if(iden->name == "R0")  address = 0;
    else if(iden->name == "R1")  address = 1;
    
    uint32_t readRegInfo[] = {address};
    tlv = tlvCreatePacket(TLV_READ_REGISTER, 4, (uint8_t *)readRegInfo);
    return tlv;
  }
  else if(num->value == 5) {
    tlv = tlvCreatePacket(TLV_EXIT, 0, 0);
    return tlv;
  }
}
