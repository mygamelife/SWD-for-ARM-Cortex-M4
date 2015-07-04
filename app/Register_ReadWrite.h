#ifndef Register_ReadWrite_H
#define Register_ReadWrite_H

#include <stdint.h>
#include "Clock.h"
#include "Bit_Operations.h"
#include "configurePort.h"
#include "swd_Utilities.h"
#include "IO_Operations.h"

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
#define IDCODE_REG        0x00
#define ABORT_REG         0x00
#define CTRLSTAT_REG      0x04
#define RESEND_REG        0x08
#define SELECT_REG        0x08
#define RDBUFF_REG        0x0C

//CONTROL_STATUS Register
#define SWD_WDATAERR_MASK     (1 << 7)
#define SWD_STICKYERR_MASK    (1 << 5)
#define SWD_STICKYCMP_MASK    (1 << 4)
#define SWD_STICKYORUN_MASK   (1 << 1)
#define POWERUP_SYSTEM        ((uint32_t)0x50000000)

#define swdReadCtrlStatus(ack, parity, readData)    SWDRegister_Read(CTRLSTAT_REG, DP, ack, parity, readData);
#define swdWriteCtrlStatus(ack, writeData)          SWDRegister_Write(CTRLSTAT_REG, DP, ack, writeData);
void powerUpSystemAndDebug();

//ABORT Register
#define WDERRCLR      (1 << 3)
#define STKERRCLR     (1 << 2)
#define STKCMPCLR     (1 << 1)
#define ORUNERRCLR    (1 << 4)
#define DAPABOT       (1 << 0)

#define swdWriteAbort(ack, writeData)   SWDRegister_Write(ABORT_REG, DP, ack, writeData);

//SELECT Register
#define BANK_0  0x00
#define BANK_1  0x10
#define BANK_F  0xF0

#define swdWriteSelect(ack, writeData)   SWDRegister_Write(SELECT_REG, DP, ack, writeData);

//-------------------------------- SWD-DP Register --------------------------------------------
#define swdReadDP(address, ack, parity, data)     SWDRegister_Read(address, DP, ack, parity, data);
#define swdWriteDP(address, ack, data)            SWDRegister_Write(address, DP, ack, data);
#define swdWriteAP(address, ack, data)            SWDRegister_Write(address, AP, ack, data);
void SWDRegister_Read(int Address,int APnDP,int *ACK,int *Parity, uint32_t *data);
void SWDRegister_Write(int Address,int APnDP,int *ACK, uint32_t data);

//-------------------------------- AHB-AP Register --------------------------------------------
void readAhbIDR(uint32_t *data_IDR);

int memoryAccessRead(uint32_t Address,uint32_t *dataRead);
int memoryAccessWrite(uint32_t Address,uint32_t WriteData);

int swdReadAP(int Address,int *ACK,int *Parity, uint32_t *data);
#endif // Register_ReadWrite_H
