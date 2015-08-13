#include "DWT_Utilities.h"

/**
 *  Use to get the number for the selected DWT Comparator
 *
 *  Input :  dwtCompNumber is the selected DWT Comparator register
 *				   Possible value :
 *					    DWT_COMP0	
 *					    DWT_COMP1	
 *					    DWT_COMP2	
 *					    DWT_COMP3	
 *
 *  Output :  return number of the comparator
 *			      return -1 for invalid comparator
 */
int getDWTComparatorNumber(uint32_t dwtCompNumber)
{	
	if (dwtCompNumber == DWT_COMP0)
		return 0 ;
	
	if (dwtCompNumber == DWT_COMP1)
		return 1 ;
	
	if (dwtCompNumber == DWT_COMP2)
		return 2 ;
	
	if (dwtCompNumber == DWT_COMP3)
		return 3 ;
	
	return -1 ;
}
