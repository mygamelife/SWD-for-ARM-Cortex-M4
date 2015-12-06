#include "DWTUnit.h"

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


/**
 *  Set watchpoint for address comparison
 *  Note : Will automatically set processor to CORE_DEBUG_MODE
 *
 *  Input : dwtCompNumber is the comparator going to be used 
 *          Possible value :
 *              COMPARATOR_0               DWT Comparator Number 0
 *              COMPARATOR_1               DWT Comparator Number 1
 *              COMPARATOR_2               DWT Comparator Number 2
 *              COMPARATOR_3               DWT Comparator Number 3
 *
 *          address is the address to be compared
 *          addressMask is the mask going to be applied to the address 
 *          Possible value :
 *              WATCHPOINT_MASK_NOTHING  		  Compare all 32 bits of address set in DWT_COMPn 
 *							WATCHPOINT_MASK_BIT0  			  Ignore Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT1_BIT0,		Ignore Bit1 and Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT2_BIT0,		Ignore Bit2 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT3_BIT0,		Ignore Bit3 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT4_BIT0     Ignore Bit4 to Bit 0 of address set in DWT_COMPn during comparison
 *                        ""                                              ""
 *							WATCHPOINT_MASK_BIT12_BIT0		Ignore Bit12 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT13_BIT0		Ignore Bit13 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT14_BIT0	  Ignore Bit14 to Bit 0 of address set in DWT_COMPn during comparison
 *
 *          accessMode is the access mode for the watchpoint
 *          Possible value :
 *              WATCHPOINT_READ         Watchpoint on read access
 *              WATCHPOINT_WRITE        Watchpoint on write access
 *              WATCHPOINT_READWRITE    Watchpoint on read/write access
 *
 *  Output :  return 0 if watchpoint is set 
 *            return -1 if invalid comparator is selected
 */
int setAddressWatchpoint(int dwtCompNumber,uint32_t address,Watchpoint_AddressMask addressMask,Watchpoint_AccessMode accessMode)
{ 
  int result = 0 ;
  int valid = checkForValidDWTComparator(dwtCompNumber) ;
  
  if(valid == -1)
    return -1 ;
  
  enableDWTandITM(); //enable global enable for DWT (Needed for reading & writing DWT Register)
  
  memoryWriteWord((uint32_t)(&DWT_COMP[dwtCompNumber].FUNCTION),DISABLE_DWTCOMPARATOR); //disable selected comparator first

  memoryWriteWord((uint32_t)(&DWT_COMP[dwtCompNumber].COMP),address);
  memoryWriteWord((uint32_t)(&DWT_COMP[dwtCompNumber].MASK),addressMask);
  
  if(accessMode != DISABLE_DWTCOMPARATOR)//prevent setting to disable mode twice
    memoryWriteWord((uint32_t)(&DWT_COMP[dwtCompNumber].FUNCTION),accessMode);
  
  return result ;
}

/**
 *  Set watchpoint for data comparison with another 1 comparator used for data address matching
 *  Note : Will automatically set processor to CORE_DEBUG_MODE
 *
 *  Input : matchingCompNumber is the comparator going to be used for data address matching
 *          Possible value :
 *              COMPARATOR_0               DWT Comparator Number 0
 *              COMPARATOR_2               DWT Comparator Number 2
 *              COMPARATOR_3               DWT Comparator Number 3
 *
 *          address is the address to be used for data address matching
 *          addressMask is the mask going to be applied to the address 
 *          Possible value :
 *              WATCHPOINT_MASK_NOTHING  		  Compare all 32 bits of address set in DWT_COMPn 
 *							WATCHPOINT_MASK_BIT0  			  Ignore Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT1_BIT0,		Ignore Bit1 and Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT2_BIT0,		Ignore Bit2 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT3_BIT0,		Ignore Bit3 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT4_BIT0     Ignore Bit4 to Bit 0 of address set in DWT_COMPn during comparison
 *                        ""                                              ""
 *							WATCHPOINT_MASK_BIT12_BIT0		Ignore Bit12 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT13_BIT0		Ignore Bit13 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT14_BIT0,	  Ignore Bit14 to Bit 0 of address set in DWT_COMPn during comparison
 *
 *          matchedData is the data going to be matched/compared
 *          dataSize is the data size going to be compared
 *          Possible value :
 *              WATCHPOINT_BYTE         
 *              WATCHPOINT_HALFWORD
 *              WATCHPOINT_WORD
 *
 *          accessMode is the access mode for the watchpoint
 *          Possible value :
 *              WATCHPOINT_READ         Watchpoint on read access
 *              WATCHPOINT_WRITE        Watchpoint on write access
 *              WATCHPOINT_READWRITE    Watchpoint on read/write access
 *
 *  Output :  return 0 if watchpoint is set 
 *            return -1 if invalid comparator is selected 
 */
