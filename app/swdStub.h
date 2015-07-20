#ifndef swdStub_H
#define swdStub_H

#include <stdint.h>
#include "Flash.h"

/** SWD Instruction address 
  */
#define TWO_KBYTES                        2000 //2k byte
#define FOUR_KBYTES                       40002 //4k byte
#define SWD_INSTRUCTION                   ((uint32_t)0x20000000)
#define SWD_BANK_SELECT                   ((uint32_t)0x20000004)
#define SWD_DATA_LENGTH                   ((uint32_t)0x20000008)
#define SWD_FLASH_START_ADDRESS           ((uint32_t)0x2000000C)
#define SWD_FLASH_END_ADDRESS             ((uint32_t)0x20000010)
#define SWD_SRAM_START_ADDRESS            ((uint32_t)0x20000014)
#define SWD_TARGET_STATUS                 ((uint32_t)0x20000018)
#define SWD_SRAM_DATA32_ADDRESS           ((uint32_t)0x20000800)

/** SWD Instruction 
  */
#define INSTRUCTION_CLEAR                 ((uint32_t)0xFFFFFFFF)
#define MASS_ERASE_BANK_1                 ((uint32_t)0x00000001)
#define MASS_ERASE_BANK_2                 ((uint32_t)0x00000002)
#define MASS_ERASE_BOTH_BANK              ((uint32_t)0x00000003)
#define INSTRUCTION_COPY                  ((uint32_t)0x00000010)
#define INSTRUCTION_SECTOR_ERASE          ((uint32_t)0x00000011)
#define INSTRUCTION_MASS_ERASE            ((uint32_t)0x00000012)

/** SWD target response
  */
#define TARGET_OK                         ((uint32_t)0x00000000)
#define TARGET_BUSY                       ((uint32_t)0x00000001)

void swdStub(uint32_t swdInstruction);
void stubCopy(void);
void stubSectorErase(void);
void stubMassErase(void);
#endif // swdStub_H
