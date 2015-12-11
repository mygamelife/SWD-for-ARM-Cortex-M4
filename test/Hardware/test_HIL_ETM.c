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