#include "ProgramVerifier.h"

int verifyLoadedProgram(uint8_t *loadedProgram, uint8_t *actualProgram, int size) {
  int i;

  for(i = 0; i < size; i++) {
    if(loadedProgram[i] != actualProgram[i]) {
      // printf("lp %x, ap %x\n", get4Byte(&loadedProgram[i]), get4Byte(&actualProgram[i]));
      return 0;
    }
  }
  return 1;
}

int isProgramExist(Tlv_Session *session, Program *p) {
  static TaskBlock taskBlock = {.state = 0};
  static uint8_t *lProgram;
  static int i, verifyStatus;
  TaskBlock *tb = &taskBlock;

  /* Start task */
  startTask(tb);
  /* Initialize static variable */
  i = 0;
  verifyStatus = VERIFY_PASSED;
  /* Read and verify */
  printf("Verifying........\n");
  for(; i < getProgramSectionSize(p); i++) {
    await((lProgram = readMemory(session, getProgramAddress(p, i), getProgramSize(p, i))), tb);
    if(!verifyLoadedProgram(lProgram, getProgramData(p, i), getProgramSize(p, i))) {
      delDataBlock(lProgram); /* free the data block used to store program read from memory */
      verifyStatus = VERIFY_FAILED; break;
    }
    delDataBlock(lProgram); /* free previous data block before re-use */
  }
  endTask(tb);
  returnThis(verifyStatus);
}