/*  
    Serial Wire Debug for ARM Cortex M4
    Copyright (C) 2015 Chiu Vui Te, <mygamelife1@gmail.com>,
    Cheah Sheng Li, <csl9408@gmail.com>
    This file is part of GDB RSP and ARM Simulator.
    This program is free software, you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY, without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with This program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ProgramVerifier.h"

int verifyLoadedProgram(uint8_t *loadedProgram, uint8_t *actualProgram, int size) {
  int i;

  for(i = 0; i < size; i++) {
    if(loadedProgram[i] != actualProgram[i]) {
      printf("i %d, lp %x, ap %x\n", i, get4Byte(&loadedProgram[i]), get4Byte(&actualProgram[i]));
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
  lProgram = NULL;
  /* Read and verify */
  printf("Verifying........\n");
  for(; i < getProgramSectionSize(p); i++) {
    if(strcmp(getProgramName(p, i), ".data") != 0) {
      await((lProgram = readMemory(session, getProgramAddress(p, i), getProgramSize(p, i))), tb);
      if(!verifyLoadedProgram(lProgram, getProgramData(p, i), getProgramSize(p, i))) {
        printf("i %d\n", i);
        delDataBlock(lProgram); /* free the data block used to store program read from memory */
        verifyStatus = VERIFY_FAILED; break;
      }
      delDataBlock(lProgram); /* free previous data block before re-use */
    }
  }
  endTask(tb);
  returnThis(verifyStatus);
}