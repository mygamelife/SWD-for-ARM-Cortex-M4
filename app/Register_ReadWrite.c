#include "Register_ReadWrite.h"

/***************************************************************
 **                   SWD-DP Register                         **
 ***************************************************************/

/******************************************************************************************************
 |  Start bit   |  APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    0      |    0     |     0      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************/

/**Register Address
 *  Address        READ          Write
 *  --------------------------------------
 *  0x00          IDCODE        ABORT
 *  0x04          CTRL/STAT     CTRL/STAT
 *  0x08          RESEND        SELECT
 *  0x0C          RDBUFF        N/A
 */

/**
 *	Action          Value
 * -------------------------------------------------------------------
 * START         Always  1
 * AP/DP1        DP  = 0,   AP  = 1
 * RW            W   = 0,   R   = 1
 * Addr[2:3]     Refer Register Address
 * Parity        APnDP + RW + Addr2 + Addr3 Even 1's = 0, Odd 1's = 1
 * STOP          Always  1
 * PARK          Always  1
 */

/**
 * Access CTRL/STATUS register by sending SWD-DP request
 */
void ctrlStatusReg(int RnW) {
  int SWD_Request;

  SWD_Request = getSWD_Request(0x04, DP, RnW);
  send8bit(SWD_Request);
}

/**
 * --------CAUTION-----------
 * It is read-only register
 * --------------------------
 * Read IDCODE register by sending SWD-DP request
 */
void readIDCODEReg() {
  int SWD_Request;

  SWD_Request = getSWD_Request(0x00, DP, READ);
  send8bit(SWD_Request);
}

/**
 * --------CAUTION-----------
 * It is write-only register
 * --------------------------
 * Write IDCODE register by sending SWD-DP request
 */
void writeAbortReg()  {
  int SWD_Request;

  SWD_Request = getSWD_Request(0x00, DP, WRITE);
  send8bit(SWD_Request);
}

/**
 * --------CAUTION-----------
 * It is write-only register
 * --------------------------
 * Write IDCODE register by sending SWD-DP request
 */
void writeSelectReg()  {
  int SWD_Request;

  SWD_Request = getSWD_Request(0x08, DP, WRITE);
  send8bit(SWD_Request);
}

/** writeDataToSelectReg is a function to write data into the SELECT REGISTER
 *
 * input :  data is the 32 bit size data that will be write into SELECT REGISTER
 *          to perform certain task
 *
 * return : NONE
 */
void writeDataToSelectReg(uint32_t data)  {
  int ack = 0;

  //write request
  writeAbortReg();

  turnAround_ToRead();
  SWDIO_InputMode();

  read3bit(&ack);

  turnAround_ToWrite();
  SWDIO_OutputMode();

  send32bit(data);
  sendBit(1); //Parity bit

  extraIdleClock(8);
}

void SWDRegister_Write(int Address,int APnDP,int *ACK, uint32_t data)
{
	int SWD_Request = 0 , parity = 0;

	SWD_Request = getSWD_Request(Address,APnDP,WRITE);
	parity = calculateParity_32bitData(data); //calculate parity before initiating transfer

	send8bit(SWD_Request);

	turnAround_ToRead();
	SWDIO_InputMode();

	read3bit(ACK);

	turnAround_ToWrite();
	SWDIO_OutputMode();

	send32bit(data);
	sendBit(parity);

	extraIdleClock(8);
}

void SWDRegister_Read(int Address,int APnDP,int *ACK,int *Parity, uint32_t *data)
{
	int SWD_Request = 0  ;

	SWD_Request = getSWD_Request(Address,APnDP,READ);

	send8bit(SWD_Request);

	turnAround_ToRead();
	SWDIO_InputMode();

	read3bit(ACK);

	read32bit(data);
	
	*Parity = readBit();

	turnAround_ToWrite();
	SWDIO_OutputMode();

	extraIdleClock(8);
	
}

int MemoryAccess_Read(uint32_t Address,uint32_t *dataRead)
{
	int ACK = 0, Parity = 0 ;
	
	SWDRegister_Write(TAR_REG,AP,&ACK,Address);
	SWD_ReadAP(DRW_REG,&ACK,&Parity,dataRead);
	
	return 0 ;
}

int MemoryAccess_Write(uint32_t Address,uint32_t WriteData)
{
	int ACK = 0, Parity = 0 ;
	
	SWDRegister_Write(TAR_REG,AP,&ACK,Address);
	SWDRegister_Write(DRW_REG,AP,&ACK,WriteData);
	
	return 0 ;
}

int SWD_ReadAP(int Address,int *ACK,int *Parity, uint32_t *data)
{
	SWDRegister_Read(Address,AP,ACK,Parity,data);
	SWDRegister_Read(Address,AP,ACK,Parity,data);
	
	return 0 ;
}



