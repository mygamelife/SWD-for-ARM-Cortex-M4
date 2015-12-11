#include "ETM_M4.h"

/**
 *  Read and return ETM ID which holds the ETM architecture variant
 *
 *  Output : return ETM ID of the device
 *           return 0 if ETM is not present
 */
uint32_t getETMID()
{
  uint32_t etmid = 0;
  
  memoryReadWord((uint32_t)&(ETM->ETMIDR),&etmid);
  
  return etmid ;
}

/**
 *  Get the implementation specific ETM configuration
 */
void getETMConfiguration()
{
  uint32_t etmccr =  0, etmccer = 0;
  
  int etmidrPresent = 0 ,traceblockPresent = 0,fifofullPresent =  0,sequencerPresent = 0 ;
  int numContextID = 0 ,numExternalOut = 0, numExternalIn = 0, numCounter = 0, numMemoryMapDecoder = 0,
      numDataValueComparator = 0, numAddressComparatorPair = 0 ;
  
  int timestampSize = 0, timestampEncoding = 0, reducedCounter = 0, virtualizationExtension = 0, timestampImplemented = 0,
      etmeibcr = 0, traceUseWatchpointInput = 0, numWatchpointInput = 0, numInstrumentationResources = 0,
      dataAddressComparison = 0, allRegReadable = 0, sizeExtendedExternalInput = 0, numExtendedExternalInputSel = 0 ;
  
  memoryReadWord((uint32_t)&(ETM->ETMCCR),&etmccr);
  memoryReadWord((uint32_t)&(ETM->ETMCCER),&etmccer);
  
  etmidrPresent = (etmccr & ETM_ETMCCR_ETMIDR_PRESENT_Msk) >> ETM_ETMCCR_ETMIDR_PRESENT_Pos ;
  traceblockPresent = (etmccr & ETM_ETMCCR_TRACE_STARTSTOP_BLOCK_PRESENT_Msk ) >> ETM_ETMCCR_TRACE_STARTSTOP_BLOCK_PRESENT_Pos ;
  fifofullPresent = (etmccr & ETM_ETMCCR_FIFOFULL_LOGIC_PRESENT_Msk ) >> ETM_ETMCCR_FIFOFULL_LOGIC_PRESENT_Pos ;
  sequencerPresent = (etmccr & ETM_ETMCCR_SEQUENCER_PRESENT_Msk ) >> ETM_ETMCCR_SEQUENCER_PRESENT_Pos ;
  numContextID = (etmccr & ETM_ETMCCR_NUMBER_OF_CONTEXTID_COMPARATOR_Msk ) >> ETM_ETMCCR_NUMBER_OF_CONTEXTID_COMPARATOR_Pos ;
  numExternalOut = (etmccr & ETM_ETMCCR_NUMBER_OF_EXTERNAL_OUTPUTS_Msk ) >> ETM_ETMCCR_NUMBER_OF_EXTERNAL_OUTPUTS_Pos ;
  numExternalIn = (etmccr & ETM_ETMCCR_NUMBER_OF_EXTERNAL_INPUTS_Msk ) >> ETM_ETMCCR_NUMBER_OF_EXTERNAL_INPUTS_Pos ;
  numCounter = (etmccr & ETM_ETMCCR_NUMBER_OF_COUNTERS_Msk ) >> ETM_ETMCCR_NUMBER_OF_COUNTERS_Pos ;
  numMemoryMapDecoder = (etmccr & ETM_ETMCCR_NUMBER_OF_MEMORYMAP_DECODERS_Msk ) >> ETM_ETMCCR_NUMBER_OF_MEMORYMAP_DECODERS_Pos ;
  numDataValueComparator = (etmccr & ETM_ETMCCR_NUMBER_OF_DATAVALUE_COMPARATOR_Msk ) >> ETM_ETMCCR_NUMBER_OF_DATAVALUE_COMPARATOR_Pos ;
  numAddressComparatorPair = (etmccr & ETM_ETMCCR_NUMBER_OF_ADDRESS_COMPARATOR_PAIRS_Msk ) >> ETM_ETMCCR_NUMBER_OF_ADDRESS_COMPARATOR_PAIRS_Pos ;
  
  timestampSize = (etmccer & ETM_ETMCCER_TIMESTAMP_PACKET_SIZE_Msk) >> ETM_ETMCCER_TIMESTAMP_PACKET_SIZE_Pos ;
  timestampEncoding = (etmccer & ETM_ETMCCER_TIMESTAMP_PACKET_ENCODING_Msk) >> ETM_ETMCCER_TIMESTAMP_PACKET_ENCODING_Pos ;
  reducedCounter = (etmccer & ETM_ETMCCER_REDUCED_FUNCTION_COUNTER_Msk) >> ETM_ETMCCER_REDUCED_FUNCTION_COUNTER_Pos ;
  virtualizationExtension = (etmccer & ETM_ETMCCER_VIRTUALIZATION_EXTERNSION_Msk) >> ETM_ETMCCER_VIRTUALIZATION_EXTERNSION_Pos ;
  timestampImplemented = (etmccer & ETM_ETMCCER_TIMESTAMPING_IMPLEMENTATION_Msk) >> ETM_ETMCCER_TIMESTAMPING_IMPLEMENTATION_Pos ;
  etmeibcr = (etmccer & ETM_ETMCCER_ETMEIBCR_IMPLEMENTED_Msk) >> ETM_ETMCCER_ETMEIBCR_IMPLEMENTED_Pos ;
  traceUseWatchpointInput = (etmccer & ETM_ETMCCER_TRACE_BLOCK_USE_EMBEDDEDICE_WATCHPOINT_INPUTS_Msk) >> ETM_ETMCCER_TRACE_BLOCK_USE_EMBEDDEDICE_WATCHPOINT_INPUTS_Pos ;
  numWatchpointInput = (etmccer & ETM_ETMCCER_NUMBER_OF_EMBEDDEDICE_WATCHPOINT_INPUTS_Msk) >> ETM_ETMCCER_NUMBER_OF_EMBEDDEDICE_WATCHPOINT_INPUTS_Pos ;
  numInstrumentationResources = (etmccer & ETM_ETMCCER_NUMBER_OF_INSTRUMENTATION_RESOURCES_Msk) >> ETM_ETMCCER_NUMBER_OF_INSTRUMENTATION_RESOURCES_Pos ;
  dataAddressComparison = (etmccer & ETM_ETMCCER_DATA_ADDRESS_COMPARISON_SUPPORT_Msk) >> ETM_ETMCCER_DATA_ADDRESS_COMPARISON_SUPPORT_Pos ;
  allRegReadable = (etmccer & ETM_ETMCCER_ALL_REGISTERS_READABLE_Msk) >> ETM_ETMCCER_ALL_REGISTERS_READABLE_Pos ;
  sizeExtendedExternalInput = (etmccer & ETM_ETMCCER_SIZE_OF_EXTENDED_EXTERNAL_INPUT_BUS_Msk) >> ETM_ETMCCER_SIZE_OF_EXTENDED_EXTERNAL_INPUT_BUS_Pos ;
  numExtendedExternalInputSel = (etmccer & ETM_ETMCCER_NUMBER_OF_EXTENDED_EXTERNAL_INPUT_SELECTORS_Msk) >> ETM_ETMCCER_NUMBER_OF_EXTENDED_EXTERNAL_INPUT_SELECTORS_Pos ;

  printf("\nETM Configuration Implemented\n");
  printf("-----------------------------\n");
  printf("ETM IDR present : %s\n", etmidrPresent ? "True":"False");
  printf("Trace start/stop block present : %s\n", traceblockPresent ? "True":"false");
  printf("FIFOFULL logic present : %s\n", fifofullPresent ? "True":"False");
  printf("Sequencer present : %s\n", sequencerPresent ? "True":"False");
  
  printf("Number of Context ID Comparators : %d\n",numContextID);
  printf("Number of External Outputs : %d\n",numExternalOut);
  printf("Number of External Inputs : %d\n",numExternalIn);
  printf("Number of Counters : %d\n",numCounter);
  printf("Number of Memory Map Decoders : %d\n",numMemoryMapDecoder);
  printf("Number of Data Value Comparators : %d\n",numDataValueComparator);
  printf("Number of Address Comparators Pairs : %d\n\n",numAddressComparatorPair);
  
  printf("Timestamping Implemented : %s\n",timestampImplemented ? "True":"False");
  printf("Timestamping Packet Size : %s\n",timestampSize ? "64bits":"48bits");
  printf("Timestamping Packet Encoding : %s\n",timestampEncoding ? "Natural binary":"Grey Code");
  printf("Reduced Function Counter : %s\n",reducedCounter ? "Counter 1 is implemented as a reduced function counter" : "All counters are implemented as full-function counters");
  printf("Virtualization Externsion Implemented : %s\n",virtualizationExtension ? "True":"False");
  printf("EmbeddedICE Behavior Control Register Implemented : %s\n",etmeibcr ? "True":"False");
  printf("Trace Start/Stop block can use EmbeddedICE Watchpoint Inputs : %s\n",traceUseWatchpointInput ?"True":"False");
  printf("Number of EmbeddedICE Watchpoint inputs Implemented : %d\n",numWatchpointInput);
  printf("Number of Instrumentation Resouces supported : %d\n",numInstrumentationResources);
  printf("Data address comparisons supported : %s\n",dataAddressComparison ? "False":"True");
  printf("All registers are readable : %s\n",allRegReadable ? "True":"False");
  printf("Size of extended external input bus : %d\n",sizeExtendedExternalInput);
  printf("Number of extended external input selector : %d\n",numExtendedExternalInputSel);
  
}
/**
 *  Check whether is ETM locked
 *
 *  Output :  return 1 if ETM is locked, writes are ignored
 *			      return 0if writes to ETM are permitted
 */
