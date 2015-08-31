#include "Misc_Utilities.h"

/**
 *	Use to check if multiple bits is set in the data with the mask
 *	
 *	Input : data contains the data going to be compared with
 *			dataMask is used to to check for the specific masked bits
 *
 *	Output : return ERR_NOERROR if all the bits specified is set
 *			 return ERR_BITS_SPECIFIED_NOT_SET if not all/non of the bits specified are set or the dataMask is equal to zero
 *	
 */
int check_BitSetWithMask(uint32_t data,uint32_t dataMask)
{
	if (dataMask == 0)
		return ERR_BITS_SPECIFIED_NOT_SET ;
	
	if ((data & dataMask) == dataMask)
		return ERR_NOERROR ;
	else
		return ERR_BITS_SPECIFIED_NOT_SET ;
}

/**
 *	Use to check whether the data written in the register is the same as the data read  
 *	
 *	Input : dataWrote contains the data written into the register earlier
 *			dataRead contains the data read from the register after writing
 *
 *	Output : return ERR_NOERROR if both data are the same
 *			 return ERR_DATARW_NOT_MATCH if both data are not the same
 *	
 */
int check_dataWrote_dataRead(uint32_t dataWrote,uint32_t dataRead)
{
	if (dataRead == dataWrote)
		return ERR_NOERROR ;
	else
		return ERR_DATARW_NOT_MATCH ;
}
