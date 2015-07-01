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
 *  -------------------------------------------------------------------
 *  START         Always  1
 *  AP/DP1        DP  = 0,   AP  = 1
 *  RW            W   = 0,   R   = 1
 *  Addr[2:3]     Refer Register Address
 *  Parity        APnDP + RW + Addr2 + Addr3 Even 1's = 0, Odd 1's = 1
 *  STOP          Always  1
 *  PARK          Always  1
 */

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
	uint32_t discardPreviousRead = 0;
	SWDRegister_Read(Address,AP,ACK,Parity,&discardPreviousRead);
	SWDRegister_Read(Address,AP,ACK,Parity,data);
	
	return 0 ;
}

/*  powerUpSystemAndDebug is function to set CDBGPWRUPREQ(bit 28) and CSYSPWRUPREQ(bit 30)
 *  in CTRL_STATUS register to power up system and debug before using any AP
 * 
 *  before function end it check if there is any set error flag in the register
 *  if there is any error it clear error flag by setting 1 in ABORT register
 *
 *  input   : NONE
 *  return  : NONE
 */
void powerUpSystemAndDebug()  {
  int ack = 0;
  uint32_t errorFlag = 0;
  
	swdWriteCtrlStatus(&ack, POWERUP_SYSTEM);
	errorFlag = checkErrorFlag();
	if(errorFlag != 0)
		swdWriteAbort(&ack, errorFlag); //Write to AP ABORT Register clear error flag
}

/*  readAHB_IDR is a function to access MEM-AP register and select BankF read the register IDR
 *
 *  input   : data_IDR is the variable pass-in by user to store the IDR
 *  return  : NONE
 */
void readAHB_IDR(uint32_t *data_IDR)	{
  int ack = 0, parity = 0;
  
  powerUpSystemAndDebug();
  swdWriteSelect(&ack, BANK_F);
  SWD_ReadAP(IDR_REG, &ack, &parity, data_IDR);
}

