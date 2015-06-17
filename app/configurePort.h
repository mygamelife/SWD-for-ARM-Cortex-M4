#ifndef configurePort_H
#define configurePort_H

#include "Delay.h"

#ifndef TEST
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#endif

#define SWDIO_PIN 			GPIO_PIN_4
#define SWDCLK_PIN			GPIO_PIN_5
#define SWDnRST_PIN			GPIO_PIN_6
#define SWD_PORT			GPIOA

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
