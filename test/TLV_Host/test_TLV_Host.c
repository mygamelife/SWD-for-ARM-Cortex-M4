#include "unity.h"
#include "TLV_Host.h"
#include "TLV_Protocol.h"
#include "mock_Serial.h"
#include "GetHeaders.h"
#include "Read_File.h"
#include "CException.h"
#include "ErrorCode.h"
#include "ProgramElf.h"
#include <stdlib.h>

void setUp(void)  {
  initElfData();
}

void tearDown(void) {}

void test_tlvHost_given_TLV_START_should_create_packet_and_pack_into_Tx_Buffer_and_change_state_to_TLV_TRANSMIT_DATA()
{
  HANDLE hSerial;
  TLVSession tlvSession;
  
  /* Open elf file */
  pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  /* Initialize TlvSeesion structure */
  tlvSession.state = TLV_START;
  tlvSession.pElf = pElfSection;
  tlvSession.hSerial = hSerial;
  
  /* Function call */
  tlvHost(&tlvSession);
  
  TEST_ASSERT_EQUAL(tlvSession.state, TLV_TRANSMIT_DATA);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
  free(dataFromElf);
}

void test_tlvHost_when_state_changed_to_TLV_TRANSMIT_DATA_should_transmit_all_the_data_inside_txBUFFER_to_probe()
{
  HANDLE hSerial;
  TLVSession tlvSession;
  
  /* Open elf file */
  pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  /* Initialize TlvSeesion structure */
  tlvSession.state = TLV_START;
  tlvSession.pElf = pElfSection;
  tlvSession.hSerial = hSerial;
  
  /* Mock */
  serialWriteByte_IgnoreAndReturn(255);
  
  /* Function call */
  tlvHost(&tlvSession);
  tlvHost(&tlvSession);
  
  /* Test */
  TEST_ASSERT_EQUAL(tlvSession.state, TLV_WAIT_REPLY);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
  free(dataFromElf);
}

void test_tlvHost_when_state_changed_to_TLV_WAIT_REPLY_should_transmit_all_txbuffer_data_and_wait_probe_reply()
{
  HANDLE hSerial;
  TLVSession tlvSession;
  
  /* Open elf file */
  pElfSection = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  /* Initialize TlvSeesion structure */
  tlvSession.state = TLV_START;
  tlvSession.pElf = pElfSection;
  tlvSession.hSerial = hSerial;
  
  /* Mock */
  serialWriteByte_IgnoreAndReturn(255);
  serialGetByte_ExpectAndReturn(tlvSession.hSerial, PROBE_OK);
  
  /* Function call */
  tlvHost(&tlvSession);
  tlvHost(&tlvSession);
  tlvHost(&tlvSession);
  
  /* Test */
  TEST_ASSERT_EQUAL(tlvSession.state, TLV_START);
  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
  free(dataFromElf);
}
