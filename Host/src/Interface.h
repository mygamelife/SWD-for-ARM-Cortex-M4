#ifndef Interface_H
#define Interface_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "Token.h"
#include "Tlv.h"

void displayOptionMenu(void);
Tlv *waitUserCommand(void);
void displayTlvData(Tlv *tlv);

#endif // Interface_H