#ifndef Gpio_H
#define Gpio_H

#include <stdint.h>
#include <stdio.h>
#include "Rcc.h"
#include "memoryReadWrite.h"

#define getGpioIndex(x)	(((uint32_t)(x) - (uint32_t)GPIOA_BASE_ADDRESS) / 0x400)
#define GPIO_OUTPUT   GPIO_MODE_OUTPUT_PP
#define GPIO_INPUT    GPIO_MODE_INPUT_PP

typedef struct GPIO_t GPIO;

struct GPIO_t {
	uint32_t MODER;     /*!< GPIO port mode register,                   Address offset: 0x00      */
	uint32_t OTYPER;    /*!< GPIO port output type register,            Address offset: 0x04      */
	uint32_t OSPEED;    /*!< GPIO port output speed register,           Address offset: 0x08      */
	uint32_t PUPDR;     /*!< GPIO port pull-up/pull-down register,      Address offset: 0x0C      */
	uint32_t IDR;       /*!< GPIO port input data register,             Address offset: 0x10      */
	uint32_t ODR;       /*!< GPIO port output data register,            Address offset: 0x14      */
	uint32_t BSRR;      /*!< GPIO port bit set/reset register,          Address offset: 0x18      */
	uint32_t LCKR;      /*!< GPIO port configuration lock register,     Address offset: 0x1C      */
	uint32_t AFRL;      /*!< GPIO alternate function registers lower,   Address offset: 0x20      */
	uint32_t AFRH;      /*!< GPIO alternate function registers upper,   Address offset: 0x24      */
};

typedef struct {
  int mode;
  int pin;
  int af;
} GPIO_Init;

#define PIN_0       0
#define PIN_1       1
#define PIN_2       2
#define PIN_3       3
#define PIN_4       4
#define PIN_5       5
#define PIN_6       6
#define PIN_7       7
#define PIN_8       8
#define PIN_9       9
#define PIN_10      10
#define PIN_11      11
#define PIN_12      12
#define PIN_13      13
#define PIN_14      14
#define PIN_15      15

#define INPUT_MODE       0
#define OUTPUT_MODE      1
#define ALTFUNC_MODE     2
#define ANALOG_MODE      3

#define LOW_SPEED        0
#define MEDIUM_SPEED     1
#define HIGH_SPEED       2
#define VERY_HIGH_SPEED  3

#define NO_PULL          0
#define PULL_UP          1
#define PULL_DOWN        2
#define _RESERVED        3

#define PUSH_PULL        0
#define OPEN_DRAIN       1

#define AF0				0
#define AF1				1
#define AF2				2
#define AF3				3
#define AF4				4
#define AF5				5
#define AF6				6
#define AF7				7
#define AF8				8
#define AF9				9
#define AF10			10
#define AF11			11
#define AF12			12
#define AF13			13
#define AF14			14
#define AF15			15

#define GPIOA_BASE_ADDRESS	0x40020000
#define GPIOB_BASE_ADDRESS	0x40020400
#define GPIOC_BASE_ADDRESS	0x40020800
#define GPIOD_BASE_ADDRESS	0x40020C00
#define GPIOE_BASE_ADDRESS	0x40021000
#define GPIOF_BASE_ADDRESS	0x40021400
#define GPIOG_BASE_ADDRESS	0x40021800

#define PORTA	((GPIO *)GPIOA_BASE_ADDRESS)
#define PORTB	((GPIO *)GPIOB_BASE_ADDRESS)
#define PORTC	((GPIO *)GPIOC_BASE_ADDRESS)
#define PORTD	((GPIO *)GPIOD_BASE_ADDRESS)
#define PORTE	((GPIO *)GPIOE_BASE_ADDRESS)
#define PORTF	((GPIO *)GPIOF_BASE_ADDRESS)
#define PORTG	((GPIO *)GPIOG_BASE_ADDRESS)

#define resetTargetClock(x)               memoryWriteWord((uint32_t)&rcc->RCC_APB1RSTR, x)
#define enableTargetClock(x)              memoryWriteWord((uint32_t)&rcc->RCC_AHB1ENR, x)

#define setPortMode(__PORT__, x)          memoryWriteWord((uint32_t)&(__PORT__)->MODER, x)
#define setPortSpeed(__PORT__, x)         memoryWriteWord((uint32_t)&(__PORT__)->OSPEED, x)
#define setPortOutputType(__PORT__, x)    memoryWriteWord((uint32_t)&(__PORT__)->OTYPER, x)
#define writePort(__PORT__, x)            memoryWriteWord((uint32_t)&(__PORT__)->ODR, x)

void gpioConfigPort(GPIO *port, GPIO_Init *init);
void pullUpDown(GPIO *port, int pinNum, int pull);
void gpioWriteOne(GPIO *port, uint16_t pinNum);
void gpioWriteZero(GPIO *port, uint16_t pinNum);
void gpioSetAltFunction(GPIO *port, int pinNum, int AF);
void gpioUnresetEnableClock(GPIO *port);

#endif // Gpio_H
