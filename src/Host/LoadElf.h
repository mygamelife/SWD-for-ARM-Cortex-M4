#ifndef LoadElf_H
#define LoadElf_H

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "GetHeaders.h"

typedef struct {
  char *name;
  int size;
  uint8_t *data;
  uint32_t address;
} Program_Section;

typedef struct
{
  int size;     /* total malloced size */
  int ssize;    /* section size */
  int tpsize;   /* total program size */
  Program_Section *ps;
} Program;

#define getNumOfSectionHeader(__ELF__)      ((__ELF__)->eh->e_shnum)
#define getNumOfProgramSection(__ELF__)     ((__ELF__)->eh->e_phnum)
#define getSectionsAddress(__ELF__, i)      ((__ELF__)->sh[i].sh_addr)
#define getSectionSize(__ELF__, i)          ((__ELF__)->sh[i].sh_size)
#define getPhysAddress(__ELF__, i)          ((__ELF__)->ph[i].p_paddr)
#define getVirtAddress(__ELF__, i)          ((__ELF__)->ph[i].p_vaddr)
#define getFileSize(__ELF__, i)             ((__ELF__)->ph[i].p_filesz)
#define getMemorySize(__ELF__, i)           ((__ELF__)->ph[i].p_memsz)
#define getDataAddress(__ELF__, i)          getSectionAddress(__ELF__, i)

#define getProgramSectionSize(__PROGRAM__)  ((__PROGRAM__)->ssize)
#define getTotalProgramSize(__PROGRAM__)    ((__PROGRAM__)->tpsize)
#define getProgramData(__PROGRAM__, i)      ((__PROGRAM__)->ps[i].data)
#define getProgramAddress(__PROGRAM__, i)   ((__PROGRAM__)->ps[i].address)
#define getProgramSize(__PROGRAM__, i)      ((__PROGRAM__)->ps[i].size)
#define getWord(x)                          (*(uint32_t *)(x))                         

Program *createProgram(int size);
Program *getLoadableSection(char *file);
uint32_t getEntryAddress(Program *p);

uint32_t getLoadMemoryAddress(ElfData *elf, int i);
uint32_t getCorrectPhysAddress(ElfData *elf, uint32_t address);

int isWithinPhysRange(ElfData *elf, uint32_t address);
int isWithinVirtRange(ElfData *elf, uint32_t address);

void delProgram(Program *p);
#endif // LoadElf_H
