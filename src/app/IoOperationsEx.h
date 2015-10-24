#ifndef IoOperationsEx_H
#define IoOperationsEx_H

/* ############################ Macros For Hardware Swd I/O Ports ############################### */
#if !defined(TEST)
#include "stm32f4xx_hal.h"

#define setHighSWCLK()		HAL_GPIO_WritePin(SWD_PORT, SWDCLK_PIN, GPIO_PIN_SET)
#define setLowSWCLK()			HAL_GPIO_WritePin(SWD_PORT, SWDCLK_PIN, GPIO_PIN_RESET)
#define setHighSWDIO()		HAL_GPIO_WritePin(SWD_PORT, SWDIO_PIN, GPIO_PIN_SET)
#define setLowSWDIO()			HAL_GPIO_WritePin(SWD_PORT, SWDIO_PIN, GPIO_PIN_RESET)
#define setHighNRST()	  	HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_SET)
#define setLowNRST()		  HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_RESET)
#define readSWDIO()       ((HAL_GPIO_ReadPin(SWD_PORT, SWDIO_PIN)) ? 1 : 0)

#else
void setHighSWCLK();
void setLowSWCLK();
void setHighSWDIO();
void setLowSWDIO();
void setHighNRST();
void setLowNRST();
int readSWDIO();
#endif // !defined(TEST)

#endif // IoOperationsEx_H
