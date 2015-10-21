#ifndef IdentifierToken_H
#define IdentifierToken_H

#include <malloc.h>
#include <stdio.h>
#include "Token.h"
#include "StringObject.h"

typedef struct
{
	TokenTypeDef type;
	char *name;
	String *line;
}Identifier;

Identifier *identifierNew(char *name);
void identifierDel(Identifier *iden);
#endif // IdentifierToken_H
