#include <stdio.h>
#include "swd_Utilities.h"

/**
 * Calculate the parity bit for the SWD request header
 * by determining 	:	Address bit3 & bit 2 of the AP/DP register
 *					:	APnDP access, Read / Write access
 *
 * Input : Address_bit3 is the address bit 3 of the AP/DP register
 *		   Address_bit2	is the address bit 2 of the AP/DP register
 *         APnDP is the Access Port or Debug Port access
 *         ReadWrite is the Read or Write access
 *
 * Output : return the value of parity calculated
 */
int calculateParity_SWDRequest(int Address_bit3,int Address_bit2,int APnDP, int ReadWrite)
{
	int sum = 0 ;

	sum = Address_bit3 + Address_bit2 + APnDP + ReadWrite ;

	if (sum & 1)
		return 1 ;
	else
		return 0 ;

}

/**
 * Calculate the parity bit for a 32 bit data
 *
 * Input : data is to be evaluated to calculate the parity
 *
 * Output : return the value of parity calculated
 */
int calculateParity_32bitData(uint32_t data)
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

/**
 * Compare whether the input parity is the same as the calculated parity of the data received
 *
 * Input : data contains the data to be calculated for parity 
 *		   Parity contains the input parity to be compared
 *
 * Output : return ERR_NOERROR if both of the parity are match
 *			return ERR_INVALID_PARITY_RECEIVED if both of the parity does not match
 *			
 */
int compare_ParityWithData(uint32_t data,int Parity)
{
	int calculatedParity = 0 ;
	
	calculatedParity = calculateParity_32bitData(data);
	if(calculatedParity != Parity)
		return ERR_INVALID_PARITY_RECEIVED ;
	else
		return ERR_NOERROR ;
}

/**
 * Obtain the bit 3 and bit 2 of the address for AP/DP register
 *
 * Input : Address_bit3 is used to store the bit 3 of the address
 *		   Address_bit2 is used to store the bit 2 of the address
 *		   Address is the address going to be evaluated
 *
 */
void getSWD_AddressBit(int *Address_bit3,int *Address_bit2,int Address)
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
 * Input : Address is the address of the AP/DP register going to be accessed
 *         APnDP is used to determine Debug Port (DP) or Access Port (AP) transaction
 *         ReadWrite is used to determine Read or Write access
 *
 * Output : return the value of SWD request header
 */
int getSWD_Request(int Address,int APnDP,int ReadWrite)
{
	int SWD_Request = 0 ;
	int AddressBit2 = 0, AddressBit3 = 0, ParityBit = 0 ;

	getSWD_AddressBit(&AddressBit3,&AddressBit2,Address);
	ParityBit = calculateParity_SWDRequest(AddressBit3,AddressBit2,APnDP,ReadWrite);


	SWD_Request = SWD_Request | STARTBIT 		; //LSB
	SWD_Request = SWD_Request | APnDP 		<< 1;
	SWD_Request = SWD_Request | ReadWrite 	<< 2;
	SWD_Request = SWD_Request | AddressBit2 << 3;
	SWD_Request = SWD_Request | AddressBit3 << 4;
	SWD_Request = SWD_Request | ParityBit	<< 5;
	SWD_Request = SWD_Request | STOPBIT		<< 6;
	SWD_Request = SWD_Request | PARKBIT		<< 7;

	return SWD_Request ;
}

/**
 * Check error flag from CTRL/STATUS Register and return the error flag
 *
 * Input : NONE
 *
 * Return : errorFlag can be one of the following value
 *            + SWD_STICKYORUN_ERROR_FLAG   0x00
 *            + SWD_STICKYCMP_ERROR_FLAG    0x01
 *            + SWD_STICKYERR_ERROR_FLAG    0x02
 *            + SWD_WDATAERR_ERROR_FLAG     0x03
 */
