#include "ETM_M4.h"

uint32_t maxETMFIFOSize = 0 ;

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
  
  memoryReadWord((uint32_t)&(ETM->ETMCR),&data);
  data += ETM_ETMCR_ETMPROGBIT_Msk ;
  memoryWriteWord((uint32_t)&(ETM->ETMCR),data);
  
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
  
  memoryReadWord((uint32_t)&(ETM->ETMCR),&data);
  data -= ETM_ETMCR_ETMPROGBIT_Msk ;
  memoryWriteWord((uint32_t)&(ETM->ETMCR),data);
  
  while(isETMProgrammingBitSet()); 
}

/**
 *  Obtain the size of FIFO implemented for ETM and update global variable maxETMFIFOSize
 *	
 */
void getETMFIFOSize()
{
  maxETMFIFOSize = 0 ;
  
  memoryWriteWord((uint32_t)&(ETM->ETMFFLR),0xFFFFFFFF);
  memoryReadWord((uint32_t)&(ETM->ETMFFLR),&maxETMFIFOSize);
  printf("ETM FIFO Size : %d bytes\n",maxETMFIFOSize);
}

/**
 *  Select the size of the FIFO when dropped below the specified number of bytes will assert the FIFOFULL signal
 *
 *  Input   :	numberOfBytes is to specify the number of bytes left in the FIFO  to assert signal FIFOFULL
 */
void setFIFOFullSize(int numberOfBytes)
{
  if(maxETMFIFOSize == 0)
    getETMFIFOSize();
  
  if(numberOfBytes >= maxETMFIFOSize)
    memoryWriteWord((uint32_t)&(ETM->ETMFFLR),BACKUP_FIFOFULL_SIZE);
  else
    memoryWriteWord((uint32_t)&(ETM->ETMFFLR),numberOfBytes);
}

/**
 *  Check whether the ETM support Cycle Accurate Tracing
 *
 *  Output  : return 1 if supported
 *            return 0 if not supported
 */
int checkCycleAccurateTracingSupport()
{
  uint32_t dataRead = 0 ;
  
  setETMProgrammingBit();
  memoryReadWord((uint32_t)&(ETM->ETMCR),&dataRead);
  memoryWriteWord((uint32_t)&(ETM->ETMCR), (dataRead +ETM_ETMCR_CYCLEACCURATE_TRACING_Msk));
  memoryReadWord((uint32_t)&(ETM->ETMCR),&dataRead);
  
  return (dataRead & ETM_ETMCR_CYCLEACCURATE_TRACING_Msk) >> ETM_ETMCR_CYCLEACCURATE_TRACING_Pos ;
}

/**
 *  Configure ETM main control register to enable/disable different options
 *
 *  Input :  timestampEnable is used to enable/disable timestamping
 *				   Possible value :
 *					    DISABLE_TIMESTAMPING            disable timestamping
 *					    ENABLE_TIMESTAMPING             enable timestamping
 *
 *          branchAllEnable is used to enable/disable output of branching of all address
 *				   Possible value : 
 *					    DISABLE_BRANCH_ALL_ADDRESS      disable output of all branch all address
 *              ENABLE_BRANCH_ALL_ADDRESS       enable output of all branch all address 
 *
 *          stallProcessorEnable is used to enable/disable stalling of the processor if FIFO is full.
 *				   Possible value : 
 *              DISABLE_STALLING_PROCESSOR      disable stalling of processor (may cause FIFO to overflow)
 *              ENABLE_STALLING_PROCESSOR       enable stalling of processor to prevent overflow (may affect the performance of the system)
 *  
 *  Output : return 1 if configuration is successful
 *           return 0 if configuration is unsuccessful
 *
 */
