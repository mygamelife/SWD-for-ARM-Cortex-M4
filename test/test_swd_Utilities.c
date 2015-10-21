#include <stdint.h>
#include "unity.h"
#include "Emulator.h"
#include "swd_Utilities.h"
#include "IoOperations.h"
#include "MemoryReadWrite.h"
#include "mock_IoOperationsEx.h"
#include "mock_configurePort.h"

void setUp(void)  {}

void tearDown(void) {}

void test_calculateParity_SWDRequest_given_0000_should_return_0()
{

	TEST_ASSERT_EQUAL(0,calculateParity_SWDRequest(0,0,0,0));
}

void test_calculateParity_SWDRequest_given_0100_should_return_1()
{

	TEST_ASSERT_EQUAL(1,calculateParity_SWDRequest(0,1,0,0));
}

void test_calculateParity_SWDRequest_given_0111_should_return_1()
{

	TEST_ASSERT_EQUAL(1,calculateParity_SWDRequest(0,1,1,1));
}


void test_calculateParity_SWDRequest_given_1111_should_return_0()
{

	TEST_ASSERT_EQUAL(0,calculateParity_SWDRequest(1,1,1,1));
}

void test_calculateParity_32bitData_given_0x0_should_return_0()
{
	TEST_ASSERT_EQUAL(0,calculateParity_32bitData(0));
}

void test_calculateParity_32bitData_given_0x1_should_return_1()
{
	TEST_ASSERT_EQUAL(1,calculateParity_32bitData(1));
}

void test_calculateParity_32bitData_given_0xF1E2D3C4_should_return_1()
{
	//1111 0001 1110 0010 1101 0011 1100 0100

	TEST_ASSERT_EQUAL(1,calculateParity_32bitData(0xF1E2D3C4));
}

void test_getSWDAddress_Bit_given_0x2_should_return_0_for_bit3_0_for_bit2()
{
	int Address_bit3 , Address_bit2 ;

	getSWD_AddressBit(&Address_bit3,&Address_bit2,0x2);

	TEST_ASSERT_EQUAL(0,Address_bit3);
	TEST_ASSERT_EQUAL(0,Address_bit2);
}


void test_getSWDAddress_Bit_given_0x4_should_return_0_for_bit3_1_for_bit2()
{
	int Address_bit3 , Address_bit2 ;

	getSWD_AddressBit(&Address_bit3,&Address_bit2,0x4);

	TEST_ASSERT_EQUAL(0,Address_bit3);
	TEST_ASSERT_EQUAL(1,Address_bit2);
}

void test_getSWDAddress_Bit_given_0x8_should_return_1_for_bit3_0_for_bit2()
{
	int Address_bit3 , Address_bit2 ;

	getSWD_AddressBit(&Address_bit3,&Address_bit2,0x8);

	TEST_ASSERT_EQUAL(1,Address_bit3);
	TEST_ASSERT_EQUAL(0,Address_bit2);
}

void test_getSWDAddress_Bit_given_0xC_should_return_1_for_bit3_1_for_bit2()
{
	int Address_bit3 , Address_bit2 ;

	getSWD_AddressBit(&Address_bit3,&Address_bit2,0xC);

	TEST_ASSERT_EQUAL(1,Address_bit3);
	TEST_ASSERT_EQUAL(1,Address_bit2);
}


void test_getSWD_Request_given_Address_0x00_DP_Read_should_return_0xA5()
{
	//DP = 0 , AP = 1
	//WRiTE = 0 ,READ = 1

	//**Note LSB
	//Start bit	|	APnDP	|	RW	|	Addr2	|	Addr3	|	Parity	|	Stop	|	Park	|
	//    1     |	0		|	1	| 	0		|	0		|	1		|	0		|	1		|

	TEST_ASSERT_EQUAL(0xA5,getSWD_Request(0x00,DP,READ));
}


void test_getSWD_Request_given_Address0x08_AP_Write_should_return_0x93()
{
	//DP = 0 , AP = 1
	//WRiTE = 0 ,READ = 1

	//**Note LSB
	//Start bit	|	APnDP	|	RW	|	Addr2	|	Addr3	|	Parity	|	Stop	|	Park	|
	//    1     |	1		|	0	| 	0		|	1		|	0		|	0		|	1		|


	TEST_ASSERT_EQUAL(0x93,getSWD_Request(0x08,AP,WRITE));

}

