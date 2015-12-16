#include "unity.h"
#include "Yield.h"
#include "Tlv.h"
#include "TlvEx.h"
#include "Uart.h"
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
#include "LoadElf.h"
#include "ProgramVerifier.h"
#include "MemoryReadWrite.h"
#include "CoreDebug.h"
#include "CoreDebugEx.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Itm.h"
#include "Tpiu.h"

static int flag = 0;

void setUp(void) {
  CEXCEPTION_T err;
  
  Try {
    if(flag == 0) {
      flag = 1;
      initMemoryReadWrite();
      itmInit();
    }    
  } Catch(err) {
    displayErrorMessage(err);
  }
}

void tearDown(void) {}

void test_ITM_configure_and_write_data_into_simulus_register_0(void) {
  uint32_t data = 0;
  
  memoryWriteWord((uint32_t)&ITM->PORT[0].u32, 0xDEADBEEF);
  // memoryWriteWord((uint32_t)&ITM->PORT[0].u8, 0xF0);

  // memoryReadWord((uint32_t)&ITM->PORT[0].u32, &data);
  // printf("data 0 %x\n", data);
  // memoryReadWord((uint32_t)&ITM->PORT[1].u32, &data);
  // printf("data 1 %x\n", data);
}