#include "Swd.h"

/**
 * Calculate the parity bit for the SWD request header
 * by determining 	:	Address bit3 & bit 2 of the AP/DP register
 *					        :	APnDP access, Read / Write access
 *
 * Input :  addressBit3 is the address bit 3 of the AP/DP register
 *          addressBit2	is the address bit 2 of the AP/DP register
 *          pointType is the Access Port or Debug Port access
 *          rw can be one of the following value :
 *            SWD_Read
 *            SWD_WRITE
 *
 * Output : return the value of parity calculated
 */
int calculateParitySwdRequest(int addressBit3, int addressBit2, int pointType, int rw)
{
	int sum = 0 ;

	sum = addressBit3 + addressBit2 + pointType + rw;

	if (sum & 1)
		return 1 ;
	else
		return 0 ;
}

/**
 * Compare whether the input parity is the same as the calculated parity of the data received
 *
 * Input : data contains the data to be calculated for parity 
 *		     parity contains the input parity to be compared
 *
 * Output : return SWD_NO_ERROR if both of the parity are match
 *			    return ERR_INVALID_PARITY_RECEIVED if both of the parity does not match
 *			
 */
int compareParityWithData(uint32_t data, int parity)
{
	int calculatedParity = 0 ;
	
	calculatedParity = calculateParity32bitData(data);
	if(calculatedParity != parity)
		return ERR_INVALID_PARITY_RECEIVED ;
	else
		return SWD_NO_ERROR;
}

/**
 * Obtain the bit 3 and bit 2 of the address for AP/DP register
 *
 * Input : Address_bit3 is used to store the bit 3 of the address
 *		   Address_bit2 is used to store the bit 2 of the address
 *		   Address is the address going to be evaluated
 *
 */
void getSwdAddressBit(int *Address_bit3,int *Address_bit2,int Address)
{
	*Address_bit2 = (Address & 1 << 2 ) >> 2 ;
	*Address_bit3 = (Address & 1 << 3 ) >> 3 ;
}


/**
 * Calculate the value of the SWD request header to be sent to the target
 ******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    0      |    1     |     0      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************
 *
 * Input : address is the address of the AP/DP register going to be accessed
 *         pointType is used to determine Debug Port (DP) or Access Port (AP) transaction
 *         rw is used to determine Read or Write access
 *
 * Output : return the value of SWD request header
 */
int getSwdRequest(int address, int pointType, int rw)
{
	int swdRequest = 0 ;
	int addressBit2 = 0, addressBit3 = 0, parityBit = 0 ;

	getSwdAddressBit(&addressBit3, &addressBit2, address);
	parityBit = calculateParitySwdRequest(addressBit3, addressBit2, pointType, rw);

	swdRequest = swdRequest | SWD_STARTBIT      ; //LSB
	swdRequest = swdRequest | pointType 		<< 1;
	swdRequest = swdRequest | rw            << 2;
	swdRequest = swdRequest | addressBit2   << 3;
	swdRequest = swdRequest | addressBit3   << 4;
	swdRequest = swdRequest | parityBit     << 5;
	swdRequest = swdRequest | SWD_STOPBIT   << 6;
	swdRequest = swdRequest | SWD_PARKBIT   << 7;

	return swdRequest;
}

/**
 * Calculate the parity bit for a 32 bit data
 *
 * Input : data is to be evaluated to calculate the parity
 *
 * Output : return the value of parity calculated
 */
int calculateParity32bitData(uint32_t data)
{
	int i , sum = 0;

	for (i = 0 ; i < 32 ; i ++)
	{
		if (data & 0x01)
			sum ++ ;

		data = data >> 1;
	}

	if (sum & 1)
		return 1 ;
	else
		return 0 ;
}

/** Check error flag from CTRL/STATUS Register and return the error flag
  *
  * Input : NONE
  *
  * Return : errorFlag can be one of the following value
  *            + SWD_STICKYORUN_ERROR_FLAG   0x00
  *            + SWD_STICKYCMP_ERROR_FLAG    0x01
  *            + SWD_STICKYERR_ERROR_FLAG    0x02
  *            + SWD_WDATAERR_ERROR_FLAG     0x03
  */