void test_getSWD_Request_given_Address0x04_DP_READ_should_return_0x8D()
{
	//DP = 0 , AP = 1
	//WRiTE = 0 ,READ = 1

	//**Note LSB
	//Start bit	|	APnDP	|	RW	|	Addr2	|	Addr3	|	Parity	|	Stop	|	Park	|
	//    1     |	0		|	1	| 	1		|	0		|	0		|	0		|	1		|


	TEST_ASSERT_EQUAL(0x8D,getSWD_Request(0x04,DP,READ));
}

void test_compare_ParityWithData_given_0xFFFFFFFF_parity_0_should_return_ERR_NO_ERR()
{
	ErrorCode error;
	
	TEST_ASSERT_EQUAL(NO_ERROR,compare_ParityWithData(0xFFFFFFFF,0));
}

void test_compare_ParityWithData_given_0x0_parity_0_should_return_ERR_NO_ERR()
{
	TEST_ASSERT_EQUAL(NO_ERROR,compare_ParityWithData(0,0));
}

void test_compare_ParityWithData_given_0xFFFFFFFF_parity_1_should_return_ERR_INVALID_PARITY_RECEIVED()
{
	TEST_ASSERT_EQUAL(ERR_INVALID_PARITY_RECEIVED,compare_ParityWithData(0xFFFFFFFF,1));
}

void test_compare_ParityWithData_given_0x1_parity_0_should_return_ERR_INVALID_PARITY_RECEIVED()
{
	TEST_ASSERT_EQUAL(ERR_INVALID_PARITY_RECEIVED,compare_ParityWithData(0x1,0));
}

void test_swdCheckErrorFlag_should_return_SWD_WDATAERR_FLAG_when_bit_7_of_the_readData_is_set_to_1() {
  uint32_t flag = 0;

	emulateSwdRegisterRead(CTRLSTAT_REG, DP, OK, 0, interconvertMSBandLSB(0x80));

  flag = swdCheckErrorFlag();
  TEST_ASSERT_EQUAL(SWD_WDATA_ERROR_FLAG, flag);
}

void test_swdCheckErrorFlag_should_return_SWD_STICKYERR_FLAG_when_bit_5_of_the_readData_is_set_to_1() {
  uint32_t flag = 0;

  emulateSwdRegisterRead(CTRLSTAT_REG, DP, OK, 0, interconvertMSBandLSB(0x20));

  flag = swdCheckErrorFlag();

  TEST_ASSERT_EQUAL(SWD_STICKY_ERROR_FLAG, flag);
}

void test_swdCheckErrorFlag_should_return_SWD_STICKYCMP_FLAG_when_bit_4_of_the_readData_is_set_to_1() {
  uint32_t flag = 0;

  emulateSwdRegisterRead(CTRLSTAT_REG, DP, OK, 0, interconvertMSBandLSB(0x10));

  flag = swdCheckErrorFlag();

  TEST_ASSERT_EQUAL(SWD_STICKYCMP_ERROR_FLAG, flag);
}

void test_swdCheckErrorFlag_should_return_SWD_STICKYORUN_FLAG_when_bit_1_of_the_readData_is_set_to_1() {
  uint32_t flag = 0;

  emulateSwdRegisterRead(CTRLSTAT_REG, DP, OK, 0, interconvertMSBandLSB(0x02));

  flag = swdCheckErrorFlag();

  TEST_ASSERT_EQUAL(SWD_STICKYORUN_ERROR_FLAG, flag);
}

void test_swdClearErrorFlag_should_clear_WDATAERR_flag() {
  uint32_t flag = 0;

  emulateSwdRegisterWrite(ABORT_REG, DP, OK, SWD_WDERR_CLEAR_FLAG);

  swdClearErrorFlag(SWD_WDATA_ERROR_FLAG);
}

void test_swdClearErrorFlag_should_clear_STICKYERR_flag() {
  uint32_t flag = 0;

  emulateSwdRegisterWrite(ABORT_REG, DP, OK, SWD_STKERR_CLEAR_FLAG);

  swdClearErrorFlag(SWD_STICKY_ERROR_FLAG);
}

void test_swdClearErrorFlag_should_clear_STICKYCMP_flag() {
  uint32_t flag = 0;

  emulateSwdRegisterWrite(ABORT_REG, DP, OK, SWD_STKCMP_CLEAR_FLAG);

  swdClearErrorFlag(SWD_STICKYCMP_ERROR_FLAG);
}

