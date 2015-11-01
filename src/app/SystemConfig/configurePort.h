#ifndef configurePort_H
#define configurePort_H

#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

#if !defined(TEST)
	#include "stm32f4xx_hal.h"
	#define turnOnLED3()        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
	#define turnOffLED3()       HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
	#define turnOnLED4()        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
	#define turnOffLED4()       HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
#else
	void turnOnLED3();
	void turnOffLED3();
	void turnOnLED4();
	void turnOffLED4();
#endif // !defined(TEST)

/* SWD Port Configuration */
#define SWD_PORT              GPIOC
#define SWDIO_PIN             GPIO_PIN_12
#define SWDCLK_PIN            GPIO_PIN_11

#define RESET_PORT            GPIOF
#define RESET_PIN             GPIO_PIN_6

#define SWO_PORT              GPIOG
#define SWO_PIN               GPIO_PIN_3

#define CLK_EN()              do{                       \
                                  __GPIOA_CLK_ENABLE();	\
                                  __GPIOB_CLK_ENABLE();	\
                                  __GPIOC_CLK_ENABLE();	\
                                  __GPIOF_CLK_ENABLE();	\
                                  __GPIOG_CLK_ENABLE();	\
                              }while(0)

/* Button configuration */
#define BUTTON_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON_PORT           GPIOA
#define BUTTON_PIN            GPIO_PIN_0

void configureClock(void);
void configureTargetResetPin(void);
void setSWDIOOutputMode(void);
void setSWDIOInputMode(void);

void configureIOPorts(void);
void configureLED(void);
void initButton(void);
uint32_t Button_GetState(void);

#endif // configurePort_H
