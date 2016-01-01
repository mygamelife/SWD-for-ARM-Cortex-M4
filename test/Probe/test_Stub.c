#include "unity.h"
#include "Stub.h"
#include "mock_Flash.h"
#include "mock_memoryRW.h"
#include "mock_SystemConfigure.h"

void setUp(void)  {}

void tearDown(void) {}

void test_stubInit_should_initialize_all_element_inside_Stub_structure() {
  Stub = malloc(sizeof(Stub_Type));
  
  stubInit();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, Stub->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, Stub->status);
  TEST_ASSERT_EQUAL(0, Stub->flashAddress);
  TEST_ASSERT_EQUAL(0, Stub->sramAddress);
  TEST_ASSERT_EQUAL(0, Stub->banks);
  TEST_ASSERT_EQUAL(0, Stub->sysClockPrescale);
  TEST_ASSERT_EQUAL(0, Stub->sysClock);
  TEST_ASSERT_EQUAL_HEX32(0x0ABCDEF0, Stub->id);
  
  free(Stub);
}

void test_stubCopy_should_get_flash_sram_start_address_length_and_call_Flash_Copy_func()  {
  
  Stub = malloc(sizeof(Stub_Type));
  
  Stub->sramAddress = 0x20000000;
  Stub->flashAddress = ADDR_FLASH_SECTOR_22;
  Stub->dataSize = 2048;
  
  flashCopyFromSramToFlash_Expect(0x20000000, ADDR_FLASH_SECTOR_22, 2048);
  
  stubCopy();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, Stub->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, Stub->status);
  
  free(Stub);
}

void test_stubErase_should_get_flash_address_and_flash_size_and_call_flashErase_func(void) {
  
  Stub = malloc(sizeof(Stub_Type));
  
  Stub->flashAddress = ADDR_FLASH_SECTOR_12;
  Stub->dataSize = 16;
  
  flashErase_Expect(ADDR_FLASH_SECTOR_12, 16);
  
  stubErase();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, Stub->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, Stub->status);
  
  free(Stub);
}

void test_stubMassErase_should_get_bank_select_from_SRAM_and_call_flashMassErase_func()  {
  
  Stub = malloc(sizeof(Stub_Type));
  
  Stub->banks = FLASH_BANK_BOTH;
  
  flashMassErase_Expect(FLASH_BANK_BOTH);
  
  stubMassErase();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, Stub->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, Stub->status);
  
  free(Stub);
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

void test_stubGetSysClk_should_return_system_clock_frequency(void) {
  Stub = malloc(sizeof(Stub_Type));
  
  getSystemClock_ExpectAndReturn(9000000);
  stubGetSystemClock();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, Stub->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, Stub->status);
  TEST_ASSERT_EQUAL(Stub->sysClock, 9000000);
  
  free(Stub);
}

void test_stubSetSysClk_should_call_configure_system_clock_with_prescale_input_argument(void) {
  Stub = malloc(sizeof(Stub_Type));
  Stub->sysClockPrescale = SYSTEM_CLOCK_PRESCALE2;
  
  configSystemClock_Expect(Stub->sysClockPrescale);
  stubPrescaleSystemClock();
  
  TEST_ASSERT_EQUAL(STUB_CLEAR, Stub->instruction);
  TEST_ASSERT_EQUAL(STUB_OK, Stub->status);
  TEST_ASSERT_EQUAL(SYSTEM_CLOCK_PRESCALE2, Stub->sysClockPrescale);
  
  free(Stub);
}