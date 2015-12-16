#include "configurePort.h"

/* *
 *  Note : Modify configurePort.h to change the Port and I/O pin for SWD
 */


/**
 * Configure the port and pin to be used as SWCLK
 *
 */
void configureClock(void)
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
void configureTargetResetPin(void)
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Mode = GPIO_MODE_OUTPUT_OD;
	GpioInfo.Pin = RESET_PIN;
	GpioInfo.Pull = GPIO_PULLUP ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(RESET_PORT, &GpioInfo);
}

/**
 * Configure the port and pin to be used to connect to the SWO pin of the target
 *
 */
void configureSWOPin(void)
{	
  //GPIO_InitTypeDef GpioInfo;

	//GpioInfo.Mode = GPIO_MODE_INPUT ;
	//GpioInfo.Pin = SWO_PIN;
	//GpioInfo.Pull = GPIO_PULLUP;
	//GpioInfo.Speed = GPIO_SPEED_FAST ;

	//HAL_GPIO_Init(SWO_PORT,&GpioInfo);
}

/**
 * Configure the port and pin as OUTPUT to be used as SWDIO
 *
 */
void setSWDIOOutputMode(void)
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
void setSWDIOInputMode(void)
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
void configureIOPorts(void)
{
	CLK_EN();

	configureClock();
	configureTargetResetPin();
	setSWDIOOutputMode();
  // configureSWOPin();
}

/**
  * Configure LED3 and LED4 for debug purpose
  *
  */
void configureLED(void)	{
  GPIO_InitTypeDef GpioInfo ;

  CLK_EN();

  GpioInfo.Mode = GPIO_MODE_OUTPUT_PP ;
  GpioInfo.Pin = GPIO_PIN_13 | GPIO_PIN_14; // Init PG13 and PG14
  GpioInfo.Pull = GPIO_NOPULL ;
  GpioInfo.Speed = GPIO_SPEED_HIGH ;

  HAL_GPIO_Init(GPIOG,&GpioInfo);  //Init LED1 on PG13 , lED 2 on PG14
}

void initButton(void) {
  GPIO_InitTypeDef pushButton;

  BUTTON_CLK_ENABLE();

  pushButton.Mode = GPIO_MODE_INPUT;
  pushButton.Pin = BUTTON_PIN;
  pushButton.Pull = GPIO_PULLDOWN;
  pushButton.Speed = GPIO_SPEED_HIGH;

  HAL_GPIO_Init(BUTTON_PORT, &pushButton);
}

uint32_t Button_GetState(void)	{
	return HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
}