uint32_t swdCheckErrorFlag(void)  {
  uint32_t readData = 0, errorFlag = 0;

  /* Access and read CTRL/STATUS Register */
  swdReadDP(CTRLSTAT_REG, &readData);
  
  if(readData & SWD_WDATAERR_MASK)
    errorFlag = SWD_WDATA_ERROR_FLAG;

  else if(readData & SWD_STICKYERR_MASK)
    errorFlag = SWD_STICKY_ERROR_FLAG;

  else if(readData & SWD_STICKYCMP_MASK)
    errorFlag = SWD_STICKYCMP_ERROR_FLAG;

  else if(readData & SWD_STICKYORUN_MASK)
    errorFlag = SWD_STICKYORUN_ERROR_FLAG;

  return errorFlag;
}

/**
 * Check error flag and set the clear error flag bit accordingly and write into ABORT register
 *
 *    CTRL/STATUS Register        ABORT Register
 * ---------------------------------------------------
 *    WDATAERR    [bit 7]         WDERRCLR    [bit 3]
 *    STICKYERR   [bit 5]         STKERRCLR   [bit 2]
 *    STICKYCMP   [bit 4]         STKCMPCLR   [bit 1]
 *    STICKYORUN  [bit 1]         ORUNERRCLR  [bit 4]
 *
 * Input : errorFlag contain the actual error flag that need to be clear
 *
 * Return : NONE
 */
void swdClearErrorFlag(uint32_t errorFlag)  {
  uint32_t clearFlag = 0;
  
  if(errorFlag == SWD_STICKYORUN_ERROR_FLAG)
    clearFlag = SWD_ORUNERR_CLEAR_FLAG;
  
  else if(errorFlag == SWD_STICKYCMP_ERROR_FLAG)
    clearFlag = SWD_STKCMP_CLEAR_FLAG;
  
  else if(errorFlag == SWD_STICKY_ERROR_FLAG)
    clearFlag = SWD_STKERR_CLEAR_FLAG;
  
  else if(errorFlag == SWD_WDATA_ERROR_FLAG)
    clearFlag = SWD_WDERR_CLEAR_FLAG;
  
  swdWriteDP(ABORT_REG, clearFlag);
}

/** swdGetAckResponse is to determine acknowledgement status
  *
  * Input   : ack is the acnowledgement
  *
  * Return  : SWD_NO_ERROR if ack is in ok status
  *           ERR_ACK_WAIT_RESPONSE if ack is in wait status
  *           ERR_ACK_FAULT_RESPONSE if ack is in fault status
  */
SwdError swdGetAckResponse(int ack) {
  if(ack == SWD_WAIT_RESPONSE)
    return ERR_ACK_WAIT_RESPONSE;
  
  else if(ack == SWD_FAULT_RESPONSE)
    return ERR_ACK_FAULT_RESPONSE;

  else return SWD_NO_ERROR;
}

/** swdReadWriteDpWithRetries retries DP read/write operation
  *
  * Input : readOrWrite is the user define variable READ/WRITE
  *         address is the address location to read/write
  *         ack is the acknowledgement of the transaction
  *         parity is the parity of the data
  *         data is the read/write 32bit data
  *         counter to store the number of retries value
  *
  * Return : None
  */
int swdReadDpWithRetries(int address, uint32_t *data, int counter) {
  int error = 0;
  
  while(counter--)  {
    error = swdReadDP(address, data);
    if(error != ERR_ACK_WAIT_RESPONSE) {
      break;
    }
  }
  return error;
}

int swdWriteDpWithRetries(int address, uint32_t data, int counter) {
  int error = 0;
  
  while(counter--)  {
    error = swdWriteDP(address, data);
    if(error != ERR_ACK_WAIT_RESPONSE) {
      break;
    }
  }
  return error;
}

/** swdReadWriteApWithRetries retries AP read/write operation
  *
  * Input : readOrWrite is the user define variable READ/WRITE
  *         address is the address location to read/write
  *         ack is the acknowledgement of the transaction
  *         parity is the parity of the data
  *         data is the read/write 32bit data
  *         counter to store the number of retries value
  *
  * Return : None
  */
