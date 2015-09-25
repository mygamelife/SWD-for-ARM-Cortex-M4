#include "unity.h"
#include "Delay.h"
#include "swdStub.h"
#include "Emulator.h"
#include "mock_Flash.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "mock_Register_ReadWrite.h"
#include "mock_configurePort.h"
#include "mock_LowLevelIO.h"

void setUp(void)  {}

void tearDown(void) {}

void test_stubCopy_should_get_flash_sram_start_address_length_and_call_Flash_Copy_func()  {
  
  STUB = malloc(sizeof(Stub_Type));
  
  STUB->sramAddress = 0x20000000;
  STUB->flashAddress = ADDR_FLASH_SECTOR_22;
  STUB->dataSize = 2048;
  
  flashCopyFromSramToFlash_Expect(0x20000000, ADDR_FLASH_SECTOR_22, 2048);
  
  stubCopy();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, STUB->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, STUB->status);
  
  free(STUB);
}

void test_stubErase_should_get_flash_address_and_flash_size_and_call_flashErase_func(void) {
  
  STUB = malloc(sizeof(Stub_Type));
  
  STUB->flashAddress = ADDR_FLASH_SECTOR_12;
  STUB->dataSize = 16;
  
  flashErase_Expect(ADDR_FLASH_SECTOR_12, 16);
  
  stubErase();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, STUB->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, STUB->status);
  
  free(STUB);
}

void test_stubMassErase_should_get_bank_select_from_SRAM_and_call_flashMassErase_func()  {
  
  STUB = malloc(sizeof(Stub_Type));
  
  STUB->banks = FLASH_BANK_BOTH;
  
  flashMassErase_Expect(FLASH_BANK_BOTH);
  
  stubMassErase();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, STUB->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, STUB->status);
  
  free(STUB);
}

void test_svcServiceHandler_should_call_request_sram_address(void) {
  unsigned int svc_args[7] = {0};
  
  svc_args[0] = SVC_REQUEST_SRAM_ADDRESS;
  
  svcServiceHandler(svc_args);
  
  TEST_ASSERT_EQUAL_HEX32(0x20012800, svc_args[1]);
  TEST_ASSERT_EQUAL(0, svc_args[0]);
}

void test_svcServiceHandler_should_call_request_copy(void) {
  unsigned int svc_args[7] = {0};
  
  svc_args[0] = SVC_REQUEST_COPY;
  svc_args[1] = 0x20000000;
  svc_args[2] = 0x08000000;
  svc_args[3] = 20;
  
  flashCopyFromSramToFlash_Expect((uint32_t)svc_args[1], (uint32_t)svc_args[2], (int)svc_args[3]);
  svcServiceHandler(svc_args);
  
  TEST_ASSERT_EQUAL_HEX32(0x20000000, svc_args[1]);
  TEST_ASSERT_EQUAL_HEX32(0x08000000, svc_args[2]);
  TEST_ASSERT_EQUAL(20, svc_args[3]);
  TEST_ASSERT_EQUAL(0, svc_args[0]);
}

void test_svcServiceHandler_should_call_request_erase(void) {
  unsigned int svc_args[7] = {0};
  
  svc_args[0] = SVC_REQUEST_ERASE;
  svc_args[1] = 0x08000000;
  svc_args[2] = 20;
  
  flashErase_Expect((uint32_t)svc_args[1], (int)svc_args[2]);
  svcServiceHandler(svc_args);
  
  TEST_ASSERT_EQUAL_HEX32(0x08000000, svc_args[1]);
  TEST_ASSERT_EQUAL(20, svc_args[2]);
  TEST_ASSERT_EQUAL(0, svc_args[0]);
}

void test_svcServiceHandler_should_call_request_mass_erase(void) {
  unsigned int svc_args[7] = {0};
  
  svc_args[0] = SVC_REQUEST_MASS_ERASE;
  svc_args[1] = FLASH_BANK_BOTH;
  
  flashMassErase_Expect((uint32_t)svc_args[1]);
  svcServiceHandler(svc_args);
  
  TEST_ASSERT_EQUAL(FLASH_BANK_BOTH, svc_args[1]);
  TEST_ASSERT_EQUAL(0, svc_args[0]);
}