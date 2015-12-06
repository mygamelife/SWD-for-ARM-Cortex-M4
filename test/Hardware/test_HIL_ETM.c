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
//#include "ETM.h"
#include "ETMEx.h"

static int initFlag = 0;

void setUp(void)
{
  if(_session == NULL)
	  _session = tlvCreateSession();
}

void tearDown(void)
{
}

void test_read_ETMIDR_should_return_ID()
{
  uint32_t id = 0 ;
  
  memoryReadWord((uint32_t)&(ETM->ETMIDR),&id);
  
  printf("ID : %x\n",id);
}
