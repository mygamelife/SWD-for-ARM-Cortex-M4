#ifndef Register_ReadWrite_H
#define Register_ReadWrite_H

#include <stdint.h>
#include "Clock.h"
#include "Bit_Operations.h"
#include "configurePort.h"
#include "swd_Utilities.h"
#include "IO_Operations.h"

/** Debug Core Register
  */
#define DHCSR             ((uint32_t)0xE000EDF0)    //Debug Halting Control Status Register
#define DCRSR             ((uint32_t)0xE000EDF4)    //Debug Core Register Selector Register
#define DCRDR             ((uint32_t)0xE000EDF8)    //Debug Core Register Data Register
#define DEMCR             ((uint32_t)0xE000EDFC)    //Debug Exception and Monitor Control Register

/** AHB-AP register
  */
#define CSW_REG           0x0
#define TAR_REG           0x4
#define IDR_REG           0xC
#define DRW_REG           0xC
#define SIZE_BYTE         0x1
#define SIZE_HALFWORD     0x2
#define SIZE_WORD         0x3
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
#define BANK_0                        ((uint32_t)0x00000000)
#define BANK_1                        ((uint32_t)0x00000010)
#define BANK_F                        ((uint32_t)0x000000F0)

/** SELECT Register bit set
  */
#define CSW_DEFAULT_MASK              ((uint32_t)0x23000040)
#define CSW_BYTE_SIZE                 ((uint32_t)0x00000000)
#define CSW_HALFWORD_SIZE             ((uint32_t)0x00000001)
#define CSW_WORD_SIZE                 ((uint32_t)0x00000002)
#define CSW_DISABLE_ADDR_INC          ((uint32_t)0x00000000)
#define CSW_ENABLE_ADDR_INC_SINGLE    ((uint32_t)0x00000010)
#define CSW_ENABLE_ADDR_INC_PACKED    ((uint32_t)0x00000020)

/**-------------------------------- SWD-DP Register Function-----------------------------------
  */
#define swdWriteDP(address, ack, data)              SWDRegister_Write(address, DP, ack, data);
#define swdWriteSelect(ack, writeData)              SWDRegister_Write(SELECT_REG, DP, ack, writeData);
#define swdWriteAbort(ack, writeData)               SWDRegister_Write(ABORT_REG, DP, ack, writeData);
#define swdWriteCtrlStatus(ack, writeData)          SWDRegister_Write(CTRLSTAT_REG, DP, ack, writeData);
#define swdReadDP(address, ack, parity, data)       SWDRegister_Read(address, DP, ack, parity, data);
#define swdReadCtrlStatus(ack, parity, readData)    SWDRegister_Read(CTRLSTAT_REG, DP, ack, parity, readData);

void powerUpSystemAndDebug();
void SWDRegister_Read(int Address,int APnDP,int *ACK,int *Parity, uint32_t *data);
void SWDRegister_Write(int Address,int APnDP,int *ACK, uint32_t data);

/**-------------------------------- AHB-AP Register Function-----------------------------------
  */
#define swdWriteAP(address, ack, data)              SWDRegister_Write(address, AP, ack, data);

int memoryAccessRead(uint32_t Address,uint32_t *dataRead);
int memoryAccessWrite(uint32_t Address,uint32_t WriteData);
int swdReadAP(int Address,int *ACK,int *Parity, uint32_t *data);
void readAhbIDR(uint32_t *data_IDR);
void swdWriteCSW(int *ack, uint32_t cswBitSet);
#endif // Register_ReadWrite_H
