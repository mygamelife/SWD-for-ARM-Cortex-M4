#include "mock_memoryRW.h"
#include "mock_Flash.h"
#include "SwdStub.h"
#include "unity.h"


void setUp(void) {}



void tearDown(void) {}



void test_stubCopy_should_get_flash_sram_start_address_length_and_call_Flash_Copy_func() {



  STUB = malloc(sizeof(Stub_Type));



  STUB->sramAddress = 0x20000000;

  STUB->flashAddress = ((uint32_t)0x081C0000);

  STUB->dataSize = 2048;



  flashCopyFromSramToFlash_CMockExpect(18, 0x20000000, ((uint32_t)0x081C0000), 2048);



  stubCopy();



  UnityAssertEqualNumber((_U_SINT)((STUB_CLEAR)), (_U_SINT)((STUB->instruction)), (((void *)0)), (_U_UINT)22, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((STUB_OK)), (_U_SINT)((STUB->status)), (((void *)0)), (_U_UINT)23, UNITY_DISPLAY_STYLE_INT);



  free(STUB);

}



void test_stubErase_should_get_flash_address_and_flash_size_and_call_flashErase_func(void) {



  STUB = malloc(sizeof(Stub_Type));



  STUB->flashAddress = ((uint32_t)0x08100000);

  STUB->dataSize = 16;



  flashErase_CMockExpect(35, ((uint32_t)0x08100000), 16);



  stubErase();



  UnityAssertEqualNumber((_U_SINT)((STUB_CLEAR)), (_U_SINT)((STUB->instruction)), (((void *)0)), (_U_UINT)39, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((STUB_OK)), (_U_SINT)((STUB->status)), (((void *)0)), (_U_UINT)40, UNITY_DISPLAY_STYLE_INT);



  free(STUB);

}



void test_stubMassErase_should_get_bank_select_from_SRAM_and_call_flashMassErase_func() {



  STUB = malloc(sizeof(Stub_Type));



  STUB->banks = ((uint32_t)((uint32_t)1) | ((uint32_t)2));



  flashMassErase_CMockExpect(51, ((uint32_t)((uint32_t)1) | ((uint32_t)2)));



  stubMassErase();



  UnityAssertEqualNumber((_U_SINT)((STUB_CLEAR)), (_U_SINT)((STUB->instruction)), (((void *)0)), (_U_UINT)55, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((STUB_OK)), (_U_SINT)((STUB->status)), (((void *)0)), (_U_UINT)56, UNITY_DISPLAY_STYLE_INT);



  free(STUB);

}



void test_svcServiceHandler_should_call_request_sram_address(void) {

  unsigned int svc_args[7] = {0};



  svc_args[0] = 1;



  svcServiceHandler(svc_args);



  UnityAssertEqualNumber((_U_SINT)(_US32)((0x20012800)), (_U_SINT)(_US32)((svc_args[1])), (((void *)0)), (_U_UINT)68, UNITY_DISPLAY_STYLE_HEX32);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((svc_args[0])), (((void *)0)), (_U_UINT)69, UNITY_DISPLAY_STYLE_INT);

}



void test_svcServiceHandler_should_call_request_copy(void) {

  unsigned int svc_args[7] = {0};



  svc_args[0] = 2;

  svc_args[1] = 0x20000000;

  svc_args[2] = 0x08000000;

  svc_args[3] = 20;



  flashCopyFromSramToFlash_CMockExpect(80, (uint32_t)svc_args[1], (uint32_t)svc_args[2], (int)svc_args[3]);

  svcServiceHandler(svc_args);



  UnityAssertEqualNumber((_U_SINT)(_US32)((0x20000000)), (_U_SINT)(_US32)((svc_args[1])), (((void *)0)), (_U_UINT)83, UNITY_DISPLAY_STYLE_HEX32);

  UnityAssertEqualNumber((_U_SINT)(_US32)((0x08000000)), (_U_SINT)(_US32)((svc_args[2])), (((void *)0)), (_U_UINT)84, UNITY_DISPLAY_STYLE_HEX32);

  UnityAssertEqualNumber((_U_SINT)((20)), (_U_SINT)((svc_args[3])), (((void *)0)), (_U_UINT)85, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((svc_args[0])), (((void *)0)), (_U_UINT)86, UNITY_DISPLAY_STYLE_INT);

}



void test_svcServiceHandler_should_call_request_erase(void) {

  unsigned int svc_args[7] = {0};



  svc_args[0] = 3;

  svc_args[1] = 0x08000000;

  svc_args[2] = 20;



  flashErase_CMockExpect(96, (uint32_t)svc_args[1], (int)svc_args[2]);

  svcServiceHandler(svc_args);



  UnityAssertEqualNumber((_U_SINT)(_US32)((0x08000000)), (_U_SINT)(_US32)((svc_args[1])), (((void *)0)), (_U_UINT)99, UNITY_DISPLAY_STYLE_HEX32);

  UnityAssertEqualNumber((_U_SINT)((20)), (_U_SINT)((svc_args[2])), (((void *)0)), (_U_UINT)100, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((svc_args[0])), (((void *)0)), (_U_UINT)101, UNITY_DISPLAY_STYLE_INT);

}



void test_svcServiceHandler_should_call_request_mass_erase(void) {

  unsigned int svc_args[7] = {0};



  svc_args[0] = 4;

  svc_args[1] = ((uint32_t)((uint32_t)1) | ((uint32_t)2));



  flashMassErase_CMockExpect(110, (uint32_t)svc_args[1]);

  svcServiceHandler(svc_args);



  UnityAssertEqualNumber((_U_SINT)((((uint32_t)((uint32_t)1) | ((uint32_t)2)))), (_U_SINT)((svc_args[1])), (((void *)0)), (_U_UINT)113, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((svc_args[0])), (((void *)0)), (_U_UINT)114, UNITY_DISPLAY_STYLE_INT);

}
