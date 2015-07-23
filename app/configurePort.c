#include "configurePort.h"

/* *
 *  Note : Modify configurePort.h to change the Port and I/O pin for SWD
 */


/**
 * Configure the port and pin to be used as SWCLK
 *
 */
void configureClock()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
	GpioInfo.Pin = SWDCLK_PIN ;
	GpioInfo.Pull = GPIO_NOPULL ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(SWD_PORT,&GpioInfo);
}

/**
 * Configure the port and pin to be used to perform a hard reset on target
 *
 */
void configureTargetResetPin()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_OD;
	GpioInfo.Pin = SWDNRST_PIN;
	GpioInfo.Pull = GPIO_PULLUP ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(SWD_PORT, &GpioInfo);
}

/**
 * Configure the port and pin as OUTPUT to be used as SWDIO
 *
 */
void SWDIO_OutputMode()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_PP;
	GpioInfo.Pin = SWDIO_PIN;
	GpioInfo.Pull = GPIO_PULLUP ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(SWD_PORT,&GpioInfo);
}

/**
 * Configure the port and pin as INPUT to be used as SWDIO
 *
 */
void SWDIO_InputMode()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_INPUT ;
	GpioInfo.Pin = SWDIO_PIN;
	GpioInfo.Pull = GPIO_PULLUP;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(SWD_PORT,&GpioInfo);
}

/**
 * Configure SWDCLK, SWDIO, SWDnRST
 *
 */
void configure_IOPorts()
{
	CLK_EN();

	configureClock();
	configureTargetResetPin();
	SWDIO_OutputMode();

	//delay(10,1,1);
}

/**
  * Configure LED3 and LED4 for debug purpose
  *
  */
void configureLED()	{
  GPIO_InitTypeDef GpioInfo ;

  CLK_EN();

  GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
  GpioInfo.Pin = GPIO_PIN_13 | GPIO_PIN_14; // Init PG13 and PG14
  GpioInfo.Pull = GPIO_NOPULL ;
  GpioInfo.Speed = GPIO_SPEED_HIGH ;

  HAL_GPIO_Init(GPIOG,&GpioInfo);  //Init LED1 on PG13 , lED 2 on PG14
}

void initButton(){
  GPIO_InitTypeDef pushButton;

  BUTTON_CLK_ENABLE();

  pushButton.Mode = GPIO_MODE_INPUT;
  pushButton.Pin = BUTTON_PIN;
  pushButton.Pull = GPIO_PULLDOWN;
  pushButton.Speed = GPIO_SPEED_HIGH;

  HAL_GPIO_Init(BUTTON_PORT, &pushButton);
}

uint32_t Button_GetState()	{
	return HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
}

/**
  *
  */
void InitUartPorts()	{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_RX_GPIO_CLK_ENABLE();
  USARTx_TX_GPIO_CLK_ENABLE();

  /* Enable USART1 clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}
