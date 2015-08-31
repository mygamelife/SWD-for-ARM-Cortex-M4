#ifndef swdStub_H
#define swdStub_H

#include <stdint.h>
#include "Flash.h"
#include "SRAM.h"

#if !defined(TEST)
#include "SystemConfigure.h"
#endif

/** SWD Instruction address 
  */
#define TWO_KBYTES                        2048 //2k byte
#define FOUR_KBYTES                       4096 //4k byte
#define SWD_INSTRUCTION                   ((uint32_t)0x20012700)
#define SWD_BANK_SELECT                   ((uint32_t)0x20012704)
#define SWD_DATA_SIZE                     ((uint32_t)0x20012708)
#define SWD_FLASH_START_ADDRESS           ((uint32_t)0x2001270C)
#define SWD_FLASH_END_ADDRESS             ((uint32_t)0x20012710)
#define SWD_SRAM_START_ADDRESS            ((uint32_t)0x20012714)
#define SWD_TARGET_STATUS                 ((uint32_t)0x20012718)
#define SWD_SRAM_DATA32_ADDRESS           ((uint32_t)0x20000800)

/** SWD Instruction 
  */
#define INSTRUCTION_CLEAR                 ((uint32_t)0xFFFFFFFF)
#define MASS_ERASE_BANK_1                 ((uint32_t)0x00000001)
#define MASS_ERASE_BANK_2                 ((uint32_t)0x00000002)
#define MASS_ERASE_BOTH_BANK              ((uint32_t)0x00000003)
#define INSTRUCTION_COPY                  ((uint32_t)0x00000010)
#define INSTRUCTION_ERASE_SECTOR          ((uint32_t)0x00000011)
#define INSTRUCTION_MASS_ERASE            ((uint32_t)0x00000012)

/** SWD target response
  */
#define TARGET_OK                         ((uint32_t)0x00000000)
#define TARGET_BUSY                       ((uint32_t)0x00000001)

void swdStub(uint32_t swdInstruction);
void stubCopyFromSRAMToFlash(void);
void stubEraseSector(void);
void stubMassErase(void);
void stubVerify(void);
void flashProgrammer(void);
#endif // swdStub_H
