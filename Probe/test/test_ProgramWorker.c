#include "unity.h"
#include "Tlv.h"
#include "Tlv_ex.h"
#include "ProgramWorker.h"
#include "CoreDebug_Utilities.h"
#include "mock_IoOperations.h"
#include "mock_uart.h"
#include "mock_CoreDebug.h"
#include "mock_FPB_Unit.h"
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
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  uint32_t address = 0xABCDABCE, data = 0x12345678;
  
  writeCoreRegister_Expect(address, data);
  
	writeTargetRegister(session, &address, &data);
}

void test_readTargetRegister_given_register_address_should_read_the_given_register_address(void)
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();

  uint32_t address = 0xBEEFBEEF, data = 0;
  
  readCoreRegister_Ignore();
 
  readTargetRegister(session, &address);
}


/*--------------performSoftResetOnTarget--------------------*/
void test_performSoftResetOnTarget_should_call_softResetTarget_and_send_TLV_ack()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  memoryWriteWord_ExpectAndReturn(AIRCR_REG,REQUEST_SYSTEM_RESET,NO_ERROR);
  
  performSoftResetOnTarget(session);
}

/*--------------performHardResetOnTarget--------------------*/
void test_performHardResetOnTarget_should_call_hardResetTarget_and_send_TLV_ack()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  hardResetTarget_Expect();
  
  performHardResetOnTarget(session);
}

// void test_programWorker_given_initial_state_waiting_packet_when_packet_arrived_should_change_state(void)
// {
  // Tlv tlv;  
  // Tlv_Session session;
  // session.state = WAITING_PACKET;

  // tlvReceive_ExpectAndReturn(&session, &tlv);
  
  // programWorker(&session);
  
  // TEST_ASSERT_EQUAL(INTERPRET_PACKET, session.state);
// }

// void test_programWorker_given_tlv_packet_with_invalid_data_should_send_tlv_error_code(void)
// {
  // Tlv tlv, error;
  // uint8_t errorCode = ERR_CORRUPTED_DATA;
  // Tlv_Session session;
  // session.state = INTERPRET_PACKET;

  // tlvVerifyData_IgnoreAndReturn(DATA_INVALID);
  // tlvCreatePacket_ExpectAndReturn(TLV_NOT_OK, 1, &errorCode, &error);
  // tlvSend_Expect(&session, &error);
  
  // programWorker(&session);
  
  // TEST_ASSERT_EQUAL(WAITING_PACKET, session.state);
// }

/*--------------haltTarget--------------------*/
void test_haltTarget_should_return_ACK_if_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  setCoreMode_Expect(CORE_DEBUG_HALT);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_HALT);
  
  haltTarget(session);
}

void test_haltTarget_should_return_NACK_and_ERR_NOT_HALTED_if_not_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  setCoreMode_Expect(CORE_DEBUG_HALT);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_MODE);
  
  haltTarget(session);
}

/*--------------runTarget--------------------*/
void test_runTarget_should_return_ACK_if_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();

  setCoreMode_Expect(CORE_DEBUG_MODE);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_MODE);
  
  runTarget(session);
}

void test_runTarget_should_return_NACK_and_ERR_NOT_RUNNING_if_unsuccessful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  setCoreMode_Expect(CORE_DEBUG_MODE);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_HALT);
  
  runTarget(session);  
}

/*---------singleStepTarget----------------------*/
void test_singleStepTarget_should_step_readPC_run_and_return_PC_if_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  setCoreMode_Expect(CORE_SINGLE_STEP);
  getCoreMode_ExpectAndReturn(CORE_SINGLE_STEP);
  readCoreRegister_Ignore();
  
  singleStepTarget(session);    
}

void test_singleStepTarget_should_return_NACK_and_ERR_NOT_STEPPED_if_unsuccessful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  setCoreMode_Expect(CORE_SINGLE_STEP);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_HALT);
  
  singleStepTarget(session);    
}
/*---------multipleStepTarget----------------------*/
void test_multipleStepTarget_should_step_readPC_run_and_return_PC_if_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  stepOnly_Expect(5);
  getCoreMode_ExpectAndReturn(CORE_SINGLE_STEP);
  readCoreRegister_Ignore();
  
  multipleStepTarget(session, 5);    
}

void test_multipleStepTarget_should_return_NACK_and_ERR_NOT_STEPPED_if_unsuccessful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  stepOnly_Expect(5);
  getCoreMode_ExpectAndReturn(CORE_NORMAL_MODE);
  
  multipleStepTarget(session, 5);    
}

/*---------setBreakpoint----------------------*/
void xtest_setBreakpoint_should_set_breakpoint_and_return_PC_if_breakpoint_occurs()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  selectNextFreeInstructionComparator_ExpectAndReturn(INSTRUCTION_COMP1);

  readCoreRegister_Ignore();
  
  setBreakpoint(0x12345678,MATCH_WORD);
}

void xtest_setBreakpoint_should_return_NACK_and_ERR_BKPT_NOTHIT_if_breakpoint_doesnt_occur()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  setBreakpoint(0x12345678,MATCH_UPPERHALFWORD);
}

void xtest_setBreakpoint_should_return_NACK_and_ERR_BKPT_MAXSET_if_all_comparators_are_in_use()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateWorkerSession();
  
  setBreakpoint(0x12345678,MATCH_LOWERHALFWORD);
}