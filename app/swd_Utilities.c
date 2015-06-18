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