void test_swdClearErrorFlag_should_clear_STICKYORUN_flag() {
  uint32_t flag = 0;

  emulateSwdRegisterWrite(ABORT_REG, DP, OK, SWD_ORUNERR_CLEAR_FLAG);

  swdClearErrorFlag(SWD_STICKYORUN_ERROR_FLAG);
}

void test_swdGetAckResponse_should_ack_response_and_return_SwdError() {
  int error = 0;

  error = swdGetAckResponse(OK_RESPONSE);
  TEST_ASSERT_EQUAL(NO_ERROR, error);
  
  error = swdGetAckResponse(WAIT_RESPONSE);
  TEST_ASSERT_EQUAL(ERR_ACK_WAIT_RESPONSE, error);
  
  error = swdGetAckResponse(FAULT_RESPONSE);
  TEST_ASSERT_EQUAL(ERR_ACK_FAULT_RESPONSE, error);
}

void test_swdReadDpWithRetries_should_repeat_readDP_transfer_3_times()  {
  int ack = 0, parity = 0;
  uint32_t readData = 0;
  
  emulateSwdRegisterRead(SELECT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xABC));
  emulateSwdRegisterRead(SELECT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xABC));
  emulateSwdRegisterRead(SELECT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xABC));
  
  swdReadDpWithRetries(SELECT_REG, &readData, 3);
  TEST_ASSERT_EQUAL(0xABC, readData);
}

void test_swdReadDpWithRetries_should_repeat_ONCE_when_ack_is_OK()  {
  int ack = 0, parity = 0;
  uint32_t readData = 0;
  
  emulateSwdRegisterRead(SELECT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xABC));
  emulateSwdRegisterRead(SELECT_REG, DP, OK, 1, interconvertMSBandLSB(0xABC));
  
  swdReadDpWithRetries(SELECT_REG, &readData, 3);
  TEST_ASSERT_EQUAL(0xABC, readData);
}

void test_swdWriteDpWithRetries_should_repeat_writeDP_transfer_4_times()  {
  int ack = 0, parity = 0;
  uint32_t writeData = 0xBEEFABC;
  
  emulateSwdRegisterWrite(ABORT_REG, DP, WAIT, writeData);
  emulateSwdRegisterWrite(ABORT_REG, DP, WAIT, writeData);
  emulateSwdRegisterWrite(ABORT_REG, DP, WAIT, writeData);
  
  swdWriteDpWithRetries(ABORT_REG, writeData, 3);
}

void test_swdWriteDpWithRetries_should_retries_one_time_with_ack_is_OK()  {
  int ack = 0, parity = 0;
  uint32_t writeData = 0xBEEFABC;
  
  emulateSwdRegisterWrite(ABORT_REG, DP, OK, writeData);
  
  swdWriteDpWithRetries(ABORT_REG, writeData, 3);
}

void test_swdWriteApWithRetries_should_repeat_writeAP_transfer_6_times()  {
  uint32_t writeData = 0xD3ADB33F;
  
  emulateSwdRegisterWrite(CSW_REG, AP, WAIT, writeData);
  emulateSwdRegisterWrite(CSW_REG, AP, WAIT, writeData);
  emulateSwdRegisterWrite(CSW_REG, AP, WAIT, writeData);
  emulateSwdRegisterWrite(CSW_REG, AP, WAIT, writeData);
  emulateSwdRegisterWrite(CSW_REG, AP, WAIT, writeData);
  emulateSwdRegisterWrite(CSW_REG, AP, OK, writeData);
  
  swdWriteApWithRetries(CSW_REG, writeData, 6);
}

void test_swdReadApWithRetries_should_repeat_readAP_transfer_2_times()  {
  uint32_t readData = 0;
  
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 0, interconvertMSBandLSB(0x1234ABCD));
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 0, interconvertMSBandLSB(0x1234ABCD));
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 0, interconvertMSBandLSB(0x1234ABCD));
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 0, interconvertMSBandLSB(0x1234ABCD));
  
  swdReadApWithRetries(TAR_REG, &readData, 2);
  TEST_ASSERT_EQUAL(0x1234ABCD, readData);
}

void test_retriesSwdOperation_given_write_operation_should_write_AP_3_times_and_return_acknowledgement()  {
  int error = 0;
  uint32_t readData = 0, dummyData = 0xDEADBEEF;
  
  emulateSwdRegisterWrite(CSW_REG, AP, WAIT, dummyData);
  emulateSwdRegisterWrite(CSW_REG, AP, OK, dummyData);
  
  error = swdRetriesOperation(WRITE, AP, CSW_REG, &dummyData, 3);
  TEST_ASSERT_EQUAL(NO_ERROR, error);
}

