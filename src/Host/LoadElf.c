#include "LoadElf.h"

Program *createProgram(int size) {
  Program *p = malloc(sizeof(Program) + (size * sizeof(Program_Section)));
  assert(p != NULL);
  p->ps = malloc(size * sizeof(Program_Section));
  p->ssize = 0;
  p->tpsize = 0;
  
  return p;
}

void delProgram(Program *p) {
  int i = 0;
  
  if(p != NULL) {
    if(p->ps != NULL) {
      free(p->ps);
      p->ps = NULL;
    }
    free(p);
    p = NULL;
  }
}

Program *getLoadableSection(char *file) {
  int i = 0, tssize = 0; uint32_t lma = 0;
  assert(file != NULL);
  ElfData *elf = openElfFile(file);
  
  /* Total section size */
  tssize = getNumOfSectionHeader(elf);
  Program *p = createProgram(tssize);

  assert(p != NULL);
  
  for(; i < getNumOfSectionHeader(elf); i++) {
    lma = getLoadMemoryAddress(elf, i);
    if(lma != -1) {
      p->ps[p->ssize].name = getSectionInfoNameUsingIndex(elf, i);  /* The actual loadable sections name  */
      p->ps[p->ssize].data = (uint8_t *)getDataAddress(elf, i);     /* The actual data need to load       */
      p->ps[p->ssize].address = lma;                                /* The actual load memory address     */
      p->ps[p->ssize].size = getSectionSize(elf, i);                /* The actual section size            */
      p->tpsize = getSectionSize(elf, i);                           /* Calculate total program size       */
      p->ssize++;                                                   /* Calculate total section            */
    }
  }
  closeElfData(elf);
  return p;
}

uint32_t getEntryAddress(Program *p) {
  int i = 0;
  
  for(; i < getProgramSectionSize(p); i++) {
    if(strcmp(p->ps[i].name, ".isr_vector") == 0) {
      return getWord(&p->ps[i].data[4]);
    }
  }
  return -1;
}

uint32_t getLoadMemoryAddress(ElfData *elf, int i) {
  uint32_t address = getSectionsAddress(elf, i);
  
  if(isWithinVirtRange(elf, address) && isWithinPhysRange(elf, address))
    return address;
  
  else return getCorrectPhysAddress(elf, address);
}

uint32_t getCorrectPhysAddress(ElfData *elf, uint32_t address) {
  int i = 0;
  int numOfProgramSection = getNumOfProgramSection(elf);
  uint32_t startAddress = 0, endAddress = 0;
  
  for(; i < numOfProgramSection; i++) {
    startAddress = getVirtAddress(elf, i);
    endAddress = startAddress + getFileSize(elf, i);

    if(startAddress <= address && address < endAddress)
      return getPhysAddress(elf, i);
  }
  return -1;
}

int isWithinPhysRange(ElfData *elf, uint32_t address) {
  int i = 0;
  int numOfProgramSection = getNumOfProgramSection(elf);
  uint32_t startAddress = 0, endAddress = 0;
  
  for(; i < numOfProgramSection; i++) {
    startAddress = getPhysAddress(elf, i);
    endAddress = startAddress + getFileSize(elf, i);
    
    if(startAddress <= address && address < endAddress)
      return 1;
  }
  
  return 0;
}

int isWithinVirtRange(ElfData *elf, uint32_t address) {
  int i = 0;
  int numOfProgramSection = getNumOfProgramSection(elf);
  uint32_t startAddress = 0, endAddress = 0;
  
  for(; i < numOfProgramSection; i++) {
    startAddress = getVirtAddress(elf, i);
    endAddress = startAddress + getFileSize(elf, i);

    if(startAddress <= address && address < endAddress)
      return 1;
  }
  
  return 0;
}