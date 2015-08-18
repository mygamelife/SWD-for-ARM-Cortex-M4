#ifndef configurePort_H
#define configurePort_H

#include "Delay.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

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

/* SWD Port Configuration */
#define SWDIO_PIN       GPIO_PIN_12
#define SWDCLK_PIN      GPIO_PIN_11
#define SWDNRST_PIN     GPIO_PIN_8
#define SWD_PORT        GPIOC

#define CLK_EN()	do{								\
							__GPIOA_CLK_ENABLE();	\
							__GPIOB_CLK_ENABLE();	\
							__GPIOC_CLK_ENABLE();	\
							__GPIOG_CLK_ENABLE();	\
					}while(0)

/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* USARTx Baud Rate */
#define USART_BAUD_RATE                  9600//115200

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART1

/* Button configuration */
#define BUTTON_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON_PORT                      GPIOA
#define BUTTON_PIN                       GPIO_PIN_0

void configureClock();
void configureTargetResetPin();
void SWDIO_OutputMode();
void SWDIO_InputMode();

void configure_IOPorts();
void configureLED();
void initButton();
uint32_t Button_GetState();
void configureUartPorts();

#endif // configurePort_H
