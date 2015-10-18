#include "mock_memoryRW.h"
#include "mock_stm32f4xx_hal_flash_ex.h"
#include "mock_stm32f4xx_hal_flash.h"
#include "mock_configurePort.h"
#include "Flash.h"
#include "unity.h"


void setUp(void){}



void tearDown(void){}



void test_flashGetSector_given_FLASH_SECTOR_0_should_select_FLASH_SECTOR_0() {

  uint32_t sector = 0;



  sector = flashGetSector(((uint32_t)0x08000000));

  UnityAssertEqualNumber((_U_SINT)((((uint32_t)0))), (_U_SINT)((sector)), (((void *)0)), (_U_UINT)16, UNITY_DISPLAY_STYLE_INT);

}



void test_flashGetSector_given_flash_sector_23_should_return_23() {

  uint32_t sector = 0;



  sector = flashGetSector(((uint32_t)0x081E0000));

  UnityAssertEqualNumber((_U_SINT)((((uint32_t)23))), (_U_SINT)((sector)), (((void *)0)), (_U_UINT)23, UNITY_DISPLAY_STYLE_INT);

}



void test_flashGetSector_given_last_address_of_flash_sector_23_should_return_23() {

  uint32_t sector = 0;



  sector = flashGetSector(0x081FFFFF);

  UnityAssertEqualNumber((_U_SINT)((((uint32_t)23))), (_U_SINT)((sector)), (((void *)0)), (_U_UINT)30, UNITY_DISPLAY_STYLE_INT);

}



void test_flashMassErase_given_bank2_should_erase_whole_bank_2() {

  HAL_FLASH_Unlock_CMockExpectAndReturn(34, HAL_OK);

  HAL_FLASHEx_Erase_CMockIgnoreAndReturn(35, HAL_OK);

  HAL_FLASH_Lock_CMockExpectAndReturn(36, HAL_OK);



  flashMassErase(((uint32_t)2));

}



void test_flashErase_given_StartSector_15_and_20() {

  HAL_FLASH_Unlock_CMockExpectAndReturn(42, HAL_OK);

  HAL_FLASHEx_Erase_CMockIgnoreAndReturn(43, HAL_OK);

  HAL_FLASH_Lock_CMockExpectAndReturn(44, HAL_OK);



  flashErase(((uint32_t)0x0810C000), 20);

}



void test_flashWriteProgram_given_FLASH_TYPEPROGRAM_WORD() {

  HAL_FLASH_Program_CMockExpectAndReturn(50, ((uint32_t)0x02), 0x081FFFFF, 0xABCDABCD, HAL_OK);



  flashWriteProgram(((uint32_t)0x02), 0x081FFFFF, 0xABCDABCD);

}



void test_flashWrite_given_data_0xABCDABCD_should_write_into_ADDR_FLASH_SECTOR_19() {

  uint32_t data = 0xABCDABCD;



  HAL_FLASH_Unlock_CMockExpectAndReturn(58, HAL_OK);

  HAL_FLASH_Program_CMockExpectAndReturn(59, ((uint32_t)0x02), ((uint32_t)0x08160000), data, HAL_OK);

  HAL_FLASH_Lock_CMockExpectAndReturn(60, HAL_OK);



  flashWrite(&data, ((uint32_t)0x08160000), 1);

}



void test_flashWrite_given_data_and_size_5_should_write_into_Flash() {

  uint32_t data[] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555};



  HAL_FLASH_Unlock_CMockExpectAndReturn(68, HAL_OK);



  HAL_FLASH_Program_CMockExpectAndReturn(70, ((uint32_t)0x02), 0x08000000, data[0], HAL_OK);

  HAL_FLASH_Program_CMockExpectAndReturn(71, ((uint32_t)0x02), 0x08000004, data[1], HAL_OK);

  HAL_FLASH_Program_CMockExpectAndReturn(72, ((uint32_t)0x02), 0x08000008, data[2], HAL_OK);

  HAL_FLASH_Program_CMockExpectAndReturn(73, ((uint32_t)0x02), 0x0800000C, data[3], HAL_OK);

  HAL_FLASH_Program_CMockExpectAndReturn(74, ((uint32_t)0x02), 0x08000010, data[4], HAL_OK);



  HAL_FLASH_Lock_CMockExpectAndReturn(76, HAL_OK);



  flashWrite(data, 0x08000000, 20);

}



void test_flashCopyFromSramToFlash_given_0x20000000_sram_address_0x08000000_flash_address_and_size_8(void) {



  HAL_FLASH_Unlock_CMockExpectAndReturn(83, HAL_OK);

  readMemoryData_CMockExpectAndReturn(84, 0x20000000, 0xDEADBEEF);

  HAL_FLASH_Program_CMockExpectAndReturn(85, ((uint32_t)0x02), 0x08000000, 0xDEADBEEF, HAL_OK);



  readMemoryData_CMockExpectAndReturn(87, 0x20000004, 0xBEEFCAFE);

  HAL_FLASH_Program_CMockExpectAndReturn(88, ((uint32_t)0x02), 0x08000004, 0xBEEFCAFE, HAL_OK);

  HAL_FLASH_Lock_CMockExpectAndReturn(89, HAL_OK);



  flashCopyFromSramToFlash(0x20000000, 0x08000000, 8);

}
