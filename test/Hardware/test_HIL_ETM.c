#include "unity.h"
#include "Tlv.h"
#include "Uart.h"
#include "TlvEx.h"
#include "SystemTime.h"
#include "ErrorCode.h"
#include "Read_File.h"
#include "GetHeaders.h"
#include "Interface.h"
#include "ProgramElf.h"
#include "ProgramLoader.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "StringObject.h"
#include "Token.h"
#include "IdentifierToken.h"
#include "NumberToken.h"
#include "OperatorToken.h"
#include "FileToken.h"
#include "MemoryReadWrite.h"
#include "ETM_M4.h"
#include "ETMEx.h"
#include "Yield.h"
#include "LoadElf.h"
#include "ProgramVerifier.h"

static int initFlag = 0;

void setUp(void)
{
  if(_session == NULL)
	  _session = tlvCreateSession();
}

void tearDown(void)
{
}

/*----------unlockETM------------*/
void test_unlockETM_should_get_0x1_when_reading_from_ETMLSR()
{
  uint32_t etmlsr = 0 ;
  
  unlockETM();
  memoryReadWord((uint32_t)&(ETM->ETMLSR),&etmlsr);
  
  TEST_ASSERT_EQUAL(0x1,etmlsr);
}

/*----------lockETM------------*/
void test_lockETM_should_get_0x3_when_reading_from_ETMLSR()
{ 
  uint32_t etmlsr = 0 ;
  
  lockETM();
  memoryReadWord((uint32_t)&(ETM->ETMLSR),&etmlsr);
  
  TEST_ASSERT_EQUAL(0x3,etmlsr);
}

/*----------isETMLocked------------*/
void test_isETMLocked_should_return_1_if_locked()
{
  lockETM();
  TEST_ASSERT_EQUAL(1,isETMLocked());
}

void test_isETMLocked_should_return_0_if_not_locked()
{
  unlockETM();
  TEST_ASSERT_EQUAL(0,isETMLocked());
}

/*----------getETMID------------*/
//CORESIGHT ETM M4 0x4114F250
void test_getETMID_should_return_etm_id_of_the_device()
{
  TEST_ASSERT_EQUAL(0x4114F250,getETMID());
}

/*------------isETMPoweredUp----------*/
void test_isETMPoweredUp_should_return_1_if_powered_up()
{
  memoryWriteByte((uint32_t)&(ETM->ETMCR),0);
  
  TEST_ASSERT_EQUAL(1,isETMPoweredUp());
}

void test_isETMPoweredUp_should_return_0_if_powered_down()
{
  memoryWriteByte((uint32_t)&(ETM->ETMCR),1);
  
  TEST_ASSERT_EQUAL(0,isETMPoweredUp());
}

/*---------isETMProgrammingBitSet------*/
void test_isETMProgrammingBitSet_should_return_1_if_set()
{
  memoryWriteWord((uint32_t)&(ETM->ETMCR),0x141E);
  
  TEST_ASSERT_EQUAL(1,isETMProgrammingBitSet());
}

void test_isETMProgrammingBitSet_should_return_0_if_cleared()
{
  memoryWriteWord((uint32_t)&(ETM->ETMCR),0x101E);
  
  TEST_ASSERT_EQUAL(0,isETMProgrammingBitSet());
}

/*---------powerUpETM------*/
void test_powerUpETM_should_powerUpETM()
{
  memoryWriteByte((uint32_t)&(ETM->ETMCR),1);
  TEST_ASSERT_EQUAL(0,isETMPoweredUp());
  
  powerUpETM();
  TEST_ASSERT_EQUAL(1,isETMPoweredUp());
}

/*---------powerDownETM------*/
void test_powerDownETM_should_powerDownETM()
{
  memoryWriteByte((uint32_t)&(ETM->ETMCR),0);
  TEST_ASSERT_EQUAL(1,isETMPoweredUp());
  
  powerDownETM();
  TEST_ASSERT_EQUAL(0,isETMPoweredUp());
}

/*---------getETMConfiguration------*/
void test_getETMConfiguration_should_printf_ETM_configuration()
{
  getETMConfiguration();
}

/*--------------configureTraceStartStopLogic------*/
void test_configureTraceStartStopLogic_should_configure_ETMTECR1_and_ETMTESSEICR()
{
  uint32_t etmecr1 = 0 , etmtesseicr = 0 ;
  powerUpETM();

  configureTraceStartStopLogic(DISABLE_TRACESTARTSTOP_LOGIC,SELECT_NONE,SELECT_NONE);
  memoryReadWord((uint32_t)&(ETM->ETMTECR1),&etmecr1);
  memoryReadWord((uint32_t)&(ETM->ETMTESSEICR),&etmtesseicr);
  
  TEST_ASSERT_EQUAL(0,etmecr1);
  TEST_ASSERT_EQUAL(0,etmtesseicr);
 
  configureTraceStartStopLogic(ENABLE_TRACESTARTSTOP_LOGIC,RESOURCE_1,RESOURCE_4);
  memoryReadWord((uint32_t)&(ETM->ETMTECR1),&etmecr1);
  memoryReadWord((uint32_t)&(ETM->ETMTESSEICR),&etmtesseicr);
  
  TEST_ASSERT_EQUAL(0x2000000,etmecr1);
  TEST_ASSERT_EQUAL(0x80001,etmtesseicr);
}

/*--------------configureTraceEnableEnablingEvent------*/
void test_configureTraceEnableEnablingEvent_should_configure_etmeevr()
{
  uint32_t etmteevr = 0 ;
  powerUpETM();
  
  configureTraceEnableEnablingEvent(A,HARD_WIRED_INPUT,HARD_WIRED_INPUT);
  memoryReadWord((uint32_t)&(ETM->ETMTEEVR),&etmteevr);
  //Resource A[6:0], Resource B[13:7], Function[16:14]
  //A                0b000
  //HARD_WIRED_INPUT 0b110 1111
  //0 0011 0111 1110 1111
  TEST_ASSERT_EQUAL(0x37EF,etmteevr);
  
  configureTraceEnableEnablingEvent(NOT_A_OR_NOT_B,COUNTER_1,HARD_WIRED_INPUT);
  memoryReadWord((uint32_t)&(ETM->ETMTEEVR),&etmteevr);
  //Resource A[6:0], Resource B[13:7], Function[17:14]
  //NOT_A_OR_NOT_B  0b111
  //Counter_1       0b100 0000
  //1 1111 0111 1100 000
  TEST_ASSERT_EQUAL(0x1F7A0,etmteevr);
  
  /*
    This test failed as both of the resources is not supported for STM32F429
    
    In ETMv3.5, if an invalid resource is programmed, such as one that is architecturally Reserved or a resource that is
    not supported by the specific implementation, the read value returned is UNKNOWN and the behavior of the event is
    UNPREDICTABLE.
  
    configureTraceEnableEnablingEvent(A_OR_B,TRACE_PROHIBITED,PROCESSOR_IN_NON_SECURE_STATE);
    memoryReadWord((uint32_t)&(ETM->ETMTEEVR),&etmteevr);
    //Resource A[6:0], Resource B[13:7], Function[17:14]
    //A_OR_B                          0b101
    //TRACE_PROHIBITED                0b110 1110
    //PROCESSOR_IN_NON_SECURE_STATE   0b110 1101
    //1 0111 0110 1110 1110
    TEST_ASSERT_EQUAL(0x176EE,etmteevr);
  */
}