int setDataWatchpoint_MatchingOneComparator(int matchingCompNumber,uint32_t address,Watchpoint_AddressMask addressMask,
                                            uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode)
{
  int result = 0 ;
  int valid = 0 ;
  uint32_t configData = 0 ;
  
  valid = checkForValidDWTComparator(matchingCompNumber);
  if(matchingCompNumber == 1 || valid == -1)
    return -1 ;

  configData = (matchingCompNumber << 16) + (matchingCompNumber << 12) + (dataSize << 10) + (DATA_COMPARISON << 8) + accessMode ;
 
  setAddressWatchpoint(matchingCompNumber,address,addressMask,DISABLE_DWTCOMPARATOR);
  
  memoryWriteWord((uint32_t)(&DWT_COMP[1].FUNCTION),DISABLE_DWTCOMPARATOR); //disable selected comparator first
  
  memoryWriteWord((uint32_t)(&DWT_COMP[1].COMP),matchedData);
  memoryWriteWord((uint32_t)(&DWT_COMP[1].MASK),WATCHPOINT_MASK_NOTHING);
  memoryWriteWord((uint32_t)(&DWT_COMP[1].FUNCTION),configData);
  
  return result ;
}

/**
 *  Set watchpoint for data comparison with another 2 comparator used for data address matching
 *  Note : Will automatically set processor to CORE_DEBUG_MODE
 *
 *  Input : matchingCompNumber1 is the first comparator going to be used for data address matching
 *          Possible value :
 *              COMPARATOR_0               DWT Comparator Number 0
 *              COMPARATOR_2               DWT Comparator Number 2
 *              COMPARATOR_3               DWT Comparator Number 3
 *
 *          address1 is the first address to be used for data address matching
 *          addressMask1 is the mask going to be applied to the first address 
 *          Possible value :
 *              WATCHPOINT_MASK_NOTHING  		  Compare all 32 bits of address set in DWT_COMPn 
 *							WATCHPOINTMASK_BIT0 			    Ignore Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT1_BIT0		  Ignore Bit1 and Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT2_BIT0		  Ignore Bit2 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINTMASK_BIT3_BIT0		  Ignore Bit3 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT4_BIT0     Ignore Bit4 to Bit 0 of address set in DWT_COMPn during comparison
 *                        ""                                              ""
 *							WATCHPOINT_MASK_BIT12_BIT0		Ignore Bit12 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT13_BIT0		Ignore Bit13 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT14_BIT0	  Ignore Bit14 to Bit 0 of address set in DWT_COMPn during comparison
 *
 *          matchingCompNumber2 is the second comparator going to be used for data address matching
 *          Possible value :
 *              COMPARATOR_0               DWT Comparator Number 0
 *              COMPARATOR_2               DWT Comparator Number 2
 *              COMPARATOR_3               DWT Comparator Number 3
 *
 *          address2 is the second address to be used for data address matching
 *          addressMask2 is the mask going to be applied to the second address 
 *          Possible value :
 *              WATCHPOINT_MASK_NOTHING  		  Compare all 32 bits of address set in DWT_COMPn 
 *							WATCHPOINT_MASK_BIT0  			  Ignore Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT1_BIT0,		Ignore Bit1 and Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT2_BIT0,		Ignore Bit2 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT3_BIT0,		Ignore Bit3 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT4_BIT0     Ignore Bit4 to Bit 0 of address set in DWT_COMPn during comparison
 *                        ""                                              ""
 *							WATCHPOINT_MASK_BIT12_BIT0		Ignore Bit12 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT13_BIT0		Ignore Bit13 to Bit 0 of address set in DWT_COMPn during comparison
 *							WATCHPOINT_MASK_BIT14_BIT0	  Ignore Bit14 to Bit 0 of address set in DWT_COMPn during comparison
 *
 *          matchedData is the data going to be matched/compared
 *          dataSize is the data size going to be compared
 *          Possible value :
 *              WATCHPOINT_BYTE         
 *              WATCHPOINT_HALFWORD
 *              WATCHPOINT_WORD
 *
 *          accessMode is the access mode for the watchpoint
 *          Possible value :
 *              WATCHPOINT_READ         Watchpoint on read access
 *              WATCHPOINT_WRITE        Watchpoint on write access
 *              WATCHPOINT_READWRITE    Watchpoint on read/write access
 *
 *  Output :  return 0 if watchpoint is set 
 *            return -1 if invalid comparator is selected 
 */