uint32_t swdCheckErrorFlag()  {
  int ack = 0, parity = 0;
  uint32_t readData = 0, errorFlag = 0;

  //Access and read CTRL/STATUS Register
  swdReadCtrlStatus(&ack, &parity, &readData);
  
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
void swdClearErrorFlagInAbort(uint32_t errorFlag)  {
  int ack = 0;
  uint32_t clearFlag = 0;
  
  if(errorFlag == SWD_STICKYORUN_ERROR_FLAG)
    clearFlag = SWD_ORUNERR_CLEAR_FLAG;
  
  else if(errorFlag == SWD_STICKYCMP_ERROR_FLAG)
    clearFlag = SWD_STKCMP_CLEAR_FLAG;
  
  else if(errorFlag == SWD_STICKY_ERROR_FLAG)
    clearFlag = SWD_STKERR_CLEAR_FLAG;
  
  else if(errorFlag == SWD_WDATA_ERROR_FLAG)
    clearFlag = SWD_WDERR_CLEAR_FLAG;
  
  swdWriteAbort(&ack, clearFlag);
}

/**
 * swdReadWriteDpWithRetries retries DP read/write operation
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
void swdReadWriteDpWithRetries(int readOrWrite, int address, int *ack, int *parity, uint32_t *data, int counter) {
  while(counter--)  {    
    if(readOrWrite == READ) {
      swdReadDP(address, ack, parity, data);
      if((*ack ) == OK_RESPONSE)
        break;
    }

    else if(readOrWrite == WRITE) {
      swdWriteDP(address, ack, *data);
      if((*ack ) == OK_RESPONSE)
        break;
    }
  }
}

/**
 * swdReadWriteApWithRetries retries AP read/write operation
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
void swdReadWriteApWithRetries(int readOrWrite, int address, int *ack, int *parity, uint32_t *data, int counter) {
  while(counter--)  {    
    if(readOrWrite == READ) {
      swdReadAP(address, ack, parity, data);
      if((*ack ) == OK_RESPONSE)
        break;
    }

    else if(readOrWrite == WRITE) {
      swdWriteAP(address, ack, *data);
      if((*ack ) == OK_RESPONSE)
        break;
    }
  }
}

/**
 * retriesSwdOperation is a function to retry the same operation after receive WAIT_RESPONSE
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
int retriesSwdOperation(int readOrWrite, int address, int APorDP, int *parity, uint32_t *data, int numOfRetires)  {
  int ack = 0;
  
  if(APorDP == DP)
    swdReadWriteDpWithRetries(readOrWrite, address, &ack, parity, data, numOfRetires);
    
  else if(APorDP == AP)
    swdReadWriteApWithRetries(readOrWrite, address, &ack, parity, data, numOfRetires);

  return ack;
}

/**
 * Clear flag according to the acknowledgement response user should pass in the current transaction
 * information (int ackResponse, int readWrite, int address, int AP_DP, int parity, uint32_t data)
 * for retries purpose
 *
 * SW-DP must not issue a WAIT response to the following requests :
 *  + R IDCODE register
 •  + R CTRL/STAT register
 •  + W ABORT register
 *
 * SW-DP must not issue a FAULT response to the following requests :
 *  + R/W IDCODE register
 •  + R/W CTRL/STAT register
 •  + R/W ABORT register
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
void swdClearFlags(int ackResponse, int readOrWrite, int address, int APorDP, int parity, uint32_t data) {
  int ack = 0;
  uint32_t errorFlag = 0;

  switch(ackResponse) {
    case  OK_RESPONSE :
      //Do nothing
      break;

    case  WAIT_RESPONSE :
      ack = retriesSwdOperation(readOrWrite, address, APorDP, &parity, &data, 3);
      
      //clear DAPABORT and resend if acknowledge still WAIT_RESPONSE
      if(ack == WAIT_RESPONSE)  {
        swdWriteAbort(&ack, SWD_DAPABORT_CLEAR_FLAG);
        
        //RESEND DP/AP operation
        if(APorDP == DP)  {
          resendSwdDpOperation(readOrWrite, address, &ack, &parity, &data);
        }
        
        else if(APorDP == AP) {
          resendSwdApOperation(readOrWrite, address, &ack, &parity, &data);
        }
      }
      break;

    case  FAULT_RESPONSE  :
      errorFlag = swdCheckErrorFlag();
      swdClearErrorFlagInAbort(errorFlag); //Clear error flag
      break;
  }
}