void test_retriesSwdOperation_given_read_operation_should_read_AP_4_times_and_return_acknowledgement()  {
  int error = 0;
  uint32_t readData = 0;
  
  emulateSwdRegisterRead(DRW_REG, AP, WAIT, 1, interconvertMSBandLSB(0x1EEA));
  emulateSwdRegisterRead(DRW_REG, AP, WAIT, 1, interconvertMSBandLSB(0x1EEA));

  error = swdRetriesOperation(READ, AP, DRW_REG, &readData, 1);
  TEST_ASSERT_EQUAL(ERR_ACK_WAIT_RESPONSE, error);
}

void test_retriesSwdOperation_given_read_operation_should_read_DP_6_times_and_return_acknowledgement()  {
  int error = 0;
  uint32_t readData = 0;
  
  emulateSwdRegisterRead(ABORT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xA123A));
  emulateSwdRegisterRead(ABORT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xA123A));
  emulateSwdRegisterRead(ABORT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xA123A));
  emulateSwdRegisterRead(ABORT_REG, DP, OK, 1, interconvertMSBandLSB(0xA123A));
  
  error = swdRetriesOperation(READ, DP, ABORT_REG, &readData, 6);
  TEST_ASSERT_EQUAL(0xA123A, readData);
  TEST_ASSERT_EQUAL(NO_ERROR, error);
}

void test_swdErrorHandler_given_WDERR_FLAG_bit_set_to_1_should_write_SWD_WDERR_CLEAR_FLAG_to_AP_ABORT_register() {
  int i = 0, data = 0;

  emulateSwdRegisterRead(CTRLSTAT_REG, DP, OK, 1, interconvertMSBandLSB(0x80));
  emulateSwdRegisterWrite(ABORT_REG, DP, OK, SWD_WDERR_CLEAR_FLAG);
  
  swdErrorHandler(ERR_ACK_FAULT_RESPONSE, 0, 0, 0, 0);
}

void test_swdErrorHandler_given_WAIT_RESPOND_should_retries_3_times_and_abort() {
  int parity = 0;
  uint32_t readData = 0;
  
  // First AP read
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 1, interconvertMSBandLSB(0xABC));
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 1, interconvertMSBandLSB(0xABC));
  // Second AP read
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 1, interconvertMSBandLSB(0xABC));
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 1, interconvertMSBandLSB(0xABC));
  // Third AP read
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 1, interconvertMSBandLSB(0xABC));
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 1, interconvertMSBandLSB(0xABC));
  
  emulateSwdRegisterWrite(ABORT_REG, DP, OK, SWD_DAPABORT_CLEAR_FLAG);
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 1, interconvertMSBandLSB(0xABC));
  emulateSwdRegisterRead(TAR_REG, AP, WAIT, 1, interconvertMSBandLSB(0xABC));
  
  swdErrorHandler(ERR_ACK_WAIT_RESPONSE, READ, AP, TAR_REG, &readData);
}

void test_swdErrorHandler_given_WAIT_RESPOND_should_AP_write_retries_3_times_if_success_dont_abort() {
  int parity = 0;
  uint32_t writeData = 0xDEADBEEF;
  
  emulateSwdRegisterWrite(DRW_REG, AP, WAIT, 0xDEADBEEF);
  emulateSwdRegisterWrite(DRW_REG, AP, WAIT, 0xDEADBEEF);
  emulateSwdRegisterWrite(DRW_REG, AP, OK, 0xDEADBEEF);
  
  swdErrorHandler(ERR_ACK_WAIT_RESPONSE, WRITE, AP, DRW_REG, &writeData);
}

void test_swdErrorHandler_given_WAIT_RESPOND_should_DP_read_retries_3_times_abort_and_Resend() {
  int parity = 0;
  uint32_t readData = 0;
  
  emulateSwdRegisterRead(SELECT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xCBA987));
  emulateSwdRegisterRead(SELECT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xCBA987));
  emulateSwdRegisterRead(SELECT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xCBA987));
  
  emulateSwdRegisterWrite(ABORT_REG, DP, OK, SWD_DAPABORT_CLEAR_FLAG);
  
  emulateSwdRegisterRead(SELECT_REG, DP, WAIT, 1, interconvertMSBandLSB(0xCBA987));
  
  swdErrorHandler(ERR_ACK_WAIT_RESPONSE, READ, DP, SELECT_REG, &readData);
}