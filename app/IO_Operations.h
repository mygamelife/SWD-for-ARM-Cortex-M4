#ifndef IO_Operations_H
#define IO_Operations_H

#include "stm32f4xx_hal_gpio.h"
#include "configurePort.h"

#if !defined(TEST)

#include "stm32f4xx_hal.h"

#define SWCLK_ON()			HAL_GPIO_WritePin(SWD_PORT, SWDCLK_PIN, GPIO_PIN_SET)
#define SWCLK_OFF()			HAL_GPIO_WritePin(SWD_PORT, SWDCLK_PIN, GPIO_PIN_RESET)
#define SWDIO_High()		HAL_GPIO_WritePin(SWD_PORT, SWDIO_PIN, GPIO_PIN_SET)
#define SWDIO_Low()			HAL_GPIO_WritePin(SWD_PORT, SWDIO_PIN, GPIO_PIN_RESET)
#define ResetPin_High()		HAL_GPIO_WritePin(SWD_PORT, SWDnRST_PIN, GPIO_PIN_SET)
#define ResetPin_Low()		HAL_GPIO_WritePin(SWD_PORT, SWDnRST_PIN, GPIO_PIN_RESET)

#else
void SWCLK_ON();
void SWCLK_OFF();
void SWDIO_High();
void SWDIO_Low();
void ResetPin_High();
void ResetPin_Low();
#endif // !defined(TEST)

int readSWDIO_Pin();

#endif // IO_Operations_H
