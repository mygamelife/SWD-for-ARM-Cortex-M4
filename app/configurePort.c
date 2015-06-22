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

	GpioInfo.Mode = GPIO_MODE_OUTPUT_OD ;
	GpioInfo.Pin = SWDnRST_PIN ;
	GpioInfo.Pull = GPIO_PULLUP ;
	GpioInfo.Speed = GPIO_SPEED_FAST ;

	HAL_GPIO_Init(SWD_PORT,&GpioInfo);
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
