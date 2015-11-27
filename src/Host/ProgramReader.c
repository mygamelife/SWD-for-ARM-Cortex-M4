#include "ProgramReader.h"

#define CURRENT_SECTION_SIZE  6

/* Selective Section Name */
static const char *ssname[] = { ".isr_vector"   ,
                                ".text"         ,
                                ".data"         ,
                                ".rodata"       ,
                                ".init_array"   ,
                                ".fini_array"   
                              };
                          
Program *getSelectiveSection(char *file) {
  int index = 0, i = 0;
  ElfData *elfData = openElfFile(file);
  Program *prog = malloc(sizeof(Program));
  
  prog->ssize = CURRENT_SECTION_SIZE;
  
  for(; i < CURRENT_SECTION_SIZE; i++) {
    index = getIndexOfSectionByName(elfData, ssname[i]);
    prog->esection[i].dataAddress = (uint8_t *)getSectionAddress(elfData, index);
    prog->esection[i].destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
    prog->tsize += (int)getSectionSize(elfData, index);
  }
  
  closeElfData(elfData);
  
  return prog;
}