int configureETMMainControl(int timestampEnable,int branchAllEnable,int stallProcessorEnable)
{
  uint32_t dataRead = 0, checkDataWrote = 0 ;
  
  setETMProgrammingBit();
  memoryReadWord((uint32_t)&(ETM->ETMCR),&dataRead);
  
  dataRead += (timestampEnable << ETM_ETMCR_TIMESTAMP_ENABLE_Pos) +  (branchAllEnable << ETM_ETMCR_BRANCH_OUTPUT_Pos) + (stallProcessorEnable << ETM_ETMCR_STALL_PROCESSOR_Pos) + ETM_ETMCR_ETMEN_Msk ;
  
  memoryWriteWord((uint32_t)&(ETM->ETMCR), dataRead);
  
  memoryReadWord((uint32_t)&(ETM->ETMCR),&checkDataWrote);
  
  if(dataRead == checkDataWrote)
    return 1 ;
  else
    return 0 ;
}


/**
 *  Configure start/stop logic to assert TraceEnable signal with conditions (Used to control when to perform instruction tracing)
 *  Note : The behavior of the trace start/stop block is UNPREDICTABLE if the same EmbeddedICE watchpoint input is used as both
 *         the start input and the stop input to the block.
 *
 *  Input :  traceStartStopLogicEnable is used to control the trace start/stop logic block
 *				   Possible value :
 *					    DISABLE_TRACESTARTSTOP_LOGIC	          Trace start/stop logic block disable
 *					    ENABLE_TRACESTARTSTOP_LOGIC	            Trace start/stop logic block enable and tracing is affected by start and stop EmbeddedICE watchpoint input
 *
 *          startResource is used to select the EmbeddedICE watchpoint input as trace start/stop logic block start resource
 *				   Possible value : 
 *					    SELECT_NONE	                            Select none
 *              RESOURCE_1                              Select EmbeddedICE watchpoint input 1 
 *              RESOURCE_2                              Select EmbeddedICE watchpoint input 2 
 *              RESOURCE_3                              Select EmbeddedICE watchpoint input 3 
 *              RESOURCE_4                              Select EmbeddedICE watchpoint input 4 
 *              ALL_RESOURCES                           Select all EmbeddedICE watchpoint input
 *              RESOURCE_1 + RESOURCE_2                 Select EmbeddedICE watchpoint input 1 and EmbeddedICE watchpoint input 2 
 *              RESOURCE_1 + RESOURCE_2 + RESOURCE_3    Select EmbeddedICE watchpoint input 1, EmbeddedICE watchpoint input 2 and EmbeddedICE watchpoint input 3 
 *                      "                                                           "
 *                          
 *
 *          stopResource is used to select the EmbeddedICE watchpoint input as trace start/stop logic block stop resource (Refer to possible value for startResource) 
 *              
 */
void configureTraceStartStopLogic(int traceStartStopLogicEnable,ResourceSelection startResource,ResourceSelection stopResource)
{
  memoryWriteWord((uint32_t)&(ETM->ETMTECR1),(traceStartStopLogicEnable << ETM_ETMTECR1_TRACECONTROL_ENABLE_Pos));
  memoryWriteWord((uint32_t)&(ETM->ETMTESSEICR),(startResource + (stopResource <<ETM_ETMTESSEICR_STOP_RESOURCE_Pos))) ;
}

/**
 *  Configure TraceEnable enabling Event to assert TraceEnable signal for tracing (Used to control when to perform instruction tracing)
 *  
 *  Sample configuration for always true event , configureTraceEnableEnablingEvent(A,HARD_WIRED_INPUT,(any of the resources))
 *
 *  Input :  function is used the boolean logic between Resource A and B that will set logical true for the event
 *				   Possible value :
 *              A                     True when Resource A is active
 *              NOT_A                 True when Resource A is inactive
 *              A_AND_B               True when Resource A and B are both active
 *              NOT_A_AND_B           True when Resource A is inactive while Resource B is active
 *              NOT_A_AND_NOT_B       True when Resource A and B are both inactive
 *              A_OR_B                True when either Resource A or B is active or both resources are active
 *              NOT_A_OR_B            True when either Resource A is inactive or Resource B is active 
 *              NOT_A_OR_NOT_B        True when either Resource A is inactive or Resource B is inactive
 *
 *          ResourceA is used to select the resource that will generate logical TRUE signal when it is active
 *				  Possible value : 
 *              SELECT_NORESOURCE                        
 *              WATCHPOINT_COMPARATOR_1
 *              WATCHPOINT_COMPARATOR_2
 *              WATCHPOINT_COMPARATOR_3
 *              WATCHPOINT_COMPARATOR_4
 *              COUNTER_1                          TRUE when Counter 1 reaches zero
 *              TRACE_STARTSTOP_RESOUCE
 *              EXTERNAL_INPUT_1
 *              EXTERNAL_INPUT_2
 *              HARD_WIRED_INPUT                   Always logical TRUE
 *
 *          ResourceB is used to select the resource that will generate logical TRUE signal when it is active (Refer to possible value for ResourceA) 
 *              
 */
