#ifndef Swd_H
#define Swd_H

typedef enum
{
  SWD_NO_ERROR                        = 0,
  ERR_ACK_WAIT_RESPONSE               = -1,
  ERR_ACK_FAULT_RESPONSE              = -2,
	ERR_INVALID_PARITY_RECEIVED         = -3,
} SwdError;

#include "configurePort.h"
#include "IoOperations.h"
#include "MemoryReadWrite.h"

/* Swd Ack Response */
#define SWD_OK_RESPONSE               0x01
#define SWD_WAIT_RESPONSE             0x02	
#define	SWD_FAULT_RESPONSE            0x04

/* Swd Protocol packet bit sequence */
#define SWD_DP                        0
#define SWD_AP                        1
#define SWD_WRITE                     0
#define SWD_READ                      1
#define SWD_STARTBIT                  1
#define SWD_STOPBIT                   0
#define SWD_PARKBIT                   1

/** AHB-AP register
  */
#define CSW_REG                       0x00
#define TAR_REG                       0x04
#define IDR_REG                       0x0C
#define DRW_REG                       0x0C
#define SIZE_BYTE                     0x01
#define SIZE_HALFWORD                 0x02
#define SIZE_WORD                     0x03
#define ROM_TABLE                     0xE00FF000

/** Debug Port bit address Register
  */
#define IDCODE_REG                    0x00
#define ABORT_REG                     0x00
#define CTRLSTAT_REG                  0x04
#define RESEND_REG                    0x08
#define SELECT_REG                    0x08
#define RDBUFF_REG                    0x0C

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

/**--------------------------- Swd Low Level Request and Parity checking Functions ---------------------------
  */
int calculateParitySwdRequest(int addressBit3,int addressBit2, int pointType, int rw);
int calculateParity32bitData(uint32_t data);
int getSwdRequest(int address, int pointType, int rw);
int compareParityWithData(uint32_t data,int Parity);
void getSwdAddressBit(int *Address_bit3,int *Address_bit2,int Address);

/**-------------------------------------- Swd Error Handling Functions ----------------------------------------
  */
SwdError swdGetAckResponse(int ack);
uint32_t swdCheckErrorFlag(void);
void swdClearErrorFlag(uint32_t errorFlag);
void swdErrorHandler(int error, int readOrWrite, int pointType, int address, uint32_t *data);

/**-------------------------------------- Swd Ap/Dp Retries Functions -----------------------------------------
  */
int swdReadDpWithRetries(int address, uint32_t *data, int counter);
int swdWriteDpWithRetries(int address, uint32_t data, int counter);
int swdReadApWithRetries(int address, uint32_t *data, int counter);
int swdWriteApWithRetries(int address, uint32_t data, int counter);
int swdRetriesOperation(int readOrWrite, int pointType, int address, uint32_t *data, int numOfRetires);
void swdResendOperation(int readOrWrite, int pointType, int address, uint32_t *data);

/**-------------------------------- SWD-DP Low-Level R/W Register Functions -----------------------------------
  */
#define swdWriteDP(address, data)               swdRegisterWrite(address, SWD_DP, data);
#define swdReadDP(address, data)                swdRegisterRead(address, SWD_DP, data);
#define swdWriteAP(address, data)               swdRegisterWrite(address, SWD_AP, data);

void powerUpSystemAndDebug(void);
SwdError swdRegisterRead(int address, int pointType, uint32_t *data);
SwdError swdRegisterWrite(int address, int pointType, uint32_t data);
SwdError swdReadAP(int address, uint32_t *data);

/**------------------------------------- AHB-AP Register Functions --------------------------------------------
  */
SwdError readAhbIDR(uint32_t *Idr);
SwdError swdSelectMemorySize(uint32_t memorySize);

void switchJTAGtoSWD(void);
SwdError swdInit(void);

#endif // Swd_H
