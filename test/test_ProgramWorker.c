#include "unity.h"
#include "Tlv_ex.h"
#include "ProgramWorker.h"
#include "CoreDebug_Utilities.h"
#include "mock_IoOperations.h"
#include "mock_UART.h"
#include "mock_CoreDebug.h"
#include "mock_Tlv_Worker.h"
#include "mock_stm32f4xx_hal_uart.h"
#include "mock_Register_ReadWrite.h"

uint32_t readDummy = 0xFFFFFFFF;

void setUp(void)  {}

void tearDown(void) {}

void test_load_SectorErase_Instruction_should_wait_untill_target_response_OK_before_load_instruction(void)
{
  uint32_t status = 0;
  memoryReadAndReturnWord_ExpectAndReturn(SWD_TARGET_STATUS, TARGET_OK);
  
  /* load flash start and end address to sram */
  memoryWriteWord_ExpectAndReturn(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_20, NO_ERROR);
  memoryWriteWord_ExpectAndReturn(SWD_FLASH_END_ADDRESS, ADDR_FLASH_SECTOR_22, NO_ERROR);
  
  // /* load instruction to sram */
  memoryWriteWord_ExpectAndReturn(SWD_INSTRUCTION, INSTRUCTION_ERASE_SECTOR, NO_ERROR);
  
  loadEraseSectorInstruction((uint32_t *)ADDR_FLASH_SECTOR_20, (uint32_t *)ADDR_FLASH_SECTOR_22);
}

void test_loadMassEraseInstruction_should_wait_untill_target_response_OK_before_load_instruction(void)
{
  memoryReadAndReturnWord_ExpectAndReturn(SWD_TARGET_STATUS, TARGET_BUSY);
  memoryReadAndReturnWord_ExpectAndReturn(SWD_TARGET_STATUS, TARGET_OK);
  
  /* load bank select to sram */
  memoryWriteWord_ExpectAndReturn(SWD_BANK_SELECT, FLASH_BANK_BOTH, NO_ERROR);
  
  /* load instruction to sram */
  memoryWriteWord_ExpectAndReturn(SWD_INSTRUCTION, INSTRUCTION_MASS_ERASE, NO_ERROR);  
  
  loadMassEraseInstruction(FLASH_BANK_BOTH);
}

void test_loadCopyInstruction_should_load_src_address_dest_address_and_length_into_SRAM_instruction_address(void)
{
  memoryReadAndReturnWord_ExpectAndReturn(SWD_TARGET_STATUS, TARGET_OK);
  
  /* load SRAM start address into sram */
  memoryWriteWord_ExpectAndReturn(SWD_SRAM_START_ADDRESS, 0x200001F0, NO_ERROR);
  
  /* load Flash start address into sram */
  memoryWriteWord_ExpectAndReturn(SWD_FLASH_START_ADDRESS, ADDR_FLASH_SECTOR_18, NO_ERROR);
  
  /* load length into sram */
  memoryWriteWord_ExpectAndReturn(SWD_DATA_SIZE, 2000, NO_ERROR);

	/* load copy instructoin into sram */
  memoryWriteWord_ExpectAndReturn(SWD_INSTRUCTION, INSTRUCTION_COPY, NO_ERROR);
  
  loadCopyFromSRAMToFlashInstruction((uint32_t *)0x200001F0, (uint32_t *)ADDR_FLASH_SECTOR_18, 2000);
}

void test_writeTargetRegister_given_register_address_and_data(void)
{
  Tlv tlv;  
  Tlv_Session session;
  
  uint32_t address = 0xABCDABCE, data = 0x12345678;
  
  writeCoreRegister_Expect(address, data);
  tlvCreatePacket_ExpectAndReturn(TLV_OK, 0, 0, &tlv);
  tlvSend_Expect(&session, &tlv);
  
	writeTargetRegister(&session, &address, &data);
}

void test_readTargetRegister_given_register_address_should_read_the_given_register_address(void)
{
  Tlv tlv;  
  Tlv_Session session;
  
  uint32_t address = 0xBEEFBEEF, data = 0;
  
  readCoreRegister_Ignore();
  tlvCreatePacket_ExpectAndReturn(TLV_READ_REGISTER, 4, (uint8_t *)&data, &tlv);
  tlvSend_Expect(&session, &tlv);
 
  readTargetRegister(&session, &address);
}


