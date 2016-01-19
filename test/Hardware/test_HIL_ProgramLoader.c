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
#include "SystemPath.h"
#include "ProgramVerifier.h"

static Tlv_Session *session = NULL;

void setUp(void) {
  if(session == NULL)
    session = tlvCreateSession();
}

void tearDown(void) {}

void test_read_write_register_command(void) {
  CEXCEPTION_T err;
  uint32_t result = 0;
  
  Try {
    while(writeRegister(session, R0, 0xDEADBEEF) != PROCESS_DONE)
    { tlvService(session); }
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  Try {
    while((result = readRegister(session, R0)) == 0)
    { tlvService(session); }
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, result);
}

void test_read_write_memory_command(void) {
  CEXCEPTION_T err;
  uint8_t *dataBlock = NULL;
  
  uint8_t data[] = {  0x78, 0x56, 0x34, 0x12, //0x12345678
                      0xad, 0xde, 0xaf, 0xde, //0xdeafdead
                      0xef, 0xbe, 0xce, 0xfa, //0xfacebeef
                      0xdd, 0xcc, 0xbb, 0xaa, //0xAABBCCDD
                   };
  
  Try {
    while(writeMemory(session, data, 0x20000000, 16, TLV_WRITE_RAM) != PROCESS_DONE)
    { tlvService(session); }
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  Try {
    while((dataBlock = readMemory(session, 0x20000000, 16)) == NULL)
    { tlvService(session); }
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  TEST_ASSERT_NOT_NULL(dataBlock);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, get4Byte(&dataBlock[0]));
  TEST_ASSERT_EQUAL_HEX32(0xdeafdead, get4Byte(&dataBlock[4]));
  TEST_ASSERT_EQUAL_HEX32(0xfacebeef, get4Byte(&dataBlock[8]));
  TEST_ASSERT_EQUAL_HEX32(0xaabbccdd, get4Byte(&dataBlock[12]));
  
  delDataBlock(dataBlock);
}

void test_loadRam_should_load_program_into_target_SRAM_and_run(void) {
  CEXCEPTION_T err;
  int result = 0;
  Program *p = getLoadableSection("test/ElfFiles/ledRam.elf");

  /* #################### Load Program Into Ram #################### */
  Try {
    while(loadRam(session, p) != PROCESS_DONE)
    { tlvService(session); }
  } Catch(err) {
    displayErrorMessage(err);
  }

  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));

  /* #################### Verify is loaded program is correct #################### */
  Try {
    while((result = isProgramExist(session, p)) == 0)
    { tlvService(session); }
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(VERIFY_PASSED, result);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));

  delProgram(p);
  
  /* #################### Hard reset should remove program in Ram #################### */
  Try {
    while(hardReset(session) != PROCESS_DONE)
    { tlvService(session); }
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  TEST_ASSERT_EQUAL(0, isYielding);
}

void test_loadFlash_should_load_program_into_target_flash(void) {
  CEXCEPTION_T err;
  int status = 0;
  Program *p = getLoadableSection("test/ElfFiles/ledFlash.elf");
  
  /* #################### Load Program Into Flash #################### */
  while(status == PROCESS_BUSY)
  { 
    Try {
      tlvService(session); 
      status = loadFlash(session, p);
    } Catch(err) {
      displayErrorMessage(err);
      break;
    }
  }
  status = 0;
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  /* #################### Verify is loaded program is correct #################### */
  while(status == PROCESS_BUSY)
  { 
    Try {
      tlvService(session); 
      status = isProgramExist(session, p);
    } Catch(err) {
      displayErrorMessage(err);
      break;
    }
  }
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(VERIFY_PASSED, status);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  status = 0;
  delProgram(p);
  /* #################### Erase target flash should remove the loaded program #################### */
  while(status == PROCESS_BUSY)
  { 
    Try {
      tlvService(session); 
      status = eraseSection(session, 0x08000000, 10000);
    } Catch(err) {
      displayErrorMessage(err);
      break;
    }
  }
  status = 0;
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  /* #################### Hard reset target #################### */
  while(status == PROCESS_BUSY)
  { 
    Try {
      tlvService(session); 
      status = hardReset(session);
    } Catch(err) {
      displayErrorMessage(err);
      break;
    }
  }
  
  TEST_ASSERT_EQUAL(0, isYielding);
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_TRANSMIT_FLAG));
  TEST_ASSERT_EQUAL(FLAG_CLEAR, GET_FLAG_STATUS(session, TLV_DATA_RECEIVE_FLAG));
  
  closePort(session);
}