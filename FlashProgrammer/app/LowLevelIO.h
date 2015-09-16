#ifndef LowLevelIO_H
#define LowLevelIO_H

#if !defined(TEST)

#include "stm32f4xx_hal.h"
#include "configurePort.h"

#define setHighSWCLK()		HAL_GPIO_WritePin(SWD_PORT, SWDCLK_PIN, GPIO_PIN_SET)
#define setLowSWCLK()			HAL_GPIO_WritePin(SWD_PORT, SWDCLK_PIN, GPIO_PIN_RESET)
#define setHighSWDIO()		HAL_GPIO_WritePin(SWD_PORT, SWDIO_PIN, GPIO_PIN_SET)
#define setLowSWDIO()			HAL_GPIO_WritePin(SWD_PORT, SWDIO_PIN, GPIO_PIN_RESET)
#define setHighNRST()	  	HAL_GPIO_WritePin(SWD_PORT, SWDNRST_PIN, GPIO_PIN_SET)
#define setLowNRST()		  HAL_GPIO_WritePin(SWD_PORT, SWDNRST_PIN, GPIO_PIN_RESET)

#else
void setHighSWCLK();
void setLowSWCLK();
void setHighSWDIO();
void setLowSWDIO();
void setHighNRST();
void setLowNRST();
#endif // !defined(TEST)

int readSWDIO();
#endif // LowLevelIO_H