void configureTraceEnableEnablingEvent(ETMEvent_FunctionEncoding function,ETMEvent_Resources resourceA,ETMEvent_Resources resourceB)
{
  memoryWriteWord((uint32_t)&(ETM->ETMTEEVR), ((function << ETM_ETMTEEVR_BOOLEANFUNCTION_Pos) + (resourceB << ETM_ETMTEEVR_RESOURCE_B_Pos) + resourceA));
}

/**
 *  Configure ETM Trigger Event (Used to control when to trigger to trace at your interest)
 *
 *  (Refer to configureTraceEnableEnablingEvent for input parameters description)
 */
void configureETMTriggerEvent(ETMEvent_FunctionEncoding function,ETMEvent_Resources resourceA,ETMEvent_Resources resourceB)
{
  memoryWriteWord((uint32_t)&(ETM->ETMTRIGGER), ((function << ETM_ETMTRIGGER_BOOLEANFUNCTION_Pos) + (resourceB << ETM_ETMTRIGGER_RESOURCE_B_Pos) + resourceA));
}

/**
 *  Configure the event that cause the insertion of timestamp into trace stream
 *  
 *  Do not set to always TRUE (HARD_WIRED_INPUT) as it will probably cause FIFO to overflow, 
 *  recommended to use counter 1 to insert timestamp into trace periodically
 *
 *
 *  (Refer to configureTraceEnableEnablingEvent for input parameters description)
 */
void configureTimeStampInsertionEvent(ETMEvent_FunctionEncoding function,ETMEvent_Resources resourceA,ETMEvent_Resources resourceB)
{
  memoryWriteWord((uint32_t)&(ETM->ETMTSEVR), ((function << ETM_ETMTSEVR_BOOLEANFUNCTION_Pos) + (resourceB << ETM_ETMTSEVR_RESOURCE_B_Pos) + resourceA));
}

/**
 *  Set the reload value for Counter 1 when it decrements to 0
 *
 *  Input : reloadValue is the value to be reloaded when counter 1 reaches zero (16bits / 2bytes / Halfword only)
 */
void setReducedFunctionCounterReloadValue(uint16_t reloadValue)
{
  memoryWriteWord((uint32_t)&(ETM->ETMCNTRLDVR[0]),reloadValue);
}

/**
 *  Setup ETM and start tracing
 *  
 */
void setupETMandStartTracing()
{
  powerUpETM();
  
  configureETMMainControl(ENABLE_TIMESTAMPING,ENABLE_BRANCH_ALL_ADDRESS,ENABLE_STALLING_PROCESSOR);
  //Configure Trigger and Trace event
  configureETMTriggerEvent(A,HARD_WIRED_INPUT,SELECT_NORESOURCE);
  configureTraceEnableEnablingEvent(A,HARD_WIRED_INPUT,SELECT_NORESOURCE);
  configureTraceStartStopLogic(DISABLE_TRACESTARTSTOP_LOGIC,SELECT_NONE,SELECT_NONE);
  configureTimeStampInsertionEvent(A,COUNTER_1,SELECT_NORESOURCE);
  
  setFIFOFullSize(BACKUP_FIFOFULL_SIZE);
  
  setReducedFunctionCounterReloadValue(1000);
  setETMTraceID();
  
  clearETMProgrammingBit();
}