int swdReadApWithRetries(int address, uint32_t *data, int counter) {
  int error = 0;
  
  while(counter--)  {    
    error = swdReadAP(address, data);
    if(error != ERR_ACK_WAIT_RESPONSE) {
      break;
    }
  }
  return error;
}

int swdWriteApWithRetries(int address, uint32_t data, int counter) {
  int error = 0;
  
  while(counter--)  {
    error = swdWriteAP(address, data);
    if(error != ERR_ACK_WAIT_RESPONSE) {
      break;
    }
  }
  return error;
}

/** retriesSwdOperation is a function to retry the same operation after receive SWD_WAIT_RESPONSE
  *
  * Input : readOrWrite is the user define variable READ/WRITE
  *         address is the address location to read/write
  *         ack is the acknowledgement of the transaction
  *         parity is the parity of the data
  *         data is the read/write 32bit data
  *         counter to store the number of retries value
  *
  * Return : ack is the acknowledgement status
  */
int swdRetriesOperation(int readOrWrite, int pointType, int address, uint32_t *data, int numOfRetires)  {
  int error;
  
  if(pointType == SWD_DP) {
    if(readOrWrite == SWD_READ)
      error = swdReadDpWithRetries(address, data, numOfRetires);
    else
      error = swdWriteDpWithRetries(address, *data, numOfRetires);
  }
  
  else  {
    if(readOrWrite == SWD_READ)
      error = swdReadApWithRetries(address, data, numOfRetires);
    else
      error = swdWriteApWithRetries(address, *data, numOfRetires);
  }

  return error;
}

void swdResendOperation(int readOrWrite, int pointType, int address, uint32_t *data)  {
  if(pointType == SWD_DP) {
    if(readOrWrite == SWD_READ) {
      swdReadDP(address, data);
    }
    else  {
      swdWriteDP(address, *data);
    }
  }
  
  else  {
    if(readOrWrite == SWD_READ) {
      swdReadAP(address, data);
    }
    else  {
      swdWriteAP(address, *data);
    }
  }
}

/**
  * Clear flag according to the acknowledgement response user should pass in the current transaction
  * information (int ackResponse, int readWrite, int address, int AP_DP, int parity, uint32_t data)
  * for retries purpose
  *
  * SW-DP must not issue a WAIT response to the following requests :
  *  + R IDCODE register
  *  + R CTRL/STAT register
  *  + W ABORT register
  *
  * SW-DP must not issue a FAULT response to the following requests :
  *  + R/W IDCODE register
  *  + R/W CTRL/STAT register
  *  + R/W ABORT register
  *
  * Input : ackResponse is the acknowledgement value sent by target
  *         readWrite is the current transaction operation R/W
  *         address is the current transaction address
  *         AP_DP is the current transaction register AP/DP
  *         parity is the current transaction parity
  *         data is the current transaction read/write 32bit data
  *
  * Return : None
  */
void swdErrorHandler(int error, int readOrWrite, int pointType, int address, uint32_t *data) {
  uint32_t errorCode = 0, errorFlag = 0;

  switch(error) {
    case SWD_NO_ERROR :
      // Do nothing
      break;

    case ERR_ACK_WAIT_RESPONSE :
      errorCode = swdRetriesOperation(readOrWrite, pointType, address, data, 3);
      
      /* clear DAPABORT and resend if acknowledge still SWD_WAIT_RESPONSE */
      if(errorCode == ERR_ACK_WAIT_RESPONSE)  {
        swdWriteDP(ABORT_REG, SWD_DAPABORT_CLEAR_FLAG);
        swdResendOperation(readOrWrite, pointType, address, data);
      }
      break;

    case ERR_ACK_FAULT_RESPONSE :
      errorFlag = swdCheckErrorFlag();
      swdClearErrorFlag(errorFlag); //Clear error flag
      break;
  }
}

/**
  ================================================================================
                       ##### Swd DP/AP Read/Write Functions #####
  ================================================================================  
  */
  
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
 *  0x0C          RDBUFF_REG    N/A
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
 