/*--------------performSoftResetOnTarget--------------------*/
void test_performSoftResetOnTarget_should_call_softResetTarget_and_send_TLV_ack()
{
  Tlv tlv;  
  Tlv_Session session;
  
  memoryWriteWord_ExpectAndReturn(AIRCR_REG,REQUEST_SYSTEM_RESET,NO_ERROR);
  
  tlvCreatePacket_ExpectAndReturn(TLV_OK, 0, 0, &tlv);
  tlvSend_Expect(&session, &tlv);
  
  performSoftResetOnTarget(&session);
}

/*--------------performHardResetOnTarget--------------------*/
void test_performHardResetOnTarget_should_call_hardResetTarget_and_send_TLV_ack()
{
  Tlv tlv;  
  Tlv_Session session;
  
  hardResetTarget_Expect();
  tlvCreatePacket_ExpectAndReturn(TLV_OK, 0, 0, &tlv);
  tlvSend_Expect(&session, &tlv);
  
  performHardResetOnTarget(&session);
}

/*--------------haltTarget--------------------*/
void test_haltTarget_should_return_ACK_if_successful()
{
  Tlv tlv;  
  Tlv_Session session;
  
  setCoreMode_Expect(CORE_DEBUG_HALT);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_HALT);
  
  tlvCreatePacket_ExpectAndReturn(TLV_OK, 0, 0, &tlv);
  tlvSend_Expect(&session, &tlv);
  
  haltTarget(&session);
}

void test_haltTarget_should_return_NACK_if_not_successful()
{
  Tlv tlv;  
  Tlv_Session session;
  
  setCoreMode_Expect(CORE_DEBUG_HALT);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_MODE);
  
  tlvCreatePacket_ExpectAndReturn(TLV_NOT_OK, 1, ERR_NOT_HALTED, &tlv);
  tlvSend_Expect(&session, &tlv);
  
  haltTarget(&session);
}

/*--------------runTarget--------------------*/
void test_runTarget_should_return_ACK_if_successful()
{
  Tlv tlv;  
  Tlv_Session session;
  
  setCoreMode_Expect(CORE_DEBUG_MODE);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_MODE);
  
  tlvCreatePacket_ExpectAndReturn(TLV_OK, 0, 0, &tlv);
  tlvSend_Expect(&session, &tlv);
  
  haltTarget(&session);
}

void test_runTarget_should_return_NACK_if_unsuccessful()
{
  Tlv tlv;  
  Tlv_Session session;
  
  setCoreMode_Expect(CORE_DEBUG_MODE);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_HALT);
  
  tlvCreatePacket_ExpectAndReturn(TLV_NOT_OK, 1, ERR_NOT_RUNNING, &tlv);
  tlvSend_Expect(&session, &tlv);
  
  haltTarget(&session);  
}

/*---------singleStepTarget----------------------*/
void test_singleStepTarget_should_step_readPC_run_and_return_PC()
{
  uint32_t data = 0 ;
  
  Tlv tlv;  
  Tlv_Session session;
  
  setCoreMode_Expect(CORE_SINGLE_STEP);
  readCoreRegister_Ignore();
  setCoreMode_Expect(CORE_DEBUG_MODE);
  
  tlvCreatePacket_ExpectAndReturn(TLV_STEP, 4, (uint8_t *)&data, &tlv);
  tlvSend_Expect(&session, &tlv);
  
  singleStepTarget(&session);    
}

/*---------multipleStepTarget----------------------*/
void test_multipleStepTarget_should_step_readPC_run_and_return_PC()
{
  uint32_t data = 0 ;
  
  Tlv tlv;  
  Tlv_Session session;
  
  stepOnly_Expect(5);
  readCoreRegister_Ignore();
  setCoreMode_Expect(CORE_DEBUG_MODE);
  
  tlvCreatePacket_ExpectAndReturn(TLV_STEP, 4, (uint8_t *)&data, &tlv);
  tlvSend_Expect(&session, &tlv);
  
  multipleStepTarget(&session,5);    
}