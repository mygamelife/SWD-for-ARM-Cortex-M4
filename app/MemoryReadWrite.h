#ifndef MemoryReadWrite_H
#define MemoryReadWrite_H

#include <stdint.h>
#include "IoOperations.h"
#include "configurePort.h"
#include "swd_Utilities.h"

/** AHB-AP register
  */
#define CSW_REG           0x00
#define TAR_REG           0x04
#define IDR_REG           0x0C
#define DRW_REG           0x0C
#define SIZE_BYTE         0x01
#define SIZE_HALFWORD     0x02
#define SIZE_WORD         0x03
#define ROM_TABLE         0xE00FF000

/** DP Register
  */
#define IDCODE_REG        0x00
#define ABORT_REG         0x00
#define CTRLSTAT_REG      0x04
#define RESEND_REG        0x08
#define SELECT_REG        0x08
#define RDBUFF_REG        0x0C

/** CONTROL_STATUS Register error mask
  */
#define SWD_STICKYORUN_MASK           ((uint32_t)0x00000002)
#define SWD_STICKYCMP_MASK            ((uint32_t)0x00000010)
#define SWD_STICKYERR_MASK            ((uint32_t)0x00000020)
#define SWD_WDATAERR_MASK             ((uint32_t)0x00000080)

/** CONTROL_STATUS Register error flag
  */
#define SWD_STICKYORUN_ERROR_FLAG     0x00
#define SWD_STICKYCMP_ERROR_FLAG      0x01
#define SWD_STICKY_ERROR_FLAG         0x02
#define SWD_WDATA_ERROR_FLAG          0x03

/** CONTROL_STATUS Register bit set
  */
#define POWERUP_SYSTEM                ((uint32_t)0x50000000)

/** ABORT Register clear error flag
  */
#define SWD_DAPABORT_CLEAR_FLAG       ((uint32_t)0x00000001)
#define SWD_STKCMP_CLEAR_FLAG         ((uint32_t)0x00000002)
#define SWD_STKERR_CLEAR_FLAG         ((uint32_t)0x00000004)
#define SWD_WDERR_CLEAR_FLAG          ((uint32_t)0x00000008)
#define SWD_ORUNERR_CLEAR_FLAG        ((uint32_t)0x00000010)

/** SELECT Register bank select bit
  */
#define SELECT_BANK0                  ((uint32_t)0x00000000)
#define SELECT_BANK1                  ((uint32_t)0x00000010)
#define SELECT_BANKF                  ((uint32_t)0x000000F0)

/** SELECT Register bit set
  */
#define CSW_DEFAULT_MASK              ((uint32_t)0x23000040)
#define CSW_BYTE_SIZE                 ((uint32_t)0x00000000)
#define CSW_HALFWORD_SIZE             ((uint32_t)0x00000001)
#define CSW_WORD_SIZE                 ((uint32_t)0x00000002)
#define CSW_DISABLE_ADDR_INC          ((uint32_t)0x00000000)
#define CSW_ENABLE_ADDR_INC_SINGLE    ((uint32_t)0x00000010)
#define CSW_ENABLE_ADDR_INC_PACKED    ((uint32_t)0x00000020)

extern int cswDataSize ;

/**-------------------------------- SWD-DP Register Function-----------------------------------
  */
#define swdWriteDP(address, data)               swdRegisterWrite(address, DP, data);
#define swdReadDP(address, data)                swdRegisterRead(address, DP, data);
#define swdWriteAP(address, data)               swdRegisterWrite(address, AP, data);

void powerUpSystemAndDebug(void);
SwdError swdRegisterRead(int address, int pointType, uint32_t *data);
SwdError swdRegisterWrite(int address, int pointType, uint32_t data);
SwdError swdReadAP(int address, uint32_t *data);

/**-------------------------------- AHB-AP Register Function-----------------------------------
  */
SwdError readAhbIDR(uint32_t *data_IDR);
SwdError swdSelectMemorySize(uint32_t memorySize);

/* Used for mocking */
uint32_t memoryReadAndReturnWord(uint32_t address);

int memoryReadWord(uint32_t address,uint32_t *dataRead);
int memoryReadHalfword(uint32_t address,uint32_t *dataRead);


SwdError memoryWriteByte(uint32_t address, uint8_t writeData);
SwdError memoryWriteHalfword(uint32_t address, uint16_t writeData);
SwdError memoryWriteWord(uint32_t address, uint32_t writeData);

uint32_t memoryWriteDataAlignment(uint32_t address, uint16_t writeData);

#endif // MemoryReadWrite_H
