#include "unity.h"
#include <string.h>
#include <malloc.h>
#include "Tlv.h"
#include "TlvEx.h"
#include "Read_File.h"
#include "GetHeaders.h"
#include "ProgramElf.h"
#include "ProgramLoader.h"
#include "ErrorCode.h"
#include "CException.h"
#include "CustomAssertion.h"
#include "SystemTime.h"
#include "mock_Uart.h"
#include "mock_Interface.h"

void setUp(void)  {}

void tearDown(void) {}

void test__tlvWriteToFlash_should_erase_section_before_writing_into_flash(void)
{
  uartInit_Ignore();
	Tlv_Session *session = tlvCreateSession();
  
  /* 0xDEADBEEF*/
  uint8_t dataAddress[] = {0xEF, 0xBE, 0xAD, 0xDE};
  
  _tlvWriteToFlash(session, dataAddress, 0x08000000, 4);
}