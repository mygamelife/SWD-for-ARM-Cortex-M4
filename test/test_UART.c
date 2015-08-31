#include "unity.h"
#include "UART.h"
#include "Delay.h"
#include "TLV_State.h"
#include "mock_configurePort.h"
#include "mock_stm32f4xx_hal_uart.h"
#include "mock_stm32f429i_discovery.h"

void setUp(void)  {}

void tearDown(void) {}

void test_initUart_UART_Handler_should_contain_all_the_configuration(void)
{
  HAL_UART_Init_IgnoreAndReturn(HAL_OK);
  UART_HandleTypeDef *uartHandle = initUart();

	TEST_ASSERT_EQUAL(USARTx,               uartHandle->Instance);
	TEST_ASSERT_EQUAL(USART_BAUD_RATE,      uartHandle->Init.BaudRate);
	TEST_ASSERT_EQUAL(UART_WORDLENGTH_8B,   uartHandle->Init.WordLength);
	TEST_ASSERT_EQUAL(UART_STOPBITS_1,      uartHandle->Init.StopBits);
	TEST_ASSERT_EQUAL(UART_PARITY_NONE,     uartHandle->Init.Parity);
	TEST_ASSERT_EQUAL(UART_HWCONTROL_NONE,  uartHandle->Init.HwFlowCtl);
	TEST_ASSERT_EQUAL(UART_MODE_TX_RX,      uartHandle->Init.Mode);
	TEST_ASSERT_EQUAL(UART_OVERSAMPLING_16, uartHandle->Init.OverSampling);
}

void test_stm32UartSendByte_given_PROBE_OK_should_transmit_over_using_Uart(void)
{
  HAL_UART_Init_IgnoreAndReturn(HAL_OK);
  UART_HandleTypeDef *uartHandle = initUart();
  
  HAL_UART_Transmit_IgnoreAndReturn(HAL_OK);
  stm32UartSendByte(uartHandle, PROBE_OK);
}

void test_stm32UartSendBytes_given_buffer_contain_multiple_bytes_data_should_transmit_over_using_Uart(void)
{
  uint8_t data[] = {0xA0, 0xB0, 0xC0, 0xD0};
  
  HAL_UART_Init_IgnoreAndReturn(HAL_OK);
  UART_HandleTypeDef *uartHandle = initUart();
  
  HAL_UART_Transmit_IgnoreAndReturn(HAL_OK);
  stm32UartSendBytes(uartHandle, data);
}

void test_stm32UartGetByte_should_receive_single_byte_data_from_uart_port(void)
{
  HAL_UART_Init_IgnoreAndReturn(HAL_OK);
  UART_HandleTypeDef *uartHandle = initUart();
  
  HAL_UART_Receive_IgnoreAndReturn(HAL_OK);
  uint8_t byteData = stm32UartGetByte(uartHandle);
}

// void test_stm32UartGetBytes_should_receive_multiple_bytes_data_from_uart_port(void)
// {
  // uint8_t buffer[1024];
  
  // HAL_UART_Init_IgnoreAndReturn(HAL_OK);
  // UART_HandleTypeDef *uartHandle = initUart();
  
  // HAL_UART_Receive_IgnoreAndReturn(HAL_OK);
  // stm32UartGetBytes(uartHandle, buffer);
// }