int setDataWatchpoint_MatchingTwoComparator(int matchingCompNumber1,uint32_t address1,Watchpoint_AddressMask addressMask1,
                                            int matchingCompNumber2,uint32_t address2,Watchpoint_AddressMask addressMask2,
                                            uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode)
{
  int result = 0 ;
  int valid1 = 0 , valid2 = 0 ;
  uint32_t configData = 0 ;
  
  valid1 = checkForValidDWTComparator(matchingCompNumber1);
  valid2 = checkForValidDWTComparator(matchingCompNumber2);

  if(matchingCompNumber1 == 1 || matchingCompNumber2 == 1 || valid1 == -1 || valid2 == -1)
    return -1 ;

  configData = (matchingCompNumber2 << 16) + (matchingCompNumber1 << 12) + (dataSize << 10) + (DATA_COMPARISON << 8) + accessMode ;

  setAddressWatchpoint(matchingCompNumber1,address1,addressMask1,DISABLE_DWTCOMPARATOR);
  setAddressWatchpoint(matchingCompNumber2,address2,addressMask2,DISABLE_DWTCOMPARATOR);
  
  memoryWriteWord((uint32_t)(&DWT_COMP[1].FUNCTION),DISABLE_DWTCOMPARATOR); //disable selected comparator first
  
  memoryWriteWord((uint32_t)(&DWT_COMP[1].COMP),matchedData);
  memoryWriteWord((uint32_t)(&DWT_COMP[1].MASK),WATCHPOINT_MASK_NOTHING);
  memoryWriteWord((uint32_t)(&DWT_COMP[1].FUNCTION),configData);
  
  return result ;
}

/**
 *  Check if data watchpoint occurs
 *
 *  Output :  return 1 for true
 *            return 0 for false
 */
int hasDataWatchpointOccurred()
{
  uint32_t dataRead = 0 ;
  
  if(hasDWTTrapDebugEventOccured())
    memoryReadWord((uint32_t)(&DWT_COMP[1].FUNCTION),&dataRead);
  
  dataRead = (dataRead & DWT_MATCHED_MASK) >> 24 ;
  return (int)dataRead ;
}

/**
 *  Check if address watchpoint occur
 *
 *  Input : dwtCompNumber is the comparator going to be checked
 *          Possible value :
 *              COMPARATOR_0               DWT Comparator Number 0
 *              COMPARATOR_1               DWT Comparator Number 1
 *              COMPARATOR_2               DWT Comparator Number 2
 *              COMPARATOR_3               DWT Comparator Number 3
 *  Output :  return 1 for true
 *            return 0 for false
 *            return -1 if invalid comparator is selected 
 */
int hasAddressWatchpointOccurred(int dwtCompNumber)
{
  uint32_t dataRead = 0 ;
  int valid = 0 ;
  
  valid = checkForValidDWTComparator(dwtCompNumber) ;
  if(valid == -1)
    return valid ;
  
  if(hasDWTTrapDebugEventOccured())
    memoryReadWord((uint32_t)(&DWT_COMP[dwtCompNumber].FUNCTION),&dataRead);
  
  dataRead = (dataRead & DWT_MATCHED_MASK) >> 24 ;
  
  return (int)dataRead ;
}

/**
 *  Disable the selected DWT Comparator
 *
 *  Input : dwtCompNumber is the comparator going to be used 
 *          Possible value :
 *              COMPARATOR_0               DWT Comparator Number 0
 *              COMPARATOR_1               DWT Comparator Number 1
 *              COMPARATOR_2               DWT Comparator Number 2
 *              COMPARATOR_3               DWT Comparator Number 3
 *
 *  Output :  return 0 if the comparator is disabled
 *            return -1 if invalid comparator is selected 
 */
int disableDWTComparator(int dwtCompNumber)
{
  int valid = 0 ;
  
  valid = checkForValidDWTComparator(dwtCompNumber) ;
  if(valid == -1)
    return valid ;
    
  memoryWriteWord((uint32_t)(&DWT_COMP[dwtCompNumber].FUNCTION),DISABLE_DWTCOMPARATOR);
  
  return 0 ;
}
