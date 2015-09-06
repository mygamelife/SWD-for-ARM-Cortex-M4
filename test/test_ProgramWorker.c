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
  Tlv_Session *session = tlvCreateSession();
  
  uint32_t address = 0xABCDABCE, data = 0x12345678;
  
  writeCoreRegister_Expect(address, data);
  
	writeTargetRegister(session, address, data);
}

void test_readTargetRegister_given_register_address_should_read_the_given_register_address(void)
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  readCoreRegister_Ignore();
 
  readTargetRegister(session, 0xBEEFBEEF);
}

/*--------------performSoftResetOnTarget--------------------*/
void test_performSoftResetOnTarget_should_call_softResetTarget_and_send_TLV_ack()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  memoryWriteWord_ExpectAndReturn(AIRCR_REG,REQUEST_SYSTEM_RESET,NO_ERROR);
  
  performSoftResetOnTarget(session);
}

/*--------------performHardResetOnTarget--------------------*/
void test_performHardResetOnTarget_should_call_hardResetTarget_and_send_TLV_ack()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  hardResetTarget_Expect();
  
  performHardResetOnTarget(session);
}

void test_probeTaskManager_given_initial_state_receive_packet_when_packet_arrived_should_change_state(void)
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  session->DATA_RECEIVE_FLAG = true;
  session->rxBuffer[0] = TLV_OK;
  session->rxBuffer[1] = 1;
  session->rxBuffer[2] = 0;
  
  probeTaskManager(session);
  
  TEST_ASSERT_EQUAL(PROBE_INTERPRET_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
}

void test_probeTaskManager_given_tlv_packet_with_invalid_data_should_send_tlv_error_code(void)
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();

  session->DATA_RECEIVE_FLAG = true;
  session->rxBuffer[0] = 0xFF; //invalid command
  
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_RECEIVE_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
}

void test_probeTaskManager_should_receive_TLV_WRITE_REGISTER_and_perform_the_task(void)
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();

  session->DATA_RECEIVE_FLAG = true;
  session->rxBuffer[0] = TLV_WRITE_REGISTER; //invalid command
  session->rxBuffer[1] = 9;
  session->rxBuffer[2] = 0x01;
  session->rxBuffer[3] = 0x00;
  session->rxBuffer[4] = 0x00;
  session->rxBuffer[5] = 0x00;
  session->rxBuffer[6] = 0xDD;
  session->rxBuffer[7] = 0xCC;
  session->rxBuffer[8] = 0xBB;
  session->rxBuffer[9] = 0xAA;
  session->rxBuffer[10] = 0xF1; //chksum
  
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_INTERPRET_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  
  writeCoreRegister_Expect(0x01, 0xAABBCCDD);
  
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_RECEIVE_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
}

void test_probeTaskManager_should_receive_TLV_READ_REGISTER_and_perform_the_task(void)
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  uint32_t readData = 0;

  session->rxBuffer[0] = TLV_READ_REGISTER; //invalid command
  session->rxBuffer[1] = 5;
  session->rxBuffer[2] = 0x44;
  session->rxBuffer[3] = 0x33;
  session->rxBuffer[4] = 0x22;
  session->rxBuffer[5] = 0x11;
  session->rxBuffer[6] = 0x56; //chksum
  
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x00); //no data arrive
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 5, 0x00); //no data arrive
  
  tlvService(session);
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_INTERPRET_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->TIMEOUT_FLAG);

  session->rxBuffer[0] = TLV_READ_REGISTER; //invalid command
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive
  
  tlvService(session);
  
  readCoreRegister_Expect(0x11223344, &readData);
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_RECEIVE_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
}

/*--------------haltTarget--------------------*/
void test_haltTarget_should_return_ACK_if_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  setCoreMode_Expect(CORE_DEBUG_HALT);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_HALT);
  
  haltTarget(session);
}

void test_haltTarget_should_return_NACK_and_ERR_NOT_HALTED_if_not_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  setCoreMode_Expect(CORE_DEBUG_HALT);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_MODE);
  
  haltTarget(session);
}

/*--------------runTarget--------------------*/
void test_runTarget_should_return_ACK_if_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();

  setCoreMode_Expect(CORE_DEBUG_MODE);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_MODE);
  
  runTarget(session);
}

void test_runTarget_should_return_NACK_and_ERR_NOT_RUNNING_if_unsuccessful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  setCoreMode_Expect(CORE_DEBUG_MODE);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_HALT);
  
  runTarget(session);  
}

/*---------singleStepTarget----------------------*/
void test_singleStepTarget_should_step_readPC_run_and_return_PC_if_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  setCoreMode_Expect(CORE_SINGLE_STEP);
  getCoreMode_ExpectAndReturn(CORE_SINGLE_STEP);
  readCoreRegister_Ignore();
  
  singleStepTarget(session);    
}

void test_singleStepTarget_should_return_NACK_and_ERR_NOT_STEPPED_if_unsuccessful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  setCoreMode_Expect(CORE_SINGLE_STEP);
  getCoreMode_ExpectAndReturn(CORE_DEBUG_HALT);
  
  singleStepTarget(session);    
}
/*---------multipleStepTarget----------------------*/
void test_multipleStepTarget_should_step_readPC_run_and_return_PC_if_successful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  stepOnly_Expect(5);
  getCoreMode_ExpectAndReturn(CORE_SINGLE_STEP);
  readCoreRegister_Ignore();
  
  multipleStepTarget(session, 5);    
}

