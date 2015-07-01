#ifndef Register_ReadWrite_H
#define Register_ReadWrite_H

#include <stdint.h>
#include "Clock.h"
#include "Bit_Operations.h"
#include "configurePort.h"
#include "swd_Utilities.h"
#include "IO_Operations.h"

#define IDCODE_REG 0x0
#define ABORT_REG 0x0
#define RDBUFF_REG 0xC
#define CTRLSTAT_REG 0x4

//Debug Core Register
#define DHCSR     ((uint32_t)0xE000EDF0)    //Debug Halting Control Status Register
#define DCRSR     ((uint32_t)0xE000EDF4)    //Debug Core Register Selector Register
#define DCRDR     ((uint32_t)0xE000EDF8)    //Debug Core Register Data Register
#define DEMCR     ((uint32_t)0xE000EDFC)    //Debug Exception and Monitor Control Register

//MEM-AP register
#define CSW_REG           0x0
#define TAR_REG           0x4
#define IDR_REG           0xC
#define DRW_REG           0xC
#define SIZE_BYTE         0x1
#define SIZE_HALFWORD     0x2
#define SIZE_WORD         0x3
#define ROM_TABLE         0xE00FF000

//DP Register
#define DP_IDCODE     0x00
#define DP_ABORT      0x00
#define DP_CTRL_STAT  0x04
#define DP_RESEND     0x08
#define DP_SELECT     0x08
#define RDBUFF        0x0C

//CONTROL_STATUS Register
#define SWD_WDATAERR_MASK     (1 << 7)
#define SWD_STICKYERR_MASK    (1 << 5)
#define SWD_STICKYCMP_MASK    (1 << 4)
#define SWD_STICKYORUN_MASK   (1 << 1)
#define POWERUP_SYSTEM        ((uint32_t)0x50000000)

#define swdReadCtrlStatus(ack, parity, readData)    SWDRegister_Read(DP_CTRL_STAT, DP, ack, parity, readData);
#define swdWriteCtrlStatus(ack, writeData)          SWDRegister_Write(DP_CTRL_STAT, DP, ack, writeData);
void powerUpSystemAndDebug();

//ABORT Register
#define WDERRCLR      (1 << 3)
#define STKERRCLR     (1 << 2)
#define STKCMPCLR     (1 << 1)
#define ORUNERRCLR    (1 << 4)
#define DAPABOT       (1 << 0)

#define swdWriteAbort(ack, writeData)   SWDRegister_Write(DP_ABORT, DP, ack, writeData);

//SELECT Register
#define BANK_0  0x00
#define BANK_1  0x10
#define BANK_F  0xF0

#define swdWriteSelect(ack, writeData)   SWDRegister_Write(DP_SELECT, DP, ack, writeData);

//-------------------------------- SWD-DP Register --------------------------------------------
void SWDRegister_Read(int Address,int APnDP,int *ACK,int *Parity, uint32_t *data);
void SWDRegister_Write(int Address,int APnDP,int *ACK, uint32_t data);

//-------------------------------- AHB-AP Register --------------------------------------------
void readAHB_IDR(uint32_t *data_IDR);

int MemoryAccess_Read(uint32_t Address,uint32_t *dataRead);
int MemoryAccess_Write(uint32_t Address,uint32_t WriteData);

int SWD_ReadAP(int Address,int *ACK,int *Parity, uint32_t *data);

#endif // Register_ReadWrite_H
