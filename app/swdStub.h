#ifndef swdStub_H
#define swdStub_H

#include <stdint.h>
#include "Flash.h"

/** SWD Instruction address 
  */
#define TWO_KBYTES                        2000 //2k byte
#define FOUR_KBYTES                       40002 //4k byte
#define SWD_INSTRUCTION                   ((uint32_t)0x20000000)
#define SWD_DATA_LENGTH                   ((uint32_t)0x20000004)
#define SWD_START_ADDRESS                 ((uint32_t)0x20000008)
#define SWD_END_ADDRESS                   ((uint32_t)0x2000000C)
#define SWD_TARGET_STATUS                 ((uint32_t)0x20000010)
#define SWD_DATA32                        ((uint32_t)0x200004CC)

/** SWD Instruction 
  */
#define INSTRUCTION_CLEAR                 ((uint32_t)0xFFFFFFFF)
#define INSTRUCTION_COPY                  ((uint32_t)0x00000010)
#define INSTRUCTION_SECTOR_ERASE          ((uint32_t)0x00000011)
#define INSTRUCTION_MASS_ERASE_BANK_1     ((uint32_t)0x00000012)
#define INSTRUCTION_MASS_ERASE_BANK_2     ((uint32_t)0x00000022)
#define INSTRUCTION_MASS_ERASE_BOTH_BANK  ((uint32_t)0x00000032)

/** SWD target response
  */
#define TARGET_OK                         ((uint32_t)0x00000000)
#define TARGET_BUSY                       ((uint32_t)0x00000001)

void swdStub(uint32_t swdInstruction);
#endif // swdStub_H
