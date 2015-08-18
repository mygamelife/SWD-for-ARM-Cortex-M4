#include "DWT_Unit.h"

/**
 *  Set watchpoint for address comparison
 *  Note : Will automatically set processor to CORE_DEBUG_MODE
 *
 *  Input : dwtCompNumber is the comparator going to be used 
 *          Possible value :
 *              DWT_COMP0       Comparator Number 0
 *              DWT_COMP1       Comparator Number 1
 *              DWT_COMP2       Comparator Number 2
 *              DWT_COMP3       Comparator Number 3
 *
 *          address is the address to be compared
 *          addressMask is the mask going to be applied to the address 
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
 *          accessMode is the access mode for the watchpoint
 *          Possible value :
 *              WATCHPOINT_READ         Watchpoint on read access
 *              WATCHPOINT_WRITE        Watchpoint on write access
 *              WATCHPOINT_READWRITE    Watchpoint on read/write access
 *
 *  Output :  return 0 if watchpoint is set 
 *            return -1 if invalid comparator is selected
 */
int setAddressWatchpoint(uint32_t dwtCompNumber,uint32_t address,Watchpoint_AddressMask addressMask,Watchpoint_AccessMode accessMode)
{ 
  int result = 0 ;
  DWT_COMP[0].COMP =123;
  uint32_t dwtMaskReg = dwtCompNumber + 4 ;
  uint32_t dwtFuncReg = dwtCompNumber + 8 ;
  
  enableDWTandITM(); //enable global enable for DWT (Needed for reading & writing DWT Register)
  setCoreMode(CORE_DEBUG_MODE);

  if(getDWTComparatorNumber(dwtCompNumber) == -1)
    return -1 ;
  
  memoryWriteWord(dwtFuncReg,DISABLE_DWTCOMPARATOR); //disable selected comparator first

  memoryWriteWord(dwtCompNumber,address);
  memoryWriteWord(dwtMaskReg,addressMask);
  
  if(accessMode !=DISABLE_DWTCOMPARATOR)//prevent setting to disable mode twice
    memoryWriteWord(dwtFuncReg,accessMode);
  
  return result ;
}

/**
 *  Set watchpoint for data comparison with another 1 comparator used for data address matching
 *  Note : Will automatically set processor to CORE_DEBUG_MODE
 *
 *  Input : dwtCompNumber is the comparator going to be used for data address matching
 *          Possible value :
 *              DWT_COMP0       Comparator Number 0
 *              DWT_COMP2       Comparator Number 2
 *              DWT_COMP3       Comparator Number 3
 *
 *          address is the address to be used for data address matching
 *          addressMask is the mask going to be applied to the address 
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
int setDataWatchpoint_MatchingOneComparator(uint32_t dwtCompNumber,uint32_t address,Watchpoint_AddressMask addressMask,
                                            uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode)
{
  int result = 0 ;
  int matchingCompNumber = 0 ;
  uint32_t configData = 0 ;
  
  matchingCompNumber = getDWTComparatorNumber(dwtCompNumber);
  if(dwtCompNumber == DWT_COMP1)
    return -1 ;
  
  if(matchingCompNumber == -1)
    return -1 ;

  configData = (matchingCompNumber << 16) + (matchingCompNumber << 12) + (dataSize << 10) + (DATA_COMPARISON << 8) + accessMode ;
 
  setAddressWatchpoint(dwtCompNumber,address,addressMask,DISABLE_DWTCOMPARATOR);
  
  memoryWriteWord(DWT_FUNC1,DISABLE_DWTCOMPARATOR); //disable selected comparator first
  
  memoryWriteWord(DWT_COMP1,matchedData);
  memoryWriteWord(DWT_MASK1,WATCHPOINT_MASK_NOTHING);
  memoryWriteWord(DWT_FUNC1,configData);
  
  return result ;
}

/**
 *  Set watchpoint for data comparison with another 2 comparator used for data address matching
 *  Note : Will automatically set processor to CORE_DEBUG_MODE
 *
 *  Input : dwtCompNumber1 is the first comparator going to be used for data address matching
 *          Possible value :
 *              DWT_COMP0       Comparator Number 0
 *              DWT_COMP2       Comparator Number 2
 *              DWT_COMP3       Comparator Number 3
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
 *          dwtCompNumber2 is the second comparator going to be used for data address matching
 *          Possible value :
 *              DWT_COMP0       Comparator Number 0
 *              DWT_COMP2       Comparator Number 2
 *              DWT_COMP3       Comparator Number 3
 *
 *          address2 is the second address to be used for data address matching
 *          addressMask2 is the mask going to be applied to the second address 
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
int setDataWatchpoint_MatchingTwoComparator(uint32_t dwtCompNumber1,uint32_t address1,Watchpoint_AddressMask addressMask1,
                                            uint32_t dwtCompNumber2,uint32_t address2,Watchpoint_AddressMask addressMask2,
                                            uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode)
{
  int result = 0 ;
  int matchingCompNumber1 = 0, matchingCompNumber2 = 0 ;
  uint32_t configData = 0 ;
  
  matchingCompNumber1 = getDWTComparatorNumber(dwtCompNumber1);
  matchingCompNumber2 = getDWTComparatorNumber(dwtCompNumber2);
  
  if(dwtCompNumber1 == DWT_COMP1 || dwtCompNumber2 == DWT_COMP1)
    return -1 ;
  
  if(matchingCompNumber1 == -1 || matchingCompNumber2 == -1  )
    return -1 ;

  configData = (matchingCompNumber2 << 16) + (matchingCompNumber1 << 12) + (dataSize << 10) + (DATA_COMPARISON << 8) + accessMode ;
 
  setAddressWatchpoint(dwtCompNumber1,address1,addressMask1,DISABLE_DWTCOMPARATOR);
  setAddressWatchpoint(dwtCompNumber2,address2,addressMask2,DISABLE_DWTCOMPARATOR);
  
  memoryWriteWord(DWT_FUNC1,DISABLE_DWTCOMPARATOR); //disable selected comparator first
  
  memoryWriteWord(DWT_COMP1,matchedData);
  memoryWriteWord(DWT_MASK1,WATCHPOINT_MASK_NOTHING);
  memoryWriteWord(DWT_FUNC1,configData);
  
  return result ;
}

/**
 *  Check if data watchpoint occur
 *
 *  Output :  return 1 for true
 *            return 0 for false
 */