void test_multipleStepTarget_should_return_NACK_and_ERR_NOT_STEPPED_if_unsuccessful()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  stepOnly_Expect(5);
  getCoreMode_ExpectAndReturn(CORE_NORMAL_MODE);
  
  multipleStepTarget(session, 5);    
}

/*---------setBreakpoint----------------------*/
void xtest_setBreakpoint_should_set_breakpoint_and_return_PC_if_breakpoint_occurs()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  // selectNextFreeComparator_ExpectAndReturn(INSTRUCTION_TYPE,INSINSTRUCTION_COMP1);

  readCoreRegister_Ignore();
  
  setBreakpoint(0x12345678,MATCH_WORD);
}

void xtest_setBreakpoint_should_return_NACK_and_ERR_BKPT_NOTHIT_if_breakpoint_doesnt_occur()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  setBreakpoint(0x12345678,MATCH_UPPERHALFWORD);
}

void xtest_setBreakpoint_should_return_NACK_and_ERR_BKPT_MAXSET_if_all_comparators_are_in_use()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  setBreakpoint(0x12345678,MATCH_LOWERHALFWORD);
}

void test_writeTargetRam_should_write_data_to_specified_RAM_address()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  uint32_t buffer[] = {0x20000000, 0x12345678, 0xABCDABCD};
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, 12, (uint8_t *)buffer);
  
  memoryWriteWord_ExpectAndReturn(0x20000000, 0x12345678, NO_ERROR);
  memoryWriteWord_ExpectAndReturn(0x20000004, 0xABCDABCD, NO_ERROR);
  
  writeTargetRam(session, &get4Byte(&tlv->value[4]), get4Byte(&tlv->value[0]), tlv->length - 4);
}

void test_readTargetRam_should_write_data_to_specified_RAM_address()
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  
  uint32_t buffer[] = {0x20000000, 8};
  Tlv *tlv = tlvCreatePacket(TLV_WRITE_RAM, 8, (uint8_t *)buffer);
  
  memoryReadAndReturnWord_ExpectAndReturn(0x20000000, 0xDEADBEEF);
  memoryReadAndReturnWord_ExpectAndReturn(0x20000004, 0xABCDABCD);
  
  readTargetRam(session, get4Byte(&tlv->value[0]), get4Byte(&tlv->value[4]));
}

void test_probeTaskManager_should_receive_TLV_WRITE_RAM_and_perform_the_task(void)
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  uint32_t readData = 0;
  
  session->rxBuffer[0] = TLV_WRITE_RAM; //invalid command
  session->rxBuffer[1] = 9;
  session->rxBuffer[2] = 0x00;
  session->rxBuffer[3] = 0x00;
  session->rxBuffer[4] = 0x00;
  session->rxBuffer[5] = 0x20;
  session->rxBuffer[6] = 0x40;
  session->rxBuffer[7] = 0x30;
  session->rxBuffer[8] = 0x20;
  session->rxBuffer[9] = 0x10;
  session->rxBuffer[10] = 0x40; //chksum
  
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x00); //no data arrive
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 9, 0x00); //no data arrive
  
  tlvService(session);
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_INTERPRET_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->TIMEOUT_FLAG);

  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive  
  tlvService(session);
  
  memoryWriteWord_ExpectAndReturn(0x20000000, 0x10203040, NO_ERROR);
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_RECEIVE_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
}

void test_probeTaskManager_should_receive_TLV_READ_RAM_and_perform_the_task(void)
{
  UART_HandleTypeDef uartHandler;
  uartInit_IgnoreAndReturn(&uartHandler);
  Tlv_Session *session = tlvCreateSession();
  uint32_t readData = 0;
  
  session->rxBuffer[0] = TLV_READ_RAM; //invalid command
  session->rxBuffer[1] = 9;
  session->rxBuffer[2] = 0x00;
  session->rxBuffer[3] = 0x00;
  session->rxBuffer[4] = 0x00;
  session->rxBuffer[5] = 0x20;
  session->rxBuffer[6] = 12;
  session->rxBuffer[7] = 0x00;
  session->rxBuffer[8] = 0x00;
  session->rxBuffer[9] = 0x00;
  session->rxBuffer[10] = 0xD4; //chksum
  
  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x00); //no data arrive
  getBytes_ExpectAndReturn(session->handler, &session->rxBuffer[2], 9, 0x00); //no data arrive
  
  tlvService(session);
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_INTERPRET_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  TEST_ASSERT_EQUAL(false, session->DATA_SEND_FLAG);
  TEST_ASSERT_EQUAL(false, session->TIMEOUT_FLAG);

  getBytes_ExpectAndReturn(session->handler, session->rxBuffer, 2, 0x01); //no data arrive  
  tlvService(session);
  
  memoryReadAndReturnWord_ExpectAndReturn(0x20000000, 0xDEADBEEF);
  memoryReadAndReturnWord_ExpectAndReturn(0x20000004, 0xABCDABCD);
  memoryReadAndReturnWord_ExpectAndReturn(0x20000008, 0xABCDABCD);
  
  probeTaskManager(session);
  TEST_ASSERT_EQUAL(PROBE_RECEIVE_PACKET, session->probeState);
  TEST_ASSERT_EQUAL(false, session->DATA_RECEIVE_FLAG);
  TEST_ASSERT_EQUAL(true, session->DATA_SEND_FLAG);
}