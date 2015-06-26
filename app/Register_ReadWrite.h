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
#define TAR_REG 0x4
#define CTRLSTAT_REG 0x4
#define DRW_REG 0xC

/**Register Address
 *  Address        READ          Write
 *  --------------------------------------
 *  0x00          IDCODE        ABORT
 *  0x04          CTRL/STAT     CTRL/STAT
 *  0x08          RESEND        SELECT
 *  0x0C          RDBUFF        N/A
 */

//-------------------------- SWD-DP Register --------------------------------
//DP Register
#define DP_IDCODE     0x00
#define DP_ABORT      0x00
#define DP_CTRL_STAT  0x04
#define DP_RESEND     0x08
#define DP_SELECT     0x08
#define RDBUFF        0x0C

//CONTROL/STATUS Register Error Flag Mask
#define SWD_WDATAERR_MASK     (1 << 7)
#define SWD_STICKYERR_MASK    (1 << 5)
#define SWD_STICKYCMP_MASK    (1 << 4)
#define SWD_STICKYORUN_MASK   (1 << 1)

#define swdReadCtrlStatus(ack, parity, readData)    SWDRegister_Read(DP_CTRL_STAT, DP, ack, parity, readData);

//ABORT Register Clear Error Flag
#define WDERRCLR      (1 << 3)
#define STKERRCLR     (1 << 2)
#define STKCMPCLR     (1 << 1)
#define ORUNERRCLR    (1 << 4)
#define DAPABOT       (1 << 0)

#define swdWriteAbort(ack, errorFlag)               SWDRegister_Write(DP_ABORT, DP, ack, errorFlag);

//#define WDERRCLR SWDRegister_Write(Address, APnDP, ACK, data);

//IDCODE Register
void readIDCODEReg();

//CONTROL/STATUS Register
void ctrlStatusReg(int RnW);
void writeDataToSelectReg(uint32_t data);

//ABORT Register
void writeAbortReg();
void writeDataToSelectReg(uint32_t data);

//SELECT Register
void writeSelectReg();
//-------------------------- SWD-DP Register --------------------------------

void SWDRegister_Read(int Address,int APnDP,int *ACK,int *Parity, uint32_t *data);
void SWDRegister_Write(int Address,int APnDP,int *ACK, uint32_t data);

//-------------------------- MEM-AP Register --------------------------------
void selectRegisterBank(uint32_t registerBank);
#endif // Register_ReadWrite_H