int isETMLocked()
{
  int result = -1 ;
  uint32_t dataRead = 0 ;
  
  memoryReadWord((uint32_t)&(ETM->ETMLSR),&dataRead);
  
  return (dataRead & ETM_ETMLSR_ETMLOCKEDUNLOCKED_Msk) >> ETM_ETMLSR_ETMLOCKEDUNLOCKED_Pos;
}

/**
 *  Check whether is ETM powered up
 *
 *  Output :  return 1 if ETM is powered up
 *			      return 0 if ETM is powered down
 */
int isETMPoweredUp()
{
  uint32_t dataRead = 0 ;
  
  memoryReadWord((uint32_t)&(ETM->ETMCR),&dataRead);
  
  return !(dataRead & ETM_ETMCR_ETMPOWERDOWN_Msk);
}

/**
 *  Check whether is the effective ETM programming bit in ETMSR set
 *
 *  Output :  return 1 if ETM programming bit is set
 *			      return 0 if ETM programming bit is cleared
 */
int isETMProgrammingBitSet()
{
  uint32_t dataRead = 0 ;
  
  memoryReadWord((uint32_t)&(ETM->ETMSR),&dataRead);
  
  return (dataRead & ETM_ETMSR_PROGBIT_Msk) >> ETM_ETMSR_PROGBIT_Pos;
}

