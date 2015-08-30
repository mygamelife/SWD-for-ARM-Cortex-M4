#include "DWT_Utilities.h"

/**
 *  Use to get the number for the selected DWT Comparator
 *
 *  Input :  dwtCompNumber is the selected DWT Comparator register
 *				   Possible value :
 *					    0	            DWT Comparator Register 0
 *					    1	            DWT Comparator Register 1
 *					    2	            DWT Comparator Register 2
 *					    3	            DWT Comparator Register 3
 *
 *  Output :  return 1 if it is valid
 *			      return -1 for invalid comparator
 */
int checkForValidDWTComparator(int dwtCompNumber)
{	
	if( dwtCompNumber == 0 || dwtCompNumber == 1 || dwtCompNumber == 2 || dwtCompNumber == 3)
    return 1 ;
  else
    return -1 ;
}