SwdError swdRegisterWrite(int address, int pointType, uint32_t data)
{
	int swdRequest, parity, ack;

	swdRequest = getSwdRequest(address, pointType, SWD_WRITE);
	parity = calculateParity32bitData(data); //calculate parity before initiating transfer

	send8bit(swdRequest);

	turnAroundRead();
	setSWDIOInputMode();

	read3bit(&ack);

	turnAroundWrite();
	setSWDIOOutputMode();

	send32bit(data);
	sendBit(parity);

	extraIdleClock(8);
  
  return swdGetAckResponse(ack);
}

SwdError swdRegisterRead(int address, int pointType, uint32_t *data)
{
	int swdRequest, ack;
	//int parity;

	swdRequest = getSwdRequest(address, pointType, SWD_READ);
	send8bit(swdRequest);

	turnAroundRead();
	setSWDIOInputMode();

	read3bit(&ack);
	read32bit(data);

	//parity = readBit();
	readBit();

	turnAroundWrite();
	setSWDIOOutputMode();

	extraIdleClock(8);
  
  return swdGetAckResponse(ack);
}

SwdError swdReadAP(int address, uint32_t *data)
{
  int error;
	uint32_t discardPreviousRead = 0;
  error = swdRegisterRead(address, SWD_AP, &discardPreviousRead);
  error = swdRegisterRead(address, SWD_AP, data);

	return error;
}

/*  readAHB_IDR is a function to access AHB-AP (based on the implementation of MEM-AP) register and select BankF read the register IDR
 *
 *  input   : data_IDR is the variable pass-in by user to store the IDR
 *  return  : NONE
 */
SwdError readAhbIDR(uint32_t *Idr)	{
  int error = 0;

  /**Power up/wake up debug system before using */
  swdWriteDP(CTRLSTAT_REG, POWERUP_SYSTEM);

  error = swdWriteDP(SELECT_REG, SELECT_BANKF);
  if(error != SWD_NO_ERROR) return error;

  error = swdReadAP(IDR_REG, Idr);
  
  return error;
}

/** swdSelectMemorySize is a function to set the memory access size bit in CSW register
  *
  * input   : memorySize can be one of the following value
  *             + CSW_BYTE_SIZE
  *             + CSW_HALFWORD_SIZE
  *             + CSW_WORD_SIZE
  *             + CSW_DISABLE_ADDR_INC
  *             + CSW_ENABLE_ADDR_INC_SINGLE
  *             + CSW_ENABLE_ADDR_INC_PACKED
  *
  * return  : NONE
  */
SwdError swdSelectMemorySize(uint32_t memorySize)  {
  int error = 0;
  
  /* Select Bank register 0 */
  error = swdWriteDP(SELECT_REG, SELECT_BANK0);
  if(error != SWD_NO_ERROR) return error;
  
  /* Write cswBitSet into CSW register */
  error = swdWriteAP(CSW_REG, memorySize);
  
  return error;
}

/**
 *  Initialise SWD on the target by 
 * 1. Perform a line reset and switch SWJ-DP from JTAG to SWD
 * 2. Perform a line reset again and generate idle clock cycles
 * 3. Send 8 bits of SWD request header (Address of IDCODE register 0x00,Debug Port access,Read)
 * 4. Generate a turn around and switch to input mode
 * 5. Read 3 bits of ACK response from the target
 * 6. Read 32 bits of IDCODE data replied by the target
 * 7. Read 1 bit of parity bit replied by the target
 * 8. Generate a turn around and switch to output mode
 * 9. Generate idle clock cycles to prepare for next transaction / stop
 */
SwdError swdInit(void)
{
	int error = 0;
	uint32_t idcode = 0;

	hardResetTarget();
	switchJTAGtoSWD();
	error = swdReadDP(IDCODE_REG, &idcode);
  
  return error;
}

/**
 * Perform the switching of SWJ-DP from JTAG to SWD as SWJ-DP is in JTAG mode on POR
 * This is done by performing a line reset first followed by sending a 16 bits of 
 * JTAG to SWD switching sequence (0xE79E) and then perform a line reset and extra idle clock cycles
 *
 */
void switchJTAGtoSWD()
{
	lineReset(55);
	send16bit(0xE79E);
	lineReset(55);

	extraIdleClock(3);
}