int isDataWatchpointOccurred()
{
  uint32_t dataRead = 0 ;
  
  if(isDWTTrapDebugEventOccured())
    memoryReadWord(DWT_FUNC1,&dataRead);
  
  dataRead = (dataRead & DWT_MATCHED_MASK) >> 24 ;
  return (int)dataRead ;
}

/**
 *  Check if address watchpoint occur
 *
 *  Output :  return 1 for true
 *            return 0 for false
 *            return -1 if invalid comparator is selected 
 */
int isAddressWatchpointOccurred(uint32_t dwtCompNumber)
{
  uint32_t dataRead = 0 ;
  uint32_t dwtFuncReg = dwtCompNumber + 8 ;
  
  if(getDWTComparatorNumber(dwtCompNumber) == -1)
    return -1 ;
  
  if(isDWTTrapDebugEventOccured())
    memoryReadWord(dwtFuncReg,&dataRead);
  
  dataRead = (dataRead & DWT_MATCHED_MASK) >> 24 ;
  
  return (int)dataRead ;
}

/**
 *  Disable the selected DWT Comparator
 *
 *  Input : dwtCompNumber is the comparator going to be used 
 *          Possible value :
 *              DWT_COMP0       Comparator Number 0
 *              DWT_COMP1       Comparator Number 1
 *              DWT_COMP2       Comparator Number 2
 *              DWT_COMP3       Comparator Number 3
 *
 *  Output :  return 0 if the comparator is disabled
 *            return -1 if invalid comparator is selected 
 */
int disableDWTComparator(uint32_t dwtCompNumber)
{
  uint32_t dwtFuncReg = dwtCompNumber + 8 ;
    
  if(getDWTComparatorNumber(dwtCompNumber) == -1)
    return -1 ;
  
  memoryWriteWord(dwtFuncReg,DISABLE_DWTCOMPARATOR);
  
  return 0 ;
}

/**
 *  Disable all DWT Comparator
 *
 */
void disableAllDWTComparators()
{ 
  memoryWriteWord(DWT_FUNC0,DISABLE_DWTCOMPARATOR);
  memoryWriteWord(DWT_FUNC1,DISABLE_DWTCOMPARATOR);
  memoryWriteWord(DWT_FUNC2,DISABLE_DWTCOMPARATOR);
  memoryWriteWord(DWT_FUNC3,DISABLE_DWTCOMPARATOR);
}

