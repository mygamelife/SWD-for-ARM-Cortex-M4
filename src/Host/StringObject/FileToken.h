#ifndef FileToken_H
#define FileToken_H

#include <malloc.h>
#include <stdio.h>
#include "Token.h"
#include "StringObject.h"

typedef struct
{
	TokenTypeDef type;
	char *name;
	String *line;
}File;

File *fileNew(char *name);
void fileDel(File *file);
#endif // FileToken_H
