#ifndef configurePort_H
#define configurePort_H

#include "Delay.h"
#include "stm32f4xx_hal_gpio.h"

#if !defined(TEST)
#include "stm32f4xx_hal.h"
#define turnOnLED3()		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
#define turnOffLED3()		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
#define turnOnLED4()		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
#define turnOffLED4()		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);

#else
void turnOnLED3();
void turnOffLED3();
void turnOnLED4();
void turnOffLED4();
#endif // !defined(TEST)

#define SWDIO_PIN 			GPIO_PIN_12
#define SWDCLK_PIN			GPIO_PIN_11
#define SWDNRST_PIN			GPIO_PIN_8
#define SWD_PORT			  GPIOC

#define CLK_EN()	do{								\
							__GPIOA_CLK_ENABLE();	\
							__GPIOB_CLK_ENABLE();	\
							__GPIOC_CLK_ENABLE();	\
							__GPIOG_CLK_ENABLE();	\
					}while(0)

void configureClock();
void configureTargetResetPin();
void SWDIO_OutputMode();
void SWDIO_InputMode();

void configure_IOPorts();

#endif // configurePort_H
