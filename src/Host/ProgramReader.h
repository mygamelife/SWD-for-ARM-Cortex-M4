#ifndef ProgramReader_H
#define ProgramReader_H

#include "GetHeaders.h"
#define SECTION_NUMBER      30

typedef struct
{
  int ssize; /* section size */
  int tsize; /* total program size */
  ElfSection esection[SECTION_NUMBER];
} Program;

Program *getSelectiveSection(char *file);
#endif // ProgramReader_H
