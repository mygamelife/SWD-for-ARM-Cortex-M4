#ifndef ProgramVerifier_H
#define ProgramVerifier_H

#include "ProgramLoader.h"
#include "Yield.h"

typedef enum {
  VERIFY_PASSED = 1,
  VERIFY_FAILED = -1
} Verify_Status;

int isProgramExist(Tlv_Session *session, Program *p);
int verifyLoadedProgram(uint8_t *loadedProgram, uint8_t *actualProgram, int size);

#endif // ProgramVerifier_H