/**
 *  Power up and enable ETM 
 */
void powerUpETM()
{
  uint32_t data = 0 ;
  
  if(isETMPoweredUp())
    return ;
  
  memoryReadByte((uint32_t)&(ETM->ETMCR),&data);
  data -= ETM_ETMCR_ETMPOWERDOWN_Msk ;
  memoryWriteByte((uint32_t)&(ETM->ETMCR),data);
}

/**
 *  Power down and disable ETM
 */
void powerDownETM()
{
  uint32_t data = 0 ;
  
  if(!isETMPoweredUp())
    return ;
  
  memoryReadByte((uint32_t)&(ETM->ETMCR),&data);
  data += ETM_ETMCR_ETMPOWERDOWN_Msk ;
  memoryWriteByte((uint32_t)&(ETM->ETMCR),data);
}

/**
 *  Set the Pogramming Bit of ETMCR and loop until it is set
 *
 */
void setETMProgrammingBit()
{
  uint32_t data = 0 ;
  
  if(isETMProgrammingBitSet())
    return ;
  
  memoryReadByte((uint32_t)&(ETM->ETMCR)+1,&data);
  data += ETM_ETMCR_ETMPROGBIT_Msk ;
  memoryWriteByte((uint32_t)&(ETM->ETMCR)+1,data);
  
  while(!isETMProgrammingBitSet()); 
}

/**
 *  Clear the Pogramming Bit of ETMCR and loop until it is cleared
 *
 */
void clearETMProgrammingBit()
{
  uint32_t data = 0 ;
  
  if(!isETMProgrammingBitSet())
    return ;
  
  memoryReadByte((uint32_t)&(ETM->ETMCR)+1,&data);
  data -= ETM_ETMCR_ETMPROGBIT_Msk ;
  memoryWriteByte((uint32_t)&(ETM->ETMCR)+1,data);
  
  while(isETMProgrammingBitSet()); 
}