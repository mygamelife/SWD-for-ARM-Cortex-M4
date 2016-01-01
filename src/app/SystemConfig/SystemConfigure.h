#ifndef SystemConfigure_H
#define SystemConfigure_H

#include "configurePort.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_pwr_ex.h"
#include "stm32f4xx_hal_rcc.h"

#define SYSTEM_CLOCK_PRESCALE2  RCC_PLLP_DIV2
#define SYSTEM_CLOCK_PRESCALE4  RCC_PLLP_DIV4 
#define SYSTEM_CLOCK_PRESCALE6  RCC_PLLP_DIV6
#define SYSTEM_CLOCK_PRESCALE8  RCC_PLLP_DIV8

void SystemClock_Config(void);
void FlashSystemConfig(void);
void configSystemClock(uint32_t prescale);
uint32_t getSystemClock();

#endif // SystemConfigure_H
